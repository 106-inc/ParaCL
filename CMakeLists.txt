cmake_minimum_required(VERSION 3.17)
project(ParaCL)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(AST)
include_directories(Driver)

add_executable(ParaCL
        AST/INode.hh
        AST/Node.cc
        AST/Node.hh
        AST/Symtab.hh
        Driver/driver.cc
        Driver/driver.hh
        main.cc
        parser/parser.hh
        parser/parser.cc)