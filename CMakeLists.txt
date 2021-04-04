cmake_minimum_required(VERSION 3.12)
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

add_executable(${INTERP_NAME} 
  driver/driver.cc
  parser/parser.cc
  AST/INode.cc
  AST/Node.cc
  AST/Interp.cc
  main.cc
  ${BISON_bison_o_OUTPUTS}
  ${FLEX_flex_o_OUTPUTS}
)

target_compile_features(${INTERP_NAME} PRIVATE cxx_std_20)
target_include_directories(${INTERP_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(${INTERP_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${INTERP_NAME} PRIVATE parser)
target_include_directories(${INTERP_NAME} PRIVATE AST)
target_include_directories(${INTERP_NAME} PRIVATE driver)

