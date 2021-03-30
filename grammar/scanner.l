%option c++

%option noyywrap
%option yylineno

%option warn nodefault debug noinput 
%option yyclass="OurFlexLexer"

%s IN_COMMENT

%{

#include "../FB_BLD/compiler.tab.hh"
#include "../parser/parser.hh"
//#include "../Driver/driver.hh"

#define YY_USER_ACTION upd_cur_loc();

%}

WS        [ \t\n\v]+
COMM      "//".*"\n"
MULCOMM   "/*" (.|WS)*? "*/"

DIGIT     [0-9]

LETTER    [a-zA-Z]
LETTER_D  [A-Za-z0-9_]

INT       {DIGIT}{DIGIT}*
NAME      {LETTER}{LETTER_D}*

%%

{WS}                 /* skip blanks and tabs */
{COMM}               /* skip comments to the end of line */

<INITIAL>"/*"        {BEGIN(IN_COMMENT);}

<IN_COMMENT>"*/"     {BEGIN(INITIAL);}
  
<IN_COMMENT>[^*\n]+  /* skip comments */
<IN_COMMENT>"*"      /* skip lone star */


"+"                  return yy::parser::token_type::ADD;
"-"                  return yy::parser::token_type::MIN;
"*"                  return yy::parser::token_type::MUL;
"/"                  return yy::parser::token_type::DIV;
"%"                  return yy::parser::token_type::MOD;
"="                  return yy::parser::token_type::ASSIGN;

"||"                 return yy::parser::token_type::OR;
"&&"                 return yy::parser::token_type::AND;
"!"                  return yy::parser::token_type::NOT;

">"                  return yy::parser::token_type::GREATER;
"<"                  return yy::parser::token_type::LESS;
">="                 return yy::parser::token_type::GR_EQ;
"<="                 return yy::parser::token_type::LS_EQ;
"=="                 return yy::parser::token_type::IS_EQ;
"!="                 return yy::parser::token_type::NOT_EQ;

","                  return yy::parser::token_type::COMMA;
":"                  return yy::parser::token_type::COLON;
";"                  return yy::parser::token_type::SCOLON;

"("                  return yy::parser::token_type::LP; /* LEFT PARENTHESS  */
")"                  return yy::parser::token_type::RP; /* RIGHT PARENTHESS */
"{"                  return yy::parser::token_type::LB; /* LEFT BRACE       */
"}"                  return yy::parser::token_type::RB; /* RIGHT BRACE      */

{INT}                return yy::parser::token_type::INT;

"if"                 return yy::parser::token_type::IF;
"else"               return yy::parser::token_type::ELSE;
"while"              return yy::parser::token_type::WHILE;
"return"             return yy::parser::token_type::RETURN;
"func"               return yy::parser::token_type::FUNC;   

"?"                  return yy::parser::token_type::SCAN;
"print"              return yy::parser::token_type::PRINT;

{NAME}               return yy::parser::token_type::NAME;

.                    return yy::parser::token_type::ERR;

%%

// nothing
