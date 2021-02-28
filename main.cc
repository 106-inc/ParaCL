#include "Driver/driver.hh"

int main(int argc, char **argv)
{
  if (argc < 2)
  {
    std::cout << "USAGE: " << argv[0] << " FILENAME" << std::endl;
    return 0;
  }


  //TODO: create class root handler
  auto root = AST::make_scope();

  CUR_SCOPE = root;
  yy::Driver driver(argv[1]);
  if (!driver.parse())
  {
    delete root;
    return 1;
  }

  root->calc();
  delete root;

  return 0;
}