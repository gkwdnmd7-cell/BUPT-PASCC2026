#include "compiler_driver.h"

#include <cctype>
#include <fstream>
#include <sstream>

#include "error_codes.h"
#include "lexer.h"
#include "log.h"
#include "parser.h"
#include "code_generator.h"
#include "semantic_declaration.h"

int CompilerDriver::run(const std::string& inputPath) {
    if (!hasPasExtension(inputPath)) {
        logutil::error("E001", "Input file must have .pas extension: " + inputPath);
        return toExitCode(ErrorCode::InvalidExtension);
    }

    std::ifstream in(inputPath);
    if (!in.good()) {
        logutil::error("E002", "Input file does not exist or is not readable: " + inputPath);
        return toExitCode(ErrorCode::InputNotFound);
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
    const CodegenResult codegenResult = codeGenerator.generate(parserResult);
    if (!codegenResult.ok) {
        logutil::error("E499", "Code generation failed: " + codegenResult.message);
        return toExitCode(ErrorCode::GenerationError);
    }

    out << codegenResult.cSource;

    logutil::info("Input:  " + inputPath);
    logutil::info("Output: " + outputPath);
    logutil::info("Tokenized items: " + std::to_string(lexResult.tokens.size()));
    logutil::info("Syntax stage: passed");
    logutil::info("Semantic stage: passed");
    logutil::info("Codegen stage: template emitted");
    return toExitCode(ErrorCode::Ok);
}

std::string CompilerDriver::deriveOutputPath(const std::string& inputPath) {
    // Replace .pas (case-insensitive) with .c
    std::string result = inputPath;
    const std::string ext = ".pas";
    if (result.size() >= ext.size()) {
        std::string tail = result.substr(result.size() - ext.size());
        for (auto& c : tail) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        if (tail == ext) {
            result.replace(result.size() - ext.size(), ext.size(), ".c");
        }
    }
    return result;
}

bool CompilerDriver::hasPasExtension(const std::string& path) {
    if (path.size() < 4) return false;
    std::string tail = path.substr(path.size() - 4);
    for (auto& c : tail) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return tail == ".pas";
}
