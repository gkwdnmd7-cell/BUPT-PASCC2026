#ifndef SEMANTIC_DECLARATION_H
#define SEMANTIC_DECLARATION_H

#include <string>
#include <vector>

#include "parser.h"
#include "semantic_tables.h"
#include "token.h"

struct SemanticError {
    std::string code;
    std::string message;
    SourcePosition pos;
};

struct SemanticResult {
    std::vector<SemanticError> errors;
};

class SemanticDeclarationAnalyzer {
public:
    SemanticResult analyze(const std::vector<Token>& tokens, const ParserResult& parseResult);
};

#endif
