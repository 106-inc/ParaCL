#include "driver.hh"

AST::IScope *CUR_SCOPE = nullptr;

//! Constructor for class Driver
//! \param name_of_file - the name of the file from which our program is read
yy::Driver::Driver(const char *name_of_file) : name_of_file_(name_of_file)
{
  std::string tmp_str;

  in_file.open(name_of_file);
  std::ifstream tmp(name_of_file);

  if (tmp.is_open())
  {
    while (tmp)
    {
      std::getline(tmp, tmp_str);
      lines_of_prog.push_back(tmp_str);
    }
  }
  else
  {
    std::string what = "File '" + name_of_file_ + "' does not exist";
    throw std::runtime_error{what};
  }

  plex_ = new OurFlexLexer;
  plex_->switch_streams(in_file, std::cout);
}

//! Functuion for calling bison yy::parser:parse()
//! \return bool in
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
//! \param ctx - the context that is created when an error is found
void yy::Driver::report_syntax_error(const parser::context &ctx)
{
  yy::location loc = ctx.location();

  std::cerr << "syntax error in ";
  std::cerr << "line: " << loc.begin.line;
  std::cerr << ", column: " << loc.begin.column << std::endl;

  // Report the tokens expected at this point.
  parser::symbol_kind_type expectd_tokns[NUM_OF_TOKENS];
  size_t num_of_expectd_tokns = ctx.expected_tokens(expectd_tokns, NUM_OF_TOKENS);

  std::cerr << "expected:";

  for (size_t i = 0; i < num_of_expectd_tokns; ++i)
  {
    if (i != 0)
      std::cerr << " or ";

    std::cerr << " <" << parser::symbol_name(expectd_tokns[i]) << "> ";
  }

  std::cerr << std::endl;

  // Report the unexpected token.
  parser::symbol_kind_type lookahead = ctx.token();

  std::cerr << "before: "
            << "<" << parser::symbol_name(lookahead) << ">" << std::endl;
  std::cerr << loc.begin.line << "   |   " << lines_of_prog[loc.begin.line - 1] << std::endl;
  std::cerr << "    |   ";

  for (int i = 0; i < loc.end.column - 1; ++i)
  {
    if (i == (ctx.lookahead().location.begin.column - 1))
      std::cerr << "^";

    else
      std::cerr << "~";
  }

  std::cerr << std::endl;
}

//! Destructor for class Driver
yy::Driver::~Driver()
{
  delete plex_;
}
