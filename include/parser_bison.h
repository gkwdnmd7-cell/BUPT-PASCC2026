#ifndef PARSER_BISON_H
#define PARSER_BISON_H

#include <vector>

#include "parser.h"
#include "token.h"

ParserResult parseWithBison(const std::vector<Token>& tokens);

#endif
