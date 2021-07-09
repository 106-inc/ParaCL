#ifndef CLI_HH
#define CLI_HH

#include <string>

namespace CLI
{
bool arg_parse(int ac, char **av);

std::string &input_filename();
std::string &output_filename();
} // namespace CLI

#endif // CLI_HH