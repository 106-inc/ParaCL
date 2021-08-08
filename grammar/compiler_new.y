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

%nterm<AST::Ops>
  bin_tok
  un_tok
  ;

%nterm<AST::pIScope>

  scope
  op_sc
  ;

%nterm<AST::pINode> 
  stm
  stms
  oper
  ;

%nterm<AST::pINode>

  if
  while
  print
  assign
  ;

%nterm<AST::pINode>

  exp1 
  expr
  expr_term
  ;

/* some tokens */
%right ASSIGN  "="

%right ELSE THEN

%left BIN
%left OR
%left AND 
%left IS_EQ NOT_EQ
%left GREATER LESS GR_EQ LS_EQ
%left ADD MIN
%left MUL DIV MOD

%left UN
%left NEG NOT

%precedence nscope
%precedence greedy

%%


program:     stms                           { WholeProgramAction(); };

scope:       op_sc stms cl_sc               { $$ = $1;};

op_sc:       LB                             {
                                              $$ = AST::make_scope(CUR_SCOPE); 
                                              CUR_SCOPE = $$.get();
                                            };

cl_sc:       RB                             { CUR_SCOPE = CUR_SCOPE->reset_scope(); };

stms:        stms stm                       { CUR_SCOPE->push($2); };
           | stm                            { CUR_SCOPE->push($1); };

stm:         exp1 SCOLON                    { $$ = $1; }
           | oper                           { $$ = $1; }
           | SCOLON                         {}

oper:        assign                         { $$ = $1; };
           | if                             { $$ = $1; };
           | while                          { $$ = $1; };
           | print                          { $$ = $1; };
           | scope                          { $$ = $1; };

assign:      NAME ASSIGN expr SCOLON        { $$ = AST::make_asgn($1, $3); };


expr:        expr bin_tok expr    %prec BIN { $$ = AST::make_op($1, $2, $3); };
           | un_tok expr          %prec UN  { $$ = AST::make_un($1, $2); };
           | expr_term                      { $$ = $1; };

exp1:        exp1 bin_tok expr    %prec BIN { $$ = AST::make_op($1, $2, $3); };
           | un_tok exp1          %prec UN  { $$ = AST::make_un($1, $2); };
           | LP exp1[e] RP                  { $$ = $e; };
           | NAME                           { $$ = AST::make_ref($1); };
           | INT                            { $$ = AST::make_cst($1); };
           | SCAN                           { $$ = AST::make_scan(); };

expr_term:   LP expr[e] RP                  { $$ = $e; };
           | NAME                           { $$ = AST::make_ref($1); };
           | INT                            { $$ = AST::make_cst($1); };
           | SCAN                           { $$ = AST::make_scan(); };
           | scope                          { 
                                              $$ = $1;
                                              CUR_SCOPE->pop();
                                            };

if:          IF LP expr[e] RP 
               stm[s] %prec THEN            {
                                              $$ = AST::make_if($e, $s);
                                              if ($s->is_scope)
                                                CUR_SCOPE->pop();
                                            };
           | IF LP expr[e] RP 
               stm[s1]
             ELSE 
               stm[s2]                      {
                                              $$ = AST::make_if_else($e, $s1, $s2);
                                              if ($s1->is_scope)
                                                CUR_SCOPE->pop();
                                              if ($s2->is_scope)
                                                CUR_SCOPE->pop();
                                            };

while:       WHILE LP expr[e] RP
                  stm[s]                    {
                                              $$ = AST::make_while($e, $s);
                                              if ($s->is_scope)
                                                CUR_SCOPE->pop();
                                            };

print:       PRINT expr SCOLON              { $$ = AST::make_print($2); };

un_tok:      MIN expr                       { $$ = AST::Ops::NEG; };
           | NOT expr                       { $$ = AST::Ops::NOT; };

bin_tok:     OR                             { $$ = AST::Ops::OR; };
           | AND                            { $$ = AST::Ops::AND; };
           | IS_EQ                          { $$ = AST::Ops::IS_EQ; };
           | NOT_EQ                         { $$ = AST::Ops::NOT_EQ; };
           | GREATER                        { $$ = AST::Ops::GREATER; };
           | LESS                           { $$ = AST::Ops::LESS; };
           | LS_EQ                          { $$ = AST::Ops::LS_EQ; };
           | GR_EQ                          { $$ = AST::Ops::GR_EQ; };
           | ADD                            { $$ = AST::Ops::ADD; };
           | MIN                            { $$ = AST::Ops::SUB; };
           | MUL                            { $$ = AST::Ops::MUL; };
           | DIV                            { $$ = AST::Ops::DIV; };
           | MOD                            { $$ = AST::Ops::MOD; };

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

