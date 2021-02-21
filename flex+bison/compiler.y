%language "c++"
%skeleton "lalr1.cc"

%locations

%param {Driver* driver}

%code requires
{
#include "../AST/INode.hh"

namespace yy
{
class Driver;
};

extern AST::IScope * cur_scope;
}


%{



%}


/* some tokens */

%define api.value.type variant

%right ASSIGN   "="

%left

  ADD SUB
  MUL DIV MOD

  GREATER LESS
  GR_EQ LS_EQ
  IS_EQ NOT_EQ
  
  AND OR
  ;

%nonassoc

  UNMIN NOT
  ;

%token

  COMMA
  COLON
  SCOLON

  LP RP
  LB RB

  IF ELSE
  WHILE

  SCAN
  PRINT

  ERR
  ;

%token <int> INT
%token <std::string> NAME

/* nonterminals */

%nterm<AST::IScope *> scope
%nterm<AST::IScope *> cl_sc

%nterm<AST::INode *> stm
%nterm<AST::INode *> stms
%nterm<AST::INode *> cur_stm

%nterm<AST::INode *> if
%nterm<AST::INode *> while
%nterm<AST::INode *> print
%nterm<AST::INode *> assign

%nterm<AST::INode *> cond
%nterm<AST::INode *> expr
%nterm<AST::INode *> expr1
%nterm<AST::INode *> expr2

%nterm<AST::Ops> pm
%nterm<AST::Ops> mdm
%nterm<AST::Ops> lgc
%nterm<AST::Ops> cmp

%%


program:     stms                                 { /* program starting */ };

scope:       op_sc stms cl_sc                     { $$ = $3; };

op_sc:       LB                                   { /* cur_scope = cur_scope->enter_new_scope(); */ };

cl_sc:       RB                                   {
                                                    /* $$ = cur_scope; */
                                                    /* cur_scope = cur_scope->parent_scope(); */
                                                  };

stms:        cur_stm                              { /* cur_scope.push($1); */ };
           | stms cur_stm                         { /* cur_scope.push($2); */ };

cur_stm:     stm                                  { $$ = $1 };
           | scope                                { $$ = $1 };

stm:         assign                               { $$ = $1 };
           | if                                   { $$ = $1 };
           | while                                { $$ = $1 };
           | print                                { $$ = $1 };

assign:      NAME[nm] ASSIGN expr[val] SCOLON     { /* $$ = make_assign($nm, $val); */ };

expr:        expr1 pm expr1                       { $$ = make_op($1, $2, $3); };
           | expr1                                { $$ = $1; };

expr1:       expr2 mdm expr2                      { $$ = make_op($1, $2, $3); };
           | expr2                                { $$ = $1; };

expr2:       LP expr[e] RP                        { $$ = $e };
           | NAME                                 { /* $$ = handle_name($1); */ };
           | INT                                  { $$ = make_value($1); };

if:          IF LP cond[c] RP cur_stm[s]          { $$ = make_if($c, $s); };
           | IF LP cond[c] RP cur_stm[s1]
             ELSE cur_stm[s2]                     { /* $$ = make_if($c, $s1, $s2); */ };

while:       WHILE LP cond[c] RP cur_stm[s]       { $$ = make_while($c, $s); };

cond:        expr lgc expr                        { $$ = make_op($1, $2, $3); };
           | NOT cond                             { /* $$ = handle_name */ };
           | expr                                 { $$ = $1 };

pm:          ADD                                  { $$ = AST::Ops::ADD; }; 
           | SUB                                  { $$ = AST::Ops::SUB; }; 

mdm:         MUL                                  { $$ = AST::Ops::MUL; };  
           | DIV                                  { $$ = AST::Ops::DIV; };
           | MOD                                  { $$ = AST::Ops::MOD; };

lgc:         AND                                  { $$ = AST::Ops::AND; };
           | OR                                   { $$ = AST::Ops::OR; };
           | cmp                                  { $$ = $1; };

cmp:         IS_EQ                                { $$ = AST::Ops::IS_EQ; };
           | NOT_EQ                               { $$ = AST::Ops::NOT_EQ; };
           | GREATER                              { $$ = AST::Ops::GREATER; };
           | LESS                                 { $$ = AST::Ops::LESS; };
           | GR_EQ                                { $$ = AST::Ops::GR_EQ; };
           | LS_EQ                                { $$ = AST::Ops::LS_EQ; };


print:       PRINT expr SCOLON                    { };

%%

namespace yy
{
  parser::token_type yylex(parser::semantic_type* yylval, Driver* driver)
  {
    driver->yylex(yylval);
  }
}

