#include "driver.hh"

AST::IScope *CUR_SCOPE = nullptr;

yy::Driver::Driver(const char *name_of_file) : name_of_file_(name_of_file)
{
  plex_ = new OurFlexLexer;
  std::string tmp_str;

  in_file.open(name_of_file);
  std::ifstream tmp(name_of_file);

  if (in_file.is_open())
  {
    while (tmp)
    {
      std::getline(tmp, tmp_str);
      lines_of_prog.push_back(tmp_str);
    }
  }

  plex_->switch_streams(in_file, std::cout);
}

bool yy::Driver::parse()
{
  yy::parser parser_(this);

  bool res = !parser_.parse();
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
    yylval->emplace<int>(std::stoi(plex_->YYText()));
    break;
  }

  case yy::parser::token_type::NAME: {
    yylval->emplace<std::string>(std::string{plex_->YYText()});
    break;
  }

  case yy::parser::token_type::ERR: {
    std::cerr << "I DON'T FUCKIN KNOW WHAT IS THIS" << std::endl;
  }

  default:
    break;
  }
  *yylloc = plex_->get_cur_location();
  return tkn_type;
}

//!  Function for processing syntax error during parsing
//! \param ctx - contex, which ...
void yy::Driver::report_syntax_error(const parser::context &ctx)
{
  std::cerr << "syntax error in ";
  std::cerr << "line:" << ctx.location().begin.line;
  std::cerr << ", column:" << ctx.location().begin.column << "-" << ctx.location().end.column << ":" << std::endl;

  // Report the tokens expected at this point.
  /*
  parser::symbol_kind_type expectd_tokns[TOKENMAX];
  int num_of_expectd_tokns = ctx.expected_tokens(expectd_tokns, TOKENMAX);

  for (size_t i = 0; i < num_of_expectd_tokns; ++i)
  {
      if (i != 0)
          std::cerr << " or ";

      std::cerr << " <" << parser::symbol_name(expectd_tokns[i]) << "> ";
  }
  */

  // Report the unexpected token.

  parser::symbol_kind_type lookahead = ctx.token();
  std::cerr << "before "
            << "<" << lookahead << ">" << std::endl;
  std::cerr << lines_of_prog[ctx.location().begin.line - 1] << std::endl;
}

yy::Driver::~Driver()
{
  delete plex_;
}