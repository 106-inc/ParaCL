#include "INode.hh"
#include "Node.hh"

/**
 * @file INode.cc
 * @brief Realization of functions from 'INode.hh' file
 */

namespace AST
{

  std::stack<pIScope> ScopeStack{};
// TODO: rewrite comments

/**
 * @fn make_cst
 * @brief Make constant value node function
 * @param[in] val value to put to node
 * @return shared pointer to created node
 */
pINode make_cst(int val)
{
  return std::make_shared<CNode>(val);
} /* End of 'make_cst' function */

/**
 * @fn make_op
 * @brief Create operator node function
 * @param[in] l  left node of operator
 * @param[in] op operator type
 * @param[in] r  right node of operator
 * @return shared pointer to created Node
 */
pINode make_op(const pINode &l, Ops op, const pINode &r)
{
  return std::make_shared<OPNode>(l, op, r);
} /* End of 'make_op' function */

/**
 * @brief Make unary operator node function
 *
 * @param[in] op  enum type of operator
 * @param[in] operand shared pointer to operand node
 * @return shared pointer to created node
 */
pINode make_un(Ops op, const pINode &operand)
{
  return std::make_shared<UNOPNode>(op, operand);
}

/**
 * @fn make_while
 * @brief Make while node fucntion
 * @param[in] cond shared pointer to condition node
 * @param[in] sc shared pointer to scope
 * @return shared pointer to created Node
 */
pINode make_while(const pINode &cond, const pIScope &sc)
{
  ScopeStack.pop();
  return std::make_shared<WHNode>(cond, sc);
} /* End of 'make_while' function */

/**
 * @fn make_if
 * @brief Create if node fucntion
 * @param[in] cond shared ptr to condition
 * @param[in] isc shared ptr to if scope
 * @param[in] esc shared  ptr to else scope (nullptr default)
 * @return shared  pointer to created Node
 */
pINode make_if(const pINode &cond, const pIScope &isc, const pIScope &esc /* = nullptr */)
{
  ScopeStack.pop();
  if (esc)
    ScopeStack.pop();

  return std::make_shared<IFNode>(cond, isc, esc);
} /* End of 'make_if' function */

/**
 * @fn make_scope
 * @brief Create scope function
 * @param[in] par shared pointer to parent node
 * @return shared pointer to created Scope
 */
pIScope make_scope(const pIScope &par /* = nullptr */ )
{
  auto new_psc = std::make_shared<Scope>(par);

  if (par)
    par->push(new_psc);

  return new_psc;
} /* End of 'create_scope' function */

/**
 * @fn make_br_scope
 * @brief Create scope function
 * @param[in] par shared pointer to parent node
 * @return shared pointer to created Scope
 */
pIScope make_br_scope(const pIScope &par )
{
  auto new_psc = std::make_shared<Scope>(par);

  if (par)
    ScopeStack.push(new_psc);

  return new_psc;
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

  return std::make_shared<VNode>(it_bl.first);
} /* End of 'make_ref' function */

/*!
 * @fn make_print
 * @brief Make print node function
 * @param[in] expr shared pointer to expression node
 * @return shared pointer to created node
 */
pINode make_print(const pINode &expr)
{
  return std::make_shared<PNode>(expr);
}

/**
 * @brief make scan node function
 * @return shared pointer to created node
 */
pINode make_scan()
{
  return std::make_shared<RNode>();
}

/**
 * @fn make_asgn
 * @brief Make assignment node function
 * @param[in] var_name name of a variable to assign to
 * @param[in] expr expression to assign
 * @return shared pointer to created node
 */
pINode make_asgn(const std::string &var_name, const pINode &expr)
{
  auto it = CUR_SCOPE->check_n_insert(var_name);

  auto pvar = std::make_shared<VNode>(it);
  return std::make_shared<ASNode>(pvar, expr);
} /* End of 'make_ass' function */

} // namespace AST
