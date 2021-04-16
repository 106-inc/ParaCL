
#ifndef PARACL_DRIVER_HH
#define PARACL_DRIVER_HH

#include <fstream>
#include <iostream>
#include <vector>
#include <filesystem>

#include "Interp.hh"
#include "parser.hh"

/* LLVM lib */
#include "llvm/ADT/APInt.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/Transforms/Utils.h"

#include "llvm/ADT/StringRef.h"
#include "llvm/Support/raw_ostream.h"

/* End of LLVM lib */

#ifndef yyFlexLexer
#include <FlexLexer.h>
#endif /* yyFlexLexer */

#if (CODEGEN == 1)

extern llvm::LLVMContext* CUR_CONTEXT;
extern llvm::IRBuilder<>* BUILDER;
extern llvm::Module* CUR_MODULE;
extern llvm::Function* CUR_FUNC;

#endif

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

  /*void codegen();*/
  void IR_builder();

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

  void report_expctd_tok(const parser::context &ctx);

  void report_unexpctd_tok(const parser::context &ctx);

  void Runtime_err_prcsng(std::runtime_error &err, const yy::parser &parser);

  using s_type = parser::symbol_kind::symbol_kind_type;

  ~Driver();
};

} // namespace yy


#endif // PARACL_DRIVER_HH
