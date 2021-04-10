#include "Node.hh"
#include "Interp.hh"

namespace AST
{

std::stack<int> ValStack{};

pINode make_cst(int val)
{
  return std::make_unique<CNode>(val);
}

pINode make_op(pINode &l, Ops op, pINode &r)
{
  return std::make_unique<OPNode>(l, op, r);
}

pINode make_un(Ops op, pINode &operand)
{
  return std::make_unique<UNOPNode>(op, operand);
}

pINode make_while(pINode &cond, pIScope &sc)
{
  return std::make_unique<WHNode>(cond, sc);
}

pINode make_if_else(pINode &cond, pIScope &isc, pIScope &esc)
{
  return std::make_unique<IFNode>(cond, isc, esc);
}

pINode make_if(pINode &cond, pIScope &isc)
{
  return std::make_unique<IFNode>(cond, isc);
}

pIScope make_scope(IScope *par /* = nullptr */)
{
  return std::make_unique<Scope>(par);
}

pINode make_ref(const std::string &var_name)
{
  auto it_bl = CUR_SCOPE->get_var(var_name);

  if (!it_bl.second)
  {
    std::string what = "Unknown variable '" + var_name + "'";
    throw std::runtime_error{what};
  }

  return std::make_unique<VNode>(it_bl.first);
}

pINode make_print(pINode &expr)
{
  return std::make_unique<PNode>(expr);
}

pINode make_scan()
{
  return std::make_unique<RNode>();
}

pINode make_asgn(const std::string &var_name, pINode &expr)
{
  auto it = CUR_SCOPE->check_n_insert(var_name);
  auto pvar = std::make_unique<VNode>(it);

  return std::make_unique<ASNode>(pvar, expr);
}

/*pINode make_asgn_func(const std::string &var_name, pINode &func)
{
  auto it = CUR_SCOPE->check_n_insert(var_name);

  auto pvar = std::make_unique<VNode>(it);

  return std::make_unique<ASNode>(pvar, func);
}*/

////////////////////////// SCOPE METHODS /////////////////

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
}

Scope::it_bool Scope::loc_check(const std::string &var_name)
{
  it_bool itbl{};

  itbl.first = var_tbl_.find(var_name);
  itbl.second = (var_tbl_.end() != itbl.first);

  return itbl;
}

var_table::iterator Scope::check_n_insert(const std::string &var_name)
{
  it_bool it_n_bool = get_var(var_name);

  if (!it_n_bool.second)
    it_n_bool.first = insert_var(var_name);

  return it_n_bool.first;
}

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
  ValStack.push(location_->second.value);
  return location_->second.value;
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
}

//////////////END OF SCOPE METHODS ////////////////////////////////

void clear(pIScope &root)
{
}

} // namespace AST
