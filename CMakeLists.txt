cmake_minimum_required(VERSION 3.17)
project(ParaCL)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(AST)
include_directories(Driver)

add_executable(ParaCL
        AST/INode.cc
        AST/Node.cc
        BUILD/compiler.tab.cc
        BUILD/lex.yy.cc
        Driver/driver.cc
        main.cc)
