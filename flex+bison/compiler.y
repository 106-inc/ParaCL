%language "c++"
%skeleton "lalr1.cc"

%locations

%code
{

// some code here

}

%{

#include "parser.hh"

extern AST::IScope * cur_scope;

%}


/* some tokens */

%token

  ADD           "+"
  SUB           "-"
  MUL           "*"
  DIV           "/"
  MOD           "%"
  ASSIGN        "="

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

  ERR
  ;

%token <int> INT
%token <std::string> NAME


/* left/right associative */

%right ASSIGN

%left ADD SUB
%left MUL DIV MOD
%left GREATER LESS GR_EQ LS_EQ IS_EQ NOT_EQ


/* nonterminals */

%nterm<AST::IScope *> scope

%nterm<AST::INode *> assign
%nterm<AST::INode *> if
%nterm<AST::INode *> while 
%nterm<AST::INode *> print

%%


program:     stms                { std::cout << "program starting" << std::endl };

scope:       op_sc stms cl_sc    { };

op_sc:       LB                  { };

cl_sc:       RB                  { };

stms:        stm                 { };
           | stms stm            { };
           | stms scope          { };

stm:         assign              { };
           | if                  { };
           | while               { };
           | print               { };


%%

