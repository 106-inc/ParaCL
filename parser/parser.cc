//
// Created by anon on 21.02.2021.
//

#include "parser.hh"


yy::OurFlexLexer::OurFlexLexer() : location_({})
{}


yy::location yy::OurFlexLexer::get_cur_location() { return cur_location_ }
std::string yy::OurFlexLexer::get_cur_str_() { return cur_str_ }




