cmake_minimum_required(VERSION 3.16)
project(ParaCL)

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

set(INTERP_NAME "${PROJECT_NAME}i")

message(STATUS "Interp: ${INTERP_NAME}")

flex_target(flex_o
  grammar/scanner.l
  ${CMAKE_CURRENT_BINARY_DIR}/lex.yy.cc
)

bison_target(bison_o
  grammar/compiler.y
  ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp
  COMPILE_FLAGS "--defines=${CMAKE_CURRENT_BINARY_DIR}/compiler.tab.hh")

add_flex_bison_dependency(flex_o bison_o)

include_directories(parser)
include_directories(AST)
include_directories(driver)

add_executable(${INTERP_NAME} 
  driver/driver.cc
  parser/parser.cc
  AST/INode.cc
  AST/Node.cc
  main.cc
  ${BISON_bison_o_OUTPUTS}
  ${FLEX_flex_o_OUTPUTS}
)

set(TARGETS
  ${INTERP_NAME}
)

# common for all targets
foreach(TNAME ${TARGETS})
  target_compile_features(${TNAME} PRIVATE cxx_std_17)
  target_include_directories(${TNAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
  target_include_directories(${TNAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
endforeach()

