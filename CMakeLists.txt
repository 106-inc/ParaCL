cmake_minimum_required(VERSION 3.13.4)
project(ParaCL)

find_package(BISON 3.7 REQUIRED)
find_package(FLEX REQUIRED)
find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

set(INTERP_NAME   "${PROJECT_NAME}i")
set(COMPILE_NAME  "${PROJECT_NAME}")

message(STATUS "Interp: ${INTERP_NAME}")

# option for debug (debug info)
set(PR_DEBUG_OPTIONS -g)

# option for release (optimization)
set(PR_RELEASE_OPTIONS -O2)

# some useful warnings
set(WARN_OPTIONS
  -Wall 
   #-Wextra commented because it causes warnings at llvm library code
  -Wunused -Wpedantic # classic
  -Wno-old-style-cast #-Waggressive-loop-optimizations
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

add_executable(${INTERP_NAME} 
  driver/driver.cc
  parser/parser.cc
  AST/Node.cc
  AST/Interp.cc
  CLI/CLI.cc
  main.cc
  ${BISON_bison_o_OUTPUTS}
  ${FLEX_flex_o_OUTPUTS}
)

add_executable(${COMPILE_NAME} 
  driver/driver.cc
  parser/parser.cc
  AST/Node.cc
  AST/Interp.cc
  CLI/CLI.cc
  main.cc
  ${BISON_bison_o_OUTPUTS}
  ${FLEX_flex_o_OUTPUTS}
)

set(TARGETS  ${COMPILE_NAME} ${INTERP_NAME})

# use llvm-config --components and some guessing to determine components
llvm_map_components_to_libnames(llvm_libs support core codegen irreader)

foreach(TARGET ${TARGETS})
  target_compile_features(${TARGET} PRIVATE cxx_std_17)
  target_compile_options(${TARGET} PRIVATE ${WARN_OPTIONS})
  
  target_include_directories(${TARGET} PRIVATE ${LLVM_INCLUDE_DIRS})
  target_include_directories(${TARGET} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
  target_include_directories(${TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
  target_include_directories(${TARGET} PRIVATE parser)
  target_include_directories(${TARGET} PRIVATE AST)
  target_include_directories(${TARGET} PRIVATE CLI)
  target_include_directories(${TARGET} PRIVATE driver)

  # Specify compiler options for debug and release
  target_compile_options(${TARGET} PRIVATE "$<$<CONFIG:DEBUG>:${PR_DEBUG_OPTIONS}>")
  target_compile_options(${TARGET} PRIVATE "$<$<CONFIG:RELEASE>:${PR_RELEASE_OPTIONS}>")

  target_link_libraries(${TARGET} ${llvm_libs})
endforeach()
target_compile_definitions(${COMPILE_NAME} PRIVATE CODEGEN=1)
target_compile_definitions(${INTERP_NAME} PRIVATE CODEGEN=0)
