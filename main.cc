#include "Interp.hh"
#include "driver.hh"

int main(int argc, char **argv)
{
  if (argc < 2)
  {
    std::cout << "USAGE: " << argv[0] << " FILENAME" << std::endl;
    return 0;
  }

  auto root = AST::make_scope();
  CUR_SCOPE = root.get();

#if (CODEGEN == 0)
  AST::Interp interp(CUR_SCOPE);
#endif

#if (CODEGEN == 1)

  CUR_CONTEXT = new llvm::LLVMContext;
  CUR_MODULE = new llvm::Module("pcl.module", *CUR_CONTEXT);
  BUILDER = new llvm::IRBuilder<>(*CUR_CONTEXT);

#endif

  try
  {
    yy::Driver driver(argv[1]);

#if (CODEGEN == 0)
    if (!driver.parse())
      return -1;

    interp.interpret();
#endif

#if (CODEGEN == 1)


  driver.IR_builder();
/*
    std::ostringstream s;
    s << std::filesystem::path(argv[1]).filename().string() << ".ll";

    std::cout << "Saving module to: " << s.str() << "\n";

    std::error_code EC;
    llvm::raw_fd_ostream outfile{s.str(), EC};

    if (EC)
      llvm::errs() << EC.message().c_str() << "\n";

    driver.codegen();

    CUR_MODULE->print(outfile, nullptr);
    outfile.close();

    if (outfile.has_error())
    {
      llvm::errs() << "Error printing to file: " << outfile.error().message() << "\n";
    }
*/
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