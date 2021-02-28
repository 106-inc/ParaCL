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
  // virtual void dump() = 0;

  virtual ~INode() = default;
};

// scope interface
struct IScope : public INode
{
  virtual void push(INode *node) = 0;

  virtual IScope *reset_scope() const = 0;

  // virtual void add_branch(INode *branch) = 0;

  virtual void add_var(const std::string &name) = 0;

  virtual std::pair<var_table::iterator, bool> check_var(const std::string &var_name) = 0;

  virtual std::pair<var_table::iterator, bool> loc_check(const std::string &var_name) = 0;

  virtual var_table::iterator check_n_insert(const std::string &var_name) = 0;
};

enum class Ops
{
  ASSIGN,

  ADD,
  SUB,
  MUL,
  DIV,
  MOD,

  GREATER,
  LESS,
  GR_EQ,
  LS_EQ,
  IS_EQ,
  NOT_EQ,

  AND,
  OR,

  UNMIN,
  NOT
};

// class predeclaration
class Scope;

void IMMA_DOIN(const char *doin_wha);

INode *make_cst(int val);
INode *make_op(INode *l, Ops op, INode *r);
INode *make_while(INode *cond, IScope *sc);
INode *make_if(INode *cond, IScope *isc, IScope *esc = nullptr);
INode *make_ass(const std::string &var_name, INode *expr);
INode *make_ref(const std::string &var_name);
INode *make_print(INode *expr);
IScope *make_scope(IScope *par = nullptr);

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

extern AST::IScope *CUR_SCOPE;

#endif /* INODE_HH */