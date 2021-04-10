cmake_minimum_required(VERSION 3.13.4)
project(ParaCL)

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

set(INTERP_NAME "${PROJECT_NAME}i")

message(STATUS "Interp: ${INTERP_NAME}")

# option for debug (debug info)
set(PR_DEBUG_OPTIONS -g)

# option for release (optimization)
set(PR_RELEASE_OPTIONS -O2)

# some useful warnings
set(WARN_OPTIONS
  -Wall -Wextra -Wunused -Wpedantic # classic
  -Wno-old-style-cast -Waggressive-loop-optimizations
  -Wunreachable-code)

flex_target(flex_o
  grammar/scanner.l
  ${CMAKE_CURRENT_BINARY_DIR}/lex.yy.cc
)

bison_target(bison_o
  grammar/compiler.y
  ${CMAKE_CURRENT_BINARY_DIR}/compiler.tab.cc
  COMPILE_FLAGS "--defines=${CMAKE_CURRENT_BINARY_DIR}/compiler.tab.hh")

add_flex_bison_dependency(flex_o bison_o)

if(FALSE)
add_custom_target(fb
  DEPENDS AST/INode.cc AST/Node.cc
  SOURCES ${BISON_bison_o_OUTPUTS} ${FLEX_flex_o_OUTPUTS}
)

add_custom_target(ast_o
  SOURCES AST/INode.cc AST/Node.cc AST/Interp.cc
)

add_custom_target(driver_o
  SOURCES driver/driver.cc parser/parser.cc
)
endif()

add_executable(${INTERP_NAME} 
  driver/driver.cc
  parser/parser.cc
  AST/Node.cc
  AST/Interp.cc
  main.cc
  ${BISON_bison_o_OUTPUTS}
  ${FLEX_flex_o_OUTPUTS}
)

#add_dependencies(${INTERP_NAME} fb driver_o ast_o)

target_compile_features(${INTERP_NAME} PRIVATE cxx_std_20)
target_compile_options(${INTERP_NAME} PRIVATE ${WARN_OPTIONS})
target_include_directories(${INTERP_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(${INTERP_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${INTERP_NAME} PRIVATE parser)
target_include_directories(${INTERP_NAME} PRIVATE AST)
target_include_directories(${INTERP_NAME} PRIVATE driver)

# Specify compiler options for debug and release
target_compile_options(${INTERP_NAME} PRIVATE "$<$<CONFIG:DEBUG>:${PR_DEBUG_OPTIONS}>")
target_compile_options(${INTERP_NAME} PRIVATE "$<$<CONFIG:RELEASE>:${PR_RELEASE_OPTIONS}>")

