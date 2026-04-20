#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum class TokenType {
    EndOfFile,
    Unknown,

    Identifier,
    IntegerLiteral,
    RealLiteral,
    CharLiteral,
    BooleanLiteral,

    KwProgram,
    KwConst,
    KwVar,
    KwProcedure,
    KwFunction,
    KwBegin,
    KwEnd,
    KwIf,
    KwThen,
    KwElse,
    KwFor,
    KwTo,
    KwDo,
    KwRead,
    KwWrite,
    KwArray,
    KwOf,
    KwInteger,
    KwReal,
    KwBoolean,
    KwChar,

    Plus,
    Minus,
    Multiply,
    Divide,
    Div,
    Mod,
    And,
    Or,
    Not,
    Assign,
    Equal,
    NotEqual,
    Less,
    LessEqual,
    Greater,
    GreaterEqual,

    LParen,
    RParen,
    LBracket,
    RBracket,
    Comma,
    Semicolon,
    Colon,
    Dot,
    DotDot,
};

struct SourcePosition {
    int line = 1;
    int column = 1;
};

struct Token {
    TokenType type = TokenType::Unknown;
    std::string lexeme;
    SourcePosition pos;
};

#endif
