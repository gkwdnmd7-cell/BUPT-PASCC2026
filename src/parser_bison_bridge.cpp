#include "parser_bison.h"

#include <string>

#include "ast.h"
#include "token.h"

// Include Bison-generated header without extern "C": the generated
// parser_bison.tab.c is compiled as C++ (LANGUAGE CXX in CMake), so all
// symbols have C++ linkage and no extern "C" wrapper is needed.
#include "parser_bison.tab.h"

// Global that the start rule in parser_bison.y sets before returning.
AstNode* g_ast_root = nullptr;

namespace {
const std::vector<Token>* g_tokens = nullptr;
size_t g_pos = 0;
Token g_lastToken;
ParserResult* g_result = nullptr;

void addParseError(const std::string& code, const std::string& message, const SourcePosition& pos) {
    if (g_result) {
        g_result->errors.push_back(ParseError{code, message, pos});
    }
}

int mapToken(TokenType type) {
    switch (type) {
        case TokenType::Unknown:         return T_UNKNOWN;
        case TokenType::Identifier:      return T_IDENTIFIER;
        case TokenType::IntegerLiteral:  return T_INTEGER;
        case TokenType::RealLiteral:     return T_REAL;
        case TokenType::CharLiteral:     return T_CHAR;
        case TokenType::StringLiteral:   return T_STRING;
        case TokenType::BooleanLiteral:  return T_BOOLEAN;
        case TokenType::KwProgram:       return T_PROGRAM;
        case TokenType::KwConst:         return T_CONST;
        case TokenType::KwType:          return T_TYPE;
        case TokenType::KwVar:           return T_VAR;
        case TokenType::KwProcedure:     return T_PROCEDURE;
        case TokenType::KwFunction:      return T_FUNCTION;
        case TokenType::KwBegin:         return T_BEGIN;
        case TokenType::KwEnd:           return T_END;
        case TokenType::KwIf:            return T_IF;
        case TokenType::KwThen:          return T_THEN;
        case TokenType::KwElse:          return T_ELSE;
        case TokenType::KwCase:          return T_CASE;
        case TokenType::KwWhile:         return T_WHILE;
        case TokenType::KwRepeat:        return T_REPEAT;
        case TokenType::KwUntil:         return T_UNTIL;
        case TokenType::KwFor:           return T_FOR;
        case TokenType::KwTo:            return T_TO;
        case TokenType::KwDownTo:        return T_DOWNTO;
        case TokenType::KwDo:            return T_DO;
        case TokenType::KwRead:          return T_READ;
        case TokenType::KwReadLn:        return T_READLN;
        case TokenType::KwWrite:         return T_WRITE;
        case TokenType::KwWriteLn:       return T_WRITELN;
        case TokenType::KwRecord:        return T_RECORD;
        case TokenType::KwArray:         return T_ARRAY;
        case TokenType::KwOf:            return T_OF;
        case TokenType::KwInteger:       return T_TYPE_INTEGER;
        case TokenType::KwReal:          return T_TYPE_REAL;
        case TokenType::KwBoolean:       return T_TYPE_BOOLEAN;
        case TokenType::KwChar:          return T_TYPE_CHAR;
        case TokenType::Plus:            return T_PLUS;
        case TokenType::Minus:           return T_MINUS;
        case TokenType::Multiply:        return T_MULTIPLY;
        case TokenType::Divide:          return T_DIVIDE;
        case TokenType::Div:             return T_DIV;
        case TokenType::Mod:             return T_MOD;
        case TokenType::And:             return T_AND;
        case TokenType::Or:              return T_OR;
        case TokenType::Not:             return T_NOT;
        case TokenType::Assign:          return T_ASSIGN;
        case TokenType::Equal:           return T_EQUAL;
        case TokenType::NotEqual:        return T_NOT_EQUAL;
        case TokenType::Less:            return T_LESS;
        case TokenType::LessEqual:       return T_LESS_EQUAL;
        case TokenType::Greater:         return T_GREATER;
        case TokenType::GreaterEqual:    return T_GREATER_EQUAL;
        case TokenType::LParen:          return T_LPAREN;
        case TokenType::RParen:          return T_RPAREN;
        case TokenType::LBracket:        return T_LBRACKET;
        case TokenType::RBracket:        return T_RBRACKET;
        case TokenType::Comma:           return T_COMMA;
        case TokenType::Semicolon:       return T_SEMICOLON;
        case TokenType::Colon:           return T_COLON;
        case TokenType::Dot:             return T_DOT;
        case TokenType::DotDot:          return T_DOTDOT;
        case TokenType::KwBreak:         return T_BREAK;
        case TokenType::KwContinue:      return T_CONTINUE;
        case TokenType::KwExit:          return T_EXIT;
        case TokenType::EndOfFile:       return 0;
        default:                         return T_UNKNOWN;
    }
}
}  // namespace

// yylex: called by the Bison-generated parser to fetch the next token.
// Populates yylval with the semantic value for tokens that carry one.
int yylex(void) {
    if (!g_tokens || g_pos >= g_tokens->size()) {
        return 0;
    }

    const Token& token = (*g_tokens)[g_pos++];
    g_lastToken = token;

    // Set yylval for tokens that carry semantic data.
    switch (token.type) {
        case TokenType::Identifier:
        case TokenType::CharLiteral:
        case TokenType::StringLiteral:
        case TokenType::IntegerLiteral:
        case TokenType::RealLiteral:
            yylval.str = new std::string(token.lexeme);
            break;
        case TokenType::BooleanLiteral:
            yylval.ival = (token.lexeme == "true") ? 1 : 0;
            break;
        default:
            break;
    }

    return mapToken(token.type);
}

void yyerror(const char* msg) {
    const SourcePosition pos = g_lastToken.pos;
    addParseError("E201", std::string("Syntax error: ") + msg, pos);
}

ParserResult parseWithBison(const std::vector<Token>& tokens) {
    ParserResult result;
    g_tokens   = &tokens;
    g_pos      = 0;
    g_ast_root = nullptr;
    g_lastToken = Token{TokenType::EndOfFile, "<EOF>", {1, 1}};
    g_result   = &result;

    const int rc = yyparse();
    if (rc != 0 && result.errors.empty()) {
        result.errors.push_back(ParseError{"E299", "Syntax analysis failed.", g_lastToken.pos});
    }
    if (result.errors.empty() && g_ast_root) {
        result.root = std::shared_ptr<ProgramNode>(
            static_cast<ProgramNode*>(g_ast_root));
        g_ast_root = nullptr;
    } else {
        // Parsing failed — leak AST nodes built so far (acceptable for error path)
        g_ast_root = nullptr;
        if (result.errors.empty()) {
            result.errors.push_back(ParseError{"E298", "No AST root produced.", {1,1}});
        }
    }

    g_tokens = nullptr;
    g_result = nullptr;
    return result;
}
