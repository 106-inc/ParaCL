/**
 * @file INode.hh
 * @brief Base declarations for AST
 */

#ifndef INODE_HH
#define INODE_HH
#include <iostream>
#include <memory>
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
  virtual void push(const pINode &node) = 0;

  virtual std::weak_ptr<IScope> reset_scope() const = 0;

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
