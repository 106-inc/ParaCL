#include <llvm/Support/CommandLine.h>
#include <string>

#include "CLI.hh"

namespace cl = llvm::cl;

static cl::opt<std::string> inp_fname(cl::Positional, cl::Required, cl::desc("<input file>"));

static cl::opt<std::string> out_fname("o", cl::desc("Specify output filename"), cl::value_desc("filename"),
                                      cl::init(""));

namespace CLI
{

bool arg_parse(int ac, char **av)
{
  cl::ParseCommandLineOptions(ac, av);
  return true;
}

std::string &input_filename()
{
  return inp_fname;
}

std::string &output_filename()
{
  if (out_fname.empty())
    return inp_fname;
  return out_fname;
}

} // namespace CLI
