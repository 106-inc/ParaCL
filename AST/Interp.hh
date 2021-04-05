#ifndef __INTERP_H__
#define __INTERP_H__

#include <stack>

#include "INode.hh"

namespace AST
{
struct StkFrame;

extern std::stack<int> ValStack;

enum class States : int
{
  END = -1,
};

class Interp final
{
private:
  IScope *globl_;

public:
  Interp(IScope *globl) : globl_(globl)
  {
  }

  void interpret();
};

struct StkFrame final
{
  INode *node;
  int state;

  StkFrame(INode *nd, int st) : node(nd), state(st)
  {
  }
};
} // namespace AST

#endif // __INTERP_H__