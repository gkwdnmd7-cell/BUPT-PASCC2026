#include <iostream>
#include <string>

#include "lexer.h"
#include "parser.h"
#include "semantic_declaration.h"

namespace {
bool expect(bool cond, const std::string& name) {
    if (!cond) {
        std::cerr << "[FAIL] " << name << std::endl;
        return false;
    }
    std::cout << "[PASS] " << name << std::endl;
    return true;
}

bool hasErrorCode(const SemanticResult& result, const std::string& code) {
    for (const auto& e : result.errors) {
        if (e.code == code) {
            return true;
        }
    }
    return false;
}
}  // namespace

int main() {
    bool ok = true;
    Lexer lexer;
    Parser parser;
    SemanticDeclarationAnalyzer analyzer;

    {
        const std::string src =
            "program demo;\n"
            "var x: integer;\n"
            "procedure p(a: integer; var b: integer);\n"
            "begin\n"
            "end;\n"
            "begin\n"
            "  p(1)\n"
            "end.\n";

        const auto lex = lexer.tokenizeDetailed(src);
        ok &= expect(lex.errors.empty(), "param_count_mismatch_lex_ok");
        const auto parsed = parser.parse(lex.tokens);
        ok &= expect(parsed.errors.empty(), "param_count_mismatch_parse_ok");
        const auto semantic = analyzer.analyze(lex.tokens, parsed);
        ok &= expect(hasErrorCode(semantic, "S202"), "param_count_mismatch_has_s202");
    }

    {
        const std::string src =
            "program demo;\n"
            "procedure p(a: integer);\n"
            "begin\n"
            "end;\n"
            "begin\n"
            "  p(true)\n"
            "end.\n";

        const auto lex = lexer.tokenizeDetailed(src);
        ok &= expect(lex.errors.empty(), "param_value_type_mismatch_lex_ok");
        const auto parsed = parser.parse(lex.tokens);
        ok &= expect(parsed.errors.empty(), "param_value_type_mismatch_parse_ok");
        const auto semantic = analyzer.analyze(lex.tokens, parsed);
        ok &= expect(hasErrorCode(semantic, "S202"), "param_value_type_mismatch_has_s202");
    }

    {
        const std::string src =
            "program demo;\n"
            "var x: integer;\n"
            "procedure p(var a: integer);\n"
            "begin\n"
            "end;\n"
            "begin\n"
            "  p(x + 1)\n"
            "end.\n";

        const auto lex = lexer.tokenizeDetailed(src);
        ok &= expect(lex.errors.empty(), "param_ref_lvalue_required_lex_ok");
        const auto parsed = parser.parse(lex.tokens);
        ok &= expect(parsed.errors.empty(), "param_ref_lvalue_required_parse_ok");
        const auto semantic = analyzer.analyze(lex.tokens, parsed);
        ok &= expect(hasErrorCode(semantic, "S202"), "param_ref_lvalue_required_has_s202");
    }

    {
        const std::string src =
            "program demo;\n"
            "var x: integer;\n"
            "procedure p(a: integer; var b: integer);\n"
            "begin\n"
            "end;\n"
            "begin\n"
            "  x := 1;\n"
            "  p(3, x)\n"
            "end.\n";

        const auto lex = lexer.tokenizeDetailed(src);
        ok &= expect(lex.errors.empty(), "param_valid_call_lex_ok");
        const auto parsed = parser.parse(lex.tokens);
        ok &= expect(parsed.errors.empty(), "param_valid_call_parse_ok");
        const auto semantic = analyzer.analyze(lex.tokens, parsed);
        ok &= expect(!hasErrorCode(semantic, "S202"), "param_valid_call_no_s202");
    }

    return ok ? 0 : 1;
}
