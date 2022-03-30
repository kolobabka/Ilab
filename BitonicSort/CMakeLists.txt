cmake_minimum_required(VERSION 3.16)

project ("Bitonic Sort" VERSION 1.0.0)

set (EXEC_FILE "bitonic_sort")
if (DEBUF)
set (TEST_FILE "tests")
endif ()

if (DEBUF)
set (GTEST_ROOT ${CMAKE_SOURCE_DIR}/googletests/gTest)
find_package(GTest REQUIRED)
endif ()
find_package(OpenCL REQUIRED)

set (HEADERS

headers/ocl.hpp
headers/bitsort.hpp
headers/common.hpp
)

set (SOURCES

sources/main.cpp
)

if (DEBUG)
set (TESTS

sources/test.cpp
)
endif ()

add_executable(${EXEC_FILE} ${SOURCES})
if (DEBUG)
add_executable(${TEST_FILE} ${TESTS})
endif ()

target_include_directories(${EXEC_FILE} PRIVATE headers)
target_link_libraries(${EXEC_FILE} OpenCL::OpenCL)


if (DEBUG)
target_include_directories(${TEST_FILE} PRIVATE headers)
target_include_directories(${TEST_FILE} PRIVATE headers "${GTEST_INCLUDE_DIRS}")
target_link_libraries(${TEST_FILE} PRIVATE "${GTEST_LIBRARIES}" "pthread" OpenCL::OpenCL)
endif ()


if (CPU)
    target_compile_definitions(${EXEC_FILE} PRIVATE CPU)
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

if (DEBUG)
target_compile_options(${TEST_FILE} PRIVATE ${COMPILER_SETTINGS})
endif ()