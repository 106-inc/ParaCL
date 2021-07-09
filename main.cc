#include "CLI.hh"
#include "Interp.hh"
#include "driver.hh"

int main(int argc, char **argv)
{
  CLI::arg_parse(argc, argv);

  auto root = AST::make_scope();
  CUR_SCOPE = root.get();

#if (CODEGEN == 0)
  AST::Interp interp(CUR_SCOPE);
#else
  CUR_CONTEXT = new llvm::LLVMContext;
  CUR_MODULE = new llvm::Module("pcl.module", *CUR_CONTEXT);
  BUILDER = new llvm::IRBuilder<>(*CUR_CONTEXT);
#endif

#if (CODEGEN == 0)
  std::string out{};
#else
  std::string out = CLI::output_filename();
#endif

  try
  {
    yy::Driver driver(CLI::input_filename(), out);

#if (CODEGEN == 0)
    if (!driver.parse())
      return -1;

    interp.interpret();
#else
    driver.IR_builder();
    delete BUILDER;
    delete CUR_MODULE;
    delete CUR_CONTEXT;
#endif
  }
  catch (std::runtime_error &err)
  {
    std::cerr << err.what() << std::endl;
  }

  return 0;
}