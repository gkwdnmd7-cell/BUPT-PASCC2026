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
    std::vector<Token> tokenize(const std::string& source);
    LexerResult tokenizeDetailed(const std::string& source);
};

#endif
