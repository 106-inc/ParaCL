#include "Interp.hh"

namespace AST
{
std::stack<int> ValStack{};
std::stack<StkFrame> FrameStack{};

/**
 * @brief Interpret AST function
 *
 */
void Interp::interpret()
{
  FrameStack.emplace(globl_, 0);

  while (FrameStack.top().state != static_cast<int>(States::END))
  {
    auto pNode = FrameStack.top().node;

    size_t chld_size = pNode->get_ch_size();

    for (size_t i = 0; i < chld_size; ++i)
    {
      
    }
  }
} /* End of 'interpret' function */
} // namespace AST