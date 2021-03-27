#ifndef NODE_HH
#define NODE_HH

/////////////////////////////////////
///// STL containers ///////////////
#include <vector>
/////////////////////////////////////

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
  std::weak_ptr<IScope> parent_{};

  var_table var_tbl_;

public:
  // constructor by parent scope ptr
  Scope(pIScope parent = nullptr) : parent_(parent)
  {
  }

  pIScope reset_scope() const override
  {
    return parent_;
  }

  /**
   * @brief Interpret the scope function (claculate)
   * @return int
   */
  int calc() const override
  {
    for (auto &&node : nodes_)
      node->calc();

    return 0;
  } /* End of 'calc' function */

  /**
   * @brief Add node to scope function
   * @param node [in] node to add
   * @return none
   */
  void push(pINode node) override
  {
    nodes_.push_back(node);
  } /* End of 'push' function */

  it_bool get_var(const std::string &var_name) override;

  it_bool loc_check(const std::string &var_name) override;

  var_table::iterator check_n_insert(const std::string &var_name) override;

private:
  /**
   * @brief Insert variable to table of current scope
   * @warning NO VALIDATION OF UNIQUE!!!
   * @param var_name
   * @return iterator to inserted variable
   */
  var_table::iterator insert_var(const std::string &var_name)
  {
    auto it_bl = var_tbl_.insert({var_name, {}});

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
   * @return
   */
  int calc() const override
  {
    return location_->second;
  }

  /**
   * Set value of variable function
   * @param[in] val
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
  int calc() const override
  {
    return val_;
  }
};

/**
 * @brief Operator node class
 */
class OPNode : public INode
{
protected:
  pINode left_{};
  pINode right_{};

public:
  /**
   * Operator's node constructor
   * @param[in] left    left node of operator
   * @param[in] right   right node of operator
   */
  OPNode(pINode left, pINode right) : left_(left), right_(right)
  {
  }
};

/**
 * @class UNOPNode
 * @brief Operator node class
 */
class UNOPNode : public INode
{
protected:
  pINode operand_{};

public:
  /**
   * Operator's node constructor
   * @param[in] operand  pointer to operand's node
   */
  UNOPNode(pINode operand) : operand_(operand)
  {
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
  WHNode(pINode cond, pIScope scope) : cond_(cond), scope_(scope)
  {
  }

  /**
   * @brief Calculate while node function
   * @return int
   */
  int calc() const override
  {
    while (cond_->calc())
      scope_->calc();

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
  IFNode(pINode cond, pIScope if_sc, pIScope el_sc = nullptr) : cond_(cond), if_scope_(if_sc), else_scope_(el_sc)
  {
  }

  /**
   * Interpret If node function
   * @return calculated result
   */
  int calc() const override
  {
    if (cond_->calc())
      if_scope_->calc();
    else if (else_scope_ != nullptr)
      else_scope_->calc();

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
  PNode(pINode expr) : expr_(expr)
  {
  }

  /**
   * Interpret print node function
   */
  int calc() const override
  {
    std::cout << expr_->calc() << std::endl;
    return 0;
  }
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
  int calc() const override
  {
    int value{};

    std::cin >> value;
    if (!std::cin.good())
      throw std::runtime_error{"Invalid symbol at stdin"};

    return value;
  } /* End of 'calc' function */

  ~RNode() = default;
};
} // namespace AST

#endif /* NODE_HH */