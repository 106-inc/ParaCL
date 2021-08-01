%language "c++"
%skeleton "lalr1.cc"

%param {Driver* driver}
%locations

%define parse.trace
%define parse.lac full

%define api.value.type variant
%define parse.error custom

%code requires
{
#include <string>
#include "INode.hh"

namespace yy
{
class Driver;
}

extern AST::IScope *CUR_SCOPE;

}

%code
{
#include "driver.hh"

namespace yy
{ parser::token_type yylex(parser::semantic_type* yylval, parser::location_type* yylloc, Driver* driver); }

void WholeProgramAction();

extern AST::IScope *CUR_SCOPE;
}



%token

  COMMA
  COLON
  SCOLON

  LP RP
  LB RB

  IF ELSE
  WHILE
  RETURN
  FUNC

  SCAN
  PRINT

  ERR
  UNKNOWN_VAR
  ;

%token <int> INT
%token <std::string> NAME

/* nonterminals */

%nterm<AST::pIScope>

  scope

  br_scope
  op_br_sc
  ;

%nterm<AST::pIScope> br_stm

%nterm<AST::pINode> 
  stm
  stms
  ;

%nterm<AST::pINode>

  if
  while
  print
  assign
  ;

%nterm<AST::pINode>

  expr
  expr_un
  expr_term
  ;

 /* 
 %nterm<AST::INode *>

  func_call
  func_def
  ;
 */

/* some tokens */
%right ASSIGN  "="

%left OR
%left AND 
%left IS_EQ NOT_EQ
%left 
      GREATER LESS 
      GR_EQ LS_EQ
      ;

%left ADD MIN
%left MUL DIV MOD 
%left NEG NOT


%%


program:     stms                           { WholeProgramAction(); };

scope:       op_sc stms cl_sc               { /* nothing */ };

br_scope:    op_br_sc stms cl_sc            { $$ = $1; };

op_sc:       LB                             { 
                                              auto par_tmp = CUR_SCOPE;
                                              auto u_ptr = AST::make_scope(par_tmp);
                                              CUR_SCOPE = u_ptr.get();

                                              if (par_tmp) par_tmp->push(u_ptr);
                                            };

op_br_sc:    LB                             { 
                                              $$ = AST::make_scope(CUR_SCOPE); 
                                              CUR_SCOPE = $$.get();
                                            };

cl_sc:       RB                             { CUR_SCOPE = CUR_SCOPE->reset_scope(); };

stms:        stm                            { CUR_SCOPE->push($1); };
           | stms stm                       { CUR_SCOPE->push($2); };
           | scope                          { /* nothing */ };
           | stms scope                     { /* nothing */ };

br_stm:     stm                             {
                                              $$ = AST::make_scope(CUR_SCOPE);
                                              $$->push($1);
                                            };
           | br_scope                       { $$ = $1; };

stm:         assign                         { $$ = $1; };
           | if                             { $$ = $1; };
           | while                          { $$ = $1; };
           | print                          { $$ = $1; };
        /* | expr SCOLON                    { $$ = $1; }; */
        /* | RETURN expr SCOLON             { SOMETHING };           */
        /* | SCOLON                         { NOTHING };             */

assign:      NAME ASSIGN expr SCOLON        { $$ = AST::make_asgn($1, $3); };
        /* | NAME ASSIGN func_def SCOLON    {} */


expr:        expr OR expr                   { $$ = AST::make_op($1, AST::Ops::OR, $3); };
           | expr AND expr                  { $$ = AST::make_op($1, AST::Ops::AND, $3); };
           | expr IS_EQ expr                { $$ = AST::make_op($1, AST::Ops::IS_EQ, $3); };
           | expr NOT_EQ expr               { $$ = AST::make_op($1, AST::Ops::NOT_EQ, $3); };
           | expr GREATER expr              { $$ = AST::make_op($1, AST::Ops::GREATER, $3); };
           | expr LESS expr                 { $$ = AST::make_op($1, AST::Ops::LESS, $3); };
           | expr LS_EQ expr                { $$ = AST::make_op($1, AST::Ops::LS_EQ, $3); };
           | expr GR_EQ expr                { $$ = AST::make_op($1, AST::Ops::GR_EQ, $3); };
           | expr ADD expr                  { $$ = AST::make_op($1, AST::Ops::ADD, $3); };
           | expr MIN expr                  { $$ = AST::make_op($1, AST::Ops::SUB, $3); };
           | expr MUL expr                  { $$ = AST::make_op($1, AST::Ops::MUL, $3); };
           | expr DIV expr                  { $$ = AST::make_op($1, AST::Ops::DIV, $3); };
           | expr MOD expr                  { $$ = AST::make_op($1, AST::Ops::MOD, $3); };
           | expr_un                        { $$ = $1; };

expr_un:     MIN expr_un  %prec NEG         { $$ = AST::make_un(AST::Ops::NEG, $2); };
           | NOT expr_un %prec NOT          { $$ = AST::make_un(AST::Ops::NOT, $2); };
           | expr_term                      { $$ = $1; };

expr_term:   LP expr[e] RP                  { $$ = $e; };
           | NAME                           { $$ = AST::make_ref($1); };
           | INT                            { $$ = AST::make_cst($1); };
           | SCAN                           { $$ = AST::make_scan(); };
        /* | scope                          { $$ = AST::make_scope(); }; */
        /* | func_call                      { $$ = AST::make_fcall(); }; */

/* 
func_call:   NAME LP call_argv RP           { SOMETHING };

func_def:    FUNC LP def_argv RP scope      { SOMETHING };
           | FUNC LP def_argv RP COLON NAME 
             scope                          { SOMETHING };

def_argv:    NAME                           { SOMETHING };
           | def_argv COMMA NAME            { SOMETHING };

call_argv:   expr                           { SOMETHING };
           | call_argv COMMA expr           { SOMETHING };
*/

if:          IF LP expr[e] RP 
               br_stm[s]                    { $$ = AST::make_if($e, $s); };
           | IF LP expr[e] RP 
               br_stm[s1]
             ELSE 
               br_stm[s2]                   { $$ = AST::make_if_else($e, $s1, $s2); };
            /* dangling else */ /* this rule creates shift-reduce conflict  */

while:       WHILE LP expr[e] RP
               br_stm[s]                    { $$ = AST::make_while($e, $s); };

print:       PRINT expr SCOLON              { $$ = AST::make_print($2); };

%%

void WholeProgramAction()
  {
    #if (CODEGEN == 1)
      CUR_SCOPE->codegen();
    #endif
  }

namespace yy
{

  void parser::error (const parser::location_type& location, const std::string& string)
  {
    std::cerr << string << " in (line.column): "<< location << std::endl;
  }

  parser::token_type yylex(parser::semantic_type* yylval, parser::location_type* yylloc, Driver* driver)
  {
    return driver->yylex(yylval, yylloc);
  }

  void parser::report_syntax_error(parser::context const& ctx) const
  {
    driver->report_syntax_error(ctx);
  }

}

