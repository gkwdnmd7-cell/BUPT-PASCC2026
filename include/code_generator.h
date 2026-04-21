#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H

#include <string>

struct CodegenResult {
    bool ok = false;
    std::string message;
    std::string cSource;
};

class CodeGenerator {
public:
    CodegenResult generateTemplate(const std::string& inputPath) const;

private:
    static std::string sanitizeIdentifier(const std::string& rawName);
};

#endif
