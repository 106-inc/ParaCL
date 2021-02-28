%language "c++"
%skeleton "lalr1.cc"

%param {Driver* driver}
%locations

%define parse.trace
%define parse.error verbose
%define parse.lac full

%define api.value.type variant

%code requires
{
#include <string>
#include "../AST/INode.hh"

namespace yy
{
class Driver;
};

extern AST::IScope * CUR_SCOPE;

}

%code
{
#include "../driver/driver.hh"

namespace yy
{ parser::token_type yylex(parser::semantic_type* yylval, parser::location_type* yylloc, Driver* driver); }

extern AST::IScope * CUR_SCOPE;
}

/* some tokens */

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
%nterm<AST::IScope *> cur_stm

%nterm<AST::INode *> if
%nterm<AST::INode *> while
%nterm<AST::INode *> print
%nterm<AST::INode *> scan
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

op_sc:       LB                                   { CUR_SCOPE = AST::make_scope(CUR_SCOPE); };

cl_sc:       RB                                   {
                                                    $$ = CUR_SCOPE;
                                                    CUR_SCOPE = CUR_SCOPE->reset_scope();
                                                  };

stms:        stm                                  { CUR_SCOPE->push($1); };
           | stms stm                             { CUR_SCOPE->push($2); };
           | scope                                { CUR_SCOPE->push($1); };
           | stms scope                           { CUR_SCOPE->push($2); };

cur_stm:     stm                                  {
                                                     $$ = AST::make_scope(CUR_SCOPE);
                                                     $$->push($1);
                                                  };
           | scope                                { $$ = $1; };

stm:         assign                               { $$ = $1; };
           | if                                   { $$ = $1; };
           | while                                { $$ = $1; };
           | print                                { $$ = $1; };
           | scan                                 { $$ = $1; };

assign:      NAME ASSIGN expr SCOLON              { $$ = AST::make_ass($1, $3); };

expr:        expr pm expr1                        { $$ = AST::make_op($1, $2, $3); }; 
           | expr1                                { $$ = $1; };

expr1:       expr1 mdm expr2                      { $$ = AST::make_op($1, $2, $3); }; 
           | expr2                                { $$ = $1; };

expr2:       LP expr[e] RP                        { $$ = $e; };
           | NAME                                 { $$ = AST::make_ref($1); };
           | INT                                  { $$ = AST::make_cst($1); };

if:          IF LP cond[c] RP cur_stm[s]          { $$ = AST::make_if($c, $s); };
           | IF LP cond[c] RP cur_stm[s1]                                               /* dangling else */
             ELSE cur_stm[s2]                     { $$ = AST::make_if($c, $s1, $s2); }; /* this rule creates shift-reduce conflict  */

while:       WHILE LP cond[c] RP cur_stm[s]       { $$ = AST::make_while($c, $s); };

cond:        expr lgc expr                        { $$ = AST::make_op($1, $2, $3); };
           | NOT cond                             { /* $$ = handle_name */ };
           | expr                                 { $$ = $1; };

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


print:       PRINT expr SCOLON                    { $$ = AST::make_print($2); };

scan:        NAME ASSIGN SCAN SCOLON              { $$ = AST::make_ass($1, AST::make_scan()); };

%%

namespace yy
{
  void parser::error( const location_type& loc, const std::string &msg )
  {
    std::cerr << msg << " in (line.column): " << loc << std::endl;
  }

  parser::token_type yylex(parser::semantic_type* yylval, parser::location_type* yylloc, Driver* driver)
  {
    return driver->yylex(yylval, yylloc);
  }
}

