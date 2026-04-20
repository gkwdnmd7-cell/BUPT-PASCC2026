#ifndef PARSER_H
#define PARSER_H

#include <memory>
#include <string>
#include <vector>

#include "ast.h"
#include "token.h"

struct ParseError {
    std::string code;
    std::string message;
    SourcePosition pos;
};

struct ParserResult {
    std::vector<ParseError> errors;
    std::shared_ptr<ProgramNode> root;
};

class Parser {
public:
    ParserResult parse(const std::vector<Token>& tokens);
};

#endif
