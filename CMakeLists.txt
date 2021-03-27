cmake_minimum_required(VERSION 3.12)
project(ParaCL)

set(CMAKE_CXX_STANDARD 20)


add_executable(paracl
        AST/INode.cc
        AST/Node.cc
        driver/driver.cc
        parser/parser.cc
        FB_BLD/lex.yy.cc
        FB_BLD/compiler.tab.cc
        main.cc)
