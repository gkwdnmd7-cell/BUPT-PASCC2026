#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>

#include "token.h"

class Lexer {
public:
    // Stage-1 skeleton: tokenize the input source into coarse tokens.
    std::vector<Token> tokenize(const std::string& source);
};

#endif
