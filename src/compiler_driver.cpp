#include "compiler_driver.h"

#include <filesystem>
#include <fstream>
#include <sstream>

#include "error_codes.h"
#include "lexer.h"
#include "log.h"
#include "parser.h"
#include "code_generator.h"
#include "semantic_declaration.h"

namespace fs = std::filesystem;

int CompilerDriver::run(const std::string& inputPath) {
    if (!hasPasExtension(inputPath)) {
        logutil::error("E001", "Input file must have .pas extension: " + inputPath);
        return toExitCode(ErrorCode::InvalidExtension);
    }

    fs::path source(inputPath);
    if (!fs::exists(source)) {
        logutil::error("E002", "Input file does not exist: " + inputPath);
        return toExitCode(ErrorCode::InputNotFound);
    }

    std::ifstream in(source);
    if (!in.good()) {
        logutil::error("E003", "Input file is not readable: " + inputPath);
        return toExitCode(ErrorCode::InputUnreadable);
    }

    std::ostringstream buffer;
    buffer << in.rdbuf();
    const std::string sourceCode = buffer.str();

    Lexer lexer;
    const LexerResult lexResult = lexer.tokenizeDetailed(sourceCode);

    if (!lexResult.errors.empty()) {
        for (const auto& err : lexResult.errors) {
            logutil::error(err.code, "(" + std::to_string(err.pos.line) + ":" + std::to_string(err.pos.column) + ") " + err.message);
        }
        logutil::error("E199", "Lexical analysis failed with " + std::to_string(lexResult.errors.size()) + " error(s).");
        return toExitCode(ErrorCode::LexicalError);
    }

    Parser parser;
    const ParserResult parserResult = parser.parse(lexResult.tokens);
    if (!parserResult.errors.empty()) {
        for (const auto& err : parserResult.errors) {
            logutil::error(err.code, "(" + std::to_string(err.pos.line) + ":" + std::to_string(err.pos.column) + ") " + err.message);
        }
        logutil::error("E299", "Syntax analysis failed with " + std::to_string(parserResult.errors.size()) + " error(s).");
        return toExitCode(ErrorCode::SyntaxError);
    }

    SemanticDeclarationAnalyzer semanticAnalyzer;
    const SemanticResult semanticResult = semanticAnalyzer.analyze(lexResult.tokens, parserResult);
    if (!semanticResult.errors.empty()) {
        for (const auto& err : semanticResult.errors) {
            logutil::error(err.code, "(" + std::to_string(err.pos.line) + ":" + std::to_string(err.pos.column) + ") " + err.message);
        }
        logutil::error("E399", "Semantic analysis failed with " + std::to_string(semanticResult.errors.size()) + " error(s).");
        return toExitCode(ErrorCode::SemanticError);
    }

    const std::string outputPath = deriveOutputPath(inputPath);
    std::ofstream out(outputPath, std::ios::trunc);
    if (!out.good()) {
        logutil::error("E004", "Failed to create output .c file: " + outputPath);
        return toExitCode(ErrorCode::OutputCreateFailed);
    }

    CodeGenerator codeGenerator;
    const CodegenResult codegenResult = codeGenerator.generateTemplate(inputPath);
    if (!codegenResult.ok) {
        logutil::error("E499", "Code generation failed: " + codegenResult.message);
        return toExitCode(ErrorCode::GenerationError);
    }

    out << codegenResult.cSource;

    logutil::info("Input:  " + source.string());
    logutil::info("Output: " + outputPath);
    logutil::info("Tokenized items: " + std::to_string(lexResult.tokens.size()));
    logutil::info("Syntax stage: passed");
    logutil::info("Semantic stage: passed");
    logutil::info("Codegen stage: template emitted");
    return toExitCode(ErrorCode::Ok);
}

std::string CompilerDriver::deriveOutputPath(const std::string& inputPath) {
    fs::path source(inputPath);
    source.replace_extension(".c");
    return source.string();
}

bool CompilerDriver::hasPasExtension(const std::string& path) {
    fs::path p(path);
    return p.has_extension() && p.extension() == ".pas";
}
