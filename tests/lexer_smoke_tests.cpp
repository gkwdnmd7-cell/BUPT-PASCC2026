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

bool containsToken(const std::vector<Token>& tokens, TokenType type, const std::string& lexeme) {
    for (const auto& t : tokens) {
        if (t.type == type && t.lexeme == lexeme) {
            return true;
        }
    }
    return false;
}

bool containsError(const std::vector<LexError>& errors, const std::string& code) {
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
        const std::string src = "PROGRAM abc123 begin end";
        const auto result = lexer.tokenizeDetailed(src);
        ok &= expect(result.errors.empty(), "keyword_and_identifier_no_error");
        ok &= expect(containsToken(result.tokens, TokenType::KwProgram, "program"), "program_keyword_case_insensitive");
        ok &= expect(containsToken(result.tokens, TokenType::Identifier, "abc123"), "identifier_letter_digit_pattern");
    }

    {
        const std::string src = "abcdefgh9";
        const auto result = lexer.tokenizeDetailed(src);
        ok &= expect(containsError(result.errors, "E105"), "identifier_length_limit_8");
    }

    {
        const std::string src = "a := 12 .. b <= c >= d <> e + - * / ( ) [ ] , ; : . = < >";
        const auto result = lexer.tokenizeDetailed(src);
        ok &= expect(result.errors.empty(), "operators_and_delimiters_no_error");
        ok &= expect(containsToken(result.tokens, TokenType::Assign, ":="), "assign_detected");
        ok &= expect(containsToken(result.tokens, TokenType::DotDot, ".."), "dotdot_detected");
        ok &= expect(containsToken(result.tokens, TokenType::LessEqual, "<="), "le_detected");
        ok &= expect(containsToken(result.tokens, TokenType::GreaterEqual, ">="), "ge_detected");
        ok &= expect(containsToken(result.tokens, TokenType::NotEqual, "<>"), "ne_detected");
    }

    {
        const std::string src = "_abc";
        const auto result = lexer.tokenizeDetailed(src);
        ok &= expect(containsError(result.errors, "E110"), "underscore_start_rejected");
    }

    {
        const std::string src = "{ comment } a { nested { bad } still }";
        const auto result = lexer.tokenizeDetailed(src);
        ok &= expect(containsToken(result.tokens, TokenType::Identifier, "a"), "comment_skipped");
        ok &= expect(containsError(result.errors, "E106"), "nested_comment_error");
    }

    {
        const std::string src = "{ not_closed";
        const auto result = lexer.tokenizeDetailed(src);
        ok &= expect(containsError(result.errors, "E107"), "unterminated_comment_error");
    }

    {
        const std::string src = "x := 12.34 y := 'a' z := 'ab'";
        const auto result = lexer.tokenizeDetailed(src);
        ok &= expect(containsToken(result.tokens, TokenType::RealLiteral, "12.34"), "real_literal_detected");
        ok &= expect(containsToken(result.tokens, TokenType::CharLiteral, "'a'"), "char_literal_detected");
        ok &= expect(containsError(result.errors, "E109"), "invalid_char_literal_error");
    }

    return ok ? 0 : 1;
}
