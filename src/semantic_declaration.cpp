#include "semantic_declaration.h"

#include <utility>

namespace {
class SemanticDeclarationAnalyzerImpl {
public:
    SemanticResult run(const std::vector<Token>& tokens, const ParserResult& parseResult) {
        tokens_ = &tokens;
        pos_ = 0;

        if (parseResult.root == nullptr) {
            result_.errors.push_back(SemanticError{"S199", "Syntax phase did not produce AST root.", {1, 1}});
            return result_;
        }

        TableSet* global = scopes_.push("global");
        builtins_.installInto(global);

        parseProgram();
        return result_;
    }

private:
    const std::vector<Token>* tokens_ = nullptr;
    std::size_t pos_ = 0;
    SemanticResult result_;
    ScopeStack scopes_;
    BuiltinTypePool builtins_;

    const Token& current() const {
        if (tokens_ == nullptr || tokens_->empty()) {
            static const Token kFallback{TokenType::EndOfFile, "<EOF>", {1, 1}};
            return kFallback;
        }
        if (pos_ < tokens_->size()) {
            return (*tokens_)[pos_];
        }
        return tokens_->back();
    }

    bool isAtEnd() const {
        return current().type == TokenType::EndOfFile;
    }

    void advance() {
        if (!isAtEnd()) {
            ++pos_;
        }
    }

    bool match(TokenType type) {
        if (current().type == type) {
            advance();
            return true;
        }
        return false;
    }

    void parseProgram() {
        if (!match(TokenType::KwProgram)) {
            return;
        }

        // Skip program header until ';'.
        while (!isAtEnd() && current().type != TokenType::Semicolon) {
            advance();
        }
        match(TokenType::Semicolon);

        parseBlock();
    }

    void parseBlock() {
        skipConstDeclarations();
        skipTypeDeclarations();
        parseVarDeclarationsInCurrentScope();

        while (current().type == TokenType::KwProcedure || current().type == TokenType::KwFunction) {
            parseSubprogram();
        }

        skipCompoundStatement();
    }

    void skipConstDeclarations() {
        if (!match(TokenType::KwConst)) {
            return;
        }
        while (!isAtEnd()) {
            if (current().type == TokenType::KwType || current().type == TokenType::KwVar || current().type == TokenType::KwProcedure ||
                current().type == TokenType::KwFunction || current().type == TokenType::KwBegin) {
                return;
            }
            advance();
        }
    }

    void skipTypeDeclarations() {
        if (!match(TokenType::KwType)) {
            return;
        }
        while (!isAtEnd()) {
            if (current().type == TokenType::KwVar || current().type == TokenType::KwProcedure || current().type == TokenType::KwFunction ||
                current().type == TokenType::KwBegin) {
                return;
            }
            advance();
        }
    }

    void parseVarDeclarationsInCurrentScope() {
        if (!match(TokenType::KwVar)) {
            return;
        }

        while (!isAtEnd()) {
            if (current().type == TokenType::KwBegin || current().type == TokenType::KwProcedure || current().type == TokenType::KwFunction) {
                return;
            }

            std::vector<Token> idTokens;
            if (current().type == TokenType::Identifier) {
                idTokens.push_back(current());
                advance();
                while (match(TokenType::Comma)) {
                    if (current().type == TokenType::Identifier) {
                        idTokens.push_back(current());
                        advance();
                    } else {
                        break;
                    }
                }
            }

            if (!match(TokenType::Colon)) {
                skipToSemicolonOrBlockBoundary();
                match(TokenType::Semicolon);
                continue;
            }

            std::string typeName;
            if (current().type == TokenType::KwInteger) {
                typeName = "integer";
            } else if (current().type == TokenType::KwReal) {
                typeName = "real";
            } else if (current().type == TokenType::KwBoolean) {
                typeName = "boolean";
            } else if (current().type == TokenType::KwChar) {
                typeName = "char";
            } else if (current().type == TokenType::Identifier) {
                typeName = current().lexeme;
            }

            if (!typeName.empty()) {
                for (const auto& idTok : idTokens) {
                    declareVariable(idTok, typeName);
                }
                advance();
            }

            match(TokenType::Semicolon);
        }
    }

    void parseSubprogram() {
        std::string scopeTag = "subprogram";

        // Read header and capture subprogram name.
        advance();  // consume procedure/function keyword
        if (current().type == TokenType::Identifier) {
            scopeTag = current().lexeme;
            advance();
        }

        while (!isAtEnd() && current().type != TokenType::Semicolon) {
            advance();
        }
        match(TokenType::Semicolon);

        scopes_.push(scopeTag);
        parseBlock();
        scopes_.pop();

        // Grammar has trailing ';' after subprogram body in declaration list.
        match(TokenType::Semicolon);
    }

    void skipCompoundStatement() {
        if (!match(TokenType::KwBegin)) {
            return;
        }

        int depth = 1;
        while (!isAtEnd() && depth > 0) {
            if (current().type == TokenType::KwBegin) {
                ++depth;
            } else if (current().type == TokenType::KwEnd) {
                --depth;
            }
            advance();
        }
    }

    void skipToSemicolonOrBlockBoundary() {
        while (!isAtEnd()) {
            if (current().type == TokenType::Semicolon || current().type == TokenType::KwBegin || current().type == TokenType::KwProcedure ||
                current().type == TokenType::KwFunction) {
                return;
            }
            advance();
        }
    }

    void declareVariable(const Token& idToken, const std::string& typeName) {
        const TableSet* currentScope = scopes_.current();
        if (currentScope == nullptr) {
            return;
        }

        bool localType = false;
        auto resolvedType = currentScope->searchEntry<TypeTemplate>(typeName, &localType);
        if (resolvedType == nullptr) {
            result_.errors.push_back(SemanticError{"S102", "Undefined type in declaration: " + typeName, idToken.pos});
            return;
        }

        auto variable = std::make_shared<VariableSymbol>(idToken.lexeme, resolvedType);
        auto mutableScope = const_cast<TableSet*>(currentScope);
        if (!mutableScope->insert(idToken.lexeme, std::move(variable))) {
            result_.errors.push_back(SemanticError{"S101", "Redefined identifier in same scope: " + idToken.lexeme, idToken.pos});
        }
    }
};
}  // namespace

SemanticResult SemanticDeclarationAnalyzer::analyze(const std::vector<Token>& tokens, const ParserResult& parseResult) {
    return SemanticDeclarationAnalyzerImpl().run(tokens, parseResult);
}
