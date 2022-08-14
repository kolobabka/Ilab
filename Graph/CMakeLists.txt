cmake_minimum_required(VERSION 3.16)

project ("graph" VERSION 1.0.0)

find_package (FLEX REQUIRED)

flex_target (scanner

    lexer/graph.l
    ${CMAKE_CURRENT_BINARY_DIR}/lexer.cc
)

set (EXEC_FILE "graph")

set (CMAKE_CXX_STANDARD 20)

set (SOURCES 

    sources/main.cpp
    customLexer/customLexer.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/lexer.cc

)

set (HEADERS

    headers/graph.hpp
    customLexer/customLexer.hpp
)

add_executable(${EXEC_FILE} ${SOURCES})

target_include_directories(${EXEC_FILE} PRIVATE headers customLexer)
# target_include_directories (${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
