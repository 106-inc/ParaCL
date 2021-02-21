%language "c++"
%skeleton "lalr1.cc"

%locations

%{

#include "INode.hh"
#include "Driver.hh"
#include "parser.hh"

extern AST::IScope * cur_scope;

%}


/* some tokens */

%define api.value.type variant

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

  AND           "&&"
  OR            "||"

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

%left IS_EQ NOT_EQ
%left GREATER LESS
%left GR_EQ LS_EQ

%left AND OR

/* nonterminals */

%nterm<AST::IScope *> scope
%nterm<AST::INode *> op_sc
%nterm<AST::INode *> cl_sc

%nterm<AST::INode *> stm
%nterm<AST::INode *> stms

%nterm<AST::INode *> if
%nterm<AST::INode *> while
%nterm<AST::INode *> print
%nterm<AST::INode *> assign

%nterm<AST::INode *> expr
  /* %nterm<AST::INode *> expr_l1 */
  /* %nterm<AST::INode *> expr_l2 */


%%


program:     stms                        { };

scope:       op_sc stms cl_sc            { };

op_sc:       LB                          { };

cl_sc:       RB                          { };

stms:        stm                         { };
           | stms stm                    { };
           | stms scope                  { };

stm:         assign                      { };
           | if                          { };
           | while                       { };
           | print                       { };

assign:      NAME ASSIGN expr SCOLON     { };

expr:        expr ADD expr               { };
           | expr SUB expr               { };
           | expr MUL expr               { };
           | expr DIV expr               { };
           | expr MOD expr               { };
           | NAME                        { };
           | INT                         { };

if:          IF LP cond RP scope         { };

while:       WHILE LP cond RP scope      { };

cond:        expr AND expr               { };
           | expr OR expr                { };
           | expr IS_EQ expr             { };
           | expr NOT_EQ expr            { };
           | expr GREATER expr           { };
           | expr LESS expr              { };
           | expr GR_EQ expr             { };
           | expr LS_EQ expr             { };
           | expr                        { };

print:       PRINT expr SCOLON           { };

%%

