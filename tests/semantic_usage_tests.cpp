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

bool hasErrorAt(const SemanticResult& result, const std::string& code, int line) {
    for (const auto& e : result.errors) {
        if (e.code == code && e.pos.line == line) {
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
            "begin\n"
            "  y := 1\n"
            "end.\n";

        const auto lex = lexer.tokenizeDetailed(src);
        ok &= expect(lex.errors.empty(), "usage_undef_var_lex_ok");
        const auto parsed = parser.parse(lex.tokens);
        ok &= expect(parsed.errors.empty(), "usage_undef_var_parse_ok");
        const auto semantic = analyzer.analyze(lex.tokens, parsed);
        ok &= expect(hasErrorCode(semantic, "S103"), "usage_undef_var_has_s103");
        ok &= expect(hasErrorAt(semantic, "S103", 4), "usage_undef_var_line_recorded");
    }

    {
        const std::string src =
            "program demo;\n"
            "var x: integer;\n"
            "begin\n"
            "  x := 1\n"
            "end.\n";

        const auto lex = lexer.tokenizeDetailed(src);
        ok &= expect(lex.errors.empty(), "usage_defined_var_lex_ok");
        const auto parsed = parser.parse(lex.tokens);
        ok &= expect(parsed.errors.empty(), "usage_defined_var_parse_ok");
        const auto semantic = analyzer.analyze(lex.tokens, parsed);
        ok &= expect(!hasErrorCode(semantic, "S103"), "usage_defined_var_no_s103");
    }

    {
        const std::string src =
            "program demo;\n"
            "procedure p;\n"
            "begin\n"
            "end;\n"
            "begin\n"
            "  p\n"
            "end.\n";

        const auto lex = lexer.tokenizeDetailed(src);
        ok &= expect(lex.errors.empty(), "usage_defined_proc_lex_ok");
        const auto parsed = parser.parse(lex.tokens);
        ok &= expect(parsed.errors.empty(), "usage_defined_proc_parse_ok");
        const auto semantic = analyzer.analyze(lex.tokens, parsed);
        ok &= expect(!hasErrorCode(semantic, "S103"), "usage_defined_proc_no_s103");
    }

    {
        const std::string src =
            "program demo;\n"
            "begin\n"
            "  p\n"
            "end.\n";

        const auto lex = lexer.tokenizeDetailed(src);
        ok &= expect(lex.errors.empty(), "usage_undef_proc_lex_ok");
        const auto parsed = parser.parse(lex.tokens);
        ok &= expect(parsed.errors.empty(), "usage_undef_proc_parse_ok");
        const auto semantic = analyzer.analyze(lex.tokens, parsed);
        ok &= expect(hasErrorCode(semantic, "S103"), "usage_undef_proc_has_s103");
        ok &= expect(hasErrorAt(semantic, "S103", 3), "usage_undef_proc_line_recorded");
    }

    return ok ? 0 : 1;
}
