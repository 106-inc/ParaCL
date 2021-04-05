/**
 * @file INode.hh
 * @brief Base declarations for AST
 */

#ifndef INODE_HH
#define INODE_HH
#include <iostream>
#include <memory>
#include <stack>
#include <string>
#include <unordered_map>

/**
 * @namespace AST
 * @brief namespace for AST tree interfaces
 */
namespace AST
{

/** @brief forward declarations */
class IScope;
class INode;

using pINode = std::shared_ptr<INode>;
using pIScope = std::shared_ptr<IScope>;
/**
 * @typedef var_table
 * @brief Useful typedef for variables table type
 */
using var_table = std::unordered_map<std::string, int>;

// node interface
class INode
{
protected:
  size_t childs_am_;

  INode(size_t childs_am = 0) : childs_am_(childs_am)
  {
  }

public:
  virtual int calc() const = 0;

  virtual pINode get_i_child(size_t i) const
  {
    return nullptr;
  }

  INode(const INode &) = delete;
  INode &operator=(const INode &) = delete;

  size_t get_ch_size() const
  {
    return childs_am_;
  }
  // MB for ruture
  // virtual void dump() = 0;

  virtual ~INode() = default;
};

// scope interface
struct IScope : public INode
{
protected:
  IScope(size_t c_am = 0) : INode(c_am)
  {
  }

public:
  // TODO: docs
  virtual void push(const pINode &node) = 0;

  virtual pIScope reset_scope() const = 0;

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

pINode make_cst(int val);
pINode make_op(const pINode &l, Ops op, const pINode &r);
pINode make_un(Ops op, const pINode &operand);
pINode make_while(const pINode &cond, const pIScope &sc);
pINode make_if(const pINode &cond, const pIScope &isc, const pIScope &esc = nullptr);
pINode make_asgn(const std::string &var_name, const pINode &expr);
pINode make_ref(const std::string &var_name);
pINode make_print(const pINode &expr);
pINode make_scan();
pIScope make_scope(const pIScope &par = nullptr);
pIScope make_br_scope(const pIScope &par);

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

extern AST::pIScope CUR_SCOPE;

#endif /* INODE_HH */
