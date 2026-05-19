#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>

#include "code_generator.h"

namespace {
bool expect(bool cond, const std::string& name) {
    if (!cond) {
        std::cerr << "[FAIL] " << name << std::endl;
        return false;
    }
    std::cout << "[PASS] " << name << std::endl;
    return true;
}

bool contains(const std::string& haystack, const std::string& needle) {
    return haystack.find(needle) != std::string::npos;
}
}  // namespace

int main() {
    bool ok = true;
    CodeGenerator generator;
    namespace fs = std::filesystem;

    {
        const auto result = generator.generateTemplate("../samples/01_features.pas");
        ok &= expect(result.ok, "codegen_template_basic_ok");
        ok &= expect(contains(result.cSource, "#include <stdio.h>"), "codegen_template_has_stdio");
        ok &= expect(contains(result.cSource, "#include <stdbool.h>"), "codegen_template_has_stdbool");
        ok &= expect(contains(result.cSource, "int main(void)"), "codegen_template_has_main");
        ok &= expect(contains(result.cSource, "int a1 = 0;"), "codegen_template_translates_var_decl_a1");
        ok &= expect(contains(result.cSource, "int b2 = 0;"), "codegen_template_translates_var_decl_b2");
        ok &= expect(contains(result.cSource, "a1 = 12;"), "codegen_template_translates_assign_a1");
        ok &= expect(contains(result.cSource, "b2 = 34;"), "codegen_template_translates_assign_b2");
        ok &= expect(contains(result.cSource, "assignment: x = expr;"), "codegen_template_covers_assignment");
        ok &= expect(contains(result.cSource, "if/else: if (cond)"), "codegen_template_covers_if");
        ok &= expect(contains(result.cSource, "while: while (cond)"), "codegen_template_covers_while");
        ok &= expect(contains(result.cSource, "for: for (i = begin; i <= end; ++i)"), "codegen_template_covers_for");
        ok &= expect(contains(result.cSource, "read/readln: x = pas_read_int();"), "codegen_template_covers_read");
        ok &= expect(contains(result.cSource, "write/writeln: pas_write_int(expr); pas_writeln();"), "codegen_template_covers_write");
    }

    {
        const auto result = generator.generateTemplate("../samples/03_controlflow.pas");
        ok &= expect(result.ok, "codegen_controlflow_basic_ok");
        ok &= expect(contains(result.cSource, "while (i < 3) {"), "codegen_controlflow_while");
        ok &= expect(contains(result.cSource, "if (i == 1) {"), "codegen_controlflow_if");
        ok &= expect(contains(result.cSource, "else {"), "codegen_controlflow_else");
        ok &= expect(contains(result.cSource, "for (i = 1; i <= 2; ++i) {"), "codegen_controlflow_for_to");
        ok &= expect(contains(result.cSource, "for (i = 3; i >= 1; --i) {"), "codegen_controlflow_for_downto");
        ok &= expect(contains(result.cSource, "sum = sum + i;"), "codegen_controlflow_has_sum_update");
        ok &= expect(!contains(result.cSource, "i = 1 2 sum"), "codegen_controlflow_no_concat_bug");
    }

    {
        const auto result = generator.generateTemplate("../samples/04_routines.pas");
        ok &= expect(result.ok, "codegen_routines_basic_ok");
        ok &= expect(contains(result.cSource, "void inc1(int a) {"), "codegen_routines_procedure_decl");
        ok &= expect(contains(result.cSource, "int add1(int v) {"), "codegen_routines_function_decl");
        ok &= expect(contains(result.cSource, "__ret_add1 = v + 1;"), "codegen_routines_function_return");
        ok &= expect(contains(result.cSource, "inc1(2);"), "codegen_routines_procedure_call");
        ok &= expect(contains(result.cSource, "x = add1 ( x );"), "codegen_routines_function_call_expr");
    }

    {
        const auto result = generator.generateTemplate("../samples/05_realmap.pas");
        ok &= expect(result.ok, "codegen_realmap_basic_ok");
        ok &= expect(contains(result.cSource, "float x = 0.0f;"), "codegen_realmap_global_real_decl");
        ok &= expect(contains(result.cSource, "float addh(float v) {"), "codegen_realmap_function_real_signature");
        ok &= expect(contains(result.cSource, "__ret_addh = v + 0.5;"), "codegen_realmap_function_real_return");
        ok &= expect(contains(result.cSource, "x = 1.25;"), "codegen_realmap_real_literal_assign");
        ok &= expect(contains(result.cSource, "x = addh ( x );"), "codegen_realmap_call_expr");
    }

    {
        const fs::path tempFile = fs::temp_directory_path() / "1-invalid-name.pas";
        {
            std::ofstream out(tempFile, std::ios::trunc);
            out << "program p; begin end.";
        }
        const auto result = generator.generateTemplate(tempFile.string());
        ok &= expect(result.ok, "codegen_template_sanitize_ok");
        ok &= expect(contains(result.cSource, "Source unit: _1_invalid_name"), "codegen_template_sanitize_name");
    }

    return ok ? 0 : 1;
}
