cmake_minimum_required(VERSION 3.16)
project(ParaCL)

set(COMPILE_NAME ${PROJECT_NAME})
  
find_package (LLVM REQUIRED CONFIG)
message (STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message (STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories (${LLVM_INCLUDE_DIRS})
separate_arguments (LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions (${LLVM_DEFINITIONS_LIST})

llvm_map_components_to_libnames(llvm_libs support core codegen irreader)

set(FRONTEND
  frontend
)
message(STATUS "Collected frontend: ${FRONTEND}")

foreach(FRONT ${FRONTEND})
  add_subdirectory(${FRONT})
endforeach()

foreach(FRONT ${FRONTEND})
  target_include_directories(${FRONT} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/${FRONT})
  target_include_directories(${FRONT} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
  foreach(DIR ${LEXPARSE})
    target_include_directories(${FRONT} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/${FRONT}/${DIR})
    target_include_directories(${FRONT} PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/${FRONT}/${DIR})
  target_link_libraries(${FRONT} ${llvm_libs})

  endforeach()
endforeach()

set(BACKEND
  backend
)

# foreach(BACK ${BACKEND})
#   add_subdirectory(${BACK})
# endforeach()

add_executable(${COMPILE_NAME} 
  driver/driver.cpp
)

set(TARGETS
  ${COMPILE_NAME}
)

# common for all targets
foreach(TNAME ${TARGETS})
  target_compile_features(${TNAME} PRIVATE cxx_std_20)
  foreach(FRONT ${FRONTEND})
    target_include_directories(${TNAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/${FRONT})
  endforeach()
  foreach(BACK ${BACKEND})
    target_include_directories(${TNAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/${BACK})
  endforeach()
  target_link_libraries(${TNAME} ${FRONTEND})
  message(STATUS "LIBS LLVM: ${llvm_libs}")
  target_link_libraries(${TNAME} ${llvm_libs})
endforeach()

# # build for codegen
# target_compile_definitions(${COMPILE_NAME} PRIVATE CODEGEN=1)

# add_executable(${INTERP_NAME}
#   driver.cc
#   Node.cc
#   Codegen.cc
#   ${BISON_parser_OUTPUTS}
#   ${FLEX_scanner_OUTPUTS}
# )

# # build as interpreter
# target_compile_definitions(${INTERP_NAME} PRIVATE CODEGEN=0)


# target_compile_definitions(${COMPILE_NAME} PRIVATE
#   $<$<CONFIG:Debug>:DUMPALL>
# )

