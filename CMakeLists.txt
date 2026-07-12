cmake_minimum_required(VERSION 3.10)

project(compiler VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(compiler 
    main.cpp 
    src/lexer.cpp
    src/parser.cpp
    src/pratt.cpp
)