#ifndef PARACL_PARSER_HH
#define PARACL_PARSER_HH

#include <cctype>
#include <iostream>

#ifndef yyFlexLexer
#include <FlexLexer.h>
#endif /* yyFlexLexer */

#include "compiler.tab.hh"

class OurFlexLexer : public yyFlexLexer
{
private:
  yy::location cur_location_{};

  int last_num_of_line{};

public:
  OurFlexLexer() = default;

  OurFlexLexer(const OurFlexLexer &flx) = delete;
  OurFlexLexer &operator=(const OurFlexLexer &) = delete;

  OurFlexLexer(OurFlexLexer &&flx) = delete;
  OurFlexLexer &operator=(OurFlexLexer &&) = delete;

  yy::location get_cur_location();
  int get_last_line_() const;

  static bool is_empty_line(const char *str);

  void upd_cur_loc();

  int yylex() override;

  ~OurFlexLexer() override = default;
};

#endif // PARACL_PARSER_HH