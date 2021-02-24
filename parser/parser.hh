#ifndef PARACL_PARSER_HH
#define PARACL_PARSER_HH

#include <iostream>

#include "../FB_BLD/compiler.tab.hh"
#include "../AST/INode.hh"


#ifndef yyFlexLexer
#include <FlexLexer.h>
#endif


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

#endif // PARACL_PARSER_HH