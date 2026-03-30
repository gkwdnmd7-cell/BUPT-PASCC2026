#ifndef COMPILER_DRIVER_H
#define COMPILER_DRIVER_H

#include <string>

class CompilerDriver {
public:
    int run(const std::string& inputPath);

private:
    static std::string deriveOutputPath(const std::string& inputPath);
    static bool hasPasExtension(const std::string& path);
};

#endif
