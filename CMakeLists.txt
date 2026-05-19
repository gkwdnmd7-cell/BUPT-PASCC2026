cmake_minimum_required(VERSION 3.16)
project(pascc LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()

find_program(BISON_EXECUTABLE NAMES win_bison bison)
if(NOT BISON_EXECUTABLE)
    message(FATAL_ERROR "Bison executable not found. Please install WinFlexBison.")
endif()

find_program(FLEX_EXECUTABLE NAMES win_flex flex)
if(NOT FLEX_EXECUTABLE)
    message(FATAL_ERROR "Flex executable not found. Please install WinFlexBison.")
endif()

set(BISON_OUTPUT_C ${CMAKE_CURRENT_BINARY_DIR}/parser_bison.tab.c)
set(BISON_OUTPUT_H ${CMAKE_CURRENT_BINARY_DIR}/parser_bison.tab.h)
set(FLEX_OUTPUT_C ${CMAKE_CURRENT_BINARY_DIR}/lexer_flex.cpp)
set(FLEX_OUTPUT_H ${CMAKE_CURRENT_BINARY_DIR}/lexer_flex.h)

add_custom_command(
    OUTPUT ${BISON_OUTPUT_C} ${BISON_OUTPUT_H}
    COMMAND ${BISON_EXECUTABLE} -d -o ${BISON_OUTPUT_C} ${CMAKE_CURRENT_SOURCE_DIR}/src/parser_bison.y
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/parser_bison.y
    VERBATIM
)

add_custom_command(
    OUTPUT ${FLEX_OUTPUT_C} ${FLEX_OUTPUT_H}
    COMMAND ${FLEX_EXECUTABLE} --header-file=${FLEX_OUTPUT_H} -o ${FLEX_OUTPUT_C} ${CMAKE_CURRENT_SOURCE_DIR}/src/lexer_flex.l
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/lexer_flex.l ${BISON_OUTPUT_H}
    VERBATIM
)

add_library(parser_bison
    ${BISON_OUTPUT_C}
    src/parser_bison_bridge.cpp
)

target_include_directories(parser_bison PUBLIC
    include
    ${CMAKE_CURRENT_BINARY_DIR}
)

add_library(lexer_frontend
    src/lexer.cpp
    ${FLEX_OUTPUT_C}
)

target_include_directories(lexer_frontend PUBLIC
    include
    ${CMAKE_CURRENT_BINARY_DIR}
)
if(WIN32)
    target_compile_definitions(lexer_frontend PRIVATE YY_NO_UNISTD_H=1)
endif()

add_executable(pascc
    src/main.cpp
    src/compiler_driver.cpp
    src/code_generator.cpp
    src/parser.cpp
    src/semantic_declaration.cpp
)

target_include_directories(pascc PRIVATE include)
target_link_libraries(pascc PRIVATE parser_bison lexer_frontend)

# Fully static link so the binary runs on systems with older glibc/libstdc++
if(NOT WIN32)
    target_link_options(pascc PRIVATE -static -static-libstdc++ -static-libgcc)
endif()

add_executable(lexer_smoke_tests
    tests/lexer_smoke_tests.cpp
    src/parser.cpp
)

target_include_directories(lexer_smoke_tests PRIVATE include)
target_link_libraries(lexer_smoke_tests PRIVATE parser_bison lexer_frontend)

add_test(NAME lexer_smoke_tests COMMAND lexer_smoke_tests)

add_executable(lexer_tests
    tests/lexer_tests.cpp
    src/parser.cpp
)

target_include_directories(lexer_tests PRIVATE include)
target_link_libraries(lexer_tests PRIVATE parser_bison lexer_frontend)

add_test(NAME lexer_tests COMMAND lexer_tests)

add_executable(parser_tests
    tests/parser_tests.cpp
    src/parser.cpp
    src/semantic_declaration.cpp
)

target_include_directories(parser_tests PRIVATE include)
target_link_libraries(parser_tests PRIVATE parser_bison lexer_frontend)

add_test(NAME parser_tests COMMAND parser_tests)

add_executable(bison_parser_tests
    tests/bison_parser_tests.cpp
)

target_include_directories(bison_parser_tests PRIVATE include)
target_link_libraries(bison_parser_tests PRIVATE parser_bison lexer_frontend)

add_test(NAME bison_parser_tests COMMAND bison_parser_tests)

add_executable(semantic_tableset_tests
    tests/semantic_tableset_tests.cpp
)

target_include_directories(semantic_tableset_tests PRIVATE include)

add_test(NAME semantic_tableset_tests COMMAND semantic_tableset_tests)

add_executable(semantic_typepool_tests
    tests/semantic_typepool_tests.cpp
)

target_include_directories(semantic_typepool_tests PRIVATE include)

add_test(NAME semantic_typepool_tests COMMAND semantic_typepool_tests)

add_executable(semantic_declaration_tests
    tests/semantic_declaration_tests.cpp
    src/parser.cpp
    src/semantic_declaration.cpp
)

target_include_directories(semantic_declaration_tests PRIVATE include)
target_link_libraries(semantic_declaration_tests PRIVATE parser_bison lexer_frontend)

add_test(NAME semantic_declaration_tests COMMAND semantic_declaration_tests)

add_executable(semantic_usage_tests
    tests/semantic_usage_tests.cpp
    src/parser.cpp
    src/semantic_declaration.cpp
)

target_include_directories(semantic_usage_tests PRIVATE include)
target_link_libraries(semantic_usage_tests PRIVATE parser_bison lexer_frontend)

add_test(NAME semantic_usage_tests COMMAND semantic_usage_tests)

add_executable(semantic_expression_tests
    tests/semantic_expression_tests.cpp
    src/parser.cpp
    src/semantic_declaration.cpp
)

target_include_directories(semantic_expression_tests PRIVATE include)
target_link_libraries(semantic_expression_tests PRIVATE parser_bison lexer_frontend)

add_test(NAME semantic_expression_tests COMMAND semantic_expression_tests)

add_executable(semantic_param_tests
    tests/semantic_param_tests.cpp
    src/parser.cpp
    src/semantic_declaration.cpp
)

target_include_directories(semantic_param_tests PRIVATE include)
target_link_libraries(semantic_param_tests PRIVATE parser_bison lexer_frontend)

add_test(NAME semantic_param_tests COMMAND semantic_param_tests)

add_executable(semantic_gate_tests
    tests/semantic_gate_tests.cpp
    src/compiler_driver.cpp
    src/code_generator.cpp
    src/parser.cpp
    src/semantic_declaration.cpp
)

target_include_directories(semantic_gate_tests PRIVATE include)
target_link_libraries(semantic_gate_tests PRIVATE parser_bison lexer_frontend)

add_test(NAME semantic_gate_tests COMMAND semantic_gate_tests)

add_executable(codegen_template_tests
    tests/codegen_template_tests.cpp
    src/code_generator.cpp
)

target_include_directories(codegen_template_tests PRIVATE include)
target_link_libraries(codegen_template_tests PRIVATE lexer_frontend)

add_test(NAME codegen_template_tests COMMAND codegen_template_tests)
