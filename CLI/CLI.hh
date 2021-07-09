#ifndef CLI_HH
#define CLI_HH

#include <string>

namespace CLI
{
bool arg_parse(int ac, char **av);

std::string &input_filename();

#if (CODEGEN == 1)

std::string &output_filename();

#endif

} // namespace CLI

#endif // CLI_HH