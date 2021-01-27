%option c++

%{

// TODO: ТУТ ЯВНО НЕ ВСЕ - РАЗОБРАТЬСЯ
#include "compiler.tab.hh"

%}

WS        [ \t\n\v]+
DIGIT     [0-9]
DIGIT1    [1-9]
LETTER    [A-Za-z]
LETTER_D  [A-Za-z0-9_]


%%

// yy::parser::token_type:: - не окончательно, просто так было на лекциях

{WS}                 /* skip blanks and tabs */

"+"                  return yy::parser::token_type::PLUS;
"-"                  return yy::parser::token_type::MINUS;
"*"                  return yy::parser::token_type::MULTIPLY;
"/"                  return yy::parser::token_type::DIVIDE;
"%"                  return yy::parser::token_type::DON'T REMEMBER; // TODO: ВСПОМНИТЬ СЛОВО
"="                  return yy::parser::token_type::EQUAL;

">"                  return yy::parser::token_type::GREATER;
">="                 return yy::parser::token_type::GR_EQ;
"<"                  return yy::parser::token_type::LESS;
"<="                 return yy::parser::token_type::LS_EQ;
"=="                 return yy::parser::token_type::IS_EQ;
"!="                 return yy::parser::token_type::NOT_EQ;

";"                  return yy::parser::token_type::SCOLON;

{DIGIT1}{DIGIT}*     return yy::parser::token_type::NUMBER;
"0"                  return yy::parser::token_type::NUMBER;

"?"                  return yy::parser::token_type::SCAN;
"if"                 return yy::parser::token_type::IF;
"while"              return yy::parser::token_type::WHILE;
"print"              return yy::parser::token_type::PRINT;

{LETTER}{LETTER_D}*  return yy::parser::token_type::WORD; // TODO: МОЖЕТ ПЕРЕИМЕНОВАТЬ В VAR?

.                    return yy::parser::token_type::ERR;

%%

// nothing