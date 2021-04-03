#ifndef NODE_HH
#define NODE_HH

/////////////////////////////////////
///// STL containers ///////////////
#include <stack>
#include <vector>
/////////////////////////////////////

////// OUR HEADERS //////////////////
#include "INode.hh"
#include "Interp.hh"
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
  Scope(const pIScope &parent = nullptr) : parent_(parent)
  {
  }

  pIScope reset_scope() const override
  {
    return parent_.lock();
  }

  /**
   * @brief Interpret the scope function (claculate)
   * @return int
   */
  int calc() const override
  {
    int ret_val{};

    for (auto &&node : nodes_)
      ret_val = node->calc();

    return ret_val;
  } /* End of 'calc' function */

  pINode get_i_child(size_t i) const override
  {
    if (i >= nodes_.size())
      return nullptr;

    return nodes_[i];
  }

  /**
   * @brief Add node to scope function
   * @param[in] node node to add
   * @return none
   */
  void push(const pINode &node) override
  {
    nodes_.push_back(node);
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
   * @return current value of a variable
   */
  int calc() const override
  {
    return location_->second;
  }

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
  int calc() const override
  {
    return val_;
  }
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
  OPNode(const pINode &left, Ops op_type, const pINode &right) : INode(2), left_(left), right_(right), op_type_(op_type)
  {
  }

  pINode get_i_child(size_t i) const override
  {
    if (i >= childs_am_)
      return nullptr;

    return i == 1 ? right_ : left_;
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
  UNOPNode(Ops op_type, const pINode &operand) : INode(1), operand_(operand), op_type_(op_type)
  {
  }

  pINode get_i_child(size_t i) const override
  {
    return i >= childs_am_ ? nullptr : operand_;
  }

  int calc() const override;
};

/**
 * @brief Assignment operator's node class
 */
class ASNode final : public INode
{
private:
  std::shared_ptr<VNode> dst_; // variable to assign
  pINode expr_;                // expression
public:
  /**
   * @brief Assignment node class ctor
   * @param[in] dst pointer to destination variable node
   * @param[in] expr pointer to expression node(-s)
   */
  ASNode(const std::shared_ptr<VNode> &dst, const pINode &expr) : INode(2), dst_(dst), expr_(expr)
  {
  }

  pINode get_i_child(size_t i) const override
  {
    if (i >= childs_am_)
      return nullptr;

    return i == 1 ? expr_ : dst_;
  }

  /**
   * @brief Calculate an assignment function
   * @return calculated assigned value
   */
  int calc() const override
  {
    int expr_res = expr_->calc();

    dst_->set_val(expr_res);

    return expr_res;
  } /* End of 'calc' function */
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
  RETNode(const pINode &expr) : INode(1), expr_(expr)
  {
  }

  pINode get_i_child(size_t i) const override
  {
    return i >= childs_am_ ? nullptr : expr_;
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
  WHNode(const pINode &cond, const pIScope &scope) : INode(2), cond_(cond), scope_(scope)
  {
  }

  pINode get_i_child(size_t i) const override
  {
    if (i >= childs_am_)
      return nullptr;

    return i == 1 ? scope_ : cond_;
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
  IFNode(const pINode &cond, const pIScope &if_sc, const pIScope &el_sc = nullptr)
      : INode(el_sc == nullptr ? 2 : 3), cond_(cond), if_scope_(if_sc), else_scope_(el_sc)
  {
  }

  pINode get_i_child(size_t i) const override
  {
    if (i >= childs_am_)
      return nullptr;

    switch (i)
    {
    case 0:
      return cond_;
    case 1:
      return if_scope_;
    default:
      return else_scope_;
    }
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
  PNode(const pINode &expr) : INode(1), expr_(expr)
  {
  }

  pINode get_i_child(size_t i) const override
  {
    return i >= childs_am_ ? nullptr : expr_;
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