#include "driver.hh"

AST::IScope *CUR_SCOPE = nullptr;

llvm::LLVMContext *CUR_CONTEXT{};
llvm::IRBuilder<> *BUILDER{};
llvm::Module *CUR_MODULE{};
llvm::Function *CUR_FUNC{};

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

bool yy::Driver::parse()
{
  yy::parser parser_(this);
  bool res;

  try
  {
    res = !parser_.parse();
  }
  catch (std::runtime_error &err)
  {
    Runtime_err_prcsng(err, parser_);
  }

  return res;
}

void yy::Driver::codegen()
{
  llvm::Type *prototypes[] = {llvm::Type::getInt32Ty(*CUR_CONTEXT)};

  /* prototype for print function */

  llvm::FunctionType *func_type_print = llvm::FunctionType::get(llvm::Type::getVoidTy(*CUR_CONTEXT), prototypes, false);

  llvm::Function::Create(func_type_print, llvm::Function::ExternalLinkage, "__pcl_print", CUR_MODULE);

  /* prototype for scan function */

  llvm::FunctionType *func_type_scan = llvm::FunctionType::get(llvm::Type::getInt32Ty(*CUR_CONTEXT), false);

  llvm::Function::Create(func_type_scan, llvm::Function::ExternalLinkage, "__pcl_scan", CUR_MODULE);

  /* protype for start function */

  llvm::FunctionType *func_type = llvm::FunctionType::get(llvm::Type::getVoidTy(*CUR_CONTEXT), false);

  CUR_FUNC = llvm::Function::Create(func_type, llvm::Function::ExternalLinkage, "__pcl_start", CUR_MODULE);

  /* creating basic block */

  auto bas_block = llvm::BasicBlock::Create(*CUR_CONTEXT, "entry", CUR_FUNC);

  BUILDER->SetInsertPoint(bas_block);

  parse();

  BUILDER->CreateRetVoid();
}

void yy::Driver::IR_builder()
{
  /* there is shuld be code from main */
}

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
    std::cerr << "UNKNOWN TOKEN" << std::endl;
  }

  default:
    break;
  }
  *yylloc = plex_->get_cur_location();
  return tkn_type;
}

void yy::Driver::report_syntax_error(const parser::context &ctx)
{
  yy::location loc = ctx.location();
  parser::symbol_kind_type lookahead = ctx.token();

  std::cerr << "syntax error in ";
  std::cerr << "line: " << loc.begin.line;
  std::cerr << ", column: " << loc.begin.column << std::endl;

  if (lookahead == s_type::S_UNKNOWN_VAR)
  {
    report_unexpctd_tok(ctx);
    return;
  }

  report_expctd_tok(ctx);
  report_unexpctd_tok(ctx);
}

void yy::Driver::report_expctd_tok(const yy::parser::context &ctx)
{
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
}

void yy::Driver::report_unexpctd_tok(const yy::parser::context &ctx)
{
  // Report the unexpected token.
  yy::location loc = ctx.location();
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

yy::Driver::~Driver()
{
  delete plex_;
}

void yy::Driver::Runtime_err_prcsng(std::runtime_error &err, const yy::parser &parser_)
{
  //! TODO: create more cases for processing: unknown var, unknown op etc ...
  //! But now there is so dumb realization
  parser::symbol_type s_type{parser::token::token_kind_type::UNKNOWN_VAR, plex_->get_cur_location()};
  parser::context ctx{parser_, s_type};

  std::cerr << err.what() << std::endl;
  report_syntax_error(ctx);
}
