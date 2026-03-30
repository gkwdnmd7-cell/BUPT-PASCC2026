#include "lexer.h"

#include <cctype>
#include <string>
#include <unordered_map>

namespace {
bool isIdentifierStart(char ch) {
    return std::isalpha(static_cast<unsigned char>(ch)) || ch == '_';
}

bool isIdentifierPart(char ch) {
    return std::isalnum(static_cast<unsigned char>(ch)) || ch == '_';
}

bool isBoundaryChar(char ch) {
    return std::isspace(static_cast<unsigned char>(ch));
}

std::string toLowerCopy(const std::string& s) {
    std::string out;
    out.reserve(s.size());
    for (char ch : s) {
        out.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(ch))));
    }
    return out;
}

bool isKeywordLikeToken(TokenType type) {
    switch (type) {
        case TokenType::KwProgram:
        case TokenType::KwConst:
        case TokenType::KwVar:
        case TokenType::KwProcedure:
        case TokenType::KwFunction:
        case TokenType::KwBegin:
        case TokenType::KwEnd:
        case TokenType::KwIf:
        case TokenType::KwThen:
        case TokenType::KwElse:
        case TokenType::KwFor:
        case TokenType::KwTo:
        case TokenType::KwDo:
        case TokenType::KwRead:
        case TokenType::KwWrite:
        case TokenType::KwArray:
        case TokenType::KwOf:
        case TokenType::KwInteger:
        case TokenType::KwReal:
        case TokenType::KwBoolean:
        case TokenType::KwChar:
        case TokenType::BooleanLiteral:
        case TokenType::Div:
        case TokenType::Mod:
        case TokenType::And:
        case TokenType::Or:
        case TokenType::Not:
            return true;
        default:
            return false;
    }
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
        {"div", TokenType::Div},
        {"mod", TokenType::Mod},
        {"and", TokenType::And},
        {"or", TokenType::Or},
        {"not", TokenType::Not},
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
    return tokenizeDetailed(source).tokens;
}

LexerResult Lexer::tokenizeDetailed(const std::string& source) {
    LexerResult result;
    int line = 1;
    int column = 1;

    auto addError = [&](const std::string& code, const std::string& message, int errorLine, int errorColumn) {
        result.errors.push_back(LexError{code, message, {errorLine, errorColumn}});
    };

    for (size_t i = 0; i < source.size();) {
        const char ch = source[i];

        if (ch == '\r') {
            ++i;
            continue;
        }

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

        if (ch == '{') {
            const int startLine = line;
            const int startCol = column;
            ++i;
            ++column;

            int depth = 1;
            while (i < source.size()) {
                if (source[i] == '\r') {
                    ++i;
                    continue;
                }
                if (source[i] == '\n') {
                    ++line;
                    column = 1;
                    ++i;
                    continue;
                }
                if (source[i] == '{') {
                    addError("E106", "Nested comment is not allowed.", line, column);
                    ++depth;
                    ++i;
                    ++column;
                    continue;
                }
                if (source[i] == '}') {
                    --depth;
                    ++i;
                    ++column;
                    if (depth == 0) {
                        break;
                    }
                    continue;
                }
                ++i;
                ++column;
            }

            if (depth != 0) {
                addError("E107", "Unterminated comment.", startLine, startCol);
                break;
            }
            continue;
        }

        if (isIdentifierStart(ch)) {
            const int startCol = column;
            const size_t startIndex = i;
            std::string lexeme;
            while (i < source.size() && isIdentifierPart(source[i])) {
                lexeme.push_back(source[i]);
                ++i;
                ++column;
            }

            if (lexeme.size() > 10) {
                addError("E105", "Identifier length exceeds 10 characters: " + lexeme, line, startCol);
            }

            const std::string lowered = toLowerCopy(lexeme);
            const TokenType resolved = resolveIdentifierToken(lowered);

            if (isKeywordLikeToken(resolved)) {
                bool beforeOk = (startIndex == 0) || isBoundaryChar(source[startIndex - 1]);
                bool afterOk = (i >= source.size()) || isBoundaryChar(source[i]);

                if (resolved == TokenType::KwEnd && i < source.size() && source[i] == '.') {
                    afterOk = true;
                }

                if (!(beforeOk && afterOk)) {
                    addError("E108", "Keyword must be surrounded by whitespace/newline: " + lowered, line, startCol);
                }
            }

            Token token;
            token.type = resolved;
            token.lexeme = lowered;
            token.pos = {line, startCol};
            result.tokens.push_back(token);
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
            result.tokens.push_back(token);
            continue;
        }

        if (ch == '\'') {
            const int startCol = column;
            if (i + 2 < source.size() && source[i + 2] == '\'' && source[i + 1] != '\\' && source[i + 1] != '\'' && source[i + 1] != '\n' && source[i + 1] != '\r') {
                Token token;
                token.type = TokenType::CharLiteral;
                token.lexeme = source.substr(i, 3);
                token.pos = {line, startCol};
                result.tokens.push_back(token);
                i += 3;
                column += 3;
            } else {
                addError("E109", "Invalid char literal. Only single non-escaped char is supported.", line, startCol);
                ++i;
                ++column;
            }
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
            case ':':
                if (i + 1 < source.size() && source[i + 1] == '=') {
                    token.type = TokenType::Assign;
                    token.lexeme = ":=";
                    ++i;
                    ++column;
                } else {
                    token.type = TokenType::Colon;
                }
                break;
            case '.':
                if (i + 1 < source.size() && source[i + 1] == '.') {
                    token.type = TokenType::DotDot;
                    token.lexeme = "..";
                    ++i;
                    ++column;
                } else {
                    token.type = TokenType::Dot;
                }
                break;
            case '=': token.type = TokenType::Equal; break;
            case '<':
                if (i + 1 < source.size() && source[i + 1] == '=') {
                    token.type = TokenType::LessEqual;
                    token.lexeme = "<=";
                    ++i;
                    ++column;
                } else if (i + 1 < source.size() && source[i + 1] == '>') {
                    token.type = TokenType::NotEqual;
                    token.lexeme = "<>";
                    ++i;
                    ++column;
                } else {
                    token.type = TokenType::Less;
                }
                break;
            case '>':
                if (i + 1 < source.size() && source[i + 1] == '=') {
                    token.type = TokenType::GreaterEqual;
                    token.lexeme = ">=";
                    ++i;
                    ++column;
                } else {
                    token.type = TokenType::Greater;
                }
                break;
            default:
                token.type = TokenType::Unknown;
                addError("E110", "Unknown character: " + std::string(1, ch), line, column);
                break;
        }

        result.tokens.push_back(token);
        ++i;
        ++column;
    }

    Token eof;
    eof.type = TokenType::EndOfFile;
    eof.lexeme = "<EOF>";
    eof.pos = {line, column};
    result.tokens.push_back(eof);
    return result;
}
