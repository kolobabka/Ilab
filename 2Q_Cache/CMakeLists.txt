cmake_minimum_required (VERSION 3.16)           # VERSION

project (2Q_Cache VERSION 1.0.0)                # PROJECT TITLE

set (EXECSOURCES source/main.cpp) # SOURCES *.cpp
set (TESTSOURCES test/unTests.cpp)
set (BELADYSOURCES belady/belady.cpp)
set (HEADERS headers/cache.hpp)                 # HEADERS *.hpp

set (COMPILER_SETTINGS -Wall -Wextra -O3)       # COMPILER SETTINGS

add_compile_options (${COMPILER_SETTINGS})
add_executable (cache ${EXECSOURCES})
add_executable (unTest ${TESTSOURCES})
add_executable (belady ${BELADYSOURCES})
