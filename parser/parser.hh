#ifndef PARACL_PARSER_HH
#define PARACL_PARSER_HH

#include <iostream>

#include <FlexLexer.h>

#include "../flex+bison/compiler.tab.hh"
#include "INode.hh"

namespace yy
{

class OurFlexLexer : public yyFlexLexer
{
  private:
    yy::location cur_location_;
    std::string cur_str_;

  public:
    OurFlexLexer();

    yy::location get_cur_location();
    std::string get_cur_str_();

    void upd_cur_loc();

    int yylex() override;

    ~OurFlexLexer();
};
} // namespace yy

#endif // PARACL_PARSER_HH