cmake_minimum_required(VERSION 3.16)

project ("Bitonic Sort" VERSION 1.0.0)

set (EXEC_FILE "bitonic_sort")
set (TEST_FILE "tests")

set (GTEST_ROOT ${CMAKE_SOURCE_DIR}/googletests/gTest)
find_package(GTest REQUIRED)

find_package(OpenCL REQUIRED)

set (HEADERS

headers/ocl.hpp
headers/bitsort.hpp
headers/common.hpp
)

set (SOURCES

sources/main.cpp
)

set (TESTS

sources/test.cpp
)

add_executable(${EXEC_FILE} ${SOURCES})
add_executable(${TEST_FILE} ${TESTS})

target_include_directories(${EXEC_FILE} PRIVATE headers)
target_link_libraries(${EXEC_FILE} OpenCL::OpenCL)


target_include_directories(${TEST_FILE} PRIVATE headers)
target_include_directories(${TEST_FILE} PRIVATE headers "${GTEST_INCLUDE_DIRS}")
target_link_libraries(${TEST_FILE} PRIVATE "${GTEST_LIBRARIES}" "pthread" OpenCL::OpenCL)


target_compile_definitions(${TEST_FILE} PRIVATE DEBUG)

if (SIMPLE)
    target_compile_definitions(${EXEC_FILE} PRIVATE SIMPLE)
endif()

if (STD)
    target_compile_definitions(${EXEC_FILE} PRIVATE STD)
endif()


set(COMPILER_SETTINGS

    -Wall
    -Wextra
    -Wpedantic

    -O2
)

target_compile_options(${EXEC_FILE} PRIVATE ${COMPILER_SETTINGS})
target_compile_options(${TEST_FILE} PRIVATE ${COMPILER_SETTINGS})
