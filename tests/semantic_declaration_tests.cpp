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
            "var x: integer; x: real;\n"
            "begin\n"
            "  x := 1\n"
            "end.\n";

        const auto lex = lexer.tokenizeDetailed(src);
        ok &= expect(lex.errors.empty(), "decl_dup_same_scope_lex_ok");
        const auto parsed = parser.parse(lex.tokens);
        ok &= expect(parsed.errors.empty(), "decl_dup_same_scope_parse_ok");
        const auto semantic = analyzer.analyze(lex.tokens, parsed);
        ok &= expect(hasErrorCode(semantic, "S101"), "decl_dup_same_scope_has_s101");
        ok &= expect(hasErrorAt(semantic, "S101", 2), "decl_dup_same_scope_line_recorded");
    }

    {
        const std::string src =
            "program demo;\n"
            "var x: integer;\n"
            "procedure p;\n"
            "var x: real;\n"
            "begin\n"
            "end;\n"
            "begin\n"
            "  x := 1\n"
            "end.\n";

        const auto lex = lexer.tokenizeDetailed(src);
        ok &= expect(lex.errors.empty(), "decl_shadowing_lex_ok");
        const auto parsed = parser.parse(lex.tokens);
        ok &= expect(parsed.errors.empty(), "decl_shadowing_parse_ok");
        const auto semantic = analyzer.analyze(lex.tokens, parsed);
        ok &= expect(!hasErrorCode(semantic, "S101"), "decl_shadowing_no_s101");
    }

    {
        const std::string src =
            "program demo;\n"
            "var y: mytype;\n"
            "begin\n"
            "  y := 1\n"
            "end.\n";

        const auto lex = lexer.tokenizeDetailed(src);
        ok &= expect(lex.errors.empty(), "decl_undefined_type_lex_ok");
        const auto parsed = parser.parse(lex.tokens);
        ok &= expect(parsed.errors.empty(), "decl_undefined_type_parse_ok");
        const auto semantic = analyzer.analyze(lex.tokens, parsed);
        ok &= expect(hasErrorCode(semantic, "S102"), "decl_undefined_type_has_s102");
        ok &= expect(hasErrorAt(semantic, "S102", 2), "decl_undefined_type_line_recorded");
    }

    return ok ? 0 : 1;
}
