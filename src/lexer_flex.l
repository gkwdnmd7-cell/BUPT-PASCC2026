%option noyywrap
%option nounput
%option noinput
%option prefix="pascclex"
%x COMMENT

%{
#include <cctype>
#include <string>
#include <unordered_map>
#include <vector>

#ifdef _WIN32
#include <io.h>
#define isatty _isatty
#define fileno _fileno
#endif

#include "lexer.h"
#include "parser_bison.tab.h"

static std::vector<LexError>* g_errors = nullptr;
static int g_line = 1;
static int g_col = 1;
static int g_comment_start_line = 1;
static int g_comment_start_col = 1;
static std::string g_last_lexeme;
static int g_last_line = 1;
static int g_last_col = 1;

static std::string toLower(const char* text, int len) {
    std::string out;
    out.reserve(static_cast<std::size_t>(len));
    for (int i = 0; i < len; ++i) {
        out.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(text[i]))));
    }
    return out;
}

static void addError(const char* code, const std::string& message, int line, int col) {
    if (g_errors == nullptr) {
        return;
    }
    g_errors->push_back(LexError{code, message, {line, col}});
}

static int keywordOrIdentifier(const std::string& lowered) {
    static const std::unordered_map<std::string, int> kKeywords = {
        {"program", T_PROGRAM},
        {"const", T_CONST},
        {"type", T_TYPE},
        {"var", T_VAR},
        {"procedure", T_PROCEDURE},
        {"function", T_FUNCTION},
        {"begin", T_BEGIN},
        {"end", T_END},
        {"if", T_IF},
        {"then", T_THEN},
        {"else", T_ELSE},
        {"case", T_CASE},
        {"while", T_WHILE},
        {"repeat", T_REPEAT},
        {"until", T_UNTIL},
        {"for", T_FOR},
        {"to", T_TO},
        {"downto", T_DOWNTO},
        {"do", T_DO},
        {"read", T_READ},
        {"readln", T_READLN},
        {"write", T_WRITE},
        {"writeln", T_WRITELN},
        {"record", T_RECORD},
        {"array", T_ARRAY},
        {"of", T_OF},
        {"integer", T_TYPE_INTEGER},
        {"real", T_TYPE_REAL},
        {"boolean", T_TYPE_BOOLEAN},
        {"char", T_TYPE_CHAR},
        {"div", T_DIV},
        {"mod", T_MOD},
        {"and", T_AND},
        {"or", T_OR},
        {"not", T_NOT},
        {"true", T_BOOLEAN},
        {"false", T_BOOLEAN},
    };
    auto it = kKeywords.find(lowered);
    if (it != kKeywords.end()) {
        return it->second;
    }
    return T_IDENTIFIER;
}

#define EMIT_TOKEN(code, lexeme_value)    \
    do {                                  \
        g_last_line = g_line;             \
        g_last_col = g_col;               \
        g_last_lexeme = (lexeme_value);   \
        g_col += yyleng;                  \
        return (code);                    \
    } while (0)

void pasccLexerResetState(std::vector<LexError>* errors) {
    g_errors = errors;
    g_line = 1;
    g_col = 1;
    g_comment_start_line = 1;
    g_comment_start_col = 1;
    g_last_line = 1;
    g_last_col = 1;
    g_last_lexeme.clear();
}

const char* pasccLexerLastLexeme() {
    return g_last_lexeme.c_str();
}

int pasccLexerLastLine() {
    return g_last_line;
}

int pasccLexerLastColumn() {
    return g_last_col;
}

int pasccLexerCurrentLine() {
    return g_line;
}

int pasccLexerCurrentColumn() {
    return g_col;
}
%}

%%
[ \t\r]+                   { g_col += yyleng; }
\n                         { ++g_line; g_col = 1; }

"{"                        { g_comment_start_line = g_line; g_comment_start_col = g_col; g_col += yyleng; BEGIN(COMMENT); }
<COMMENT>"{"               { addError("E106", "Nested comment is not allowed.", g_line, g_col); g_col += yyleng; }
<COMMENT>"}"               { g_col += yyleng; BEGIN(INITIAL); }
<COMMENT>\n                { ++g_line; g_col = 1; }
<COMMENT>.                 { g_col += yyleng; }
<COMMENT><<EOF>>           { addError("E107", "Unterminated comment.", g_comment_start_line, g_comment_start_col); BEGIN(INITIAL); return 0; }

":="                       { EMIT_TOKEN(T_ASSIGN, ":="); }
"<="                       { EMIT_TOKEN(T_LESS_EQUAL, "<="); }
">="                       { EMIT_TOKEN(T_GREATER_EQUAL, ">="); }
"<>"                       { EMIT_TOKEN(T_NOT_EQUAL, "<>"); }
".."                       { EMIT_TOKEN(T_DOTDOT, ".."); }

"+"                        { EMIT_TOKEN(T_PLUS, "+"); }
"-"                        { EMIT_TOKEN(T_MINUS, "-"); }
"*"                        { EMIT_TOKEN(T_MULTIPLY, "*"); }
"/"                        { EMIT_TOKEN(T_DIVIDE, "/"); }
"("                        { EMIT_TOKEN(T_LPAREN, "("); }
")"                        { EMIT_TOKEN(T_RPAREN, ")"); }
"["                        { EMIT_TOKEN(T_LBRACKET, "["); }
"]"                        { EMIT_TOKEN(T_RBRACKET, "]"); }
","                        { EMIT_TOKEN(T_COMMA, ","); }
";"                        { EMIT_TOKEN(T_SEMICOLON, ";"); }
":"                        { EMIT_TOKEN(T_COLON, ":"); }
"."                        { EMIT_TOKEN(T_DOT, "."); }
"="                        { EMIT_TOKEN(T_EQUAL, "="); }
"<"                        { EMIT_TOKEN(T_LESS, "<"); }
">"                        { EMIT_TOKEN(T_GREATER, ">"); }

[0-9]+"."[0-9]+            { EMIT_TOKEN(T_REAL, std::string(yytext, static_cast<std::size_t>(yyleng))); }
[0-9]+                     { EMIT_TOKEN(T_INTEGER, std::string(yytext, static_cast<std::size_t>(yyleng))); }

\'([^\\\'\n\r])\'          { EMIT_TOKEN(T_CHAR, std::string(yytext, static_cast<std::size_t>(yyleng))); }
\'([^\'\n\r]|\\.)*\'       {
                             addError("E109", "Invalid char literal. Only single non-escaped char is supported.", g_line, g_col);
                             g_col += yyleng;
                           }
\'([^\'\n\r]|\\.)*         {
                             addError("E109", "Invalid char literal. Only single non-escaped char is supported.", g_line, g_col);
                             g_col += yyleng;
                           }

[A-Za-z][A-Za-z0-9]*       {
                             const std::string lowered = toLower(yytext, yyleng);
                             const int token = keywordOrIdentifier(lowered);
                             if (token == T_IDENTIFIER && yyleng > 8) {
                                 addError("E105", "Identifier length exceeds 8 characters: " + std::string(yytext, static_cast<std::size_t>(yyleng)), g_line, g_col);
                             }
                             EMIT_TOKEN(token, lowered);
                           }

.                          {
                             addError("E110", "Unknown character: " + std::string(yytext, static_cast<std::size_t>(yyleng)), g_line, g_col);
                             EMIT_TOKEN(T_UNKNOWN, std::string(yytext, static_cast<std::size_t>(yyleng)));
                           }
%%
