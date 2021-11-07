cmake_minimum_required (VERSION 3.16)           # VERSION

project (Matrix VERSION 1.0.0)                # PROJECT TITLE

set (EXECSOURCES source/main.cpp) # SOURCES *.cpp
set (HEADERS matrix/cache.hpp)                 # HEADERS *.hpp

add_compile_options(

    -Wall
    -Wextra
    
    -O2

)

set( EXEC_NAME "matrix")

set( TARGETS ${EXEC_NAME} ${TEST_NAME} ${BELADY_NAME})

add_executable( ${EXEC_NAME} )
target_sources( ${EXEC_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/source/main.cpp")

foreach( TARGET IN LISTS TARGETS )

    target_include_directories( ${TARGET} PRIVATE "${CMAKE_SOURCE_DIR}/headers" )


endforeach()
