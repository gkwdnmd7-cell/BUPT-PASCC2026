#include "parser.h"

#include <unordered_set>

namespace {
class ParserImpl {
public:
    explicit ParserImpl(const std::vector<Token>& tokens) : tokens_(tokens) {}

    ParserResult run() {
        parseProgram();
        return result_;
    }

private:
    const std::vector<Token>& tokens_;
    size_t pos_ = 0;
    ParserResult result_;

    const Token& current() const {
        if (tokens_.empty()) {
            static const Token kFallback{TokenType::EndOfFile, "<EOF>", {1, 1}};
            return kFallback;
        }
        if (pos_ < tokens_.size()) {
            return tokens_[pos_];
        }
        return tokens_.back();
    }

    bool isAtEnd() const {
        return current().type == TokenType::EndOfFile;
    }

    void advance() {
        if (pos_ < tokens_.size()) {
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

    void addError(const std::string& code, const std::string& message) {
        result_.errors.push_back(ParseError{code, message, current().pos});
    }

    void synchronize(const std::unordered_set<TokenType>& syncSet) {
        while (!isAtEnd() && syncSet.find(current().type) == syncSet.end()) {
            advance();
        }
    }

    bool expect(TokenType type, const std::string& message, const std::unordered_set<TokenType>& syncSet) {
        if (match(type)) {
            return true;
        }
        addError("E201", message);
        synchronize(syncSet);
        return false;
    }

    static bool isStatementStart(TokenType type) {
        return type == TokenType::Identifier || type == TokenType::KwBegin || type == TokenType::KwRead || type == TokenType::KwWrite;
    }

    void parseProgram() {
        expect(TokenType::KwProgram, "Expected 'program' at program start.", {TokenType::Identifier, TokenType::Semicolon, TokenType::KwBegin, TokenType::Dot});
        expect(TokenType::Identifier, "Expected program identifier after 'program'.", {TokenType::Semicolon, TokenType::KwBegin, TokenType::Dot});
        expect(TokenType::Semicolon, "Expected ';' after program header.", {TokenType::KwBegin, TokenType::Dot});
        parseBlock();
        expect(TokenType::Dot, "Expected '.' at end of program.", {TokenType::EndOfFile});
        if (!isAtEnd()) {
            addError("E202", "Unexpected tokens after program end.");
        }
    }

    void parseBlock() {
        if (!expect(TokenType::KwBegin, "Expected 'begin' to start block.", {TokenType::Semicolon, TokenType::KwEnd, TokenType::Dot})) {
            return;
        }
        parseStatementList();
        expect(TokenType::KwEnd, "Expected 'end' to close block.", {TokenType::Dot, TokenType::Semicolon});
    }

    void parseStatementList() {
        while (!isAtEnd() && current().type != TokenType::KwEnd && current().type != TokenType::Dot) {
            parseStatement();

            if (match(TokenType::Semicolon)) {
                continue;
            }

            if (isStatementStart(current().type)) {
                addError("E203", "Expected ';' between statements.");
                continue;
            }

            if (current().type != TokenType::KwEnd && current().type != TokenType::Dot) {
                addError("E204", "Unexpected token in statement list.");
                synchronize({TokenType::Semicolon, TokenType::KwEnd, TokenType::Dot});
                match(TokenType::Semicolon);
            }
        }
    }

    void parseStatement() {
        switch (current().type) {
            case TokenType::Identifier:
                parseAssignment();
                break;
            case TokenType::KwRead:
                parseReadCall();
                break;
            case TokenType::KwWrite:
                parseWriteCall();
                break;
            case TokenType::KwBegin:
                parseBlock();
                break;
            default:
                addError("E205", "Invalid statement start.");
                advance();
                break;
        }
    }

    void parseAssignment() {
        expect(TokenType::Identifier, "Expected identifier at assignment start.", {TokenType::Assign, TokenType::Semicolon, TokenType::KwEnd});
        expect(TokenType::Assign, "Expected ':=' in assignment.", {TokenType::Semicolon, TokenType::KwEnd, TokenType::Dot});
        parseExpression();
    }

    void parseReadCall() {
        expect(TokenType::KwRead, "Expected 'read'.", {TokenType::LParen, TokenType::Semicolon, TokenType::KwEnd});
        expect(TokenType::LParen, "Expected '(' after 'read'.", {TokenType::Identifier, TokenType::RParen, TokenType::Semicolon});
        expect(TokenType::Identifier, "Expected identifier argument in 'read'.", {TokenType::RParen, TokenType::Semicolon});
        expect(TokenType::RParen, "Expected ')' after 'read' argument.", {TokenType::Semicolon, TokenType::KwEnd});
    }

    void parseWriteCall() {
        expect(TokenType::KwWrite, "Expected 'write'.", {TokenType::LParen, TokenType::Semicolon, TokenType::KwEnd});
        expect(TokenType::LParen, "Expected '(' after 'write'.", {TokenType::Identifier, TokenType::IntegerLiteral, TokenType::RealLiteral, TokenType::CharLiteral, TokenType::BooleanLiteral, TokenType::RParen, TokenType::Semicolon});
        parseExpression();
        expect(TokenType::RParen, "Expected ')' after 'write' argument.", {TokenType::Semicolon, TokenType::KwEnd});
    }

    void parseExpression() {
        parseTerm();
        while (current().type == TokenType::Plus || current().type == TokenType::Minus || current().type == TokenType::Or) {
            advance();
            parseTerm();
        }
    }

    void parseTerm() {
        parseFactor();
        while (current().type == TokenType::Multiply || current().type == TokenType::Divide || current().type == TokenType::Div ||
               current().type == TokenType::Mod || current().type == TokenType::And) {
            advance();
            parseFactor();
        }
    }

    void parseFactor() {
        switch (current().type) {
            case TokenType::Identifier:
            case TokenType::IntegerLiteral:
            case TokenType::RealLiteral:
            case TokenType::CharLiteral:
            case TokenType::BooleanLiteral:
                advance();
                return;
            case TokenType::LParen:
                advance();
                parseExpression();
                expect(TokenType::RParen, "Expected ')' after expression.", {TokenType::Semicolon, TokenType::KwEnd, TokenType::Dot});
                return;
            case TokenType::Not:
                advance();
                parseFactor();
                return;
            default:
                addError("E206", "Expected expression factor.");
                advance();
                return;
        }
    }
};
}  // namespace

ParserResult Parser::parse(const std::vector<Token>& tokens) {
    return ParserImpl(tokens).run();
}
