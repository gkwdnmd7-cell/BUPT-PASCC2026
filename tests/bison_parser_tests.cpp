#include <iostream>
#include <string>

#include "lexer.h"
#include "parser_bison.h"

namespace {
bool expect(bool cond, const std::string& name) {
    if (!cond) {
        std::cerr << "[FAIL] " << name << std::endl;
        return false;
    }
    std::cout << "[PASS] " << name << std::endl;
    return true;
}

bool hasErrorCode(const ParserResult& result, const std::string& code) {
    for (const auto& e : result.errors) {
        if (e.code == code) {
            return true;
        }
    }
    return false;
}

int countErrorCode(const ParserResult& result, const std::string& code) {
    int count = 0;
    for (const auto& e : result.errors) {
        if (e.code == code) {
            ++count;
        }
    }
    return count;
}
}  // namespace

int main() {
    Lexer lexer;
    bool ok = true;

    {
        const std::string src =
            "program demo;\n"
            "begin\n"
            "  x := 1;\n"
            "  write(x)\n"
            "end.\n";
        const auto lex = lexer.tokenizeDetailed(src);
        ok &= expect(lex.errors.empty(), "bison_valid_program_lex_ok");
        const auto parsed = parseWithBison(lex.tokens);
        ok &= expect(parsed.errors.empty(), "bison_valid_program_no_error");
        ok &= expect(parsed.root != nullptr, "bison_valid_program_has_ast_root");
    }

    {
        const std::string src =
            "program demo;\n"
            "var a1, b2: integer;\n"
            "begin\n"
            "  a1 := 12;\n"
            "  b2 := 34;\n"
            "  write(a1)\n"
            "end.\n";
        const auto lex = lexer.tokenizeDetailed(src);
        ok &= expect(lex.errors.empty(), "bison_var_section_lex_ok");
        const auto parsed = parseWithBison(lex.tokens);
        ok &= expect(parsed.errors.empty(), "bison_var_section_no_error");
        ok &= expect(parsed.root != nullptr, "bison_var_section_has_ast_root");
    }

    {
        const std::string src =
            "program demo;\n"
            "begin\n"
            "  x := 1;\n"
            "  write(x);\n"
            "end.\n";
        const auto lex = lexer.tokenizeDetailed(src);
        ok &= expect(lex.errors.empty(), "bison_trailing_semicolon_lex_ok");
        const auto parsed = parseWithBison(lex.tokens);
        ok &= expect(!parsed.errors.empty(), "bison_trailing_semicolon_error");
        ok &= expect(hasErrorCode(parsed, "E201"), "bison_trailing_semicolon_e201");
        ok &= expect(parsed.root == nullptr, "bison_error_program_no_ast_root");
    }

    {
        const std::string src =
            "program demo(input, output);\n"
            "const n = 10;\n"
            "var i: integer;\n"
            "begin\n"
            "  i := 0;\n"
            "  for i := 1 to n do\n"
            "    if i > 0 then\n"
            "      write(i)\n"
            "end.\n";
        const auto lex = lexer.tokenizeDetailed(src);
        ok &= expect(lex.errors.empty(), "bison_program_head_const_for_if_lex_ok");
        const auto parsed = parseWithBison(lex.tokens);
        ok &= expect(parsed.errors.empty(), "bison_program_head_const_for_if_no_error");
        ok &= expect(parsed.root != nullptr, "bison_program_head_const_for_if_has_ast_root");
    }

    {
        const std::string src =
            "program demo\n"
            "begin\n"
            "  x := 1;\n"
            "end.\n";
        const auto lex = lexer.tokenizeDetailed(src);
        ok &= expect(lex.errors.empty(), "bison_missing_semicolon_header_lex_ok");
        const auto parsed = parseWithBison(lex.tokens);
        ok &= expect(!parsed.errors.empty(), "bison_missing_semicolon_header_error");
        ok &= expect(hasErrorCode(parsed, "E201"), "bison_e201_present");
        ok &= expect(parsed.root == nullptr, "bison_missing_semicolon_no_ast_root");
    }

    {
        const std::string src =
            "program demo;\n"
            "begin\n"
            "  x := ;\n"
            "  write(;)\n"
            "end.\n";
        const auto lex = lexer.tokenizeDetailed(src);
        ok &= expect(lex.errors.empty(), "bison_recovery_metric_lex_ok");
        const auto parsed = parseWithBison(lex.tokens);
        ok &= expect(!parsed.errors.empty(), "bison_recovery_metric_has_error");
        ok &= expect(countErrorCode(parsed, "E201") >= 2, "bison_recovery_metric_at_least_two_e201");
    }

    return ok ? 0 : 1;
}
