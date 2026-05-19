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
    StringLiteral,
    BooleanLiteral,

    KwProgram,
    KwConst,
    KwType,
    KwVar,
    KwProcedure,
    KwFunction,
    KwBegin,
    KwEnd,
    KwIf,
    KwThen,
    KwElse,
    KwCase,
    KwWhile,
    KwRepeat,
    KwUntil,
    KwFor,
    KwTo,
    KwDownTo,
    KwDo,
    KwRead,
    KwReadLn,
    KwWrite,
    KwWriteLn,
    KwRecord,
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
    KwBreak,
    KwContinue,
    KwExit,
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
