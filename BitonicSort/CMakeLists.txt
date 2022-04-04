cmake_minimum_required(VERSION 3.16)

project ("Bitonic Sort" VERSION 1.0.0)

set (EXEC_FILE "bitonic_sort")

if (DEBUG)
    set (TEST_FILE "tests")
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
    target_link_libraries(${TEST_FILE} OpenCL::OpenCL)

endif ()

if (KERNEL)
    target_compile_definitions(${EXEC_FILE} PRIVATE KERNEL="${KERNEL}")
endif()

if (NVIDIA)
    target_compile_definitions(${EXEC_FILE} PRIVATE NVIDIA)
endif()

if (STD)
    target_compile_definitions(${EXEC_FILE} PRIVATE STD)
endif()

if (DEBUG)
    # target_compile_definitions(${TEST_FILE} PRIVATE NUM_TESTS=list(LENGHT TEST_LIST))
    target_compile_definitions(${TEST_FILE} PRIVATE TESTS=${TEST_FILES})
endif ()

if (DEBUG)
    target_compile_features(${TEST_FILE} PRIVATE cxx_std_17)
endif ()

target_compile_features(${EXEC_FILE} PRIVATE cxx_std_17)
