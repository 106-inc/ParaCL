#ifndef PARACL_PARSER_HH
#define PARACL_PARSER_HH

#include <iostream>

#ifndef yyFlexLexer
#include <FlexLexer.h>
#endif /* yyFlexLexer */

#include "../AST/INode.hh"
#include "../FB_BLD/compiler.tab.hh"

class OurFlexLexer : public yyFlexLexer
{
private:
  yy::location cur_location_{};
  std::string cur_str_{};

public:
  OurFlexLexer() = default;

  OurFlexLexer(const OurFlexLexer &flx) = delete;
  OurFlexLexer &operator=(const OurFlexLexer &) = delete;

  OurFlexLexer(OurFlexLexer &&flx) = delete;

  yy::location get_cur_location();
  std::string get_cur_str_();

  void upd_cur_loc();

  int yylex() override;

  ~OurFlexLexer() = default;
};

#endif // PARACL_PARSER_HH