cmake_minimum_required(VERSION 3.16)
project(pascc LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()

add_executable(pascc
    src/main.cpp
    src/compiler_driver.cpp
    src/lexer.cpp
)

target_include_directories(pascc PRIVATE include)

add_executable(lexer_tests
    tests/lexer_tests.cpp
    src/lexer.cpp
)

target_include_directories(lexer_tests PRIVATE include)

add_test(NAME lexer_tests COMMAND lexer_tests)
