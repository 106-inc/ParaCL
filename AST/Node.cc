#include "Node.hh"

namespace AST
{

std::stack<int> ValStack{};

// TODO: rewrite comments

/**
 * @fn make_cst
 * @brief Make constant value node function
 * @param[in] val value to put to node
 * @return shared pointer to created node
 */
pINode make_cst(int val)
{
  return std::make_unique<CNode>(val);
} /* End of 'make_cst' function */

/**
 * @fn make_op
 * @brief Create operator node function
 * @param[in] l  left node of operator
 * @param[in] op operator type
 * @param[in] r  right node of operator
 * @return shared pointer to created Node
 */
pINode make_op(pINode &l, Ops op, pINode &r)
{
  return std::make_unique<OPNode>(l, op, r);
} /* End of 'make_op' function */

/**
 * @brief Make unary operator node function
 *
 * @param[in] op  enum type of operator
 * @param[in] operand shared pointer to operand node
 * @return shared pointer to created node
 */
pINode make_un(Ops op, pINode &operand)
{
  return std::make_unique<UNOPNode>(op, operand);
}

/**
 * @fn make_while
 * @brief Make while node fucntion
 * @param[in] cond shared pointer to condition node
 * @param[in] sc shared pointer to scope
 * @return shared pointer to created Node
 */
pINode make_while(pINode &cond, pIScope &sc)
{
  return std::make_unique<WHNode>(cond, sc);
} /* End of 'make_while' function */

/**
 * @fn make_if_else
 * @brief Create if_else node fucntion
 * @param[in] cond condition
 * @param[in] isc if scope
 * @param[in] esc else scope
 * @return pointer to created Node
 */
pINode make_if_else(pINode &cond, pIScope &isc, pIScope &esc)
{
  return std::make_unique<IFNode>(cond, isc, esc);
} /* End of 'make_if_else' function */

/**
 * @fn make_if
 * @brief Create if node fucntion
 * @param[in] cond condition
 * @param[in] isc if scope
 * @param[in] esc else scope
 * @return pointer to created Node
 */
pINode make_if(pINode &cond, pIScope &isc)
{
  return std::make_unique<IFNode>(cond, isc);
} /* End of 'make_if_else' function */

/**
 * @fn make_scope
 * @brief Create scope function
 * @param[in] par shared pointer to parent node
 * @return shared pointer to created Scope
 */
pIScope make_scope(IScope *par /* = nullptr */)
{
  return std::make_unique<Scope>(par);
} /* End of 'create_scope' function */

/**
 * @fn make ref
 * @brief Make var node for expression
 * @param[in] var_name name of a variable
 * @return shared pointer to created node
 */
pINode make_ref(const std::string &var_name)
{
  auto it_bl = CUR_SCOPE->get_var(var_name);

  if (!it_bl.second)
  {
    std::string what = "Unknown variable '" + var_name + "'";
    throw std::runtime_error{what};
  }
  /* std::hdd::format(); */

  return std::make_unique<VNode>(it_bl.first);
} /* End of 'make_ref' function */

/*!
 * @fn make_print
 * @brief Make print node function
 * @param[in] expr shared pointer to expression node
 * @return shared pointer to created node
 */
pINode make_print(pINode &expr)
{
  return std::make_unique<PNode>(expr);
}

/**
 * @brief make scan node function
 * @return shared pointer to created node
 */
pINode make_scan()
{
  return std::make_unique<RNode>();
}

/**
 * @fn make_asgn
 * @brief Make assignment node function
 * @param[in] var_name name of a variable to assign to
 * @param[in] expr expression to assign
 * @return shared pointer to created node
 */
pINode make_asgn(const std::string &var_name, pINode &expr)
{
  auto it = CUR_SCOPE->check_n_insert(var_name);
  auto pvar = std::make_unique<VNode>(it);

  return std::make_unique<ASNode>(pvar, expr);
} /* End of 'make_ass' function */

////////////////////////// SCOPE METHODS /////////////////

/**
 * @brief Check var in all available scopes function
 * @param[in] var_name name of a var to get access to
 * @return pair of iterator to var table and bool, which:
 * TRUE - iterator is valid, variable found,
 * FALSE - iterator is not valid (end()), variable was not found
 */
std::pair<var_table::iterator, bool> Scope::get_var(const std::string &var_name)
{
  // find var in parent's scopes
  std::pair<var_table::iterator, bool> it_n_bool{loc_check(var_name)};

  if (it_n_bool.second)
    return it_n_bool;

  auto pscope = reset_scope();

  while (pscope != nullptr)
  {
    it_n_bool = pscope->loc_check(var_name);

    if (it_n_bool.second)
      break;
    else /* variable wasn't found */
      pscope = pscope->reset_scope();
  }

  return it_n_bool;
} /* End of 'access' function */

/**
 * @brief Check variable in current scope function.
 * @param[in] var_name name of a var to find
 * @return pair of iterator to var table and bool, which:
 * TRUE - iterator is valid, variable found,
 * FALSE - iterator is not valid (end()), variable was not found
 */
Scope::it_bool Scope::loc_check(const std::string &var_name)
{
  it_bool itbl{};

  itbl.first = var_tbl_.find(var_name);
  itbl.second = (var_tbl_.end() != itbl.first);

  return itbl;
} /* End of 'loc_check' function */

/**
 * @brief Chek and insert (if neccesary) variable function
 * @param[in] var_name name of a variable to possible insertion
 * @return iterator to variable in var table (in both cases)
 */
var_table::iterator Scope::check_n_insert(const std::string &var_name)
{
  it_bool it_n_bool = get_var(var_name);

  if (!it_n_bool.second)
    it_n_bool.first = insert_var(var_name);

  return it_n_bool.first;
} /* End of 'check_n_insert' function */

int OPNode::calc() const
{
  int right_val = ValStack.top();
  ValStack.pop();
  int left_val = ValStack.top();
  ValStack.pop();

  int res = 0;

  switch (op_type_)
  {
  case Ops::ADD:
    res = left_val + right_val;
    break;
  case Ops::SUB:
    res = left_val - right_val;
    break;
  case Ops::MUL:
    res = left_val * right_val;
    break;
  case Ops::DIV:
    if (right_val == 0)
      throw std::runtime_error{"Dividing by ZERO!"};
    res = left_val / right_val;
    break;
  case Ops::MOD:
    if (right_val == 0)
      throw std::runtime_error{"Dividing by ZERO!"};
    res = left_val % right_val;
    break;
  case Ops::GREATER:
    res = left_val > right_val;
    break;
  case Ops::GR_EQ:
    res = left_val >= right_val;
    break;
  case Ops::LESS:
    res = left_val < right_val;
    break;
  case Ops::LS_EQ:
    res = left_val <= right_val;
    break;
  case Ops::IS_EQ:
    res = left_val == right_val;
    break;
  case Ops::NOT_EQ:
    res = left_val != right_val;
    break;
  case Ops::AND:
    res = left_val && right_val;
    break;
  case Ops::OR:
    res = left_val || right_val;
    break;
  default:
    throw std::runtime_error("Unrecognized binary operator number\n");
  }

  ValStack.push(res);

  return res;
}

int UNOPNode::calc() const
{
  int val = ValStack.top();
  ValStack.pop();

  int res = 0;

  switch (op_type_)
  {
  case Ops::NEG:
    res = -val;
    break;
  case Ops::NOT:
    res = !val;
    break;
  default:
    throw std::runtime_error("Unrecognized unary operator number\n");
  }

  ValStack.push(res);

  return res;
}

int VNode::calc() const
{
  ValStack.push(location_->second);
  return location_->second;
}

int CNode::calc() const
{
  ValStack.push(val_);
  return val_;
}

int ASNode::calc() const
{
  int expr_res = ValStack.top();
  ValStack.pop();

  ValStack.pop(); // delete dummy var value

  dst_->set_val(expr_res);

  ValStack.push(expr_res);
  return expr_res;
} /* End of 'calc' function */

INode *WHNode::get_i_child(size_t i) const
{
  i %= 2;

  if (i == 0)
    return cond_.get();

  int cond_calc = ValStack.top();
  ValStack.pop();

  if (cond_calc)
    return scope_.get();

  return nullptr;
}

INode *IFNode::get_i_child(size_t i) const
{
  if (i >= childs_am_)
    throw std::runtime_error{"Incorrect children amount"};

  if (i == 0)
    return cond_.get();

  int calc_cond = ValStack.top();
  ValStack.pop();

  if (calc_cond)
    return if_scope_.get();
  else if (else_scope_ != nullptr)
    return else_scope_.get();

  return nullptr;
}

int PNode::calc() const
{
  int val = ValStack.top();
  ValStack.pop();

  std::cout << val << std::endl;
  return 0;
}

int RNode::calc() const
{
  int value{};

  std::cin >> value;
  if (!std::cin.good())
    throw std::runtime_error{"Invalid symbol at stdin"};

  ValStack.push(value);

  return value;
} /* End of 'calc' function */

//////////////END OF SCOPE METHODS ////////////////////////////////
} // namespace AST
