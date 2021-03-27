#include "INode.hh"
#include "OPNode.hh"

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
 * @param val [in] value to put to node
 * @return unique pointer to created node
 */
pINode make_cst(int val)
{
  return std::make_shared<CNode>(val);
} /* End of 'make_cst' function */

/**
 * @fn make_op
 * @brief Create operator node function
 * @param l  [in] left node of operator
 * @param op [in] operator type
 * @param r  [in] right node of operator
 * @return pointer to created Node
 */
pINode make_op(pINode l, Ops op, pINode r)
{
  switch (op)
  {
  case Ops::ADD:
    return std::make_shared<PLNode>(l, r);
  case Ops::SUB:
    return std::make_shared<SBNode>(l, r);
  case Ops::MUL:
    return std::make_shared<MLNode>(l, r);
  case Ops::DIV:
    return std::make_shared<DVNode>(l, r);
  case Ops::MOD:
    return std::make_shared<MDNode>(l, r);
  case Ops::GREATER:
    return std::make_shared<GNode>(l, r);
  case Ops::GR_EQ:
    return std::make_shared<GENode>(l, r);
  case Ops::LESS:
    return std::make_shared<LNode>(l, r);
  case Ops::LS_EQ:
    return std::make_shared<LENode>(l, r);
  case Ops::IS_EQ:
    return std::make_shared<EQNode>(l, r);
  case Ops::NOT_EQ:
    return std::make_shared<NEQNode>(l, r);
  case Ops::AND:
    return std::make_shared<ANDNode>(l, r);
  case Ops::OR:
    return std::make_shared<ORNode>(l, r);
  default:
    throw std::runtime_error("Operator is not implemented\n");
  }
} /* End of 'make_op' function */

/**
 * @brief Make unary operator node function
 *
 * @param[in] op  enum type of operator
 * @param[in] operand pointer to operand node
 * @return INode*
 */
pINode make_un(Ops op, pINode operand)
{
  switch (op)
  {
  case Ops::NEG:
    return std::make_shared<NEGNode>(operand);
  case Ops::NOT:
    return std::make_shared<NOTNode>(operand);
  default:
    throw std::runtime_error("Operator is not implemented\n");
  }
}

/**
 * @fn make_while
 * @brief Make while node fucntion
 * @param cond pointer to condition node
 * @param sc - pointer to scope
 * @return pointer to created Node
 */
pINode make_while(pINode cond, pIScope sc)
{
  return std::make_shared<WHNode>(cond, sc);
} /* End of 'make_while' function */

/**
 * @fn make_if
 * @brief Create if node fucntion
 * @param cond ptr to condition
 * @param isc ptr to if scope
 * @param esc ptr to else scope (nullptr default)
 * @return pointer to created Node
 */
pINode make_if(pINode cond, pIScope isc, pIScope esc /* = nullptr */)
{
  return std::make_shared<IFNode>(cond, isc, esc);
} /* End of 'make_if' function */

/**
 * @fn make_scope
 * @brief Create scope function
 * @param par [in] - pointer to parent node
 * @return pointer to created Scope
 */
pIScope make_scope(pIScope par /* = nullptr */)
{
  return std::make_shared<Scope>(par);
} /* End of 'create_scope' function */

/**
 * @fn make ref
 * @brief Make var node for expression
 * @param var_name name of a variable
 * @return pointer to created node
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
 * @param[in] expr pointer to expression node
 * @return pointer to created node
 */
pINode make_print(pINode expr)
{
  return std::make_shared<PNode>(expr);
}

/**
 * @brief make scan node function
 * @return pointer to created node
 */
pINode make_scan()
{
  return std::make_shared<RNode>();
}

/**
 * @fn make_asgn
 * @brief Make assignment node function
 * @param var_name
 * @param expr
 * @return pointer to created node
 */
pINode make_asgn(const std::string &var_name, pINode expr)
{
  auto it = CUR_SCOPE->check_n_insert(var_name);

  auto pvar = std::make_shared<VNode>(it);
  return std::make_shared<ASNode>(pvar, expr);
} /* End of 'make_ass' function */

} // namespace AST
