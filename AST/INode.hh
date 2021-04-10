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

using pINode = std::unique_ptr<INode>;
using pIScope = std::unique_ptr<IScope>;
using pFunc = std::shared_ptr<INode>;
/**
 * @typedef var_table
 * @brief Useful typedef for variables table type
 */

namespace detail
{
enum class SymType
{
  FUNC,
  VAR
};

struct var_tbl_member final
{
  SymType type;
  pFunc func;
  int value;
  
  var_tbl_member() = default;

  var_tbl_member(SymType tpe, int val, pFunc pfn = nullptr) : type(tpe), func(pfn), value(val)
  {}

};
}

using var_table = std::unordered_map<std::string, detail::var_tbl_member>;
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

  virtual INode *get_i_child(size_t) const
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
  virtual void push(pINode &node) = 0;

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

/**
 * @fn make_cst
 * @brief Make constant value node function
 * @param[in] val value to put to node
 * @return shared pointer to created node
 */
pINode make_cst(int val);

/**
 * @fn make_op
 * @brief Create operator node function
 * @param[in] l  left node of operator
 * @param[in] op operator type
 * @param[in] r  right node of operator
 * @return shared pointer to created Node
 */
pINode make_op(pINode &l, Ops op, pINode &r);

/**
 * @brief Make unary operator node function
 *
 * @param[in] op  enum type of operator
 * @param[in] operand shared pointer to operand node
 * @return shared pointer to created node
 */
pINode make_un(Ops op, pINode &operand);

/**
 * @fn make_while
 * @brief Make while node fucntion
 * @param[in] cond shared pointer to condition node
 * @param[in] sc shared pointer to scope
 * @return shared pointer to created Node
 */
pINode make_while(pINode &cond, pIScope &sc);

/**
 * @fn make_if_else
 * @brief Create if_else node fucntion
 * @param[in] cond condition
 * @param[in] isc if scope
 * @param[in] esc else scope
 * @return pointer to created Node
 */
pINode make_if_else(pINode &cond, pIScope &isc, pIScope &esc);

/**
 * @fn make_if
 * @brief Create if node fucntion
 * @param[in] cond condition
 * @param[in] isc if scope
 * @param[in] esc else scope
 * @return pointer to created Node
 */
pINode make_if(pINode &cond, pIScope &isc);

/**
 * @fn make_asgn
 * @brief Make assignment node function
 * @param[in] var_name name of a variable to assign to
 * @param[in] expr expression to assign
 * @return shared pointer to created node
 */
pINode make_asgn(const std::string &var_name, pINode &expr);

/**
 * @fn make_asgn_func
 * @brief Make assignment node function
 * @param[in] var_name name of a variable to assign function to
 * @param[in] func function to assign
 * @return shared pointer to created node
 */
//pINode make_asgn_func(const std::string &var_name, pINode &func);
// TODO: FINISH

/**
 * @fn make ref
 * @brief Make var node for expression
 * @param[in] var_name name of a variable
 * @return shared pointer to created node
 */
pINode make_ref(const std::string &var_name);

/*!
 * @fn make_print
 * @brief Make print node function
 * @param[in] expr shared pointer to expression node
 * @return shared pointer to created node
 */
pINode make_print(pINode &expr);

/**
 * @brief make scan node function
 * @return shared pointer to created node
 */
pINode make_scan();

/**
 * @fn make_scope
 * @brief Create scope function
 * @param[in] par shared pointer to parent node
 * @return shared pointer to created Scope
 */
pIScope make_scope(IScope *par = nullptr);


void clear(pIScope & root);


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
