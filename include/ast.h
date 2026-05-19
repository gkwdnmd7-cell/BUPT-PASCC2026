#ifndef AST_H
#define AST_H

#include <memory>
#include <string>
#include <vector>

// ---------------------------------------------------------------------------
// AstKind — discriminator for all AST node types
// ---------------------------------------------------------------------------
enum class AstKind {
    // Top-level
    Program, Block,
    // Declarations
    ConstDecl, TypeDecl, VarDecl, Routine, ParamGroup,
    // Types
    BasicType, ArrayType, RecordType, NamedType, Period,
    // Internal / parser helpers
    IdentTail,
    // Statements
    CompoundStmt, AssignStmt, CallStmt,
    IfStmt, CaseStmt, CaseBranch,
    WhileStmt, RepeatStmt, ForStmt,
    ReadStmt, WriteStmt,
    BreakStmt, ContinueStmt, ExitStmt,
    // Expressions
    BinaryExpr, UnaryExpr, LiteralExpr, VarExpr, CallExpr,
    VarPart,
};

// ---------------------------------------------------------------------------
// Base
// ---------------------------------------------------------------------------
struct AstNode {
    AstKind kind;
    explicit AstNode(AstKind k) : kind(k) {}
    virtual ~AstNode() = default;
};

// ---------------------------------------------------------------------------
// Type nodes
// ---------------------------------------------------------------------------
struct TypeNode : AstNode {
    explicit TypeNode(AstKind k) : AstNode(k) {}
};

struct BasicTypeNode : TypeNode {
    std::string name; // "integer", "real", "boolean", "char"
    explicit BasicTypeNode(std::string n)
        : TypeNode(AstKind::BasicType), name(std::move(n)) {}
};

struct PeriodNode : AstNode {
    std::string low;  // C representation of lower bound (e.g. "1" or "LOW")
    std::string high; // C representation of upper bound (e.g. "100" or "MAXN")
    PeriodNode(std::string lo, std::string hi)
        : AstNode(AstKind::Period), low(std::move(lo)), high(std::move(hi)) {}
};

struct ArrayTypeNode : TypeNode {
    std::vector<PeriodNode*> periods;
    TypeNode* elemType = nullptr;
    ArrayTypeNode() : TypeNode(AstKind::ArrayType) {}
};

struct RecordTypeNode : TypeNode {
    std::vector<AstNode*> fields; // VarDeclNode*
    RecordTypeNode() : TypeNode(AstKind::RecordType) {}
};

struct NamedTypeNode : TypeNode {
    std::string name;
    explicit NamedTypeNode(std::string n)
        : TypeNode(AstKind::NamedType), name(std::move(n)) {}
};

// ---------------------------------------------------------------------------
// Expression nodes
// ---------------------------------------------------------------------------
struct ExprNode : AstNode {
    explicit ExprNode(AstKind k) : AstNode(k) {}
};

struct BinaryExprNode : ExprNode {
    std::string op; // "+", "-", "*", "/", "div", "mod", "and", "or",
                    // "=", "<>", "<", "<=", ">", ">="
    ExprNode* left  = nullptr;
    ExprNode* right = nullptr;
    BinaryExprNode(std::string o, ExprNode* l, ExprNode* r)
        : ExprNode(AstKind::BinaryExpr), op(std::move(o)), left(l), right(r) {}
};

struct UnaryExprNode : ExprNode {
    std::string op; // "-", "+", "not"
    ExprNode* operand = nullptr;
    UnaryExprNode(std::string o, ExprNode* x)
        : ExprNode(AstKind::UnaryExpr), op(std::move(o)), operand(x) {}
};

enum class LiteralKind { Int, Real, Char, Str, Bool };

struct LiteralNode : ExprNode {
    LiteralKind litKind;
    std::string raw; // original token text (Char includes single-quotes; Str has no quotes)
    LiteralNode(LiteralKind k, std::string r)
        : ExprNode(AstKind::LiteralExpr), litKind(k), raw(std::move(r)) {}
};

struct VarPartNode : AstNode {
    bool isField = false;
    std::string fieldName;           // if isField
    std::vector<ExprNode*> indices;  // if !isField (bracket subscript)
    VarPartNode() : AstNode(AstKind::VarPart) {}
};

struct VarExprNode : ExprNode {
    std::string name;
    std::vector<VarPartNode*> parts;
    VarExprNode(std::string n, std::vector<VarPartNode*> p)
        : ExprNode(AstKind::VarExpr), name(std::move(n)), parts(std::move(p)) {}
};

struct CallExprNode : ExprNode {
    std::string name;
    std::vector<ExprNode*> args;
    CallExprNode(std::string n, std::vector<ExprNode*> a)
        : ExprNode(AstKind::CallExpr), name(std::move(n)), args(std::move(a)) {}
};

// ---------------------------------------------------------------------------
// Statement nodes
// ---------------------------------------------------------------------------
struct StmtNode : AstNode {
    explicit StmtNode(AstKind k) : AstNode(k) {}
};

struct CompoundStmtNode : StmtNode {
    std::vector<StmtNode*> stmts;
    CompoundStmtNode() : StmtNode(AstKind::CompoundStmt) {}
};

struct AssignStmtNode : StmtNode {
    std::string varName;
    std::vector<VarPartNode*> varParts;
    ExprNode* rhs = nullptr;
    AssignStmtNode() : StmtNode(AstKind::AssignStmt) {}
};

struct CallStmtNode : StmtNode {
    std::string name;
    std::vector<ExprNode*> args;
    CallStmtNode(std::string n, std::vector<ExprNode*> a)
        : StmtNode(AstKind::CallStmt), name(std::move(n)), args(std::move(a)) {}
};

struct IfStmtNode : StmtNode {
    ExprNode* cond  = nullptr;
    StmtNode* then_ = nullptr;
    StmtNode* else_ = nullptr; // null if no else clause
    IfStmtNode(ExprNode* c, StmtNode* t, StmtNode* e)
        : StmtNode(AstKind::IfStmt), cond(c), then_(t), else_(e) {}
};

struct CaseBranchNode : AstNode {
    std::vector<std::string> values; // C-form const_value strings for each label
    StmtNode* body = nullptr;
    CaseBranchNode() : AstNode(AstKind::CaseBranch) {}
};

struct CaseStmtNode : StmtNode {
    ExprNode* expr = nullptr;
    std::vector<CaseBranchNode*> branches;
    CaseStmtNode() : StmtNode(AstKind::CaseStmt) {}
};

struct WhileStmtNode : StmtNode {
    ExprNode* cond = nullptr;
    StmtNode* body = nullptr;
    WhileStmtNode(ExprNode* c, StmtNode* b)
        : StmtNode(AstKind::WhileStmt), cond(c), body(b) {}
};

struct RepeatStmtNode : StmtNode {
    CompoundStmtNode* body = nullptr;
    ExprNode*         cond = nullptr;
    RepeatStmtNode(CompoundStmtNode* b, ExprNode* c)
        : StmtNode(AstKind::RepeatStmt), body(b), cond(c) {}
};

struct ForStmtNode : StmtNode {
    std::string var;
    ExprNode*   from_ = nullptr;
    bool        isTo  = true; // true = TO, false = DOWNTO
    ExprNode*   to_   = nullptr;
    StmtNode*   body  = nullptr;
    ForStmtNode() : StmtNode(AstKind::ForStmt) {}
};

struct ReadStmtNode : StmtNode {
    std::vector<VarExprNode*> vars;
    bool withLn = false;
    ReadStmtNode() : StmtNode(AstKind::ReadStmt) {}
};

struct WriteStmtNode : StmtNode {
    std::vector<ExprNode*> exprs;
    bool withLn = false;
    WriteStmtNode() : StmtNode(AstKind::WriteStmt) {}
};

struct BreakStmtNode    : StmtNode { BreakStmtNode()    : StmtNode(AstKind::BreakStmt) {} };
struct ContinueStmtNode : StmtNode { ContinueStmtNode() : StmtNode(AstKind::ContinueStmt) {} };

struct ExitStmtNode : StmtNode {
    ExprNode* value = nullptr; // null if plain "exit"
    ExitStmtNode() : StmtNode(AstKind::ExitStmt) {}
};

// Internal helper node for identifier_stmt_tail grammar reduction
struct IdentTailNode : AstNode {
    enum class Kind { Assign, Call, BareCall } tailKind = Kind::BareCall;
    std::vector<VarPartNode*> varParts; // for Assign: the lhs subscripts/fields
    ExprNode*                 rhs  = nullptr; // for Assign
    std::vector<ExprNode*>    args;           // for Call
    IdentTailNode() : AstNode(AstKind::IdentTail) {}
};

// ---------------------------------------------------------------------------
// Declaration nodes
// ---------------------------------------------------------------------------
struct ParamGroupNode : AstNode {
    bool byRef = false;
    std::vector<std::string> names;
    std::string typeName; // "integer", "real", "boolean", "char"
    ParamGroupNode() : AstNode(AstKind::ParamGroup) {}
};

struct ConstDeclNode : AstNode {
    std::string name;
    std::string cType;  // "int", "float", "char", "string"
    std::string cValue; // C-side value, e.g. "42", "3.14f", "'x'", "\"hello\""
    ConstDeclNode() : AstNode(AstKind::ConstDecl) {}
};

struct VarDeclNode : AstNode {
    std::vector<std::string> names;
    TypeNode* typeNode = nullptr;
    VarDeclNode() : AstNode(AstKind::VarDecl) {}
};

struct TypeDeclNode : AstNode {
    std::string name;
    TypeNode*   typeNode = nullptr;
    TypeDeclNode() : AstNode(AstKind::TypeDecl) {}
};

struct BlockNode : AstNode {
    std::vector<ConstDeclNode*> consts;
    std::vector<TypeDeclNode*>  types;
    std::vector<VarDeclNode*>   vars;
    std::vector<AstNode*>       routines; // RoutineNode*
    CompoundStmtNode*           compound  = nullptr;
    BlockNode() : AstNode(AstKind::Block) {}
};

struct RoutineNode : AstNode {
    bool isFunction = false;
    std::string name;
    std::vector<ParamGroupNode*> params;
    std::string returnTypeName; // for functions: "integer", "real", "boolean", "char"
    BlockNode*  body = nullptr;
    RoutineNode() : AstNode(AstKind::Routine) {}
};

struct ProgramNode : AstNode {
    std::string              name;
    std::vector<std::string> params; // optional file-parameter list after program name
    BlockNode*               body   = nullptr;
    ProgramNode() : AstNode(AstKind::Program) {}
};

#endif // AST_H
