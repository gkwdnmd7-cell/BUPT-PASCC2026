#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>

#include "token.h"

struct ParseError {
    std::string code;
    std::string message;
    SourcePosition pos;
};

struct ParserResult {
    std::vector<ParseError> errors;
};

class Parser {
public:
    ParserResult parse(const std::vector<Token>& tokens);
};

#endif
