#ifndef AST_H
#define AST_H

#include <memory>
#include <vector>

enum class AstKind {
    Program,
    Statement,
    Expression,
};

class AstNode {
public:
    explicit AstNode(AstKind kind) : kind_(kind) {}
    virtual ~AstNode() = default;

    AstKind kind() const { return kind_; }

    void addChild(std::shared_ptr<AstNode> child) {
        children_.push_back(std::move(child));
    }

    const std::vector<std::shared_ptr<AstNode>>& children() const {
        return children_;
    }

private:
    AstKind kind_;
    std::vector<std::shared_ptr<AstNode>> children_;
};

class ProgramNode : public AstNode {
public:
    ProgramNode() : AstNode(AstKind::Program) {}
};

class StatementNode : public AstNode {
public:
    StatementNode() : AstNode(AstKind::Statement) {}
};

class ExpressionNode : public AstNode {
public:
    ExpressionNode() : AstNode(AstKind::Expression) {}
};

#endif
