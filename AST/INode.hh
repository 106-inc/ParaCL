#ifndef INODE_HH
#define INODE_HH

#include <iostream>
#include <string>
#include <unordered_map>

namespace AST
{
  /* Useful typedef */
  using var_table = std::unordered_map<std::string, int>;

  // node interface
  struct INode
  {
      virtual int calc() const = 0;

      // MB for ruture
      //virtual void dump() = 0;

      virtual ~INode() = default;
  };

  // scope interface
  struct IScope : public INode
  {
      virtual void push(INode *node) = 0;

      virtual IScope *reset_scope()  const = 0;

      //virtual void add_branch(INode *branch) = 0;

      virtual void add_var(const std::string &name) = 0;

      virtual std::pair<var_table::iterator, bool> access(const std::string &var_name) = 0;
      /*
      virtual INode *visible(std::string const &var_name) = 0;
       */
  };

  enum class Ops
  {
    ADD, SUB,
    MUL, DIV, MOD,

    GREATER,
    LESS,
    GR_EQ,
    LS_EQ,
    IS_EQ,
    NOT_EQ,

    AND, OR,

    UNMIN, NOT
  };

extern IScope * Cur_scope;

// class predeclaration
class Scope;

INode *make_cst(int val);
INode *make_op(INode *l, Ops op, INode *r);
INode *make_while(INode *cond, IScope *sc);
INode *make_if(INode *cond, IScope *isc, IScope *esc = nullptr);
IScope *make_scope( Scope *par = nullptr );

////////////////// TYPES OF NODES ////////////////////////
/*
 * 1. Variable -> just iterator to var table OK
 * 2. Infix operator -> OK
 * 3. While -> condition + scope
 * 4. if -> condition + scope
 * 5. else -> scope only
 * 5. I think it's all fir now
 */
//////////////////////////////////////////////////////////

} // namespace AST



#endif /* INODE_HH */