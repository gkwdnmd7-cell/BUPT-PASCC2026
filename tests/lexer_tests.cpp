#include <iostream>
#include <string>
#include <vector>

#include "lexer.h"

namespace {
bool hasErrorCode(const std::vector<LexError>& errors, const std::string& code) {
    for (const auto& err : errors) {
        if (err.code == code) {
            return true;
        }
    }
    return false;
}

bool expect(bool cond, const std::string& name) {
    if (!cond) {
        std::cerr << "[FAIL] " << name << std::endl;
        return false;
    }
    std::cout << "[PASS] " << name << std::endl;
    return true;
}
}  // namespace

int main() {
    Lexer lexer;
    bool ok = true;

    {
        const std::string src =
            "program demo\n"
            "var x, y: integer ;\n"
            "begin\n"
            "  { ok comment }\n"
            "  x := 12;\n"
            "  if x >= 1 then\n"
            "    write ( x )\n"
            "end.\n";
        const auto result = lexer.tokenizeDetailed(src);
        ok &= expect(result.errors.empty(), "valid_program_no_lex_errors");
    }

    {
        const std::string src = "program demo\nbegin\nwrite(x)\nend.\n";
        const auto result = lexer.tokenizeDetailed(src);
        ok &= expect(hasErrorCode(result.errors, "E108"), "keyword_boundary_error_detected");
    }

    {
        const std::string src = "program demo\nvar thisIdentifierNameTooLong: integer ;\nbegin\nend.\n";
        const auto result = lexer.tokenizeDetailed(src);
        ok &= expect(hasErrorCode(result.errors, "E105"), "identifier_length_error_detected");
    }

    {
        const std::string src = "program demo\nbegin\n  { nested { comment } still invalid }\nend.\n";
        const auto result = lexer.tokenizeDetailed(src);
        ok &= expect(hasErrorCode(result.errors, "E106"), "nested_comment_error_detected");
    }

    {
        const std::string src = "program demo\nbegin\n  x := 'ab';\nend.\n";
        const auto result = lexer.tokenizeDetailed(src);
        ok &= expect(hasErrorCode(result.errors, "E109"), "invalid_char_literal_detected");
    }

    return ok ? 0 : 1;
}
