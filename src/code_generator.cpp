#include "code_generator.h"

#include <cctype>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "ast.h"
#include "lexer.h"
#include "parser.h"
#include "parser_bison.h"
#include "token.h"

// ---------------------------------------------------------------------------
// Code generation context
// ---------------------------------------------------------------------------
namespace {

struct CodeGenContext {
    // Current function info (empty when in main or in a procedure)
    std::string currentFuncName;
    std::string currentFuncRetVar; // "__ret_funcname" for functions

    // Type map: identifier → C type ("int", "float", "char", "string")
    std::unordered_map<std::string, std::string> typeMap;

    // All routine names (functions + procedures)
    std::unordered_set<std::string> allRoutines;
    // Routine name → C return type (for functions)
    std::unordered_map<std::string, std::string> routineRetType;
    // Routine name → byRef flags for each param position
    std::unordered_map<std::string, std::vector<bool>> routineByRef;
    // byRef (var) parameter names in current scope — these are int* in C
    std::unordered_set<std::string> byRefParams;
};

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

std::string indent(int level) {
    return std::string(static_cast<std::size_t>(level * 4), ' ');
}

// Convert Pascal basic type name to C type string
std::string basicTypeToCType(const std::string& name) {
    if (name == "integer" || name == "boolean") return "int";
    if (name == "real")    return "float";
    if (name == "char")    return "char";
    return "int"; // fallback for type aliases
}

// ---------------------------------------------------------------------------
// Type emission from TypeNode*
// ---------------------------------------------------------------------------
struct TypeEmit {
    std::string cType;   // e.g. "int", "float", "char"
    std::string suffix;  // C array suffix, e.g. "[101]" or "[5][10]"
};

TypeEmit emitTypeNode(TypeNode* t) {
    if (!t) return {"int", ""};

    if (auto* bt = dynamic_cast<BasicTypeNode*>(t)) {
        return {basicTypeToCType(bt->name), ""};
    }
    if (auto* at = dynamic_cast<ArrayTypeNode*>(t)) {
        TypeEmit elem = emitTypeNode(at->elemType);
        std::string suffix = elem.suffix;
        for (auto* period : at->periods) {
            // Pascal array[low..high]: C must accommodate indices 0..high.
            // Emit size = high + 1 so Pascal 1-based access is in-bounds.
            // Append (not prepend) to preserve Pascal's declared dimension order.
            try {
                int hv = std::stoi(period->high);
                suffix = suffix + "[" + std::to_string(hv + 1) + "]";
            } catch (...) {
                // high is a constant name (e.g. MAXN)
                suffix = suffix + "[" + period->high + " + 1]";
            }
        }
        return {elem.cType, suffix};
    }
    if (dynamic_cast<RecordTypeNode*>(t)) {
        // Minimal record support: emit as int (placeholder)
        return {"int", ""};
    }
    if (dynamic_cast<NamedTypeNode*>(t)) {
        // Type alias: default to int
        return {"int", ""};
    }
    return {"int", ""};
}

// Default C initializer for a given cType
std::string defaultInitializer(const std::string& cType, const std::string& suffix) {
    if (!suffix.empty()) return ""; // arrays: no initializer (global = zero-init)
    if (cType == "float") return " = 0.0f";
    if (cType == "char")  return " = '\\0'";
    return " = 0";
}

// ---------------------------------------------------------------------------
// Expression emission
// ---------------------------------------------------------------------------
std::string emitExpr(ExprNode* e, const CodeGenContext& ctx);

std::string emitExpr(ExprNode* e, const CodeGenContext& ctx) {
    if (!e) return "0";

    if (auto* bin = dynamic_cast<BinaryExprNode*>(e)) {
        const std::string& op = bin->op;
        std::string cOp;
        if      (op == "=")   cOp = "==";
        else if (op == "<>")  cOp = "!=";
        else if (op == "and") cOp = "&&";
        else if (op == "or")  cOp = "||";
        else if (op == "div") cOp = "/";
        else if (op == "mod") cOp = "%";
        else                  cOp = op; // +, -, *, /, <, <=, >, >=
        // Keep parentheses to preserve Pascal operator precedence in C
        return "(" + emitExpr(bin->left, ctx) + " " + cOp + " " + emitExpr(bin->right, ctx) + ")";
    }

    if (auto* un = dynamic_cast<UnaryExprNode*>(e)) {
        if (un->op == "not") {
            // Boolean operand (relational/logical op) → logical NOT (!);
            // Integer operand → bitwise complement (~).
            bool isBoolExpr = false;
            if (auto* bin = dynamic_cast<BinaryExprNode*>(un->operand)) {
                const std::string& bop = bin->op;
                if (bop == "=" || bop == "<>" || bop == "<" || bop == "<=" ||
                    bop == ">" || bop == ">=" || bop == "and" || bop == "or") {
                    isBoolExpr = true;
                }
            }
            return (isBoolExpr ? "! " : "~ ") + emitExpr(un->operand, ctx);
        }
        // Add space after unary +/- to prevent adjacent -- or ++ tokens.
        return un->op + " " + emitExpr(un->operand, ctx);
    }

    if (auto* lit = dynamic_cast<LiteralNode*>(e)) {
        switch (lit->litKind) {
            case LiteralKind::Int:  return lit->raw;
            case LiteralKind::Real: return lit->raw; // no 'f' suffix; rely on implicit double→float conv
            case LiteralKind::Char: return lit->raw; // already has single-quotes
            case LiteralKind::Str: {
                std::string result = "\"";
                for (char c : lit->raw) {
                    if (c == '"')  result += "\\\"";
                    else if (c == '\\') result += "\\\\";
                    else result += c;
                }
                result += "\"";
                return result;
            }
            case LiteralKind::Bool:
                return lit->raw == "true" ? "1" : "0";
        }
    }

    if (auto* var = dynamic_cast<VarExprNode*>(e)) {
        // If this variable name refers to the current function's return slot,
        // redirect to the hidden __ret_ variable.
        std::string name = var->name;
        if (!ctx.currentFuncRetVar.empty() && name == ctx.currentFuncName) {
            name = ctx.currentFuncRetVar;
        } else if (var->parts.empty() && ctx.allRoutines.count(name)) {
            // Bare identifier that refers to a routine (no subscripts/fields).
            // In Pascal, parameterless functions can be called without ().
            // Emit as a call to avoid function-pointer-to-int truncation bugs.
            if (ctx.routineRetType.count(name)) {
                return name + " ( )";
            }
            // Procedure used as expression: return 0 as placeholder
            return "0";
        }

        // byRef parameter (int* in C): dereference when used as a value.
        // Only applies to simple (no-subscript) access; subscripted byRef is rare.
        if (var->parts.empty() && ctx.byRefParams.count(var->name)) {
            return "(*" + name + ")";
        }

        std::string result = name;
        for (auto* part : var->parts) {
            if (part->isField) {
                result += "." + part->fieldName;
            } else {
                result += "[";
                for (std::size_t i = 0; i < part->indices.size(); ++i) {
                    if (i > 0) result += "][";
                    result += emitExpr(part->indices[i], ctx);
                }
                result += "]";
            }
        }
        return result;
    }

    if (auto* call = dynamic_cast<CallExprNode*>(e)) {
        // Emit with spaces: name ( arg1, arg2 ) — matches old generator style
        std::string result = call->name + " ( ";
        auto it = ctx.routineByRef.find(call->name);
        for (std::size_t i = 0; i < call->args.size(); ++i) {
            if (i > 0) result += ", ";
            bool byRef = false;
            if (it != ctx.routineByRef.end() && i < it->second.size()) {
                byRef = it->second[i];
            }
            if (byRef) result += "&";
            result += emitExpr(call->args[i], ctx);
        }
        result += " )";
        return result;
    }

    return "0";
}

// ---------------------------------------------------------------------------
// Type inference for write
// ---------------------------------------------------------------------------
std::string inferExprType(ExprNode* e, const CodeGenContext& ctx) {
    if (!e) return "int";

    if (auto* lit = dynamic_cast<LiteralNode*>(e)) {
        switch (lit->litKind) {
            case LiteralKind::Real: return "float";
            case LiteralKind::Char: return "char";
            case LiteralKind::Str:  return "string";
            default:                return "int";
        }
    }
    if (auto* var = dynamic_cast<VarExprNode*>(e)) {
        std::string lookupName = var->name;
        // Function self-reference → its return type
        if (lookupName == ctx.currentFuncName && !ctx.currentFuncRetVar.empty()) {
            lookupName = ctx.currentFuncRetVar;
        }
        auto it = ctx.typeMap.find(lookupName);
        if (it != ctx.typeMap.end()) return it->second;
        return "int";
    }
    if (auto* call = dynamic_cast<CallExprNode*>(e)) {
        auto it = ctx.routineRetType.find(call->name);
        if (it != ctx.routineRetType.end()) return it->second;
        return "int";
    }
    if (auto* bin = dynamic_cast<BinaryExprNode*>(e)) {
        auto lt = inferExprType(bin->left, ctx);
        auto rt = inferExprType(bin->right, ctx);
        if (lt == "float" || rt == "float") return "float";
        return "int";
    }
    if (auto* un = dynamic_cast<UnaryExprNode*>(e)) {
        return inferExprType(un->operand, ctx);
    }
    return "int";
}

// ---------------------------------------------------------------------------
// Statement emission (forward-declared)
// ---------------------------------------------------------------------------
void emitStmt(StmtNode* s, int lvl, const CodeGenContext& ctx, std::ostream& out);

void emitCompound(CompoundStmtNode* cs, int lvl, const CodeGenContext& ctx, std::ostream& out) {
    out << indent(lvl) << "{\n";
    if (cs) {
        for (auto* s : cs->stmts) emitStmt(s, lvl + 1, ctx, out);
    }
    out << indent(lvl) << "}\n";
}

void emitStmt(StmtNode* s, int lvl, const CodeGenContext& ctx, std::ostream& out) {
    if (!s) return;

    if (auto* cs = dynamic_cast<CompoundStmtNode*>(s)) {
        emitCompound(cs, lvl, ctx, out);
        return;
    }

    if (auto* as = dynamic_cast<AssignStmtNode*>(s)) {
        // Build LHS expression string
        std::string lhs;
        // Redirect function return assignment to __ret_var
        if (!ctx.currentFuncRetVar.empty() && as->varName == ctx.currentFuncName) {
            lhs = ctx.currentFuncRetVar;
        } else if (as->varParts.empty() && ctx.byRefParams.count(as->varName)) {
            // byRef parameter on LHS: write through pointer
            lhs = "*" + as->varName;
        } else {
            lhs = as->varName;
        }
        for (auto* part : as->varParts) {
            if (part->isField) {
                lhs += "." + part->fieldName;
            } else {
                lhs += "[";
                for (std::size_t i = 0; i < part->indices.size(); ++i) {
                    if (i > 0) lhs += "][";
                    lhs += emitExpr(part->indices[i], ctx);
                }
                lhs += "]";
            }
        }
        out << indent(lvl) << lhs << " = " << emitExpr(as->rhs, ctx) << ";\n";
        return;
    }

    if (auto* cs = dynamic_cast<CallStmtNode*>(s)) {
        out << indent(lvl) << cs->name << "(";
        auto it = ctx.routineByRef.find(cs->name);
        for (std::size_t i = 0; i < cs->args.size(); ++i) {
            if (i > 0) out << ", ";
            bool byRef = false;
            if (it != ctx.routineByRef.end() && i < it->second.size()) {
                byRef = it->second[i];
            }
            if (byRef) out << "&";
            out << emitExpr(cs->args[i], ctx);
        }
        out << ");\n";
        return;
    }

    if (auto* is = dynamic_cast<IfStmtNode*>(s)) {
        out << indent(lvl) << "if (" << emitExpr(is->cond, ctx) << ") {\n";
        emitStmt(is->then_, lvl + 1, ctx, out);
        out << indent(lvl) << "}\n";
        if (is->else_) {
            out << indent(lvl) << "else {\n";
            emitStmt(is->else_, lvl + 1, ctx, out);
            out << indent(lvl) << "}\n";
        }
        return;
    }

    if (auto* cs = dynamic_cast<CaseStmtNode*>(s)) {
        out << indent(lvl) << "switch (" << emitExpr(cs->expr, ctx) << ") {\n";
        for (auto* branch : cs->branches) {
            for (const auto& val : branch->values) {
                out << indent(lvl) << "case " << val << ":\n";
            }
            out << indent(lvl) << "{\n";
            emitStmt(branch->body, lvl + 1, ctx, out);
            out << indent(lvl + 1) << "break;\n";
            out << indent(lvl) << "}\n";
        }
        out << indent(lvl) << "}\n";
        return;
    }

    if (auto* ws = dynamic_cast<WhileStmtNode*>(s)) {
        out << indent(lvl) << "while (" << emitExpr(ws->cond, ctx) << ") {\n";
        emitStmt(ws->body, lvl + 1, ctx, out);
        out << indent(lvl) << "}\n";
        return;
    }

    if (auto* rs = dynamic_cast<RepeatStmtNode*>(s)) {
        out << indent(lvl) << "do {\n";
        if (rs->body) {
            for (auto* st : rs->body->stmts) emitStmt(st, lvl + 1, ctx, out);
        }
        out << indent(lvl) << "} while (!(" << emitExpr(rs->cond, ctx) << "));\n";
        return;
    }

    if (auto* fs = dynamic_cast<ForStmtNode*>(s)) {
        const std::string fromStr = emitExpr(fs->from_, ctx);
        const std::string toStr   = emitExpr(fs->to_,   ctx);
        if (fs->isTo) {
            out << indent(lvl) << "for (" << fs->var << " = " << fromStr
                << "; " << fs->var << " <= " << toStr
                << "; ++" << fs->var << ") {\n";
        } else {
            out << indent(lvl) << "for (" << fs->var << " = " << fromStr
                << "; " << fs->var << " >= " << toStr
                << "; --" << fs->var << ") {\n";
        }
        emitStmt(fs->body, lvl + 1, ctx, out);
        out << indent(lvl) << "}\n";
        return;
    }

    if (auto* rs = dynamic_cast<ReadStmtNode*>(s)) {
        for (auto* var : rs->vars) {
            // Determine if this is a read into the function's return slot
            std::string varStr;
            if (!ctx.currentFuncRetVar.empty() && var->name == ctx.currentFuncName) {
                varStr = ctx.currentFuncRetVar;
            } else {
                varStr = emitExpr(static_cast<ExprNode*>(var), ctx);
            }

            std::string lookupName = var->name;
            if (lookupName == ctx.currentFuncName && !ctx.currentFuncRetVar.empty()) {
                lookupName = ctx.currentFuncRetVar;
            }
            auto it = ctx.typeMap.find(lookupName);
            std::string cType = (it != ctx.typeMap.end()) ? it->second : "int";

            if (cType == "float") {
                out << indent(lvl) << "scanf(\"%f\", &" << varStr << ");\n";
            } else if (cType == "char") {
                out << indent(lvl) << "scanf(\" %c\", &" << varStr << ");\n";
            } else {
                out << indent(lvl) << "scanf(\"%d\", &" << varStr << ");\n";
            }
        }
        if (rs->withLn) {
            // consume rest of line
            out << indent(lvl) << "{ int _c; while ((_c = getchar()) != '\\n' && _c != EOF) {} }\n";
        }
        return;
    }

    if (auto* ws = dynamic_cast<WriteStmtNode*>(s)) {
        for (auto* expr : ws->exprs) {
            const std::string exprStr = emitExpr(expr, ctx);
            const std::string etype   = inferExprType(expr, ctx);
            if (etype == "float") {
                out << indent(lvl) << "pas_write_real(" << exprStr << ");\n";
            } else if (etype == "char") {
                out << indent(lvl) << "pas_write_char(" << exprStr << ");\n";
            } else if (etype == "string") {
                out << indent(lvl) << "pas_write_str(" << exprStr << ");\n";
            } else {
                out << indent(lvl) << "pas_write_int(" << exprStr << ");\n";
            }
        }
        if (ws->withLn) {
            out << indent(lvl) << "pas_writeln();\n";
        }
        return;
    }

    if (dynamic_cast<BreakStmtNode*>(s)) {
        out << indent(lvl) << "break;\n";
        return;
    }
    if (dynamic_cast<ContinueStmtNode*>(s)) {
        out << indent(lvl) << "continue;\n";
        return;
    }
    if (auto* es = dynamic_cast<ExitStmtNode*>(s)) {
        if (es->value) {
            out << indent(lvl) << "return " << emitExpr(es->value, ctx) << ";\n";
        } else {
            out << indent(lvl) << "return;\n";
        }
        return;
    }
}

// ---------------------------------------------------------------------------
// Build context from the program AST (typeMap, routineRetType, etc.)
// ---------------------------------------------------------------------------
void buildContext(ProgramNode* prog, CodeGenContext& ctx) {
    if (!prog || !prog->body) return;
    BlockNode* body = prog->body;

    // Constants
    for (auto* c : body->consts) {
        ctx.typeMap[c->name] = c->cType;
    }

    // Global variables
    for (auto* vd : body->vars) {
        TypeEmit te = emitTypeNode(vd->typeNode);
        for (const auto& nm : vd->names) {
            ctx.typeMap[nm] = te.cType;
        }
    }

    // Routines
    for (auto* n : body->routines) {
        auto* r = static_cast<RoutineNode*>(n);
        ctx.allRoutines.insert(r->name);
        if (r->isFunction) {
            ctx.routineRetType[r->name] = basicTypeToCType(r->returnTypeName);
        }
        // Build byRef map
        std::vector<bool> byRef;
        for (auto* pg : r->params) {
            for (std::size_t i = 0; i < pg->names.size(); ++i) {
                byRef.push_back(pg->byRef);
            }
        }
        ctx.routineByRef[r->name] = byRef;
    }
}

// ---------------------------------------------------------------------------
// Emit: preamble (#include + runtime helpers)
// ---------------------------------------------------------------------------
void emitPreamble(std::ostream& out) {
    out << "#include <stdio.h>\n";
    out << "#include <stdbool.h>\n";
    out << "\n";
    out << "static int    pas_read_int(void)    { int v = 0; (void)scanf(\"%d\", &v); return v; }\n";
    out << "static float  pas_read_real(void)   { float v = 0.0f; (void)scanf(\"%f\", &v); return v; }\n";
    out << "static char   pas_read_char(void)   { char v = 0; (void)scanf(\" %c\", &v); return v; }\n";
    out << "static void   pas_write_int(int v)  { (void)printf(\"%d\", v); }\n";
    out << "static void   pas_write_real(float v)  { (void)printf(\"%f\", v); }\n";
    out << "static void   pas_write_char(char v)   { (void)printf(\"%c\", v); }\n";
    out << "static void   pas_write_str(const char* v) { (void)printf(\"%s\", v); }\n";
    out << "static void   pas_writeln(void)     { (void)printf(\"\\n\"); }\n";
    out << "\n";
}

// ---------------------------------------------------------------------------
// Emit: global constants
// ---------------------------------------------------------------------------
void emitGlobalConsts(BlockNode* body, std::ostream& out) {
    if (!body || body->consts.empty()) return;
    for (auto* c : body->consts) {
        if (c->cType == "float") {
            // Use static const to avoid macro-expansion conflicts with
            // identically-named function parameters (Pascal is case-insensitive,
            // so a const RADIUS and a param radius are the same identifier).
            out << "static const float " << c->name << " = " << c->cValue << ";\n";
        } else {
            out << "#define " << c->name << " " << c->cValue << "\n";
        }
    }
    out << "\n";
}

// ---------------------------------------------------------------------------
// Emit: global variable declarations
// ---------------------------------------------------------------------------
void emitGlobalVars(BlockNode* body, const CodeGenContext& ctx, std::ostream& out) {
    if (!body || body->vars.empty()) return;
    for (auto* vd : body->vars) {
        TypeEmit te = emitTypeNode(vd->typeNode);
        for (const auto& nm : vd->names) {
            out << te.cType << " " << nm << te.suffix
                << defaultInitializer(te.cType, te.suffix) << ";\n";
        }
    }
    out << "\n";
}

// ---------------------------------------------------------------------------
// Emit: forward declarations for all routines
// ---------------------------------------------------------------------------
void emitForwardDecl(RoutineNode* r, std::ostream& out) {
    if (r->isFunction) {
        out << basicTypeToCType(r->returnTypeName) << " " << r->name << "(";
    } else {
        out << "void " << r->name << "(";
    }
    bool first = true;
    for (auto* pg : r->params) {
        std::string cType = basicTypeToCType(pg->typeName);
        for (const auto& nm : pg->names) {
            if (!first) out << ", ";
            first = false;
            if (pg->byRef) out << cType << "* " << nm;
            else           out << cType << " "  << nm;
        }
    }
    out << ");\n";
}

// ---------------------------------------------------------------------------
// Emit: a single routine definition
// ---------------------------------------------------------------------------
void emitRoutine(RoutineNode* r, const CodeGenContext& globalCtx, std::ostream& out) {
    // Build a local context inheriting from globalCtx
    CodeGenContext ctx = globalCtx;
    ctx.currentFuncName   = r->isFunction ? r->name : "";
    ctx.currentFuncRetVar = r->isFunction ? ("__ret_" + r->name) : "";

    // Signature
    if (r->isFunction) {
        out << basicTypeToCType(r->returnTypeName) << " " << r->name << "(";
    } else {
        out << "void " << r->name << "(";
    }
    bool first = true;
    for (auto* pg : r->params) {
        std::string cType = basicTypeToCType(pg->typeName);
        for (const auto& nm : pg->names) {
            if (!first) out << ", ";
            first = false;
            if (pg->byRef) out << cType << "* " << nm;
            else           out << cType << " "  << nm;
            ctx.typeMap[nm] = cType;
            if (pg->byRef) ctx.byRefParams.insert(nm);
        }
    }
    out << ") {\n";

    BlockNode* body = r->body;
    if (body) {
        // Register nested routines in local context so bare-name calls work
        for (auto* n : body->routines) {
            auto* nr = static_cast<RoutineNode*>(n);
            ctx.allRoutines.insert(nr->name);
            if (nr->isFunction) {
                ctx.routineRetType[nr->name] = basicTypeToCType(nr->returnTypeName);
            }
            std::vector<bool> byRef;
            for (auto* pg : nr->params) {
                for (std::size_t i = 0; i < pg->names.size(); ++i) {
                    byRef.push_back(pg->byRef);
                }
            }
            ctx.routineByRef[nr->name] = byRef;
        }

        // Local constants
        for (auto* c : body->consts) {
            if (c->cType == "float") {
                out << "    static const float " << c->name << " = " << c->cValue << ";\n";
            } else {
                out << "    #define " << c->name << " " << c->cValue << "\n";
            }
            ctx.typeMap[c->name] = c->cType;
        }

        // Local variables
        for (auto* vd : body->vars) {
            TypeEmit te = emitTypeNode(vd->typeNode);
            for (const auto& nm : vd->names) {
                out << "    " << te.cType << " " << nm << te.suffix
                    << defaultInitializer(te.cType, te.suffix) << ";\n";
                ctx.typeMap[nm] = te.cType;
            }
        }
    }

    // Function return variable
    if (r->isFunction) {
        std::string retCType = basicTypeToCType(r->returnTypeName);
        out << "    " << retCType << " " << ctx.currentFuncRetVar
            << defaultInitializer(retCType, "") << ";\n";
        ctx.typeMap[ctx.currentFuncRetVar] = retCType;
    }

    // Body statements
    if (body && body->compound) {
        for (auto* st : body->compound->stmts) {
            emitStmt(st, 1, ctx, out);
        }
    }

    // Return
    if (r->isFunction) {
        out << "    return " << ctx.currentFuncRetVar << ";\n";
    }
    out << "}\n\n";

    // Nested routine definitions — emitted AFTER the enclosing function closes,
    // so they become top-level C functions (standard C, no GCC nested-fn extension).
    if (body) {
        for (auto* n : body->routines) {
            auto* nr = static_cast<RoutineNode*>(n);
            emitRoutine(nr, ctx, out);
        }
    }
}

// ---------------------------------------------------------------------------
// Emit: main function
// ---------------------------------------------------------------------------
void emitMain(BlockNode* body, const CodeGenContext& ctx, std::ostream& out) {
    out << "int main(void) {\n";

    CodeGenContext mainCtx = ctx;
    mainCtx.currentFuncName   = "";
    mainCtx.currentFuncRetVar = "";

    if (body) {
        // Local constants in main block (already in global ctx, skip)
        // Local variables in main block (already global, skip)
        if (body->compound) {
            for (auto* st : body->compound->stmts) {
                emitStmt(st, 1, mainCtx, out);
            }
        }
    }

    out << "    return 0;\n";
    out << "}\n";
}

} // namespace

// ---------------------------------------------------------------------------
// Public API
// ---------------------------------------------------------------------------

CodegenResult CodeGenerator::generate(const ParserResult& parseResult) const {
    if (!parseResult.root) {
        return {false, "No AST root: parsing failed or produced no tree.", ""};
    }

    auto* prog = static_cast<ProgramNode*>(parseResult.root.get());
    if (!prog->body) {
        return {false, "Program has no body block.", ""};
    }

    std::ostringstream out;

    // 1. Build global code-generation context
    CodeGenContext ctx;
    buildContext(prog, ctx);

    // 2. Preamble
    emitPreamble(out);

    // 3. Global constants
    emitGlobalConsts(prog->body, out);

    // 4. Global variables
    emitGlobalVars(prog->body, ctx, out);

    // 5. Forward declarations
    for (auto* n : prog->body->routines) {
        emitForwardDecl(static_cast<RoutineNode*>(n), out);
    }
    if (!prog->body->routines.empty()) out << "\n";

    // 6. Routine definitions
    for (auto* n : prog->body->routines) {
        emitRoutine(static_cast<RoutineNode*>(n), ctx, out);
    }

    // 7. main()
    emitMain(prog->body, ctx, out);

    return {true, "", out.str()};
}

std::string CodeGenerator::sanitizeIdentifier(const std::string& rawName) {
    // Extract just the base filename (strip directory)
    std::string base = rawName;
    const std::size_t slash = base.find_last_of("/\\");
    if (slash != std::string::npos) base = base.substr(slash + 1);
    // Strip extension (.pas, .PAS, etc.)
    const std::size_t dot = base.rfind('.');
    if (dot != std::string::npos) base = base.substr(0, dot);
    // Replace non-alphanumeric with '_'
    std::string result;
    result.reserve(base.size() + 1);
    for (char c : base) {
        result += (std::isalnum(static_cast<unsigned char>(c)) ? c : '_');
    }
    // Prefix with '_' if empty or starts with a digit
    if (result.empty() || std::isdigit(static_cast<unsigned char>(result[0]))) {
        result = "_" + result;
    }
    return result;
}

CodegenResult CodeGenerator::generateTemplate(const std::string& inputPath) const {
    std::ifstream in(inputPath);
    if (!in.good()) {
        return {false, "Cannot open input file: " + inputPath, ""};
    }
    std::ostringstream buffer;
    buffer << in.rdbuf();
    const std::string sourceCode = buffer.str();

    Lexer lexer;
    const LexerResult lexResult = lexer.tokenizeDetailed(sourceCode);
    if (!lexResult.errors.empty()) {
        return {false, "Lexical errors in " + inputPath, ""};
    }

    const ParserResult parserResult = parseWithBison(lexResult.tokens);
    if (!parserResult.errors.empty()) {
        return {false, "Syntax errors in " + inputPath, ""};
    }

    CodegenResult result = generate(parserResult);
    if (result.ok) {
        // Prepend source-unit comment
        const std::string unitName = sanitizeIdentifier(inputPath);
        result.cSource = "// Source unit: " + unitName + "\n" + result.cSource;
    }
    return result;
}
