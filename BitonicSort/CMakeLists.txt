cmake_minimum_required(VERSION 3.16)

project ("Bitonic Sort" VERSION 1.0.0)

set (EXEC_FILE "bitonic_sort")

find_package(OpenCL REQUIRED)

set (HEADERS

    headers/bitsort.hpp
)

set (SOURCES

    sources/main.cpp
    sources/bitsort.cpp
)

add_executable(${EXEC_FILE} ${SOURCES})
target_include_directories(${EXEC_FILE} PRIVATE headers)
target_link_libraries(${EXEC_FILE} OpenCL::OpenCL)

set(COMPILER_SETTINGS

    -Wall
    -Wextra
    -Wpedantic

    -O2
)

target_compile_options(${EXEC_FILE} PRIVATE ${COMPILER_SETTINGS})