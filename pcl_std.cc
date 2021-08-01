#include <iostream>

using IntT = int32_t;

extern "C" void __pcl_start();

extern "C" void __pcl_print(IntT n)
{
  std::cout << n << std::endl;
}

extern "C" IntT __pcl_scan()
{
  IntT n{};
  std::cin >> n;
  if (!std::cin)
    throw std::runtime_error{"Problem reading stdin"};

  return n;
}

int main()
{
  try
  {
    __pcl_start();
  }
  catch (std::runtime_error &err)
  {
    std::cerr << "Error ocurred:" << std::endl;
    std::cerr << err.what() << std::endl;
    return -1;
  }
  return 0;
};
