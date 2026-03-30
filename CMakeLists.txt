cmake_minimum_required(VERSION 3.16)
project(pascc LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(pascc
    src/main.cpp
    src/compiler_driver.cpp
    src/lexer.cpp
)

target_include_directories(pascc PRIVATE include)
