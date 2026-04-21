#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include "compiler_driver.h"
#include "error_codes.h"

namespace fs = std::filesystem;

namespace {
bool expect(bool cond, const std::string& name) {
    if (!cond) {
        std::cerr << "[FAIL] " << name << std::endl;
        return false;
    }
    std::cout << "[PASS] " << name << std::endl;
    return true;
}

bool writeTextFile(const fs::path& path, const std::string& text) {
    std::ofstream out(path, std::ios::trunc);
    if (!out.good()) {
        return false;
    }
    out << text;
    return out.good();
}
}  // namespace

int main() {
    bool ok = true;
    CompilerDriver driver;

    const fs::path tempRoot = fs::temp_directory_path() / "pascc_semantic_gate_tests";
    std::error_code ec;
    fs::create_directories(tempRoot, ec);
    ok &= expect(!ec, "gate_prepare_temp_dir");

    {
        const fs::path input = tempRoot / "semantic_fail_case.pas";
        const fs::path output = tempRoot / "semantic_fail_case.c";
        fs::remove(output, ec);

        const std::string src =
            "program demo;\n"
            "procedure p(a: integer);\n"
            "begin\n"
            "end;\n"
            "begin\n"
            "  p(true)\n"
            "end.\n";

        ok &= expect(writeTextFile(input, src), "gate_semantic_fail_write_input");
        const int exitCode = driver.run(input.string());

        ok &= expect(exitCode == toExitCode(ErrorCode::SemanticError), "gate_semantic_fail_exit_code_9");
        ok &= expect(!fs::exists(output), "gate_semantic_fail_no_output_file");
    }

    {
        const fs::path input = tempRoot / "semantic_pass_case.pas";
        const fs::path output = tempRoot / "semantic_pass_case.c";
        fs::remove(output, ec);

        const std::string src =
            "program demo;\n"
            "var x: integer;\n"
            "begin\n"
            "  x := 1\n"
            "end.\n";

        ok &= expect(writeTextFile(input, src), "gate_semantic_pass_write_input");
        const int exitCode = driver.run(input.string());

        ok &= expect(exitCode == toExitCode(ErrorCode::Ok), "gate_semantic_pass_exit_code_0");
        ok &= expect(fs::exists(output), "gate_semantic_pass_output_generated");
    }

    return ok ? 0 : 1;
}
