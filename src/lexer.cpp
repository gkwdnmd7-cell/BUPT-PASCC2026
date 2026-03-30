#include "lexer.h"

#include <cctype>
#include <unordered_map>

namespace {
bool isIdentifierStart(char ch) {
    return std::isalpha(static_cast<unsigned char>(ch)) || ch == '_';
}

bool isIdentifierPart(char ch) {
    return std::isalnum(static_cast<unsigned char>(ch)) || ch == '_';
}

TokenType resolveIdentifierToken(const std::string& lexeme) {
    static const std::unordered_map<std::string, TokenType> kKeywordMap = {
        {"program", TokenType::KwProgram},
        {"const", TokenType::KwConst},
        {"var", TokenType::KwVar},
        {"procedure", TokenType::KwProcedure},
        {"function", TokenType::KwFunction},
        {"begin", TokenType::KwBegin},
        {"end", TokenType::KwEnd},
        {"if", TokenType::KwIf},
        {"then", TokenType::KwThen},
        {"else", TokenType::KwElse},
        {"for", TokenType::KwFor},
        {"to", TokenType::KwTo},
        {"do", TokenType::KwDo},
        {"read", TokenType::KwRead},
        {"write", TokenType::KwWrite},
        {"array", TokenType::KwArray},
        {"of", TokenType::KwOf},
        {"integer", TokenType::KwInteger},
        {"real", TokenType::KwReal},
        {"boolean", TokenType::KwBoolean},
        {"char", TokenType::KwChar},
        {"true", TokenType::BooleanLiteral},
        {"false", TokenType::BooleanLiteral},
    };

    auto it = kKeywordMap.find(lexeme);
    if (it != kKeywordMap.end()) {
        return it->second;
    }
    return TokenType::Identifier;
}
}  // namespace

std::vector<Token> Lexer::tokenize(const std::string& source) {
    std::vector<Token> tokens;
    int line = 1;
    int column = 1;

    for (size_t i = 0; i < source.size();) {
        const char ch = source[i];

        if (ch == '\n') {
            ++line;
            column = 1;
            ++i;
            continue;
        }

        if (std::isspace(static_cast<unsigned char>(ch))) {
            ++column;
            ++i;
            continue;
        }

        if (isIdentifierStart(ch)) {
            const int startCol = column;
            std::string lexeme;
            while (i < source.size() && isIdentifierPart(source[i])) {
                lexeme.push_back(source[i]);
                ++i;
                ++column;
            }

            Token token;
            token.type = resolveIdentifierToken(lexeme);
            token.lexeme = lexeme;
            token.pos = {line, startCol};
            tokens.push_back(token);
            continue;
        }

        if (std::isdigit(static_cast<unsigned char>(ch))) {
            const int startCol = column;
            std::string lexeme;
            bool isReal = false;
            while (i < source.size() && std::isdigit(static_cast<unsigned char>(source[i]))) {
                lexeme.push_back(source[i]);
                ++i;
                ++column;
            }

            if (i + 1 < source.size() && source[i] == '.' && std::isdigit(static_cast<unsigned char>(source[i + 1]))) {
                isReal = true;
                lexeme.push_back(source[i]);
                ++i;
                ++column;

                while (i < source.size() && std::isdigit(static_cast<unsigned char>(source[i]))) {
                    lexeme.push_back(source[i]);
                    ++i;
                    ++column;
                }
            }

            Token token;
            token.type = isReal ? TokenType::RealLiteral : TokenType::IntegerLiteral;
            token.lexeme = lexeme;
            token.pos = {line, startCol};
            tokens.push_back(token);
            continue;
        }

        Token token;
        token.lexeme = std::string(1, ch);
        token.pos = {line, column};

        switch (ch) {
            case '+': token.type = TokenType::Plus; break;
            case '-': token.type = TokenType::Minus; break;
            case '*': token.type = TokenType::Multiply; break;
            case '/': token.type = TokenType::Divide; break;
            case '(': token.type = TokenType::LParen; break;
            case ')': token.type = TokenType::RParen; break;
            case '[': token.type = TokenType::LBracket; break;
            case ']': token.type = TokenType::RBracket; break;
            case ',': token.type = TokenType::Comma; break;
            case ';': token.type = TokenType::Semicolon; break;
            case ':': token.type = TokenType::Colon; break;
            case '.': token.type = TokenType::Dot; break;
            case '=': token.type = TokenType::Equal; break;
            case '<': token.type = TokenType::Less; break;
            case '>': token.type = TokenType::Greater; break;
            default: token.type = TokenType::Unknown; break;
        }

        tokens.push_back(token);
        ++i;
        ++column;
    }

    Token eof;
    eof.type = TokenType::EndOfFile;
    eof.lexeme = "<EOF>";
    eof.pos = {line, column};
    tokens.push_back(eof);
    return tokens;
}
