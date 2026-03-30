#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>

#include "token.h"

struct LexError {
    std::string code;
    std::string message;
    SourcePosition pos;
};

struct LexerResult {
    std::vector<Token> tokens;
    std::vector<LexError> errors;
};

class Lexer {
public:
    // Stage-1 compatibility API.
    std::vector<Token> tokenize(const std::string& source);

    // Preferred API with lexical diagnostics.
    LexerResult tokenizeDetailed(const std::string& source);
};

#endif
