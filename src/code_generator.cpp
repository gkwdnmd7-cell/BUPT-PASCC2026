#include "code_generator.h"

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <vector>

#include "lexer.h"
#include "token.h"

namespace fs = std::filesystem;

namespace {
struct VarDecl {
    std::string name;
    std::string cType;
};

struct RoutineParam {
    std::string name;
    std::string cType;
    bool byRef = false;
};

struct RoutineDecl {
    std::string name;
    std::string returnType;
    bool isFunction = false;
    std::vector<RoutineParam> params;
    std::vector<VarDecl> localDecls;
    std::vector<std::string> body;
};

struct StatementContext {
    std::string currentFunctionName;
};

struct RoutineParseResult {
    std::vector<RoutineDecl> routines;
    std::size_t mainBeginIndex = static_cast<std::size_t>(-1);
};

using CStatements = std::vector<std::string>;

std::string mapTypeTokenToC(TokenType type) {
    switch (type) {
        case TokenType::KwInteger: return "int";
        case TokenType::KwReal: return "float";
        case TokenType::KwBoolean: return "bool";
        case TokenType::KwChar: return "char";
        default: return "";
    }
}

std::string tokenToExprPiece(const Token& tok) {
    switch (tok.type) {
        case TokenType::Identifier:
        case TokenType::IntegerLiteral:
        case TokenType::RealLiteral:
        case TokenType::CharLiteral:
            return tok.lexeme;
        case TokenType::BooleanLiteral:
            return tok.lexeme == "true" ? "true" : "false";
        case TokenType::Plus: return "+";
        case TokenType::Minus: return "-";
        case TokenType::Multiply: return "*";
        case TokenType::Divide: return "/";
        case TokenType::Div: return "/";
        case TokenType::Mod: return "%";
        case TokenType::And: return "&&";
        case TokenType::Or: return "||";
        case TokenType::Not: return "!";
        case TokenType::Equal: return "==";
        case TokenType::NotEqual: return "!=";
        case TokenType::Less: return "<";
        case TokenType::LessEqual: return "<=";
        case TokenType::Greater: return ">";
        case TokenType::GreaterEqual: return ">=";
        case TokenType::LParen: return "(";
        case TokenType::RParen: return ")";
        case TokenType::Comma: return ",";
        default: return "";
    }
}

std::vector<VarDecl> parseGlobalVarDecls(const std::vector<Token>& tokens) {
    std::vector<VarDecl> decls;
    std::size_t i = 0;
    while (i < tokens.size() && tokens[i].type != TokenType::KwVar && tokens[i].type != TokenType::KwBegin &&
           tokens[i].type != TokenType::EndOfFile) {
        ++i;
    }

    if (i >= tokens.size() || tokens[i].type != TokenType::KwVar) {
        return decls;
    }
    ++i;

    while (i < tokens.size()) {
        if (tokens[i].type == TokenType::KwBegin || tokens[i].type == TokenType::KwProcedure || tokens[i].type == TokenType::KwFunction ||
            tokens[i].type == TokenType::EndOfFile) {
            break;
        }

        std::vector<std::string> names;
        if (tokens[i].type == TokenType::Identifier) {
            names.push_back(tokens[i].lexeme);
            ++i;
            while (i < tokens.size() && tokens[i].type == TokenType::Comma) {
                ++i;
                if (i < tokens.size() && tokens[i].type == TokenType::Identifier) {
                    names.push_back(tokens[i].lexeme);
                    ++i;
                } else {
                    break;
                }
            }
        }

        if (i >= tokens.size() || tokens[i].type != TokenType::Colon) {
            while (i < tokens.size() && tokens[i].type != TokenType::Semicolon && tokens[i].type != TokenType::KwBegin) {
                ++i;
            }
            if (i < tokens.size() && tokens[i].type == TokenType::Semicolon) {
                ++i;
            }
            continue;
        }
        ++i;

        const std::string cType = (i < tokens.size()) ? mapTypeTokenToC(tokens[i].type) : "";
        if (i < tokens.size()) {
            ++i;
        }

        for (const auto& name : names) {
            if (!cType.empty()) {
                decls.push_back(VarDecl{name, cType});
            }
        }

        while (i < tokens.size() && tokens[i].type != TokenType::Semicolon && tokens[i].type != TokenType::KwBegin) {
            ++i;
        }
        if (i < tokens.size() && tokens[i].type == TokenType::Semicolon) {
            ++i;
        }
    }

    return decls;
}

std::vector<VarDecl> parseVarDeclsInRange(const std::vector<Token>& tokens, std::size_t start, std::size_t endExclusive) {
    std::vector<VarDecl> decls;
    std::size_t i = start;

    while (i < endExclusive) {
        if (tokens[i].type != TokenType::KwVar) {
            ++i;
            continue;
        }
        ++i;

        while (i < endExclusive && tokens[i].type != TokenType::KwBegin && tokens[i].type != TokenType::KwProcedure &&
               tokens[i].type != TokenType::KwFunction) {
            std::vector<std::string> names;
            if (tokens[i].type == TokenType::Identifier) {
                names.push_back(tokens[i].lexeme);
                ++i;
                while (i < endExclusive && tokens[i].type == TokenType::Comma) {
                    ++i;
                    if (i < endExclusive && tokens[i].type == TokenType::Identifier) {
                        names.push_back(tokens[i].lexeme);
                        ++i;
                    } else {
                        break;
                    }
                }
            }

            if (i >= endExclusive || tokens[i].type != TokenType::Colon) {
                while (i < endExclusive && tokens[i].type != TokenType::Semicolon && tokens[i].type != TokenType::KwBegin) {
                    ++i;
                }
                if (i < endExclusive && tokens[i].type == TokenType::Semicolon) {
                    ++i;
                }
                continue;
            }
            ++i;

            const std::string cType = (i < endExclusive) ? mapTypeTokenToC(tokens[i].type) : "";
            if (i < endExclusive) {
                ++i;
            }

            for (const auto& name : names) {
                if (!cType.empty()) {
                    decls.push_back(VarDecl{name, cType});
                }
            }

            while (i < endExclusive && tokens[i].type != TokenType::Semicolon && tokens[i].type != TokenType::KwBegin) {
                ++i;
            }
            if (i < endExclusive && tokens[i].type == TokenType::Semicolon) {
                ++i;
            }
        }
    }

    return decls;
}

std::string joinPieces(const std::vector<std::string>& pieces) {
    std::ostringstream out;
    for (std::size_t i = 0; i < pieces.size(); ++i) {
        if (i > 0) {
            out << " ";
        }
        out << pieces[i];
    }
    return out.str();
}

bool isStopToken(TokenType t, const std::vector<TokenType>& stopTokens) {
    return std::find(stopTokens.begin(), stopTokens.end(), t) != stopTokens.end();
}

std::string parseExpressionUntil(const std::vector<Token>& tokens, std::size_t& i, const std::vector<TokenType>& stopTokens) {
    std::vector<std::string> pieces;
    int parenDepth = 0;

    while (i < tokens.size()) {
        const TokenType type = tokens[i].type;
        if (type == TokenType::LParen) {
            ++parenDepth;
        } else if (type == TokenType::RParen && parenDepth > 0) {
            --parenDepth;
        }

        if (parenDepth == 0 && isStopToken(type, stopTokens)) {
            break;
        }

        const std::string piece = tokenToExprPiece(tokens[i]);
        if (!piece.empty()) {
            pieces.push_back(piece);
        }
        ++i;
    }

    return joinPieces(pieces);
}

void emitStatementLine(CStatements& out, int indentLevel, const std::string& stmt) {
    out.push_back(std::string(static_cast<std::size_t>(indentLevel) * 4U, ' ') + stmt);
}

void parseSingleStatement(const std::vector<Token>& tokens, std::size_t& i, CStatements& out, int indentLevel,
                          const StatementContext& ctx);

void parseCompoundBody(const std::vector<Token>& tokens, std::size_t& i, CStatements& out, int indentLevel,
                       const StatementContext& ctx) {
    while (i < tokens.size() && tokens[i].type != TokenType::KwEnd && tokens[i].type != TokenType::EndOfFile) {
        parseSingleStatement(tokens, i, out, indentLevel, ctx);
        if (i < tokens.size() && tokens[i].type == TokenType::Semicolon) {
            ++i;
        }
    }
    if (i < tokens.size() && tokens[i].type == TokenType::KwEnd) {
        ++i;
    }
}

void parseIfStatement(const std::vector<Token>& tokens, std::size_t& i, CStatements& out, int indentLevel,
                      const StatementContext& ctx) {
    ++i;  // consume if
    const std::string cond = parseExpressionUntil(tokens, i, {TokenType::KwThen});
    if (i < tokens.size() && tokens[i].type == TokenType::KwThen) {
        ++i;
    }

    emitStatementLine(out, indentLevel, "if (" + (cond.empty() ? "/* TODO */ 0" : cond) + ") {");
    parseSingleStatement(tokens, i, out, indentLevel + 1, ctx);
    emitStatementLine(out, indentLevel, "}");

    if (i < tokens.size() && tokens[i].type == TokenType::KwElse) {
        ++i;
        emitStatementLine(out, indentLevel, "else {");
        parseSingleStatement(tokens, i, out, indentLevel + 1, ctx);
        emitStatementLine(out, indentLevel, "}");
    }
}

void parseWhileStatement(const std::vector<Token>& tokens, std::size_t& i, CStatements& out, int indentLevel,
                         const StatementContext& ctx) {
    ++i;  // consume while
    const std::string cond = parseExpressionUntil(tokens, i, {TokenType::KwDo});
    if (i < tokens.size() && tokens[i].type == TokenType::KwDo) {
        ++i;
    }

    emitStatementLine(out, indentLevel, "while (" + (cond.empty() ? "/* TODO */ 0" : cond) + ") {");
    parseSingleStatement(tokens, i, out, indentLevel + 1, ctx);
    emitStatementLine(out, indentLevel, "}");
}

void parseForStatement(const std::vector<Token>& tokens, std::size_t& i, CStatements& out, int indentLevel,
                       const StatementContext& ctx) {
    ++i;  // consume for
    std::string loopVar;
    if (i < tokens.size() && tokens[i].type == TokenType::Identifier) {
        loopVar = tokens[i].lexeme;
        ++i;
    }
    if (i < tokens.size() && tokens[i].type == TokenType::Assign) {
        ++i;
    }

    const std::string beginExpr = parseExpressionUntil(tokens, i, {TokenType::KwTo, TokenType::KwDownTo, TokenType::KwDo});
    bool isDownTo = false;
    if (i < tokens.size() && (tokens[i].type == TokenType::KwTo || tokens[i].type == TokenType::KwDownTo)) {
        isDownTo = tokens[i].type == TokenType::KwDownTo;
        ++i;
    }
    const std::string endExpr = parseExpressionUntil(tokens, i, {TokenType::KwDo});
    if (i < tokens.size() && tokens[i].type == TokenType::KwDo) {
        ++i;
    }

    const std::string var = loopVar.empty() ? "i" : loopVar;
    const std::string begin = beginExpr.empty() ? "0" : beginExpr;
    const std::string end = endExpr.empty() ? "0" : endExpr;
    const std::string cond = isDownTo ? (var + " >= " + end) : (var + " <= " + end);
    const std::string step = isDownTo ? ("--" + var) : ("++" + var);

    emitStatementLine(out, indentLevel, "for (" + var + " = " + begin + "; " + cond + "; " + step + ") {");
    parseSingleStatement(tokens, i, out, indentLevel + 1, ctx);
    emitStatementLine(out, indentLevel, "}");
}

std::vector<std::string> parseExprList(const std::vector<Token>& tokens, std::size_t& i) {
    std::vector<std::string> exprs;
    if (i >= tokens.size() || tokens[i].type != TokenType::LParen) {
        return exprs;
    }
    ++i;

    while (i < tokens.size() && tokens[i].type != TokenType::RParen && tokens[i].type != TokenType::EndOfFile) {
        const std::string expr = parseExpressionUntil(tokens, i, {TokenType::Comma, TokenType::RParen});
        if (!expr.empty()) {
            exprs.push_back(expr);
        }
        if (i < tokens.size() && tokens[i].type == TokenType::Comma) {
            ++i;
            continue;
        }
    }

    if (i < tokens.size() && tokens[i].type == TokenType::RParen) {
        ++i;
    }
    return exprs;
}

void parseReadStatement(const std::vector<Token>& tokens, std::size_t& i, CStatements& out, int indentLevel) {
    ++i;  // consume read/readln
    const auto args = parseExprList(tokens, i);
    for (const auto& arg : args) {
        emitStatementLine(out, indentLevel, arg + " = pas_read_int();");
    }
}

void parseWriteStatement(const std::vector<Token>& tokens, std::size_t& i, CStatements& out, int indentLevel, bool withNewline) {
    ++i;  // consume write/writeln
    const auto args = parseExprList(tokens, i);
    for (const auto& arg : args) {
        emitStatementLine(out, indentLevel, "pas_write_int(" + arg + ");");
    }
    if (withNewline) {
        emitStatementLine(out, indentLevel, "pas_writeln();");
    }
}

std::string joinByComma(const std::vector<std::string>& items) {
    std::ostringstream out;
    for (std::size_t idx = 0; idx < items.size(); ++idx) {
        if (idx > 0) {
            out << ", ";
        }
        out << items[idx];
    }
    return out.str();
}

void parseAssignmentStatement(const std::vector<Token>& tokens, std::size_t& i, CStatements& out, int indentLevel,
                              const StatementContext& ctx) {
    const std::string lhs = tokens[i].lexeme;
    i += 2;  // identifier + assign
    const std::string rhs = parseExpressionUntil(tokens, i, {TokenType::Semicolon, TokenType::KwEnd, TokenType::KwElse});

    if (!ctx.currentFunctionName.empty() && lhs == ctx.currentFunctionName) {
        emitStatementLine(out, indentLevel, "return " + (rhs.empty() ? "0" : rhs) + ";");
        return;
    }
    emitStatementLine(out, indentLevel, lhs + " = " + (rhs.empty() ? "0" : rhs) + ";");
}

void parseCallStatement(const std::vector<Token>& tokens, std::size_t& i, CStatements& out, int indentLevel) {
    const std::string name = tokens[i].lexeme;
    ++i;

    if (i < tokens.size() && tokens[i].type == TokenType::LParen) {
        const auto args = parseExprList(tokens, i);
        emitStatementLine(out, indentLevel, name + "(" + joinByComma(args) + ");");
        return;
    }
    emitStatementLine(out, indentLevel, name + "();");
}

void parseSingleStatement(const std::vector<Token>& tokens, std::size_t& i, CStatements& out, int indentLevel,
                          const StatementContext& ctx) {
    if (i >= tokens.size()) {
        return;
    }

    if (tokens[i].type == TokenType::KwBegin) {
        ++i;
        parseCompoundBody(tokens, i, out, indentLevel, ctx);
        return;
    }

    if (tokens[i].type == TokenType::Identifier && i + 1 < tokens.size() && tokens[i + 1].type == TokenType::Assign) {
        parseAssignmentStatement(tokens, i, out, indentLevel, ctx);
        return;
    }

    if (tokens[i].type == TokenType::Identifier) {
        parseCallStatement(tokens, i, out, indentLevel);
        return;
    }

    if (tokens[i].type == TokenType::KwIf) {
        parseIfStatement(tokens, i, out, indentLevel, ctx);
        return;
    }

    if (tokens[i].type == TokenType::KwWhile) {
        parseWhileStatement(tokens, i, out, indentLevel, ctx);
        return;
    }

    if (tokens[i].type == TokenType::KwFor) {
        parseForStatement(tokens, i, out, indentLevel, ctx);
        return;
    }

    if (tokens[i].type == TokenType::KwRead || tokens[i].type == TokenType::KwReadLn) {
        parseReadStatement(tokens, i, out, indentLevel);
        return;
    }

    if (tokens[i].type == TokenType::KwWrite) {
        parseWriteStatement(tokens, i, out, indentLevel, false);
        return;
    }

    if (tokens[i].type == TokenType::KwWriteLn) {
        parseWriteStatement(tokens, i, out, indentLevel, true);
        return;
    }

    ++i;
}

CStatements parseMainStatements(const std::vector<Token>& tokens, std::size_t mainBeginIndex) {
    CStatements stmts;
    if (mainBeginIndex >= tokens.size() || tokens[mainBeginIndex].type != TokenType::KwBegin) {
        return stmts;
    }

    std::size_t i = mainBeginIndex;
    ++i;  // consume program-level begin
    parseCompoundBody(tokens, i, stmts, 1, StatementContext{});
    return stmts;
}

std::vector<RoutineParam> parseRoutineParams(const std::vector<Token>& tokens, std::size_t& i) {
    std::vector<RoutineParam> params;
    if (i >= tokens.size() || tokens[i].type != TokenType::LParen) {
        return params;
    }
    ++i;

    while (i < tokens.size() && tokens[i].type != TokenType::RParen && tokens[i].type != TokenType::EndOfFile) {
        bool byRef = false;
        if (tokens[i].type == TokenType::KwVar) {
            byRef = true;
            ++i;
        }

        std::vector<std::string> names;
        if (i < tokens.size() && tokens[i].type == TokenType::Identifier) {
            names.push_back(tokens[i].lexeme);
            ++i;
            while (i < tokens.size() && tokens[i].type == TokenType::Comma) {
                ++i;
                if (i < tokens.size() && tokens[i].type == TokenType::Identifier) {
                    names.push_back(tokens[i].lexeme);
                    ++i;
                } else {
                    break;
                }
            }
        }

        if (i >= tokens.size() || tokens[i].type != TokenType::Colon) {
            while (i < tokens.size() && tokens[i].type != TokenType::Semicolon && tokens[i].type != TokenType::RParen) {
                ++i;
            }
            if (i < tokens.size() && tokens[i].type == TokenType::Semicolon) {
                ++i;
            }
            continue;
        }
        ++i;

        const std::string cType = (i < tokens.size()) ? mapTypeTokenToC(tokens[i].type) : "";
        if (i < tokens.size()) {
            ++i;
        }

        for (const auto& name : names) {
            if (!cType.empty()) {
                params.push_back(RoutineParam{name, cType, byRef});
            }
        }

        if (i < tokens.size() && tokens[i].type == TokenType::Semicolon) {
            ++i;
        }
    }

    if (i < tokens.size() && tokens[i].type == TokenType::RParen) {
        ++i;
    }
    return params;
}

bool parseRoutineAt(const std::vector<Token>& tokens, std::size_t start, RoutineDecl& outRoutine, std::size_t& nextIndex) {
    if (start >= tokens.size()) {
        return false;
    }
    if (tokens[start].type != TokenType::KwProcedure && tokens[start].type != TokenType::KwFunction) {
        return false;
    }

    RoutineDecl routine;
    routine.isFunction = tokens[start].type == TokenType::KwFunction;
    routine.returnType = routine.isFunction ? "int" : "void";

    std::size_t i = start + 1;
    if (i < tokens.size() && tokens[i].type == TokenType::Identifier) {
        routine.name = tokens[i].lexeme;
        ++i;
    } else {
        routine.name = "pas_routine";
    }

    if (i < tokens.size() && tokens[i].type == TokenType::LParen) {
        routine.params = parseRoutineParams(tokens, i);
    }

    if (routine.isFunction && i < tokens.size() && tokens[i].type == TokenType::Colon) {
        ++i;
        if (i < tokens.size()) {
            const std::string mapped = mapTypeTokenToC(tokens[i].type);
            if (!mapped.empty()) {
                routine.returnType = mapped;
            }
            ++i;
        }
    }

    while (i < tokens.size() && tokens[i].type != TokenType::Semicolon && tokens[i].type != TokenType::EndOfFile) {
        ++i;
    }
    if (i < tokens.size() && tokens[i].type == TokenType::Semicolon) {
        ++i;
    }

    const std::size_t declStart = i;
    while (i < tokens.size() && tokens[i].type != TokenType::KwBegin && tokens[i].type != TokenType::EndOfFile) {
        ++i;
    }
    if (i >= tokens.size() || tokens[i].type != TokenType::KwBegin) {
        nextIndex = i;
        outRoutine = routine;
        return true;
    }

    const std::size_t bodyBeginIndex = i;
    routine.localDecls = parseVarDeclsInRange(tokens, declStart, bodyBeginIndex);

    std::size_t bodyCursor = bodyBeginIndex + 1;
    parseCompoundBody(tokens, bodyCursor, routine.body, 1,
                      StatementContext{routine.isFunction ? routine.name : std::string()});

    bool hasReturn = false;
    for (const auto& line : routine.body) {
        if (line.find("return ") != std::string::npos) {
            hasReturn = true;
            break;
        }
    }
    if (routine.isFunction && !hasReturn) {
        routine.body.push_back("    return 0;");
    }

    nextIndex = bodyCursor;
    if (nextIndex < tokens.size() && tokens[nextIndex].type == TokenType::Semicolon) {
        ++nextIndex;
    }

    outRoutine = routine;
    return true;
}

RoutineParseResult parseRoutinesAndMain(const std::vector<Token>& tokens) {
    RoutineParseResult result;

    std::size_t i = 0;
    if (i < tokens.size() && tokens[i].type == TokenType::KwProgram) {
        while (i < tokens.size() && tokens[i].type != TokenType::Semicolon && tokens[i].type != TokenType::EndOfFile) {
            ++i;
        }
        if (i < tokens.size() && tokens[i].type == TokenType::Semicolon) {
            ++i;
        }
    }

    while (i < tokens.size() && tokens[i].type != TokenType::EndOfFile) {
        if (tokens[i].type == TokenType::KwProcedure || tokens[i].type == TokenType::KwFunction) {
            RoutineDecl routine;
            std::size_t next = i + 1;
            if (parseRoutineAt(tokens, i, routine, next)) {
                result.routines.push_back(routine);
                i = next;
                continue;
            }
        }

        if (tokens[i].type == TokenType::KwBegin) {
            result.mainBeginIndex = i;
            break;
        }
        ++i;
    }

    return result;
}

std::string formatRoutineParams(const std::vector<RoutineParam>& params) {
    std::ostringstream out;
    for (std::size_t i = 0; i < params.size(); ++i) {
        if (i > 0) {
            out << ", ";
        }
        out << params[i].cType;
        if (params[i].byRef) {
            out << " *";
        } else {
            out << " ";
        }
        out << params[i].name;
    }
    return out.str();
}

void emitRoutineDefinitions(std::ostringstream& out, const std::vector<RoutineDecl>& routines) {
    if (routines.empty()) {
        out << "/* TODO(T-028): emit translated procedure/function declarations here. */\n\n";
        return;
    }

    for (const auto& routine : routines) {
        out << routine.returnType << " " << routine.name << "(" << formatRoutineParams(routine.params) << ") {\n";
        for (const auto& decl : routine.localDecls) {
            out << "    " << decl.cType << " " << decl.name << " = 0;\n";
        }
        if (!routine.localDecls.empty() && !routine.body.empty()) {
            out << "\n";
        }
        if (routine.body.empty()) {
            if (routine.isFunction) {
                out << "    return 0;\n";
            }
        } else {
            for (const auto& stmt : routine.body) {
                out << stmt << "\n";
            }
        }
        out << "}\n\n";
    }
}
}  // namespace

CodegenResult CodeGenerator::generateTemplate(const std::string& inputPath) const {
    CodegenResult result;

    fs::path sourcePath(inputPath);
    const std::string unitName = sanitizeIdentifier(sourcePath.stem().string());
    if (unitName.empty()) {
        result.ok = false;
        result.message = "Cannot derive valid C unit name from input path.";
        return result;
    }

    std::ifstream in(inputPath);
    if (!in.good()) {
        result.ok = false;
        result.message = "Failed to read source file for code generation.";
        return result;
    }
    std::ostringstream srcStream;
    srcStream << in.rdbuf();

    Lexer lexer;
    const LexerResult lexResult = lexer.tokenizeDetailed(srcStream.str());
    const auto decls = parseGlobalVarDecls(lexResult.tokens);
    const auto routineParse = parseRoutinesAndMain(lexResult.tokens);
    const auto statements = parseMainStatements(lexResult.tokens, routineParse.mainBeginIndex);

    std::ostringstream out;
    out << "/* Auto-generated by pascc (M5 template stage). */\n";
    out << "/* Source unit: " << unitName << " */\n\n";
    out << "#include <stdio.h>\n";
    out << "#include <stdbool.h>\n\n";

    out << "/* ---------- Runtime I/O helpers ---------- */\n";
    out << "static int pas_read_int(void) { int v = 0; (void)scanf(\"%d\", &v); return v; }\n";
    out << "static void pas_write_int(int v) { (void)printf(\"%d\", v); }\n";
    out << "static void pas_writeln(void) { (void)printf(\"\\n\"); }\n\n";

    out << "/* ---------- Global declarations ---------- */\n";
    if (decls.empty()) {
        out << "/* TODO(T-026): emit translated global const/type/var declarations here. */\n";
    } else {
        for (const auto& decl : decls) {
            out << decl.cType << " " << decl.name << " = 0;\n";
        }
    }
    out << "\n";

    out << "/* ---------- Routine declarations ---------- */\n";
    emitRoutineDefinitions(out, routineParse.routines);

    out << "int main(void) {\n";
    out << "    /* ---------- Main translated statements ---------- */\n";
    out << "    /* Core mapping targets:\n";
    out << "       - assignment: x = expr;\n";
    out << "       - if/else: if (cond) { ... } else { ... }\n";
    out << "       - while: while (cond) { ... }\n";
    out << "       - for: for (i = begin; i <= end; ++i) { ... }\n";
    out << "       - read/readln: x = pas_read_int();\n";
    out << "       - write/writeln: pas_write_int(expr); pas_writeln();\n";
    out << "    */\n";

    if (statements.empty()) {
        out << "    /* TODO(T-026/T-027): emit translated statements. */\n";
    } else {
        for (const auto& stmt : statements) {
            out << stmt << "\n";
        }
    }
    out << "\n";
    out << "    return 0;\n";
    out << "}\n";

    result.ok = true;
    result.message = "Template generated.";
    result.cSource = out.str();
    return result;
}

std::string CodeGenerator::sanitizeIdentifier(const std::string& rawName) {
    std::string out;
    out.reserve(rawName.size() + 4);

    for (char c : rawName) {
        if (std::isalnum(static_cast<unsigned char>(c)) || c == '_') {
            out.push_back(c);
        } else {
            out.push_back('_');
        }
    }

    if (out.empty()) {
        return out;
    }
    if (std::isdigit(static_cast<unsigned char>(out.front()))) {
        out.insert(out.begin(), '_');
    }
    return out;
}
