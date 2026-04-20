#include "lexer.h"

#include <cctype>
#include <string>
#include <unordered_map>

namespace {
bool isLetter(char ch) {
    return std::isalpha(static_cast<unsigned char>(ch)) != 0;
}

bool isDigit(char ch) {
    return std::isdigit(static_cast<unsigned char>(ch)) != 0;
}

bool isValidCharLiteralContent(char ch) {
    return ch != '\\' && ch != '\'' && ch != '\n' && ch != '\r';
}

std::string toLowerCopy(const std::string& s) {
    std::string out;
    out.reserve(s.size());
    for (char ch : s) {
        out.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(ch))));
    }
    return out;
}

TokenType resolveWordToken(const std::string& loweredWord) {
    static const std::unordered_map<std::string, TokenType> kWordMap = {
        {"program", TokenType::KwProgram},
        {"const", TokenType::KwConst},
        {"type", TokenType::KwType},
        {"var", TokenType::KwVar},
        {"procedure", TokenType::KwProcedure},
        {"function", TokenType::KwFunction},
        {"begin", TokenType::KwBegin},
        {"end", TokenType::KwEnd},
        {"if", TokenType::KwIf},
        {"then", TokenType::KwThen},
        {"else", TokenType::KwElse},
        {"case", TokenType::KwCase},
        {"while", TokenType::KwWhile},
        {"repeat", TokenType::KwRepeat},
        {"until", TokenType::KwUntil},
        {"for", TokenType::KwFor},
        {"to", TokenType::KwTo},
        {"downto", TokenType::KwDownTo},
        {"do", TokenType::KwDo},
        {"read", TokenType::KwRead},
        {"readln", TokenType::KwReadLn},
        {"write", TokenType::KwWrite},
        {"writeln", TokenType::KwWriteLn},
        {"record", TokenType::KwRecord},
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

    auto it = kWordMap.find(loweredWord);
    if (it != kWordMap.end()) {
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

    auto addError = [&](const std::string& code, const std::string& message, int errLine, int errColumn) {
        result.errors.push_back(LexError{code, message, {errLine, errColumn}});
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

            bool closed = false;
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
                }
                if (source[i] == '}') {
                    ++i;
                    ++column;
                    closed = true;
                    break;
                }
                ++i;
                ++column;
            }

            if (!closed) {
                addError("E107", "Unterminated comment.", startLine, startCol);
            }
            continue;
        }

        if (isLetter(ch)) {
            const int startCol = column;
            std::string word;
            while (i < source.size() && (isLetter(source[i]) || isDigit(source[i]))) {
                word.push_back(source[i]);
                ++i;
                ++column;
            }

            const std::string lowered = toLowerCopy(word);
            const TokenType tokenType = resolveWordToken(lowered);

            if (tokenType == TokenType::Identifier && word.size() > 8) {
                addError("E105", "Identifier length exceeds 8 characters: " + word, line, startCol);
            }

            result.tokens.push_back(Token{tokenType, lowered, {line, startCol}});
            continue;
        }

        if (isDigit(ch)) {
            const int startCol = column;
            std::string number;
            while (i < source.size() && isDigit(source[i])) {
                number.push_back(source[i]);
                ++i;
                ++column;
            }

            bool isReal = false;
            if (i + 1 < source.size() && source[i] == '.' && isDigit(source[i + 1])) {
                isReal = true;
                number.push_back(source[i]);
                ++i;
                ++column;
                while (i < source.size() && isDigit(source[i])) {
                    number.push_back(source[i]);
                    ++i;
                    ++column;
                }
            }

            result.tokens.push_back(Token{isReal ? TokenType::RealLiteral : TokenType::IntegerLiteral, number, {line, startCol}});
            continue;
        }

        if (ch == '\'') {
            const int startCol = column;
            if (i + 2 < source.size() && source[i + 2] == '\'' && isValidCharLiteralContent(source[i + 1])) {
                result.tokens.push_back(Token{TokenType::CharLiteral, source.substr(i, 3), {line, startCol}});
                i += 3;
                column += 3;
                continue;
            }

            addError("E109", "Invalid char literal. Only single non-escaped char is supported.", line, startCol);
            ++i;
            ++column;
            continue;
        }

        {
            const int startCol = column;
            TokenType type = TokenType::Unknown;
            std::string lexeme(1, ch);
            bool matched = true;

            switch (ch) {
                case '+':
                    type = TokenType::Plus;
                    break;
                case '-':
                    type = TokenType::Minus;
                    break;
                case '*':
                    type = TokenType::Multiply;
                    break;
                case '/':
                    type = TokenType::Divide;
                    break;
                case '(':
                    type = TokenType::LParen;
                    break;
                case ')':
                    type = TokenType::RParen;
                    break;
                case '[':
                    type = TokenType::LBracket;
                    break;
                case ']':
                    type = TokenType::RBracket;
                    break;
                case ',':
                    type = TokenType::Comma;
                    break;
                case ';':
                    type = TokenType::Semicolon;
                    break;
                case ':':
                    if (i + 1 < source.size() && source[i + 1] == '=') {
                        type = TokenType::Assign;
                        lexeme = ":=";
                        ++i;
                        ++column;
                    } else {
                        type = TokenType::Colon;
                    }
                    break;
                case '.':
                    if (i + 1 < source.size() && source[i + 1] == '.') {
                        type = TokenType::DotDot;
                        lexeme = "..";
                        ++i;
                        ++column;
                    } else {
                        type = TokenType::Dot;
                    }
                    break;
                case '=':
                    type = TokenType::Equal;
                    break;
                case '<':
                    if (i + 1 < source.size() && source[i + 1] == '=') {
                        type = TokenType::LessEqual;
                        lexeme = "<=";
                        ++i;
                        ++column;
                    } else if (i + 1 < source.size() && source[i + 1] == '>') {
                        type = TokenType::NotEqual;
                        lexeme = "<>";
                        ++i;
                        ++column;
                    } else {
                        type = TokenType::Less;
                    }
                    break;
                case '>':
                    if (i + 1 < source.size() && source[i + 1] == '=') {
                        type = TokenType::GreaterEqual;
                        lexeme = ">=";
                        ++i;
                        ++column;
                    } else {
                        type = TokenType::Greater;
                    }
                    break;
                default:
                    matched = false;
                    break;
            }

            if (matched) {
                result.tokens.push_back(Token{type, lexeme, {line, startCol}});
                ++i;
                ++column;
                continue;
            }
        }

        addError("E110", "Unknown character: " + std::string(1, ch), line, column);
        result.tokens.push_back(Token{TokenType::Unknown, std::string(1, ch), {line, column}});
        ++i;
        ++column;
    }

    result.tokens.push_back(Token{TokenType::EndOfFile, "<EOF>", {line, column}});
    return result;
}
