#include "Driver/driver.hh"

int main(int argc, char **argv)
{
  if (argc < 2)
  {
    std::cout << "USAGE: " << argv[0] << " FILENAME" << std::endl;
    return 0;
  }

  yy::Driver driver(argv[1]);
  driver.parse();
  delete CUR_SCOPE;
  return 0;
}