#ifndef NODE_HH
#define NODE_HH

/////////////////////////////////////
///// STL containers ///////////////
#include <stack>
#include <vector>
/////////////////////////////////////

#include <limits>

////// OUR HEADERS //////////////////
#include "INode.hh"
/////////////////////////////////////

namespace AST
{
// Scope structure
class Scope : public IScope // final(?)
{
private:
  using it_bool = std::pair<var_table::iterator, bool>;

  // vector with nodes of this scope
  std::vector<pINode> nodes_{};

  // Pointer to parent scope
  IScope *parent_;

  var_table var_tbl_;

public:
  // constructor by parent scope ptr
  Scope(IScope *parent) : parent_(parent)
  {
  }

  IScope *reset_scope() const override
  {
    return parent_;
  }

  /**
   * @brief Interpret the scope function (claculate)
   * @return int
   */
  int calc() const override
  {
    return 0;
  } /* End of 'calc' function */

  INode *get_i_child(size_t i) const override
  {
    return nodes_.at(i).get();
  }

  /**
   * @brief Add node to scope function
   * @param[in] node node to add
   * @return none
   */
  void push(pINode &node) override
  {
    nodes_.push_back(std::move(node));
    childs_am_ = nodes_.size();
  } /* End of 'push' function */

  it_bool get_var(const std::string &var_name) override;

  it_bool loc_check(const std::string &var_name) override;

  var_table::iterator check_n_insert(const std::string &var_name) override;

private:
  /**
   * @brief Insert variable to table of current scope
   * @warning ONLY FOR PRIVATE USE - NO VALIDATION OF UNIQUE!!!
   * @param[in] var_name
   * @return iterator to inserted variable
   */
  var_table::iterator insert_var(const std::string &var_name)
  {
    auto it_bl = var_tbl_.insert({var_name, {0}});

    return it_bl.first;
  } /* End of 'insert_var' function */
};

/**
 * @brief Variable node class
 */
class VNode final : public INode
{
private:
  // No ptrs, because this node will always be a leaf

  var_table::iterator location_{};

public:
  /**
   * @brief Varibale node ctor
   * @param[in] loc iterator to location in variables table
   */
  VNode(var_table::iterator loc) : location_(loc)
  {
  }

  /**
   * Get variable name function
   * @return name of a variable
   */
  const std::string &get_name() const
  {
    return location_->first;
  }

  /**
   * Get variable location in table function
   * @return iterator to variable's location in var table
   */
  var_table::iterator get_loc() const
  {
    return location_;
  }

  /**
   * Calculate value of a variable function
   * @return current value of a variable
   */
  int calc() const override;

  /**
   * Set value of variable function
   * @param[in] val value to set in variable
   */
  void set_val(int val)
  {
    location_->second = val;
  }
};

/**
 * @brief Constant node class
 */
class CNode final : public INode
{
private:
  const int val_;

public:
  /**
   * Constant node ctor
   * @param[in] val  value of a node
   */
  CNode(int val) : val_(val)
  {
  }

  /**
   * Calculate the value of node
   * @return value of a node
   */
  int calc() const override;
};

/**
 * @brief Operator node class
 */
class OPNode final : public INode
{
private:
  pINode left_{};
  pINode right_{};

  /* To determine operator's type */
  Ops op_type_;

public:
  /**
   * Operator's node constructor
   * @param[in] left    left node of operator
   * @param[in] right   right node of operator
   */
  OPNode(pINode &left, Ops op_type, pINode &right)
      : INode(2), left_(std::move(left)), right_(std::move(right)), op_type_(op_type)
  {
  }

  INode *get_i_child(size_t i) const override
  {
    if (i >= childs_am_)
      throw std::runtime_error{"Incorrect children amount"};

    return i == 1 ? right_.get() : left_.get();
  }

  int calc() const override;
};

/**
 * @class UNOPNode
 * @brief Operator node class
 */
class UNOPNode final : public INode
{
private:
  pINode operand_{};

  /* To determine operator's type */
  Ops op_type_;

public:
  /**
   * Operator's node constructor
   * @param[in] operand  pointer to operand's node
   */
  UNOPNode(Ops op_type, pINode &operand) : INode(1), operand_(std::move(operand)), op_type_(op_type)
  {
  }

  INode *get_i_child(size_t i) const override
  {
    if (i >= childs_am_)
      throw std::runtime_error{"Incorrect children amount"};

    return operand_.get();
  }

  int calc() const override;
};

/**
 * @brief Assignment operator's node class
 */
class ASNode final : public INode
{
private:
  std::unique_ptr<VNode> dst_; // variable to assign
  pINode expr_;                // expression
public:
  /**
   * @brief Assignment node class ctor
   * @param[in] dst pointer to destination variable node
   * @param[in] expr pointer to expression node(-s)
   */
  ASNode(std::unique_ptr<VNode> &dst, pINode &expr) : INode(2), dst_(std::move(dst)), expr_(std::move(expr))
  {
  }

  INode *get_i_child(size_t i) const override
  {
    if (i >= childs_am_)
      throw std::runtime_error{"Incorrect children amount"};

    return i == 1 ? expr_.get() : dst_.get();
  }

  /**
   * @brief Calculate an assignment function
   * @return calculated assigned value
   */
  int calc() const override;
};

/**
 * @brief return operand node class
 *
 */
class RETNode final : public INode
{
private:
  pINode expr_;

public:
  /**
   * @brief Construct a new RETNode object - return node operand
   *
   * @param[in] expr shared pointer to expression node
   */
  RETNode(pINode &expr) : INode(1), expr_(std::move(expr))
  {
  }

  INode *get_i_child(size_t i) const override
  {
    if (i >= childs_am_)
      throw std::runtime_error{"Incorrect children amount"};

    return expr_.get();
  }

  /**
   * @brief calculate ret value function
   *
   * @return int return value
   */
  int calc() const override
  {
    return expr_->calc();
  }
};

/**
 * @brief While node class
 */
class WHNode final : public INode
{
private:
  pINode cond_{};
  pIScope scope_{};

public:
  WHNode(pINode &cond, pIScope &scope)
      : INode(std::numeric_limits<size_t>::max()), cond_(std::move(cond)), scope_(std::move(scope))
  // because while potentially has infinity number of children (iterations)
  {
  }

  INode *get_i_child(size_t i) const override;

  /**
   * @brief Calculate while node function
   * @return int
   */
  int calc() const override
  {
    return 0;
  }
};

/**
 * @brief If node class
 */
class IFNode final : public INode
{
private:
  pINode cond_{};
  pIScope if_scope_{}; // scope if condition is correct

  /* scope if condition is incorrect (optionally) */
  pIScope else_scope_{};

public:
  IFNode(pINode &cond, pIScope &if_sc, pIScope &el_sc)
      : INode(2), cond_(std::move(cond)), if_scope_(std::move(if_sc)), else_scope_(std::move(el_sc))
  {
  }

  IFNode(pINode &cond, pIScope &if_sc)
      : INode(2), cond_(std::move(cond)), if_scope_(std::move(if_sc)), else_scope_(nullptr)
  {
  }

  INode *get_i_child(size_t i) const override;

  /**
   * Interpret If node function
   * @return calculated result
   */
  int calc() const override
  {
    return 0;
  }
};

/**
 * Print node class function
 */
class PNode final : public INode
{
private:
  pINode expr_;

public:
  PNode(pINode &expr) : INode(1), expr_(std::move(expr))
  {
  }

  INode *get_i_child(size_t i) const override
  {
    if (i >= childs_am_)
      throw std::runtime_error{"Incorrect children amount"};

    return expr_.get();
  }

  /**
   * Interpret print node function
   */
  int calc() const override;
};

/**
 * Scanf node class
 */
class RNode final : public INode
{
public:
  RNode() = default;

  /**
   * @brief Interpret read node function
   * @return read value
   */
  int calc() const override;

  ~RNode() = default;
};
} // namespace AST

#endif /* NODE_HH */