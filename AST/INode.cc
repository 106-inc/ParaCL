#include "INode.hh"
#include "Node.hh"

/**
 * @file INode.cc
 * @brief Realization of functions from 'INode.hh' file
 */
namespace AST
{

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

} // namespace AST
