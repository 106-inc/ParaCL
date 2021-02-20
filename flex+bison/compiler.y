%language "c++"
%skeleton "lalr1.cc"

%location

%code
{

// some code here

}

%{

#include "parser.hh"

%}



%token

  ADD           "+"
  SUB           "-"
  MUL           "*"
  DIV           "/"
  MOD           "%"
  EQUAL         "="

  GREATER       ">"
  LESS          "<"
  GR_EQ         ">="
  LS_EQ         "<="
  IS_EQ         "=="
  NOT_EQ        "!="

  COMMA         ","
  COLON         ":"
  SCOLON        ";"

  LP            "("
  RP            ")"
  LB            "{"
  RB            "}"

  IF            "if"
  WHILE         "while"

  SCAN          "?"
  PRINT         "print"

  ;

%%


program:     stms

scope:       op_sc stms cl_sc

op_sc:       LB

cl_sc:       RB

stms:        stm
           | stms stm
           | stms scope

stm:         assign
           | if
           | while
           | print



