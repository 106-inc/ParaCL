/**
 * @file INode.hh
 * @brief Base declarations for AST
 */

#ifndef INODE_HH
#define INODE_HH

#include <forward_list>
#include <iostream>
#include <string>
#include <unordered_map>

/**
 * @namespace AST
 * @brief namespace for AST tree interfaces
 */
namespace AST
{
/**
 * @typedef var_table
 * @brief Useful typedef for variables table type
 */
using var_table = std::unordered_map<std::string, int>;

// node interface
struct INode
{
  INode() = default;

  virtual int calc() const = 0;

  INode(const INode &) = delete;
  INode &operator=(const INode &) = delete;

  // MB for ruture
  // virtual void dump() = 0;

  virtual ~INode() = default;
};

// scope interface
struct IScope : public INode
{
  // TODO: docs
  virtual void push(INode *node) = 0;

  virtual IScope *reset_scope() const = 0;

  virtual std::pair<var_table::iterator, bool> get_var(const std::string &var_name) = 0;

  virtual std::pair<var_table::iterator, bool> loc_check(const std::string &var_name) = 0;

  virtual var_table::iterator check_n_insert(const std::string &var_name) = 0;
};

// class predeclaration
class Scope;

/**
 * @enum Ops
 * @brief enum class with operators
 */
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

  NEG,
  NOT
};

class MemMan final
{
private:
  /**
   * It is not a vector because we need only to push object here
   * So we choose the most economic data structure in our opinion
   * without reallocations
   */
  std::forward_list<INode *> pnodes_;

public:
  MemMan(MemMan const &) = delete;
  MemMan &operator=(MemMan const &) = delete;

  static MemMan &manager()
  {
    static MemMan SingleTone{};

    return SingleTone;
  }

  INode *make_cst(int val);
  INode *make_op(INode *l, Ops op, INode *r);
  INode *make_un(Ops op, INode *operand);
  INode *make_while(INode *cond, IScope *sc);
  INode *make_if(INode *cond, IScope *isc, IScope *esc = nullptr);
  INode *make_asgn(const std::string &var_name, INode *expr);
  INode *make_ref(const std::string &var_name);
  INode *make_print(INode *expr);
  INode *make_scan();
  IScope *make_scope(IScope *par = nullptr);

  ~MemMan()
  {
    for (auto *pnode : pnodes_)
      delete pnode;
  }

private:
  MemMan() = default;
};

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

/////////////////////THOUGHTS ABOUT ERROR PROCESSING/////////////////////////
/*
 * 1) just delete input ptrs in case of exception thrown (try / catch)
 * 2) use ptr handlers for auto releaseaing the resource
 *  (has a problem with releasing after end of scope)
 */
/////////////////////////////////////////////////////////////////////////////

} // namespace AST

extern AST::IScope *CUR_SCOPE;

#endif /* INODE_HH */
