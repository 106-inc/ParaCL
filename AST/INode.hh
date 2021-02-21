#ifndef INODE_HH
#define INODE_HH

#include <string>

namespace AST
{
  // node interface
  struct INode
  {
      virtual int calc() = 0;

      virtual void dump() = 0;

      virtual ~INode() = default;
  };

  // scope interface
  struct IScope : public INode
  {
      virtual void push(INode *node) = 0;

      virtual IScope *reset_scope()  const = 0;

      virtual void add_branch(INode *branch) = 0;

      virtual INode *access(std::string const &var_name) = 0;

      virtual INode *visible(std::string const &var_name) = 0;
  };

  enum class Ops
  {
    ADD, SUB,
    MUL, DIV, MOD,

    GREATER, LESS,
    GR_EQ, LS_EQ,
    IS_EQ, NOT_EQ,

    AND, OR,

    UNMIN, NOT
  };

  INode *make_value(int val);

  INode *make_op(INode *l, Ops o, INode *r);

  INode *make_while(INode *o, INode *s);

  INode *make_if(INode *o, INode *s);

  IScope *create_scope();
} // namespace

#endif /* INODE_HH */