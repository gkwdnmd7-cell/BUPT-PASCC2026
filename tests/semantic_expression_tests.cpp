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
            "var b: boolean;\n"
            "begin\n"
            "  b := 1 + 2\n"
            "end.\n";

        const auto lex = lexer.tokenizeDetailed(src);
        ok &= expect(lex.errors.empty(), "expr_bool_assign_num_lex_ok");
        const auto parsed = parser.parse(lex.tokens);
        ok &= expect(parsed.errors.empty(), "expr_bool_assign_num_parse_ok");
        const auto semantic = analyzer.analyze(lex.tokens, parsed);
        ok &= expect(hasErrorCode(semantic, "S201"), "expr_bool_assign_num_has_s201");
        ok &= expect(hasErrorAt(semantic, "S201", 4), "expr_bool_assign_num_line_recorded");
    }

    {
        const std::string src =
            "program demo;\n"
            "var b: boolean; x: integer;\n"
            "begin\n"
            "  x := 1;\n"
            "  b := x > 0\n"
            "end.\n";

        const auto lex = lexer.tokenizeDetailed(src);
        ok &= expect(lex.errors.empty(), "expr_relation_bool_lex_ok");
        const auto parsed = parser.parse(lex.tokens);
        ok &= expect(parsed.errors.empty(), "expr_relation_bool_parse_ok");
        const auto semantic = analyzer.analyze(lex.tokens, parsed);
        ok &= expect(!hasErrorCode(semantic, "S201"), "expr_relation_bool_no_s201");
    }

    {
        const std::string src =
            "program demo;\n"
            "var i: integer; r: real;\n"
            "begin\n"
            "  r := 1;\n"
            "  i := 1.5\n"
            "end.\n";

        const auto lex = lexer.tokenizeDetailed(src);
        ok &= expect(lex.errors.empty(), "expr_real_to_int_assign_lex_ok");
        const auto parsed = parser.parse(lex.tokens);
        ok &= expect(parsed.errors.empty(), "expr_real_to_int_assign_parse_ok");
        const auto semantic = analyzer.analyze(lex.tokens, parsed);
        ok &= expect(hasErrorCode(semantic, "S201"), "expr_real_to_int_assign_has_s201");
        ok &= expect(hasErrorAt(semantic, "S201", 5), "expr_real_to_int_assign_line_recorded");
    }

    {
        const std::string src =
            "program demo;\n"
            "var x: integer;\n"
            "begin\n"
            "  x := true + 1\n"
            "end.\n";

        const auto lex = lexer.tokenizeDetailed(src);
        ok &= expect(lex.errors.empty(), "expr_bool_plus_num_lex_ok");
        const auto parsed = parser.parse(lex.tokens);
        ok &= expect(parsed.errors.empty(), "expr_bool_plus_num_parse_ok");
        const auto semantic = analyzer.analyze(lex.tokens, parsed);
        ok &= expect(hasErrorCode(semantic, "S201"), "expr_bool_plus_num_has_s201");
    }

    return ok ? 0 : 1;
}
