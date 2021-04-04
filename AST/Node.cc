#include "Node.hh"

namespace AST
{
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

//////////////END OF SCOPE METHODS ////////////////////////////////
} // namespace AST
