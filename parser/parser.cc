#include "parser.hh"

yy::location OurFlexLexer::get_cur_location()
{
  return cur_location_;
}

bool OurFlexLexer::is_empty_line(const char *str) const
{
  char sym = *str;
  return std::isspace(sym) && std::iscntrl(sym);
}

int OurFlexLexer::get_last_line_() const
{
  return last_num_of_line;
}

void OurFlexLexer::upd_cur_loc()
{
  auto cur_num_of_line = lineno();

  auto prev_end_column = cur_location_.end.column;
  cur_location_.begin.line = cur_location_.end.line = cur_num_of_line;

  if (is_empty_line(yytext))
    cur_location_.begin.column = cur_location_.end.column = 1;

  else
  {
    cur_location_.begin.column = prev_end_column;
    cur_location_.end.column = cur_location_.begin.column + YYLeng();
  }
  last_num_of_line = cur_num_of_line;
}
