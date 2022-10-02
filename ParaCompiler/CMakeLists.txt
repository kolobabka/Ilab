cmake_minimum_required(VERSION 3.16)
project(ParaCL)


set(COMPILE_NAME ${PROJECT_NAME})
  
set(FRONTEND
  frontend
)
message(STATUS "Collected frontend: ${FRONTEND}")

foreach(FRONT ${FRONTEND})
  add_subdirectory(${FRONT})
endforeach()

foreach(FRONT ${FRONTEND})
  message(STATUS "${FRONT}")
  target_include_directories(${FRONT} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/${FRONT})
  target_include_directories(${FRONT} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
  foreach(DIR ${LEXPARSE})
  message(STATUS "${DIR}")
  target_include_directories(${FRONT} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/${FRONT}/${DIR})
  target_include_directories(${FRONT} PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/${FRONT}/${DIR})
  endforeach()
endforeach()

add_executable(${COMPILE_NAME} 
  driver/driver.cpp
)

set(TARGETS
  ${COMPILE_NAME}
)

# common for all targets
foreach(TNAME ${TARGETS})
  target_compile_features(${TNAME} PRIVATE cxx_std_20)
  target_include_directories(${TNAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/driver)
  target_link_libraries(${TNAME} ${FRONTEND})
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

