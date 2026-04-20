#include "parser.h"

#include "parser_bison.h"

ParserResult Parser::parse(const std::vector<Token>& tokens) {
    return parseWithBison(tokens);
}
