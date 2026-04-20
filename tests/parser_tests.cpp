#include <iostream>
#include <string>

#include "lexer.h"
#include "parser.h"

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
}  // namespace

int main() {
    Lexer lexer;
    Parser parser;
    bool ok = true;

    {
        const std::string src =
            "program demo;\n"
            "begin\n"
            "  x := 1;\n"
            "  write(x)\n"
            "end.\n";

        const auto lex = lexer.tokenizeDetailed(src);
        ok &= expect(lex.errors.empty(), "parser_valid_program_lex_ok");
        const auto parsed = parser.parse(lex.tokens);
        ok &= expect(parsed.errors.empty(), "parser_valid_program_no_syntax_error");
        ok &= expect(parsed.root != nullptr, "parser_valid_program_has_ast_root");
    }

    {
        const std::string src =
            "program demo\n"
            "begin\n"
            "  x := 1\n"
            "end.\n";

        const auto lex = lexer.tokenizeDetailed(src);
        ok &= expect(lex.errors.empty(), "parser_missing_semicolon_header_lex_ok");
        const auto parsed = parser.parse(lex.tokens);
        ok &= expect(!parsed.errors.empty(), "parser_missing_semicolon_header_error");
        ok &= expect(hasErrorCode(parsed, "E201"), "parser_e201_present");
        ok &= expect(parsed.root == nullptr, "parser_missing_semicolon_no_ast_root");
    }

    {
        const std::string src =
            "program demo;\n"
            "begin\n"
            "  x := ;\n"
            "end.\n";

        const auto lex = lexer.tokenizeDetailed(src);
        ok &= expect(lex.errors.empty(), "parser_invalid_expr_lex_ok");
        const auto parsed = parser.parse(lex.tokens);
        ok &= expect(!parsed.errors.empty(), "parser_invalid_expr_error");
        ok &= expect(hasErrorCode(parsed, "E201"), "parser_e201_present_for_invalid_expr");
        ok &= expect(parsed.root == nullptr, "parser_invalid_expr_no_ast_root");
    }

    {
        const std::string src =
            "program demo(input, output);\n"
            "const n = 3;\n"
            "var i: integer;\n"
            "begin\n"
            "  i := 0;\n"
            "  while i < n do\n"
            "  begin\n"
            "    i := i + 1;\n"
            "    write(i)\n"
            "  end\n"
            "end.\n";

        const auto lex = lexer.tokenizeDetailed(src);
        ok &= expect(lex.errors.empty(), "parser_expanded_grammar_lex_ok");
        const auto parsed = parser.parse(lex.tokens);
        ok &= expect(parsed.errors.empty(), "parser_expanded_grammar_no_syntax_error");
        ok &= expect(parsed.root != nullptr, "parser_expanded_grammar_has_ast_root");
    }

    return ok ? 0 : 1;
}
