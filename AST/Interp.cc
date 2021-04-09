#include "Interp.hh"

namespace AST
{

/**
 * @brief Interpret AST function
 *
 */
void Interp::interpret()
{
  std::stack<StkFrame> FrameStack{};

  /* We have to start from smth. Let it be global scope. */
  FrameStack.emplace(globl_, 0);

  while (!FrameStack.empty())
  {
    auto &fr_top = FrameStack.top();

    /* Get the current node ptr */
    INode *pNode = fr_top.node;

    size_t childs_am = pNode->get_ch_size();

    /* Index of child that we will visit now */
    int i_next = fr_top.state;

    /* If there is no more children - calculate current node and pop it from stack. */
    if (i_next == static_cast<int>(States::END))
    {
      fr_top.node->calc();
      FrameStack.pop();
      continue;
    }
    if (i_next < 0)
      throw std::runtime_error{"Negative child index"};

    /* Changing child index to next */
    fr_top.state++;

    /* If index is out of range - set state to END. */
    if (childs_am != std::numeric_limits<size_t>::max() && fr_top.state >= static_cast<int>(childs_am))
      fr_top.state = static_cast<int>(States::END);

    /* Get pointer to child */
    auto pch = pNode->get_i_child(i_next);
    if (pch == nullptr)
    {
      fr_top.state = static_cast<int>(States::END);
      continue;
    }
    /* Append frame stack */
    FrameStack.emplace(pch, 0);

    if (pch->get_ch_size() == 0)
      FrameStack.top().state = static_cast<int>(States::END);
  }
} /* End of 'interpret' function */
} // namespace AST