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

extern AST::IScope *CUR_SCOPE;
}

/* some tokens */

%right ASSIGN   "="

%left

  ADD MIN
  MUL DIV MOD

  GREATER LESS
  GR_EQ LS_EQ
  IS_EQ NOT_EQ
  
  AND OR
  ;

%nonassoc

  NEG NOT
  ;

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
  expr_or
  expr_and
  expr_eqty
  expr_cmp
  expr_pm
  expr_mdm
  expr_un
  expr_term
  ;

%nterm<AST::Ops>

  pm
  mdm
  cmp
  eq_ty
  un
  ;

 /* 
 %nterm<AST::INode *>

  func_call
  func_def
  ;
 */

%%


program:     stms                           { /* program starting */ };

scope:       op_sc stms cl_sc               { /* nothing */ };

br_scope:    op_br_sc stms cl_sc            { $$ = std::move($1); };

op_sc:       LB                             { 
                                              auto par_tmp = CUR_SCOPE;
                                              auto u_ptr = AST::make_scope(par_tmp);
                                              CUR_SCOPE = u_ptr.get();
                                              if (par_tmp) par_tmp->push(std::move(u_ptr));
                                            };

op_br_sc:    LB                             { 
                                              $$ = AST::make_scope(CUR_SCOPE); 
                                              CUR_SCOPE = $$.get();
                                            };

cl_sc:       RB                             { CUR_SCOPE = CUR_SCOPE->reset_scope(); };

stms:        stm                            { CUR_SCOPE->push(std::move($1)); };
           | stms stm                       { CUR_SCOPE->push(std::move($2)); };
           | scope                          { /* nothing */ };
           | stms scope                     { /* nothing */ };

br_stm:     stm                             {
                                              $$ = AST::make_scope(CUR_SCOPE);
                                              $$->push(std::move($1));
                                            };
           | br_scope                       { $$ = std::move($1); };

stm:         assign                         { $$ = std::move($1); };
           | if                             { $$ = std::move($1); };
           | while                          { $$ = std::move($1); };
           | print                          { $$ = std::move($1); };
        /* | expr                           { $$ = std::move($1); }; */
        /* | RETURN expr                    { SOMETHING }; */

assign:      NAME ASSIGN expr SCOLON        { $$ = AST::make_asgn(std::move($1), std::move($3)); };


expr:        expr_or                        { $$ = std::move($1); };

expr_or:     expr_or OR expr_and            { $$ = AST::make_op(std::move($1), AST::Ops::OR, std::move($3)); };
           | expr_and                       { $$ = std::move($1); };

expr_and:    expr_and AND expr_eqty         { $$ = AST::make_op(std::move($1), AST::Ops::AND, std::move($3)); };
           | expr_eqty                      { $$ = std::move($1); };

expr_eqty:   expr_eqty eq_ty expr_cmp       { $$ = AST::make_op(std::move($1), $2, std::move($3)); };
           | expr_cmp                       { $$ = std::move($1); };

expr_cmp:    expr_cmp cmp expr_pm           { $$ = AST::make_op(std::move($1), $2, std::move($3)); };
           | expr_pm                        { $$ = std::move($1); };

expr_pm:     expr_pm pm expr_mdm            { $$ = AST::make_op(std::move($1), $2, std::move($3)); };
           | expr_mdm                       { $$ = std::move($1); };

expr_mdm:    expr_mdm mdm expr_term         { $$ = AST::make_op(std::move($1), $2, std::move($3)); };
           | expr_un                        { $$ = std::move($1); };

expr_un:     un expr_un                     { $$ = AST::make_un($1, std::move($2)); };
           | expr_term                      { $$ = std::move($1); };

expr_term:   LP expr[e] RP                  { $$ = std::move($e); };
           | NAME                           { $$ = AST::make_ref(std::move($1)); };
           | INT                            { $$ = AST::make_cst(std::move($1)); };
           | SCAN                           { $$ = AST::make_scan(); };
        /* | scope                          { $$ = AST::make_scope(); }; */
        /* | func_call                      { $$ = AST::make_fcall(); }; */
        /* | func_def                       { $$ = AST::make_fdef(); }; */

/* 
func_call:   NAME LP call_argv RP           { SOMETHING };

func_def:    FUNC LP def_argv RP scope      { SOMETHING };
           | FUNC LP def_argv RP COLON NAME 
             scope                          { SOMETHING };

def_argv:    NAME                           { SOMETHING };
           | def_argv COMMA NAME            { SOMETHING };

call_argv:   INT                            { SOMETHING };
           | call_argv COMMA INT            { SOMETHING };
*/

if:          IF LP expr[e] RP 
               br_stm[s]                   { $$ = AST::make_if(std::move($e), std::move($s)); };
           | IF LP expr[e] RP 
               br_stm[s1]
             ELSE 
               br_stm[s2]                  { $$ = AST::make_if(std::move($e), std::move($s1), std::move($s2)); };
            /* dangling else */ /* this rule creates shift-reduce conflict  */

while:       WHILE LP expr[e] RP
               br_stm[s]                   { $$ = AST::make_while(std::move($e), std::move($s)); };

pm:          ADD                            { $$ = AST::Ops::ADD; }; 
           | MIN                            { $$ = AST::Ops::SUB; }; 

mdm:         MUL                            { $$ = AST::Ops::MUL; };  
           | DIV                            { $$ = AST::Ops::DIV; };
           | MOD                            { $$ = AST::Ops::MOD; };

cmp:         GREATER                        { $$ = AST::Ops::GREATER; };
           | LESS                           { $$ = AST::Ops::LESS; };
           | GR_EQ                          { $$ = AST::Ops::GR_EQ; };
           | LS_EQ                          { $$ = AST::Ops::LS_EQ; };

eq_ty:       IS_EQ                          { $$ = AST::Ops::IS_EQ; };
           | NOT_EQ                         { $$ = AST::Ops::NOT_EQ; };

un:          MIN                            { $$ = AST::Ops::NEG; };
           | NOT                            { $$ = AST::Ops::NOT; };

print:       PRINT expr SCOLON                    { $$ = AST::make_print(std::move($2)); };

%%

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

