cmake_minimum_required(VERSION 3.16)

project ("Bitonic Sort" VERSION 1.0.1)

set (EXEC_FILE "bitonic_sort")
set (TIME_FILE "time_profile")

if (DEBUG)
    set (TEST_FILE "tests")
endif ()

set (TIME "profile")

find_package(OpenCL REQUIRED)

set (HEADERS

    headers/ocl.hpp
    headers/bitsort.hpp
    headers/common.hpp
)

set (TIME_SRC 

    sources/time.cpp
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
add_executable(${TIME_FILE} ${TIME_SRC})

if (DEBUG)
    add_executable(${TEST_FILE} ${TESTS})
endif ()

target_include_directories(${EXEC_FILE} PRIVATE headers)
target_link_libraries(${EXEC_FILE} OpenCL::OpenCL)

target_include_directories(${TIME_FILE} PRIVATE headers)
target_link_libraries(${TIME_FILE} OpenCL::OpenCL)



if (DEBUG)
    target_include_directories(${TEST_FILE} PRIVATE headers)
    target_link_libraries(${TEST_FILE} OpenCL::OpenCL)

endif ()

if (KERNEL)
    target_compile_definitions(${EXEC_FILE} PRIVATE KERNEL="${KERNEL}")
    target_compile_definitions(${TIME_FILE} PRIVATE KERNEL="${KERNEL}")

endif()

if (NVIDIA)
    target_compile_definitions(${EXEC_FILE} PRIVATE NVIDIA)
    if (DEBUG)
        target_compile_definitions(${TEST_FILE} PRIVATE NVIDIA)
    endif()
endif()

if (DEBUG)
    target_compile_definitions(${TEST_FILE} PRIVATE TESTS=${TEST_FILES})
endif ()

if (DEBUG)
    target_compile_features(${TEST_FILE} PRIVATE cxx_std_17)
endif ()

target_compile_features(${EXEC_FILE} PRIVATE cxx_std_17)
target_compile_features(${TIME_FILE} PRIVATE cxx_std_17)

