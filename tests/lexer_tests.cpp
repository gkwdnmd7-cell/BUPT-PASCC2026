#include <iostream>
#include <string>
#include <vector>

#include "lexer.h"

namespace {
bool expect(bool cond, const std::string& name) {
    if (!cond) {
        std::cerr << "[FAIL] " << name << std::endl;
        return false;
    }
    std::cout << "[PASS] " << name << std::endl;
    return true;
}

bool hasToken(const std::vector<Token>& tokens, TokenType type, const std::string& lexeme) {
    for (const auto& t : tokens) {
        if (t.type == type && t.lexeme == lexeme) {
            return true;
        }
    }
    return false;
}

bool hasError(const std::vector<LexError>& errors, const std::string& code) {
    for (const auto& e : errors) {
        if (e.code == code) {
            return true;
        }
    }
    return false;
}
}  // namespace

int main() {
    Lexer lexer;
    bool ok = true;

    {
        const std::string src =
            "PROGRAM demo\n"
            "TYPE idx = integer;\n"
            "var a1, b2 : integer;\n"
            "begin\n"
            "  readln(a1);\n"
            "  a1 := 12;\n"
            "  b2 := 12.34;\n"
            "  for a1 := 10 downto 1 do\n"
            "    writeln(a1);\n"
            "  write(a1);\n"
            "end.\n";
        const auto result = lexer.tokenizeDetailed(src);
        ok &= expect(result.errors.empty(), "valid_program_no_lex_errors");
        ok &= expect(hasToken(result.tokens, TokenType::KwProgram, "program"), "program_keyword_detected");
        ok &= expect(hasToken(result.tokens, TokenType::KwType, "type"), "type_keyword_detected");
        ok &= expect(hasToken(result.tokens, TokenType::Identifier, "a1"), "identifier_detected");
        ok &= expect(hasToken(result.tokens, TokenType::KwReadLn, "readln"), "readln_keyword_detected");
        ok &= expect(hasToken(result.tokens, TokenType::KwDownTo, "downto"), "downto_keyword_detected");
        ok &= expect(hasToken(result.tokens, TokenType::KwWriteLn, "writeln"), "writeln_keyword_detected");
        ok &= expect(hasToken(result.tokens, TokenType::RealLiteral, "12.34"), "real_literal_detected");
    }

    {
        const std::string src = "abcdefgh9";
        const auto result = lexer.tokenizeDetailed(src);
        ok &= expect(hasError(result.errors, "E105"), "identifier_length_limit_8");
    }

    {
        const std::string src = "begin { nested { comment } still } end";
        const auto result = lexer.tokenizeDetailed(src);
        ok &= expect(hasError(result.errors, "E106"), "nested_comment_error");
    }

    {
        const std::string src = "begin { unterminated";
        const auto result = lexer.tokenizeDetailed(src);
        ok &= expect(hasError(result.errors, "E107"), "unterminated_comment_error");
    }

    {
        const std::string src = "x := 'ab'";
        const auto result = lexer.tokenizeDetailed(src);
        ok &= expect(hasError(result.errors, "E109"), "invalid_char_literal_error");
    }

    {
        const std::string src = "_bad";
        const auto result = lexer.tokenizeDetailed(src);
        ok &= expect(hasError(result.errors, "E110"), "unknown_character_error");
    }

    return ok ? 0 : 1;
}
