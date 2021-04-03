#include "AST/Node.hh"
#include "driver.hh"

int main(int argc, char **argv)
{
  if (argc < 2)
  {
    std::cout << "USAGE: " << argv[0] << " FILENAME" << std::endl;
    return 0;
  }

  auto root = AST::make_scope();

  CUR_SCOPE = root;

  try
  {
    yy::Driver driver(argv[1]);
    if (!driver.parse())
      return 1;

    root->calc();
  }
  catch (std::runtime_error &err)
  {
    std::cerr << err.what() << std::endl;
  }

  return 0;
}