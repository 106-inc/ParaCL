
#ifndef PARACL_DRIVER_HH
#define PARACL_DRIVER_HH

#include <fstream>
#include <iostream>
#include <vector>

#include "Interp.hh"
#include "parser.hh"

#ifndef yyFlexLexer
#include <FlexLexer.h>
#endif /* yyFlexLexer */

enum
{
  NUM_OF_TOKENS = 30
};

namespace yy
{
class Driver final
{
private:
  std::string name_of_file_;
  std::ifstream in_file;

  OurFlexLexer *plex_;
  std::vector<std::string> lines_of_prog;

public:
  /**
   * @brief Constructor for class Driver
   * @param name_of_file - the name of the file from which our program is read
   */
  Driver(const char *name_of_file);

  Driver(const Driver &drvr) = delete;

  Driver &operator=(const Driver &) = delete;

  /**
   * @brief Functuion for calling bison yy::parser:parse()
   * @return bool in
   */
  bool parse();

  /**
   * @brief The lexical analyzer function, yylex, recognizes tokens from the input stream and returns them to the
   * parser.
   * @param yylval
   * @param yylloc
   * @return token type
   */
  parser::token_type yylex(parser::semantic_type *yylval, parser::location_type *yylloc);

  /**
   * @brief Function for processing syntax error during parsing
   * @param ctx - the context that is created when an error is found
   */
  void report_syntax_error(const parser::context &ctx);

  //! Destructor for class Driver
  ~Driver();
};

} // namespace yy

#endif // PARACL_DRIVER_HH
