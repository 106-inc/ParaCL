#ifndef __INTERP_H__
#define __INTERP_H__

#include <stack>

#include "INode.hh"

namespace AST
{
struct StkFrame;

extern std::stack<int> ValStack;
extern std::stack<StkFrame> FrameStack;

enum class States : int
{
  END = -1,
};

class Interp final
{
private:
  pIScope globl_;

public:
  Interp(pIScope globl) : globl_(globl)
  {
  }

  void interpret();
};

struct StkFrame final
{
  pINode node;
  int state;

  StkFrame(const pINode &nd, int st) : node(nd), state(st)
  {
  }
};
} // namespace AST

#endif // __INTERP_H__