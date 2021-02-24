#include "driver.hh"

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cout << "Lack of arguments!\n";
        return 0;
    }

    yy::Driver driver(argv[1]);
    driver.parse();
}