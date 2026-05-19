#include "code_generator.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "lexer.h"
#include "token.h"

namespace {

// ---------------------------------------------------------------------------
// Data structures
// ---------------------------------------------------------------------------

struct ConstDecl {
    std::string name;
    std::string cType;   // "int", "double", "char"
    std::string cValue;  // C-side value string
};

struct VarDecl {
    std::string name;
    std::string cType;
    bool isArray = false;
    std::string arrayLow;
    std::string arrayHigh;
    int arraySize = 0;       // first-dim size (or 0)
    std::string arraySuffix; // full C array suffix, e.g. "[10]" or "[5][5][7]"
};

struct RoutineParam {
    std::string name;
    std::string cType;
    bool byRef = false;
};

struct RoutineDecl {
    std::string name;
    std::string returnType;
    bool isFunction = false;
    std::vector<RoutineParam> params;
    std::vector<VarDecl> localDecls;
    std::vector<std::string> body;
};

struct StatementContext {
    std::string currentFunctionName;
    const std::unordered_set<std::string>* funcNames = nullptr;
    const std::unordered_map<std::string, std::string>* typeMap = nullptr;
    const std::unordered_set<std::string>* varParams = nullptr;
    const std::unordered_map<std::string, std::vector<bool>>* routineByRef = nullptr;
};

struct RoutineParseResult {
    std::vector<RoutineDecl> routines;
    std::size_t mainBeginIndex = static_cast<std::size_t>(-1);
};

using CStatements = std::vector<std::string>;

// ---------------------------------------------------------------------------
// Type helpers
// ---------------------------------------------------------------------------

std::string mapTypeTokenToC(TokenType type) {
    switch (type) {
        case TokenType::KwInteger: return "int";
        case TokenType::KwReal:    return "float";
        case TokenType::KwBoolean: return "int";
        case TokenType::KwChar:    return "char";
        default: return "";
    }
}

// Parse array[low..high, low..high, ...] of elementType.
// Returns the C element type; outLow/outHigh get the first dimension bounds;
// outSuffix gets the full C array dimension string e.g. "[10]" or "[18][18][7]".
std::string parseArrayTypeToC(const std::vector<Token>& tokens, std::size_t& i,
                               std::string& outLow, std::string& outHigh,
                               std::string& outSuffix) {
    ++i;  // consume 'array'
    outLow.clear();
    outHigh.clear();
    outSuffix.clear();

    if (i < tokens.size() && tokens[i].type == TokenType::LBracket) {
        ++i;  // consume '['
        bool firstDim = true;
        while (i < tokens.size() && tokens[i].type != TokenType::RBracket &&
               tokens[i].type != TokenType::EndOfFile) {
            // Parse optional sign before low bound
            bool negLow = false;
            if (tokens[i].type == TokenType::Minus) { negLow = true; ++i; }
            else if (tokens[i].type == TokenType::Plus) { ++i; }
            std::string low, high;
            if (i < tokens.size() && (tokens[i].type == TokenType::IntegerLiteral ||
                                      tokens[i].type == TokenType::Identifier)) {
                low = (negLow ? "-" : "") + tokens[i].lexeme;
                ++i;
            }
            if (i < tokens.size() && tokens[i].type == TokenType::DotDot) ++i;
            bool negHigh = false;
            if (i < tokens.size() && tokens[i].type == TokenType::Minus) { negHigh = true; ++i; }
            else if (i < tokens.size() && tokens[i].type == TokenType::Plus) { ++i; }
            if (i < tokens.size() && (tokens[i].type == TokenType::IntegerLiteral ||
                                      tokens[i].type == TokenType::Identifier)) {
                high = (negHigh ? "-" : "") + tokens[i].lexeme;
                ++i;
            }
            if (firstDim) { outLow = low; outHigh = high; firstDim = false; }
            // Compute dimension size
            int sz = 1;
            if (!low.empty() && !high.empty()) {
                try { sz = std::stoi(high) - std::stoi(low) + 1; } catch (...) {}
            }
            outSuffix += "[" + std::to_string(sz) + "]";
            if (i < tokens.size() && tokens[i].type == TokenType::Comma) ++i;
            else break;
        }
        if (i < tokens.size() && tokens[i].type == TokenType::RBracket) ++i;
    }
    if (i < tokens.size() && tokens[i].type == TokenType::KwOf) ++i;
    if (i < tokens.size()) {
        const std::string ct = mapTypeTokenToC(tokens[i].type);
        if (!ct.empty()) {
            ++i;
            return ct;
        }
    }
    return "int";
}

// ---------------------------------------------------------------------------
// Expression piece converter
// ---------------------------------------------------------------------------

std::string tokenToExprPiece(const Token& tok) {
    switch (tok.type) {
        case TokenType::Identifier:
        case TokenType::IntegerLiteral:
        case TokenType::RealLiteral:
            return tok.lexeme;
        case TokenType::CharLiteral:
            return tok.lexeme;
        case TokenType::BooleanLiteral:
            return tok.lexeme == "true" ? "1" : "0";
        case TokenType::Plus:           return "+";
        case TokenType::Minus:          return "-";
        case TokenType::Multiply:       return "*";
        case TokenType::Divide:         return "/";
        case TokenType::Div:            return "/";
        case TokenType::Mod:            return "%";
        case TokenType::And:            return "&&";
        case TokenType::Or:             return "||";
        case TokenType::Not:            return "!";
        case TokenType::Equal:          return "==";
        case TokenType::NotEqual:       return "!=";
        case TokenType::Less:           return "<";
        case TokenType::LessEqual:      return "<=";
        case TokenType::Greater:        return ">";
        case TokenType::GreaterEqual:   return ">=";
        case TokenType::LParen:         return "(";
        case TokenType::RParen:         return ")";
        case TokenType::LBracket:       return "[";
        case TokenType::RBracket:       return "]";
        case TokenType::Comma:          return ",";
        default: return "";
    }
}

std::string joinPieces(const std::vector<std::string>& pieces) {
    std::ostringstream out;
    for (std::size_t i = 0; i < pieces.size(); ++i) {
        if (i > 0) {
            out << " ";
        }
        out << pieces[i];
    }
    return out.str();
}

bool isStopToken(TokenType t, const std::vector<TokenType>& stopTokens) {
    return std::find(stopTokens.begin(), stopTokens.end(), t) != stopTokens.end();
}

std::string parseExpressionUntil(const std::vector<Token>& tokens, std::size_t& i,
                                  const std::vector<TokenType>& stopTokens,
                                  const std::unordered_set<std::string>* funcNames = nullptr,
                                  const std::unordered_set<std::string>* varParams = nullptr,
                                  const std::unordered_map<std::string, std::vector<bool>>* routineByRef = nullptr) {
    std::vector<std::string> pieces;
    int parenDepth = 0;
    int bracketDepth = 0;

    while (i < tokens.size()) {
        const TokenType type = tokens[i].type;

        // Check stop conditions FIRST (before updating depth).
        // Only stop on a token at depth 0 — this lets nested parens/brackets
        // pass through as part of a sub-expression.
        if (parenDepth == 0 && bracketDepth == 0 && isStopToken(type, stopTokens)) {
            break;
        }

        // Update depth tracking (safe: we would have broken above if depth==0
        // and the token was a stop token like RParen/RBracket).
        if (type == TokenType::LParen) {
            ++parenDepth;
        } else if (type == TokenType::RParen) {
            --parenDepth;
        } else if (type == TokenType::LBracket) {
            ++bracketDepth;
        } else if (type == TokenType::RBracket) {
            --bracketDepth;
        }

        // Multi-dim array: comma inside brackets → "][" in C.
        // e.g. a[i, j] → a[i][j]; also works inside func calls: f(a[i,j]) → f(a[i][j])
        if (type == TokenType::Comma && bracketDepth > 0) {
            pieces.push_back("][");
            ++i;
            continue;
        }

        // Pascal 'not': use ~ for integer context, ! for boolean/paren context.
        if (type == TokenType::Not) {
            const bool nextIsParen = (i + 1 < tokens.size() && tokens[i + 1].type == TokenType::LParen);
            pieces.push_back(nextIsParen ? "!" : "~");
            ++i;
            continue;
        }

        if (type == TokenType::Identifier) {
            const std::string& nm = tokens[i].lexeme;
            const bool nextIsLParen = (i + 1 < tokens.size() && tokens[i + 1].type == TokenType::LParen);

            // VAR (by-ref) param used as a value — dereference it.
            if (!nextIsLParen && varParams && varParams->count(nm) > 0) {
                pieces.push_back("(*" + nm + ")");
                ++i;
                continue;
            }

            // Function/procedure call that has VAR param positions — wrap those args with &.
            if (nextIsLParen && routineByRef) {
                auto itByRef = routineByRef->find(nm);
                if (itByRef != routineByRef->end()) {
                    const auto& byRef = itByRef->second;
                    bool hasAnyByRef = false;
                    for (bool b : byRef) if (b) { hasAnyByRef = true; break; }
                    if (hasAnyByRef) {
                        ++i;  // consume identifier
                        ++i;  // consume '('
                        std::vector<std::string> callArgs;
                        std::size_t argIdx = 0;
                        while (i < tokens.size() && tokens[i].type != TokenType::RParen &&
                               tokens[i].type != TokenType::EndOfFile) {
                            std::string arg = parseExpressionUntil(tokens, i,
                                {TokenType::Comma, TokenType::RParen},
                                funcNames, varParams, routineByRef);
                            const bool isVarPos = argIdx < byRef.size() && byRef[argIdx];
                            callArgs.push_back(isVarPos && !arg.empty() ? "& " + arg : arg);
                            ++argIdx;
                            if (i < tokens.size() && tokens[i].type == TokenType::Comma) ++i;
                        }
                        if (i < tokens.size() && tokens[i].type == TokenType::RParen) ++i;
                        std::ostringstream call;
                        call << nm << "(";
                        for (std::size_t k = 0; k < callArgs.size(); ++k) {
                            if (k > 0) call << ", ";
                            call << callArgs[k];
                        }
                        call << ")";
                        pieces.push_back(call.str());
                        continue;
                    }
                }
            }

            // No-arg function called without parens
            // (e.g. Pascal "write(ififElse)" should become "write(ififElse())" in C).
            if (!nextIsLParen && funcNames != nullptr && funcNames->count(nm) > 0) {
                pieces.push_back(nm + "()");
                ++i;
                continue;
            }
        }

        const std::string piece = tokenToExprPiece(tokens[i]);
        if (!piece.empty()) {
            pieces.push_back(piece);
        }
        ++i;
    }

    return joinPieces(pieces);
}

void emitStatementLine(CStatements& out, int indentLevel, const std::string& stmt) {
    out.push_back(std::string(static_cast<std::size_t>(indentLevel) * 4U, ' ') + stmt);
}

// Forward declarations
void parseSingleStatement(const std::vector<Token>& tokens, std::size_t& i, CStatements& out, int indentLevel,
                          const StatementContext& ctx);
void parseCaseStatement(const std::vector<Token>& tokens, std::size_t& i, CStatements& out, int indentLevel,
                        const StatementContext& ctx);

// ---------------------------------------------------------------------------
// Const parsing
// ---------------------------------------------------------------------------

std::vector<ConstDecl> parseGlobalConstDecls(const std::vector<Token>& tokens) {
    std::vector<ConstDecl> decls;
    std::size_t i = 0;

    // Skip program header
    if (i < tokens.size() && tokens[i].type == TokenType::KwProgram) {
        while (i < tokens.size() && tokens[i].type != TokenType::Semicolon) ++i;
        if (i < tokens.size()) ++i;
    }

    if (i >= tokens.size() || tokens[i].type != TokenType::KwConst) {
        return decls;
    }
    ++i;  // consume 'const'

    while (i < tokens.size() &&
           tokens[i].type != TokenType::KwVar &&
           tokens[i].type != TokenType::KwProcedure &&
           tokens[i].type != TokenType::KwFunction &&
           tokens[i].type != TokenType::KwBegin &&
           tokens[i].type != TokenType::EndOfFile) {

        if (tokens[i].type != TokenType::Identifier) {
            ++i;
            continue;
        }

        const std::string name = tokens[i].lexeme;
        ++i;

        if (i >= tokens.size() || tokens[i].type != TokenType::Equal) {
            while (i < tokens.size() && tokens[i].type != TokenType::Semicolon &&
                   tokens[i].type != TokenType::KwVar) ++i;
            if (i < tokens.size() && tokens[i].type == TokenType::Semicolon) ++i;
            continue;
        }
        ++i;  // consume '='

        std::string cType = "int";
        std::string cValue;

        bool neg = false;
        if (i < tokens.size() && (tokens[i].type == TokenType::Plus || tokens[i].type == TokenType::Minus)) {
            neg = (tokens[i].type == TokenType::Minus);
            ++i;
        }

        if (i < tokens.size()) {
            if (tokens[i].type == TokenType::RealLiteral) {
                cType = "float";
                cValue = neg ? "-" + tokens[i].lexeme : tokens[i].lexeme;
                ++i;
            } else if (tokens[i].type == TokenType::IntegerLiteral) {
                cType = "int";
                cValue = neg ? "-" + tokens[i].lexeme : tokens[i].lexeme;
                ++i;
            } else if (tokens[i].type == TokenType::CharLiteral) {
                cType = "char";
                cValue = tokens[i].lexeme;  // keep 'x' quotes
                ++i;
            } else if (tokens[i].type == TokenType::BooleanLiteral) {
                cType = "int";
                cValue = (tokens[i].lexeme == "true") ? "1" : "0";
                ++i;
            } else if (tokens[i].type == TokenType::Identifier) {
                // Defined as another constant; emit as-is
                cType = "int";
                cValue = tokens[i].lexeme;
                ++i;
            }
        }

        if (!cValue.empty()) {
            decls.push_back(ConstDecl{name, cType, cValue});
        }

        // Skip to ';'
        while (i < tokens.size() && tokens[i].type != TokenType::Semicolon &&
               tokens[i].type != TokenType::KwVar && tokens[i].type != TokenType::KwBegin) {
            ++i;
        }
        if (i < tokens.size() && tokens[i].type == TokenType::Semicolon) ++i;
    }

    return decls;
}

// ---------------------------------------------------------------------------
// Var parsing (supports arrays)
// ---------------------------------------------------------------------------

std::vector<VarDecl> parseGlobalVarDecls(const std::vector<Token>& tokens) {
    std::vector<VarDecl> decls;
    std::size_t i = 0;

    while (i < tokens.size() &&
           tokens[i].type != TokenType::KwVar &&
           tokens[i].type != TokenType::KwBegin &&
           tokens[i].type != TokenType::EndOfFile) {
        ++i;
    }

    if (i >= tokens.size() || tokens[i].type != TokenType::KwVar) {
        return decls;
    }
    ++i;  // consume 'var'

    while (i < tokens.size()) {
        if (tokens[i].type == TokenType::KwBegin ||
            tokens[i].type == TokenType::KwProcedure ||
            tokens[i].type == TokenType::KwFunction ||
            tokens[i].type == TokenType::EndOfFile) {
            break;
        }

        std::vector<std::string> names;
        if (tokens[i].type == TokenType::Identifier) {
            names.push_back(tokens[i].lexeme);
            ++i;
            while (i < tokens.size() && tokens[i].type == TokenType::Comma) {
                ++i;
                if (i < tokens.size() && tokens[i].type == TokenType::Identifier) {
                    names.push_back(tokens[i].lexeme);
                    ++i;
                }
            }
        }

        if (i >= tokens.size() || tokens[i].type != TokenType::Colon) {
            while (i < tokens.size() && tokens[i].type != TokenType::Semicolon &&
                   tokens[i].type != TokenType::KwBegin) {
                ++i;
            }
            if (i < tokens.size() && tokens[i].type == TokenType::Semicolon) ++i;
            continue;
        }
        ++i;  // consume ':'

        if (i < tokens.size() && tokens[i].type == TokenType::KwArray) {
            std::string low, high, suffix;
            const std::string elemType = parseArrayTypeToC(tokens, i, low, high, suffix);
            int sz = 0;
            if (!low.empty() && !high.empty()) {
                try { sz = std::stoi(high) - std::stoi(low) + 1; } catch (...) {}
            }
            for (const auto& nm : names) {
                decls.push_back(VarDecl{nm, elemType, true, low, high, sz, suffix});
            }
        } else {
            const std::string cType = (i < tokens.size()) ? mapTypeTokenToC(tokens[i].type) : "";
            if (i < tokens.size()) ++i;
            for (const auto& nm : names) {
                if (!cType.empty()) {
                    decls.push_back(VarDecl{nm, cType, false, "", "", 0, ""});
                }
            }
        }

        while (i < tokens.size() && tokens[i].type != TokenType::Semicolon &&
               tokens[i].type != TokenType::KwBegin) {
            ++i;
        }
        if (i < tokens.size() && tokens[i].type == TokenType::Semicolon) ++i;
    }

    return decls;
}

std::vector<VarDecl> parseVarDeclsInRange(const std::vector<Token>& tokens,
                                           std::size_t start, std::size_t endExclusive) {
    std::vector<VarDecl> decls;
    std::size_t i = start;

    while (i < endExclusive) {
        if (tokens[i].type != TokenType::KwVar) {
            ++i;
            continue;
        }
        ++i;

        while (i < endExclusive &&
               tokens[i].type != TokenType::KwBegin &&
               tokens[i].type != TokenType::KwProcedure &&
               tokens[i].type != TokenType::KwFunction) {

            std::vector<std::string> names;
            if (tokens[i].type == TokenType::Identifier) {
                names.push_back(tokens[i].lexeme);
                ++i;
                while (i < endExclusive && tokens[i].type == TokenType::Comma) {
                    ++i;
                    if (i < endExclusive && tokens[i].type == TokenType::Identifier) {
                        names.push_back(tokens[i].lexeme);
                        ++i;
                    }
                }
            }

            if (i >= endExclusive || tokens[i].type != TokenType::Colon) {
                while (i < endExclusive && tokens[i].type != TokenType::Semicolon &&
                       tokens[i].type != TokenType::KwBegin) {
                    ++i;
                }
                if (i < endExclusive && tokens[i].type == TokenType::Semicolon) ++i;
                continue;
            }
            ++i;

            if (i < endExclusive && tokens[i].type == TokenType::KwArray) {
                std::string low, high, suffix;
                const std::string elemType = parseArrayTypeToC(tokens, i, low, high, suffix);
                int sz = 0;
                if (!low.empty() && !high.empty()) {
                    try { sz = std::stoi(high) - std::stoi(low) + 1; } catch (...) {}
                }
                for (const auto& nm : names) {
                    decls.push_back(VarDecl{nm, elemType, true, low, high, sz, suffix});
                }
            } else {
                const std::string cType = (i < endExclusive) ? mapTypeTokenToC(tokens[i].type) : "";
                if (i < endExclusive) ++i;
                for (const auto& nm : names) {
                    if (!cType.empty()) {
                        decls.push_back(VarDecl{nm, cType, false, "", "", 0, ""});
                    }
                }
            }

            while (i < endExclusive && tokens[i].type != TokenType::Semicolon &&
                   tokens[i].type != TokenType::KwBegin) {
                ++i;
            }
            if (i < endExclusive && tokens[i].type == TokenType::Semicolon) ++i;
        }
    }

    return decls;
}

// ---------------------------------------------------------------------------
// Compound body parsing
// ---------------------------------------------------------------------------

void parseCompoundBody(const std::vector<Token>& tokens, std::size_t& i, CStatements& out, int indentLevel,
                       const StatementContext& ctx) {
    while (i < tokens.size() && tokens[i].type != TokenType::KwEnd && tokens[i].type != TokenType::EndOfFile) {
        parseSingleStatement(tokens, i, out, indentLevel, ctx);
        if (i < tokens.size() && tokens[i].type == TokenType::Semicolon) {
            ++i;
        }
    }
    if (i < tokens.size() && tokens[i].type == TokenType::KwEnd) {
        ++i;
    }
}

// ---------------------------------------------------------------------------
// If / While / For / Repeat statements
// ---------------------------------------------------------------------------

void parseIfStatement(const std::vector<Token>& tokens, std::size_t& i, CStatements& out, int indentLevel,
                      const StatementContext& ctx) {
    ++i;  // consume 'if'
    const std::string cond = parseExpressionUntil(tokens, i, {TokenType::KwThen},
                                                   ctx.funcNames, ctx.varParams, ctx.routineByRef);
    if (i < tokens.size() && tokens[i].type == TokenType::KwThen) ++i;

    emitStatementLine(out, indentLevel, "if (" + (cond.empty() ? "0" : cond) + ") {");
    parseSingleStatement(tokens, i, out, indentLevel + 1, ctx);
    emitStatementLine(out, indentLevel, "}");

    if (i < tokens.size() && tokens[i].type == TokenType::KwElse) {
        ++i;
        emitStatementLine(out, indentLevel, "else {");
        parseSingleStatement(tokens, i, out, indentLevel + 1, ctx);
        emitStatementLine(out, indentLevel, "}");
    }
}

void parseWhileStatement(const std::vector<Token>& tokens, std::size_t& i, CStatements& out, int indentLevel,
                         const StatementContext& ctx) {
    ++i;  // consume 'while'
    const std::string cond = parseExpressionUntil(tokens, i, {TokenType::KwDo},
                                                   ctx.funcNames, ctx.varParams, ctx.routineByRef);
    if (i < tokens.size() && tokens[i].type == TokenType::KwDo) ++i;

    emitStatementLine(out, indentLevel, "while (" + (cond.empty() ? "0" : cond) + ") {");
    parseSingleStatement(tokens, i, out, indentLevel + 1, ctx);
    emitStatementLine(out, indentLevel, "}");
}

void parseForStatement(const std::vector<Token>& tokens, std::size_t& i, CStatements& out, int indentLevel,
                       const StatementContext& ctx) {
    ++i;  // consume 'for'
    std::string loopVar;
    if (i < tokens.size() && tokens[i].type == TokenType::Identifier) {
        loopVar = tokens[i].lexeme;
        ++i;
    }
    if (i < tokens.size() && tokens[i].type == TokenType::Assign) ++i;

    const std::string beginExpr = parseExpressionUntil(tokens, i,
        {TokenType::KwTo, TokenType::KwDownTo, TokenType::KwDo},
        ctx.funcNames, ctx.varParams, ctx.routineByRef);

    bool isDownTo = false;
    if (i < tokens.size() && (tokens[i].type == TokenType::KwTo || tokens[i].type == TokenType::KwDownTo)) {
        isDownTo = (tokens[i].type == TokenType::KwDownTo);
        ++i;
    }
    const std::string endExpr = parseExpressionUntil(tokens, i, {TokenType::KwDo},
                                                      ctx.funcNames, ctx.varParams, ctx.routineByRef);
    if (i < tokens.size() && tokens[i].type == TokenType::KwDo) ++i;

    const std::string var   = loopVar.empty() ? "i" : loopVar;
    const std::string begin = beginExpr.empty() ? "0" : beginExpr;
    const std::string end   = endExpr.empty() ? "0" : endExpr;
    const std::string cond  = isDownTo ? (var + " >= " + end) : (var + " <= " + end);
    const std::string step  = isDownTo ? ("--" + var) : ("++" + var);

    emitStatementLine(out, indentLevel, "for (" + var + " = " + begin + "; " + cond + "; " + step + ") {");
    parseSingleStatement(tokens, i, out, indentLevel + 1, ctx);
    emitStatementLine(out, indentLevel, "}");
}

void parseRepeatStatement(const std::vector<Token>& tokens, std::size_t& i, CStatements& out, int indentLevel,
                          const StatementContext& ctx) {
    ++i;  // consume 'repeat'
    emitStatementLine(out, indentLevel, "do {");

    while (i < tokens.size() && tokens[i].type != TokenType::KwUntil && tokens[i].type != TokenType::EndOfFile) {
        parseSingleStatement(tokens, i, out, indentLevel + 1, ctx);
        if (i < tokens.size() && tokens[i].type == TokenType::Semicolon) ++i;
    }

    if (i < tokens.size() && tokens[i].type == TokenType::KwUntil) ++i;
    const std::string cond = parseExpressionUntil(tokens, i,
        {TokenType::Semicolon, TokenType::KwEnd, TokenType::KwElse},
        ctx.funcNames, ctx.varParams, ctx.routineByRef);

    emitStatementLine(out, indentLevel, "} while (!(" + (cond.empty() ? "0" : cond) + "));");
}

// ---------------------------------------------------------------------------
// Case statement
// ---------------------------------------------------------------------------

void parseCaseStatement(const std::vector<Token>& tokens, std::size_t& i, CStatements& out,
                        int indentLevel, const StatementContext& ctx) {
    ++i;  // consume 'case'
    const std::string expr = parseExpressionUntil(tokens, i, {TokenType::KwOf},
                                                   ctx.funcNames, ctx.varParams, ctx.routineByRef);
    if (i < tokens.size() && tokens[i].type == TokenType::KwOf) ++i;

    emitStatementLine(out, indentLevel, "switch (" + (expr.empty() ? "0" : expr) + ") {");

    while (i < tokens.size() && tokens[i].type != TokenType::KwEnd &&
           tokens[i].type != TokenType::EndOfFile) {
        if (tokens[i].type == TokenType::Semicolon) { ++i; continue; }

        // Collect one or more case values (const_list) before ':'
        std::vector<std::string> caseValues;
        while (i < tokens.size()) {
            std::string val;
            // Optional sign
            bool neg = false;
            if (tokens[i].type == TokenType::Minus) { neg = true; ++i; }
            else if (tokens[i].type == TokenType::Plus) { ++i; }

            if (i < tokens.size() && tokens[i].type == TokenType::IntegerLiteral) {
                val = (neg ? "-" : "") + tokens[i].lexeme; ++i;
            } else if (i < tokens.size() && tokens[i].type == TokenType::CharLiteral) {
                val = tokens[i].lexeme; ++i;
            } else if (i < tokens.size() && tokens[i].type == TokenType::Identifier) {
                val = tokens[i].lexeme; ++i;
            } else if (i < tokens.size() && tokens[i].type == TokenType::BooleanLiteral) {
                val = (tokens[i].lexeme == "true") ? "1" : "0"; ++i;
            } else {
                // Not a valid case value; bail
                break;
            }
            if (!val.empty()) caseValues.push_back(val);
            if (i < tokens.size() && tokens[i].type == TokenType::Comma) ++i;
            else break;
        }

        if (caseValues.empty()) { ++i; continue; }

        if (i < tokens.size() && tokens[i].type == TokenType::Colon) ++i;

        for (const auto& v : caseValues) {
            emitStatementLine(out, indentLevel + 1, "case " + v + ":");
        }
        parseSingleStatement(tokens, i, out, indentLevel + 2, ctx);
        emitStatementLine(out, indentLevel + 2, "break;");
    }

    if (i < tokens.size() && tokens[i].type == TokenType::KwEnd) ++i;
    emitStatementLine(out, indentLevel, "}");
}

// ---------------------------------------------------------------------------
// Read / Write statements
// ---------------------------------------------------------------------------

std::vector<std::string> parseExprList(const std::vector<Token>& tokens, std::size_t& i,
                                        const std::unordered_set<std::string>* funcNames = nullptr,
                                        const std::unordered_set<std::string>* varParams = nullptr,
                                        const std::unordered_map<std::string, std::vector<bool>>* routineByRef = nullptr) {
    std::vector<std::string> exprs;
    if (i >= tokens.size() || tokens[i].type != TokenType::LParen) {
        return exprs;
    }
    ++i;

    while (i < tokens.size() && tokens[i].type != TokenType::RParen && tokens[i].type != TokenType::EndOfFile) {
        const std::string expr = parseExpressionUntil(tokens, i, {TokenType::Comma, TokenType::RParen},
                                                       funcNames, varParams, routineByRef);
        if (!expr.empty()) {
            exprs.push_back(expr);
        }
        if (i < tokens.size() && tokens[i].type == TokenType::Comma) ++i;
    }

    if (i < tokens.size() && tokens[i].type == TokenType::RParen) ++i;
    return exprs;
}

void parseReadStatement(const std::vector<Token>& tokens, std::size_t& i, CStatements& out,
                        int indentLevel, const StatementContext& ctx) {
    ++i;  // consume read/readln
    const auto args = parseExprList(tokens, i, ctx.funcNames, ctx.varParams, ctx.routineByRef);
    for (const auto& arg : args) {
        // Determine scanf format from type map
        std::string fmt = "%d";
        if (ctx.typeMap != nullptr) {
            std::string key = arg;
            // Strip array subscript for lookup: arr[i] → arr
            const auto bracket = key.find('[');
            if (bracket != std::string::npos) {
                key = key.substr(0, bracket);
                while (!key.empty() && key.back() == ' ') key.pop_back();
            }
            auto it = ctx.typeMap->find(key);
            if (it != ctx.typeMap->end()) {
                if (it->second == "float")  fmt = "%f";
                else if (it->second == "char") fmt = " %c";
            }
        }
        emitStatementLine(out, indentLevel, "scanf(\"" + fmt + "\", &" + arg + ");");
    }
}

// Infer write format: "float", "char", or "int"
std::string inferWriteType(const std::string& expr,
                            const std::unordered_map<std::string, std::string>* typeMap) {
    if (typeMap != nullptr) {
        // Strip trailing () and []... for a simple identifier lookup
        std::string key = expr;
        // Remove trailing ()
        if (key.size() >= 2 && key.back() == ')') {
            const auto p = key.rfind('(');
            if (p != std::string::npos) key = key.substr(0, p);
            while (!key.empty() && key.back() == ' ') key.pop_back();
        }
        // Remove array subscript part (use find for first '[' to handle multi-dim)
        {
            const auto p = key.find('[');
            if (p != std::string::npos) {
                key = key.substr(0, p);
                while (!key.empty() && key.back() == ' ') key.pop_back();
            }
        }
        auto it = typeMap->find(key);
        if (it != typeMap->end()) {
            if (it->second == "float" || it->second == "double") return "float";
            if (it->second == "char")   return "char";
            return "int";
        }
    }

    // Heuristics: real literal contains '.' but is not array/call expression
    bool hasComposite = (expr.find('[') != std::string::npos || expr.find('(') != std::string::npos);
    if (!hasComposite && expr.find('.') != std::string::npos) {
        return "float";
    }
    // Char literal: starts with single-quote
    if (!expr.empty() && expr.front() == '\'') {
        return "char";
    }
    return "int";
}

void parseWriteStatement(const std::vector<Token>& tokens, std::size_t& i, CStatements& out, int indentLevel,
                         bool withNewline, const StatementContext& ctx) {
    ++i;  // consume write/writeln
    const auto args = parseExprList(tokens, i, ctx.funcNames, ctx.varParams, ctx.routineByRef);

    for (const auto& arg : args) {
        const std::string wtype = inferWriteType(arg, ctx.typeMap);
        if (wtype == "float") {
            emitStatementLine(out, indentLevel, "pas_write_real(" + arg + ");");
        } else if (wtype == "char") {
            emitStatementLine(out, indentLevel, "pas_write_char(" + arg + ");");
        } else {
            emitStatementLine(out, indentLevel, "pas_write_int(" + arg + ");");
        }
    }
    if (withNewline) {
        emitStatementLine(out, indentLevel, "pas_writeln();");
    }
}

// ---------------------------------------------------------------------------
// Assignment and call statements
// ---------------------------------------------------------------------------

// Collect LHS tokens until ':=' is found; returns the C-side LHS string.
std::string collectLhsUntilAssign(const std::vector<Token>& tokens, std::size_t& i) {
    std::vector<std::string> pieces;
    int bracketDepth = 0;
    while (i < tokens.size() && tokens[i].type != TokenType::Assign) {
        const TokenType t = tokens[i].type;
        if (t == TokenType::LBracket) ++bracketDepth;
        else if (t == TokenType::RBracket) --bracketDepth;
        // Multi-dim array index comma → "][" in C
        if (t == TokenType::Comma && bracketDepth > 0) {
            pieces.push_back("][");
            ++i;
            continue;
        }
        const std::string p = tokenToExprPiece(tokens[i]);
        if (!p.empty()) pieces.push_back(p);
        ++i;
    }
    if (i < tokens.size() && tokens[i].type == TokenType::Assign) ++i;  // consume ':='
    return joinPieces(pieces);
}

void parseAssignmentStatement(const std::vector<Token>& tokens, std::size_t& i, CStatements& out,
                               int indentLevel, const StatementContext& ctx) {
    const std::string lhs = collectLhsUntilAssign(tokens, i);
    const std::string rhs = parseExpressionUntil(tokens, i,
        {TokenType::Semicolon, TokenType::KwEnd, TokenType::KwElse},
        ctx.funcNames, ctx.varParams, ctx.routineByRef);

    // Detect function-return assignment: funcname := expr → return expr;
    if (!ctx.currentFunctionName.empty() && lhs == ctx.currentFunctionName) {
        emitStatementLine(out, indentLevel, "return " + (rhs.empty() ? "0" : rhs) + ";");
        return;
    }

    // If LHS is a simple VAR (by-ref) parameter, write through the pointer.
    std::string effectiveLhs = lhs;
    if (ctx.varParams && ctx.varParams->count(lhs) > 0) {
        effectiveLhs = "*" + lhs;
    }
    emitStatementLine(out, indentLevel, effectiveLhs + " = " + (rhs.empty() ? "0" : rhs) + ";");
}

void parseCallStatement(const std::vector<Token>& tokens, std::size_t& i, CStatements& out, int indentLevel,
                        const StatementContext& ctx) {
    const std::string name = tokens[i].lexeme;
    ++i;

    if (i < tokens.size() && tokens[i].type == TokenType::LParen) {
        // Check if this routine has VAR (by-ref) params that need & wrapping.
        std::vector<bool> byRef;
        bool hasVarParams = false;
        if (ctx.routineByRef) {
            auto it = ctx.routineByRef->find(name);
            if (it != ctx.routineByRef->end()) {
                byRef = it->second;
                for (bool b : byRef) if (b) { hasVarParams = true; break; }
            }
        }

        if (hasVarParams) {
            ++i;  // consume '('
            std::vector<std::string> callArgs;
            std::size_t argIdx = 0;
            while (i < tokens.size() && tokens[i].type != TokenType::RParen &&
                   tokens[i].type != TokenType::EndOfFile) {
                std::string arg = parseExpressionUntil(tokens, i,
                    {TokenType::Comma, TokenType::RParen},
                    ctx.funcNames, ctx.varParams, ctx.routineByRef);
                const bool isVarPos = argIdx < byRef.size() && byRef[argIdx];
                callArgs.push_back(isVarPos && !arg.empty() ? "& " + arg : arg);
                ++argIdx;
                if (i < tokens.size() && tokens[i].type == TokenType::Comma) ++i;
            }
            if (i < tokens.size() && tokens[i].type == TokenType::RParen) ++i;

            std::ostringstream callLine;
            callLine << name << "(";
            for (std::size_t k = 0; k < callArgs.size(); ++k) {
                if (k > 0) callLine << ", ";
                callLine << callArgs[k];
            }
            callLine << ");";
            emitStatementLine(out, indentLevel, callLine.str());
            return;
        }

        const auto args = parseExprList(tokens, i, ctx.funcNames, ctx.varParams, ctx.routineByRef);
        emitStatementLine(out, indentLevel, name + "(" + [&]() {
            std::ostringstream o;
            for (std::size_t k = 0; k < args.size(); ++k) {
                if (k > 0) o << ", ";
                o << args[k];
            }
            return o.str();
        }() + ");");
        return;
    }
    emitStatementLine(out, indentLevel, name + "();");
}

// Detect if the LHS starting at tokens[start] contains an array subscript or
// dot access before ':='.  Returns the index of ':=' or tokens.size() if not found.
std::size_t findAssignIndex(const std::vector<Token>& tokens, std::size_t start) {
    std::size_t j = start;
    int bracketDepth = 0;
    while (j < tokens.size()) {
        if (tokens[j].type == TokenType::LBracket) { ++bracketDepth; ++j; continue; }
        if (tokens[j].type == TokenType::RBracket) { --bracketDepth; ++j; continue; }
        if (tokens[j].type == TokenType::Dot) { j += 2; continue; }
        if (bracketDepth == 0 && tokens[j].type == TokenType::Assign) return j;
        if (tokens[j].type == TokenType::Semicolon || tokens[j].type == TokenType::KwEnd ||
            tokens[j].type == TokenType::KwElse || tokens[j].type == TokenType::KwThen ||
            tokens[j].type == TokenType::KwDo || tokens[j].type == TokenType::EndOfFile ||
            tokens[j].type == TokenType::LParen) {
            return tokens.size();
        }
        ++j;
    }
    return tokens.size();
}

void parseSingleStatement(const std::vector<Token>& tokens, std::size_t& i, CStatements& out, int indentLevel,
                          const StatementContext& ctx) {
    if (i >= tokens.size()) return;

    if (tokens[i].type == TokenType::KwBegin) {
        ++i;
        parseCompoundBody(tokens, i, out, indentLevel, ctx);
        return;
    }

    if (tokens[i].type == TokenType::KwIf) {
        parseIfStatement(tokens, i, out, indentLevel, ctx);
        return;
    }

    if (tokens[i].type == TokenType::KwWhile) {
        parseWhileStatement(tokens, i, out, indentLevel, ctx);
        return;
    }

    if (tokens[i].type == TokenType::KwFor) {
        parseForStatement(tokens, i, out, indentLevel, ctx);
        return;
    }

    if (tokens[i].type == TokenType::KwRepeat) {
        parseRepeatStatement(tokens, i, out, indentLevel, ctx);
        return;
    }

    if (tokens[i].type == TokenType::KwCase) {
        parseCaseStatement(tokens, i, out, indentLevel, ctx);
        return;
    }

    if (tokens[i].type == TokenType::KwRead || tokens[i].type == TokenType::KwReadLn) {
        parseReadStatement(tokens, i, out, indentLevel, ctx);
        return;
    }

    if (tokens[i].type == TokenType::KwWrite) {
        parseWriteStatement(tokens, i, out, indentLevel, false, ctx);
        return;
    }

    if (tokens[i].type == TokenType::KwWriteLn) {
        parseWriteStatement(tokens, i, out, indentLevel, true, ctx);
        return;
    }

    if (tokens[i].type == TokenType::Identifier) {
        // Determine if this is an assignment (possibly with subscripts) or a call
        const std::size_t assignIdx = findAssignIndex(tokens, i);
        if (assignIdx < tokens.size()) {
            parseAssignmentStatement(tokens, i, out, indentLevel, ctx);
            return;
        }
        parseCallStatement(tokens, i, out, indentLevel, ctx);
        return;
    }

    ++i;  // skip unknown token
}

// ---------------------------------------------------------------------------
// Main statements
// ---------------------------------------------------------------------------

CStatements parseMainStatements(const std::vector<Token>& tokens, std::size_t mainBeginIndex,
                                 const StatementContext& ctx) {
    CStatements stmts;
    if (mainBeginIndex >= tokens.size() || tokens[mainBeginIndex].type != TokenType::KwBegin) {
        return stmts;
    }

    std::size_t i = mainBeginIndex + 1;
    parseCompoundBody(tokens, i, stmts, 1, ctx);
    return stmts;
}

// ---------------------------------------------------------------------------
// Routine parsing
// ---------------------------------------------------------------------------

std::vector<RoutineParam> parseRoutineParams(const std::vector<Token>& tokens, std::size_t& i) {
    std::vector<RoutineParam> params;
    if (i >= tokens.size() || tokens[i].type != TokenType::LParen) {
        return params;
    }
    ++i;

    while (i < tokens.size() && tokens[i].type != TokenType::RParen && tokens[i].type != TokenType::EndOfFile) {
        bool byRef = false;
        if (tokens[i].type == TokenType::KwVar) {
            byRef = true;
            ++i;
        }

        std::vector<std::string> names;
        if (i < tokens.size() && tokens[i].type == TokenType::Identifier) {
            names.push_back(tokens[i].lexeme);
            ++i;
            while (i < tokens.size() && tokens[i].type == TokenType::Comma) {
                ++i;
                if (i < tokens.size() && tokens[i].type == TokenType::Identifier) {
                    names.push_back(tokens[i].lexeme);
                    ++i;
                }
            }
        }

        if (i >= tokens.size() || tokens[i].type != TokenType::Colon) {
            while (i < tokens.size() && tokens[i].type != TokenType::Semicolon &&
                   tokens[i].type != TokenType::RParen) {
                ++i;
            }
            if (i < tokens.size() && tokens[i].type == TokenType::Semicolon) ++i;
            continue;
        }
        ++i;  // consume ':'

        const std::string cType = (i < tokens.size()) ? mapTypeTokenToC(tokens[i].type) : "";
        if (i < tokens.size()) ++i;

        for (const auto& nm : names) {
            if (!cType.empty()) {
                params.push_back(RoutineParam{nm, cType, byRef});
            }
        }

        if (i < tokens.size() && tokens[i].type == TokenType::Semicolon) ++i;
    }

    if (i < tokens.size() && tokens[i].type == TokenType::RParen) ++i;
    return params;
}

bool parseRoutineAt(const std::vector<Token>& tokens, std::size_t start, RoutineDecl& outRoutine,
                    std::size_t& nextIndex, const StatementContext& baseCtx) {
    if (start >= tokens.size()) return false;
    if (tokens[start].type != TokenType::KwProcedure && tokens[start].type != TokenType::KwFunction) {
        return false;
    }

    RoutineDecl routine;
    routine.isFunction = (tokens[start].type == TokenType::KwFunction);
    routine.returnType = routine.isFunction ? "int" : "void";

    std::size_t i = start + 1;
    if (i < tokens.size() && tokens[i].type == TokenType::Identifier) {
        routine.name = tokens[i].lexeme;
        ++i;
    } else {
        routine.name = "pas_routine";
    }

    if (i < tokens.size() && tokens[i].type == TokenType::LParen) {
        routine.params = parseRoutineParams(tokens, i);
    }

    if (routine.isFunction && i < tokens.size() && tokens[i].type == TokenType::Colon) {
        ++i;
        if (i < tokens.size()) {
            const std::string mapped = mapTypeTokenToC(tokens[i].type);
            if (!mapped.empty()) {
                routine.returnType = mapped;
            }
            ++i;
        }
    }

    // Skip to ';' after header
    while (i < tokens.size() && tokens[i].type != TokenType::Semicolon &&
           tokens[i].type != TokenType::EndOfFile) {
        ++i;
    }
    if (i < tokens.size() && tokens[i].type == TokenType::Semicolon) ++i;

    const std::size_t declStart = i;
    while (i < tokens.size() && tokens[i].type != TokenType::KwBegin &&
           tokens[i].type != TokenType::EndOfFile) {
        ++i;
    }
    if (i >= tokens.size() || tokens[i].type != TokenType::KwBegin) {
        nextIndex = i;
        outRoutine = routine;
        return true;
    }

    const std::size_t bodyBeginIndex = i;
    routine.localDecls = parseVarDeclsInRange(tokens, declStart, bodyBeginIndex);

    // Build per-routine type map (params + locals)
    std::unordered_map<std::string, std::string> localTypeMap;
    if (baseCtx.typeMap != nullptr) {
        localTypeMap = *baseCtx.typeMap;
    }
    for (const auto& p : routine.params) {
        localTypeMap[p.name] = p.cType;
    }
    for (const auto& v : routine.localDecls) {
        localTypeMap[v.name] = v.cType;
    }

    // Collect by-ref (VAR) parameter names for the body context.
    std::unordered_set<std::string> routineVarParams;
    for (const auto& p : routine.params) {
        if (p.byRef) routineVarParams.insert(p.name);
    }

    StatementContext routineCtx;
    routineCtx.currentFunctionName = routine.isFunction ? routine.name : std::string{};
    routineCtx.funcNames = baseCtx.funcNames;
    routineCtx.typeMap = &localTypeMap;
    routineCtx.varParams = routineVarParams.empty() ? nullptr : &routineVarParams;
    routineCtx.routineByRef = baseCtx.routineByRef;

    std::size_t bodyCursor = bodyBeginIndex + 1;
    parseCompoundBody(tokens, bodyCursor, routine.body, 1, routineCtx);

    // Ensure functions always have a return statement
    bool hasReturn = false;
    for (const auto& line : routine.body) {
        if (line.find("return ") != std::string::npos) {
            hasReturn = true;
            break;
        }
    }
    if (routine.isFunction && !hasReturn) {
        routine.body.push_back("    return 0;");
    }

    nextIndex = bodyCursor;
    if (nextIndex < tokens.size() && tokens[nextIndex].type == TokenType::Semicolon) {
        ++nextIndex;
    }

    outRoutine = routine;
    return true;
}

RoutineParseResult parseRoutinesAndMain(const std::vector<Token>& tokens,
                                         const StatementContext& baseCtx) {
    RoutineParseResult result;

    std::size_t i = 0;
    if (i < tokens.size() && tokens[i].type == TokenType::KwProgram) {
        while (i < tokens.size() && tokens[i].type != TokenType::Semicolon &&
               tokens[i].type != TokenType::EndOfFile) {
            ++i;
        }
        if (i < tokens.size()) ++i;
    }

    while (i < tokens.size() && tokens[i].type != TokenType::EndOfFile) {
        if (tokens[i].type == TokenType::KwProcedure || tokens[i].type == TokenType::KwFunction) {
            RoutineDecl routine;
            std::size_t next = i + 1;
            if (parseRoutineAt(tokens, i, routine, next, baseCtx)) {
                result.routines.push_back(routine);
                i = next;
                continue;
            }
        }

        if (tokens[i].type == TokenType::KwBegin) {
            result.mainBeginIndex = i;
            break;
        }
        ++i;
    }

    return result;
}

// ---------------------------------------------------------------------------
// Code emission helpers
// ---------------------------------------------------------------------------

std::string formatRoutineParams(const std::vector<RoutineParam>& params) {
    std::ostringstream out;
    for (std::size_t i = 0; i < params.size(); ++i) {
        if (i > 0) out << ", ";
        out << params[i].cType;
        if (params[i].byRef) {
            out << " *";
        } else {
            out << " ";
        }
        out << params[i].name;
    }
    return out.str();
}

void emitRoutineDefinitions(std::ostringstream& out, const std::vector<RoutineDecl>& routines) {
    if (routines.empty()) {
        return;
    }

    for (const auto& routine : routines) {
        out << routine.returnType << " " << routine.name << "(" << formatRoutineParams(routine.params) << ") {\n";

        for (const auto& decl : routine.localDecls) {
            if (decl.isArray) {
                const std::string suffix = decl.arraySuffix.empty()
                    ? "[" + std::to_string(decl.arraySize > 0 ? decl.arraySize : 1) + "]"
                    : decl.arraySuffix;
                out << "    " << decl.cType << " " << decl.name << suffix << ";\n";
            } else {
                const std::string initVal = (decl.cType == "char") ? "'\\0'" :
                                            (decl.cType == "float" || decl.cType == "double") ? "0.0f" : "0";
                out << "    " << decl.cType << " " << decl.name << " = " << initVal << ";\n";
            }
        }

        if (!routine.localDecls.empty() && !routine.body.empty()) {
            out << "\n";
        }

        if (routine.body.empty()) {
            if (routine.isFunction) out << "    return 0;\n";
        } else {
            for (const auto& stmt : routine.body) {
                out << stmt << "\n";
            }
        }
        out << "}\n\n";
    }
}

}  // namespace

// ---------------------------------------------------------------------------
// Public interface
// ---------------------------------------------------------------------------

CodegenResult CodeGenerator::generateTemplate(const std::string& inputPath) const {
    CodegenResult result;

    // Extract stem (filename without extension) using string operations
    std::string stemStr = inputPath;
    {
        const auto slashPos = stemStr.find_last_of("/\\");
        if (slashPos != std::string::npos) stemStr = stemStr.substr(slashPos + 1);
        const auto dotPos = stemStr.rfind('.');
        if (dotPos != std::string::npos) stemStr = stemStr.substr(0, dotPos);
    }
    const std::string unitName = sanitizeIdentifier(stemStr);
    if (unitName.empty()) {
        result.ok = false;
        result.message = "Cannot derive valid C unit name from input path.";
        return result;
    }

    std::ifstream in(inputPath);
    if (!in.good()) {
        result.ok = false;
        result.message = "Failed to read source file for code generation.";
        return result;
    }
    std::ostringstream srcStream;
    srcStream << in.rdbuf();

    Lexer lexer;
    const LexerResult lexResult = lexer.tokenizeDetailed(srcStream.str());
    const auto& tokens = lexResult.tokens;

    // 1. Parse constants
    const auto constDecls = parseGlobalConstDecls(tokens);

    // 2. Parse global variables
    const auto globalVars = parseGlobalVarDecls(tokens);

    // 3. Build global type map (consts + globals)
    std::unordered_map<std::string, std::string> globalTypeMap;
    for (const auto& c : constDecls) {
        globalTypeMap[c.name] = c.cType;
    }
    for (const auto& v : globalVars) {
        globalTypeMap[v.name] = v.cType;
    }

    // 4. First pass: collect routine names and VAR param info.
    std::unordered_set<std::string> funcNames;
    std::unordered_map<std::string, std::vector<bool>> routineByRef;
    {
        for (std::size_t idx = 0; idx + 1 < tokens.size(); ++idx) {
            if ((tokens[idx].type == TokenType::KwFunction ||
                 tokens[idx].type == TokenType::KwProcedure) &&
                tokens[idx + 1].type == TokenType::Identifier) {
                const std::string nm = tokens[idx + 1].lexeme;
                funcNames.insert(nm);
                std::size_t j = idx + 2;
                if (j < tokens.size() && tokens[j].type == TokenType::LParen) {
                    const auto params = parseRoutineParams(tokens, j);
                    std::vector<bool> byRef;
                    for (const auto& p : params) byRef.push_back(p.byRef);
                    routineByRef[nm] = byRef;
                } else {
                    routineByRef[nm] = {};
                }
            }
        }
    }

    // 5. Build base context
    StatementContext baseCtx;
    baseCtx.funcNames = &funcNames;
    baseCtx.typeMap = &globalTypeMap;
    baseCtx.routineByRef = &routineByRef;

    // 6. Parse routines and find main begin
    const auto routineParse = parseRoutinesAndMain(tokens, baseCtx);

    // Update type map with function return types
    for (const auto& r : routineParse.routines) {
        if (r.isFunction) {
            globalTypeMap[r.name] = r.returnType;
        }
    }

    // 7. Parse main body
    const auto statements = parseMainStatements(tokens, routineParse.mainBeginIndex, baseCtx);

    // ---------------------------------------------------------------------------
    // Emit C source
    // ---------------------------------------------------------------------------
    std::ostringstream out;
    out << "/* Auto-generated by pascc. */\n\n";
    out << "#include <stdio.h>\n";
    out << "#include <stdbool.h>\n\n";

    out << "/* Runtime I/O helpers */\n";
    out << "static int    pas_read_int(void)    { int v = 0; (void)scanf(\"%d\", &v); return v; }\n";
    out << "static float  pas_read_real(void)   { float v = 0.0f; (void)scanf(\"%f\", &v); return v; }\n";
    out << "static char   pas_read_char(void)   { char v = 0; (void)scanf(\" %c\", &v); return v; }\n";
    out << "static void   pas_write_int(int v)  { (void)printf(\"%d\", v); }\n";
    out << "static void   pas_write_real(float v)  { (void)printf(\"%f\", v); }\n";
    out << "static void   pas_write_char(char v)   { (void)printf(\"%c\", v); }\n";
    out << "static void   pas_writeln(void)     { (void)printf(\"\\n\"); }\n\n";

    // Constants: use #define for int/char (needed for switch case labels in C),
    // and static const float for real constants.
    if (!constDecls.empty()) {
        out << "/* Constants */\n";
        for (const auto& c : constDecls) {
            if (c.cType == "float") {
                out << "static const float " << c.name << " = " << c.cValue << "f;\n";
            } else {
                // int / char: use #define so they can appear in switch case labels
                out << "#define " << c.name << " " << c.cValue << "\n";
            }
        }
        out << "\n";
    }

    // Global variables
    if (!globalVars.empty()) {
        out << "/* Global variables */\n";
        for (const auto& v : globalVars) {
            if (v.isArray) {
                const std::string suffix = v.arraySuffix.empty()
                    ? "[" + std::to_string(v.arraySize > 0 ? v.arraySize : 1) + "]"
                    : v.arraySuffix;
                out << v.cType << " " << v.name << suffix << ";\n";
            } else {
                const std::string initVal = (v.cType == "char") ? "'\\0'" :
                                            (v.cType == "float" || v.cType == "double") ? "0.0f" : "0";
                out << v.cType << " " << v.name << " = " << initVal << ";\n";
            }
        }
        out << "\n";
    }

    // Forward-declare routines
    if (!routineParse.routines.empty()) {
        out << "/* Forward declarations */\n";
        for (const auto& r : routineParse.routines) {
            out << r.returnType << " " << r.name << "(" << formatRoutineParams(r.params) << ");\n";
        }
        out << "\n";
    }

    // Routine definitions
    if (!routineParse.routines.empty()) {
        out << "/* Routine definitions */\n";
        emitRoutineDefinitions(out, routineParse.routines);
    }

    // main()
    out << "int main(void) {\n";
    if (statements.empty()) {
        out << "    /* (empty program body) */\n";
    } else {
        for (const auto& stmt : statements) {
            out << stmt << "\n";
        }
    }
    out << "\n    return 0;\n}\n";

    result.ok = true;
    result.message = "Code generated.";
    result.cSource = out.str();
    return result;
}

std::string CodeGenerator::sanitizeIdentifier(const std::string& rawName) {
    std::string out;
    out.reserve(rawName.size() + 4);

    for (char c : rawName) {
        if (std::isalnum(static_cast<unsigned char>(c)) || c == '_') {
            out.push_back(c);
        } else {
            out.push_back('_');
        }
    }

    if (out.empty()) return out;
    if (std::isdigit(static_cast<unsigned char>(out.front()))) {
        out.insert(out.begin(), '_');
    }
    return out;
}
