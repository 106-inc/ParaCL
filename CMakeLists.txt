cmake_minimum_required(VERSION 3.16)
project(ParaCL)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)
include_directories(AST)
include_directories(driver)

add_executable(paracl
        AST/INode.cc
        AST/OPNode.cc
        AST/Node.cc
        driver/driver.cc
        parser/parser.cc
        FB_BLD/lex.yy.cc
        FB_BLD/compiler.tab.cc
        main.cc)
