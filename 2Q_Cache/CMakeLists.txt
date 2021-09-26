cmake_minimum_required (VERSION 3.16)           # VERSION

project (2Q_Cache VERSION 1.0.1)                # PROJECT TITLE

set (EXECSOURCES source/main.cpp) # SOURCES *.cpp
set (TESTSOURCES test/unTests.cpp)
set (BELADYSOURCES belady/belady.cpp)
set (HEADERS headers/cache.hpp)                 # HEADERS *.hpp

set (COMPILER_SETTINGS -Wall -Wextra -O3)       # COMPILER SETTINGS

set( TEST_NAME "unTest" )
set( EXEC_NAME "cache")
set( BELADY_NAME "belady")

set( TARGETS ${EXEC_NAME} ${TEST_NAME} ${BELADY_NAME})

add_executable( ${EXEC_NAME} )
add_executable( ${TEST_NAME} )
add_executable( ${BELADY_NAME} )
target_sources( ${EXEC_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/source/main.cpp")
target_sources( ${TEST_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/test/unTests.cpp" )
target_sources( ${BELADY_NAME} PRIVATE "belady/belady.cpp")

foreach( TARGET IN LISTS TARGETS )

    target_include_directories( ${TARGET} PRIVATE "${CMAKE_SOURCE_DIR}/headers" )


endforeach()
