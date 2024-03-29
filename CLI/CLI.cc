#include <llvm/Support/CommandLine.h>
#include <string>

#include "CLI.hh"

namespace cl = llvm::cl;

static cl::opt<std::string> inp_fname(cl::Positional, cl::Required, cl::desc("<input file>"));

#if (CODEGEN == 1)
static cl::opt<std::string> out_fname("o", cl::desc("Specify output filename"), cl::value_desc("filename"),
                                      cl::init(""));
#endif

static const std::string COMPILER_VERSION{"1.0"};
static const std::string INTERPRETER_VERSION{"1.0"};

static std::string version()
{
#if (CODEGEN == 1)
  return "ParaCL compiler v" + COMPILER_VERSION + "\n";
#else
  return "ParaCl interpreter v" + INTERPRETER_VERSION + "\n";
#endif
}

namespace CLI
{

bool arg_parse(int ac, char **av)
{
  cl::SetVersionPrinter([](llvm::raw_ostream &ros) { ros << version(); });
  cl::ParseCommandLineOptions(ac, av);
  return true;
}

std::string &input_filename()
{
  return inp_fname;
}

std::string &output_filename()
{
#if (CODEGEN == 0)

  static std::string empty{};
  return empty;

#else

  if (out_fname.empty())
    return inp_fname;
  return out_fname;

#endif
}

} // namespace CLI
