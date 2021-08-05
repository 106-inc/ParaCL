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
  op_sc
  ;

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
  expr_bin
  expr_term
  ;

/* some tokens */
%right ASSIGN  "="

%right ELSE THEN

%left OR
%left AND 
%left IS_EQ NOT_EQ
%left GREATER LESS GR_EQ LS_EQ
%left ADD MIN
%left MUL DIV MOD 
%left NEG NOT


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

stm:         assign                         { $$ = $1; };
           | if                             { $$ = $1; };
           | while                          { $$ = $1; };
           | print                          { $$ = $1; };
           | scope                          { $$ = $1; };

assign:      NAME ASSIGN expr SCOLON        { $$ = AST::make_asgn($1, $3); };

expr:        expr_bin                       { $$ = $1; };
           | expr_un                        { $$ = $1; };


expr_bin:    expr OR expr                   { $$ = AST::make_op($1, AST::Ops::OR, $3); };
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

expr_un:     MIN expr_un        %prec NEG   { $$ = AST::make_un(AST::Ops::NEG, $2); };
           | NOT expr_un                    { $$ = AST::make_un(AST::Ops::NOT, $2); };
           | expr_term                      { $$ = $1; };

expr_term:   LP expr[e] RP                  { $$ = $e; };
           | NAME                           { $$ = AST::make_ref($1); };
           | INT                            { $$ = AST::make_cst($1); };
           | SCAN                           { $$ = AST::make_scan(); };
           | scope                          { $$ = $1; };

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
                                              CUR_SCOPE->pop();
                                            };

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

