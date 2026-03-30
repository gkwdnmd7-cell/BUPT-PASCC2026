#ifndef TOKEN_H
#define TOKEN_H

#include <string>

// Core token set for Pascal-S lexical analysis.
enum class TokenType {
    // Special
    EndOfFile,
    Unknown,

    // Identifiers and literals
    Identifier,
    IntegerLiteral,
    RealLiteral,
    CharLiteral,
    BooleanLiteral,

    // Keywords
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

    // Operators
    Plus,
    Minus,
    Multiply,
    Divide,
    Assign,
    Equal,
    NotEqual,
    Less,
    LessEqual,
    Greater,
    GreaterEqual,

    // Delimiters and punctuation
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
