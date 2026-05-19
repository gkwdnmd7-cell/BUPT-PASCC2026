#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H

#include <string>

#include "parser.h"

struct CodegenResult {
    bool ok = false;
    std::string message;
    std::string cSource;
};

class CodeGenerator {
public:
    // Primary entry point: generate C code from a real AST.
    CodegenResult generate(const ParserResult& parseResult) const;

    // Convenience wrapper: lex + parse the given .pas file, then call generate().
    CodegenResult generateTemplate(const std::string& inputPath) const;

    // Convert a raw filename (with path and extension) to a valid C identifier.
    static std::string sanitizeIdentifier(const std::string& rawName);
};

#endif
