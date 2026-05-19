#include "lexer.h"

#include "parser_bison.tab.h"
typedef struct yy_buffer_state* YY_BUFFER_STATE;
YY_BUFFER_STATE pascclex_scan_bytes(const char* bytes, int len);
void pascclex_delete_buffer(YY_BUFFER_STATE buffer);
int pascclexlex(void);

void pasccLexerResetState(std::vector<LexError>* errors);
const char* pasccLexerLastLexeme();
int pasccLexerLastLine();
int pasccLexerLastColumn();
int pasccLexerCurrentLine();
int pasccLexerCurrentColumn();

namespace {
TokenType mapBisonTokenToInternal(int token) {
    switch (token) {
        case T_UNKNOWN: return TokenType::Unknown;
        case T_IDENTIFIER: return TokenType::Identifier;
        case T_INTEGER: return TokenType::IntegerLiteral;
        case T_REAL: return TokenType::RealLiteral;
        case T_CHAR: return TokenType::CharLiteral;
        case T_STRING: return TokenType::StringLiteral;
        case T_BOOLEAN: return TokenType::BooleanLiteral;
        case T_PROGRAM: return TokenType::KwProgram;
        case T_CONST: return TokenType::KwConst;
        case T_TYPE: return TokenType::KwType;
        case T_VAR: return TokenType::KwVar;
        case T_PROCEDURE: return TokenType::KwProcedure;
        case T_FUNCTION: return TokenType::KwFunction;
        case T_BEGIN: return TokenType::KwBegin;
        case T_END: return TokenType::KwEnd;
        case T_IF: return TokenType::KwIf;
        case T_THEN: return TokenType::KwThen;
        case T_ELSE: return TokenType::KwElse;
        case T_CASE: return TokenType::KwCase;
        case T_WHILE: return TokenType::KwWhile;
        case T_REPEAT: return TokenType::KwRepeat;
        case T_UNTIL: return TokenType::KwUntil;
        case T_FOR: return TokenType::KwFor;
        case T_TO: return TokenType::KwTo;
        case T_DOWNTO: return TokenType::KwDownTo;
        case T_DO: return TokenType::KwDo;
        case T_READ: return TokenType::KwRead;
        case T_READLN: return TokenType::KwReadLn;
        case T_WRITE: return TokenType::KwWrite;
        case T_WRITELN: return TokenType::KwWriteLn;
        case T_RECORD: return TokenType::KwRecord;
        case T_ARRAY: return TokenType::KwArray;
        case T_OF: return TokenType::KwOf;
        case T_TYPE_INTEGER: return TokenType::KwInteger;
        case T_TYPE_REAL: return TokenType::KwReal;
        case T_TYPE_BOOLEAN: return TokenType::KwBoolean;
        case T_TYPE_CHAR: return TokenType::KwChar;
        case T_PLUS: return TokenType::Plus;
        case T_MINUS: return TokenType::Minus;
        case T_MULTIPLY: return TokenType::Multiply;
        case T_DIVIDE: return TokenType::Divide;
        case T_DIV: return TokenType::Div;
        case T_MOD: return TokenType::Mod;
        case T_AND: return TokenType::And;
        case T_OR: return TokenType::Or;
        case T_NOT: return TokenType::Not;
        case T_ASSIGN: return TokenType::Assign;
        case T_EQUAL: return TokenType::Equal;
        case T_NOT_EQUAL: return TokenType::NotEqual;
        case T_LESS: return TokenType::Less;
        case T_LESS_EQUAL: return TokenType::LessEqual;
        case T_GREATER: return TokenType::Greater;
        case T_GREATER_EQUAL: return TokenType::GreaterEqual;
        case T_LPAREN: return TokenType::LParen;
        case T_RPAREN: return TokenType::RParen;
        case T_LBRACKET: return TokenType::LBracket;
        case T_RBRACKET: return TokenType::RBracket;
        case T_COMMA: return TokenType::Comma;
        case T_SEMICOLON: return TokenType::Semicolon;
        case T_COLON: return TokenType::Colon;
        case T_DOT: return TokenType::Dot;
        case T_DOTDOT: return TokenType::DotDot;
        case T_BREAK: return TokenType::KwBreak;
        case T_CONTINUE: return TokenType::KwContinue;
        case T_EXIT: return TokenType::KwExit;
        default: return TokenType::Unknown;
    }
}
}  // namespace

std::vector<Token> Lexer::tokenize(const std::string& source) {
    return tokenizeDetailed(source).tokens;
}

LexerResult Lexer::tokenizeDetailed(const std::string& source) {
    LexerResult result;
    pasccLexerResetState(&result.errors);

    YY_BUFFER_STATE buffer = pascclex_scan_bytes(source.c_str(), static_cast<int>(source.size()));
    for (;;) {
        const int token = pascclexlex();
        if (token == 0) {
            break;
        }

        result.tokens.push_back(Token{
            mapBisonTokenToInternal(token),
            pasccLexerLastLexeme(),
            SourcePosition{pasccLexerLastLine(), pasccLexerLastColumn()}
        });
    }
    pascclex_delete_buffer(buffer);

    result.tokens.push_back(Token{
        TokenType::EndOfFile,
        "<EOF>",
        SourcePosition{pasccLexerCurrentLine(), pasccLexerCurrentColumn()}
    });
    return result;
}
