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

  while (!FrameStack.empty())
  {
    auto &fr_top = FrameStack.top();   // fr_top == (global, 0)

    // get the current node ptr
    pINode pNode = fr_top.node;   // == global 
    
    size_t childs_am = pNode->get_ch_size(); // 2
    // get child of current node 
    int i_next = fr_top.state; // 0

    if (i_next == static_cast<int>(States::END))
    {
      fr_top.node->calc();
      FrameStack.pop();
      continue;
    }
    if (i_next < 0)
      throw std::runtime_error{"Negative child index"};

    // go to next state
    fr_top.state++;

    if (fr_top.state >= childs_am)
      fr_top.state = static_cast<int>(States::END);

    auto pch = pNode->get_i_child(i_next); // 
    if (pch == nullptr)
    {
      fr_top.state = static_cast<int>(States::END);
      continue;
    }
    FrameStack.emplace(pch, 0);

    if (pch->get_ch_size() == 0)
      FrameStack.top().state = static_cast<int>(States::END);

  }
} /* End of 'interpret' function */
} // namespace AST