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

%right ASSIGN   "="

%left

  ADD           "+"
  SUB           "-"

  MUL           "*"
  DIV           "/"
  MOD           "%"

  GREATER       ">"
  LESS          "<"
  GR_EQ         ">="
  LS_EQ         "<="
  IS_EQ         "=="
  NOT_EQ        "!="

  AND           "&&"
  OR            "||"
  ;

%nonassoc

  UNMIN
  NOT           "!"
  ;

%token

  COMMA         ","
  COLON         ":"
  SCOLON        ";"

  LP            "("
  RP            ")"
  LB            "{"
  RB            "}"

  IF            "if"
  ELSE          "else"
  WHILE         "while"

  SCAN          "?"
  PRINT         "print"

  ERR
  ;

%token <int> INT
%token <std::string> NAME

/* nonterminals */

%nterm<AST::IScope *> scope
%nterm<AST::INode *> op_sc /* clarify nonterminal type */
%nterm<AST::INode *> cl_sc /* clarify nonterminal type */

%nterm<AST::INode *> stm
%nterm<AST::INode *> stms

%nterm<AST::INode *> if
%nterm<AST::INode *> while
%nterm<AST::INode *> print
%nterm<AST::INode *> assign

%nterm<AST::INode *> expr
%nterm<AST::INode *> expr1
%nterm<AST::INode *> expr2
%nterm<AST::INode *> expr3


%%


program:     stms                        { /* program starting */ };

scope:       op_sc stms cl_sc            { /* cur_scope = cur_scope->enter_new_scope */ };

op_sc:       LB                          { /* cur_scope = cur_scope->enter_new_scope */ };

cl_sc:       RB                          { /* cur_scope = cur_scope->parent_scope */ };

stms:        stm                         { };
           | stms stm                    { };
           | stms scope                  { };

stm:         assign                      { /* cur_scope->push(make_op())*/ };
           | if                          { };
           | while                       { };
           | print                       { };
           | . /* Null string (epsilon) */

assign:      NAME ASSIGN expr SCOLON     { };

expr:        expr1                       { };

expr1:       expr2 ADD expr2             { };
           | expr2 SUB expr2             { };
           | expr2                       { };

expr2:       expr3 MUL expr3             { };
           | expr3 DIV expr3             { };
           | expr3 MOD expr3             { };
           | expr3

expr3:       LP expr RP                  { };
           | NAME                        { };
           | INT                         { };

if:          IF LP cond RP scope         { };
           | IF LP cond RP scope 
             ELSE scope                  { };
           | IF LP cond RP stm           { };

while:       WHILE LP cond RP scope      { };
           | WHILE LP cond RP stm        { };

cond:        expr AND expr               { };
           | expr OR expr                { };
           | NOT expr                    { };
           | expr IS_EQ expr             { };
           | expr NOT_EQ expr            { };
           | expr GREATER expr           { };
           | expr LESS expr              { };
           | expr GR_EQ expr             { };
           | expr LS_EQ expr             { };
           | expr                        { };

print:       PRINT expr SCOLON           { };

%%

