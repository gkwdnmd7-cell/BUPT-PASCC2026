#include <iostream>
#include <string>

#include "compiler_driver.h"
#include "error_codes.h"

namespace {
void printUsage() {
    std::cerr << "Usage: pascc -i <input.pas>" << std::endl;
}
}  // namespace

int main(int argc, char* argv[]) {
    if (argc != 3 || std::string(argv[1]) != "-i") {
        printUsage();
        return toExitCode(ErrorCode::Usage);
    }

    const std::string inputPath = argv[2];
    CompilerDriver driver;
    return driver.run(inputPath);
}
