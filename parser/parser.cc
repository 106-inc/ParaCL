//
// Created by anon on 21.02.2021.
//

#include "parser.hh"

yy::location OurFlexLexer::get_cur_location() { return cur_location_; }

std::string OurFlexLexer::get_cur_str_() { return cur_str_; }

void OurFlexLexer::upd_cur_loc()
{
    int cur_num_of_line = lineno();

    cur_location_.lines(cur_num_of_line - last_num_of_line);
    cur_location_.columns((YYLeng()));
    cur_location_.step();

    last_num_of_line = cur_num_of_line;
}

