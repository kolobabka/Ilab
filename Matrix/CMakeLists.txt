cmake_minimum_required(VERSION 3.16)

project("Matrix" VERSION 1.0.0)

set(EXEC_FILE "matrix")
set(TEST_FILE "tests")

set (GTEST_ROOT ${CMAKE_SOURCE_DIR}/googletests/gTest)
find_package(GTest REQUIRED)

set(HEADERS

    headers/matrix.hpp
    headers/common.hpp

)

set(SOURCES

    sources/main.cpp
    sources/common.cpp

)

set (TESTS
	
    sources/test.cpp	
    sources/common.cpp
)

#executable files
add_executable(${EXEC_FILE} ${SOURCES})
add_executable(${TEST_FILE} ${TESTS})

target_include_directories(${EXEC_FILE} PRIVATE headers)

target_link_libraries(${TEST_FILE} PRIVATE "${GTEST_LIBRARIES}" "pthread")
target_include_directories(${TEST_FILE} PRIVATE headers "${GTEST_INCLUDE_DIRS}")

#compiler settings
add_compile_options(

    -Wall
    -Wextra
    -Wpedantic

    -O2
)
