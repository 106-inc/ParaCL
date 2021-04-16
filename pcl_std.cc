#include <iostream>

extern "C" void __pcl_start();

extern "C" void __pcl_print(int n)
{
  std::cout << n << std::endl;
}

extern "C" int __pcl_scan()
{
  int n;
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
