#include "driver.hh"

AST::IScope *CUR_SCOPE = nullptr;

yy::Driver::Driver(const char *name_of_file) : name_of_file_(name_of_file)
{
  plex_ = new OurFlexLexer;
  CUR_SCOPE = AST::make_scope();

  in_file.open(name_of_file);

  /* this part of code exist for future compilation */
  /*
  if (in_file.is_open())
  {
    while (in_file)
    {
      std::string tmp_str;
      std::getline(in_file, tmp_str);
      line_of_prog.push_back(tmp_str);
    }
  }
  */
  /* but no there is just interpratation */

  plex_->switch_streams(in_file, std::cout);
}

bool yy::Driver::parse()
{
  yy::parser parser_(this);

  bool res = parser_.parse();
  return res;
}

//! The lexical analyzer function, yylex, recognizes tokens from the input stream and returns them to the parser.
//! \param yylval
//! \param yylloc
//! \return token type
yy::parser::token_type yy::Driver::yylex(yy::parser::semantic_type *yylval, parser::location_type *yylloc)
{
  yy::parser::token_type tkn_type = static_cast<yy::parser::token_type>(plex_->yylex());

  switch (tkn_type)
  {
  case yy::parser::token_type::INT: {
    yylval->as<int>() = std::stoi(plex_->YYText());
    break;
  }

  case yy::parser::token_type::NAME: {
    std::string tmp{plex_->YYText()};
    yylval->as<std::string>() = tmp;
    break;
  }

  default:
    break;
  }
  *yylloc = plex_->get_cur_location();
  return tkn_type;
}

yy::Driver::~Driver()
{
  delete plex_;
}