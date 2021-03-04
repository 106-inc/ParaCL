#include "INode.hh"
#include "OPNode.hh"

/**
 * @file INode.cc
 * @brief Realization of functions from 'INode.hh' file
 */
namespace AST
{

/**
 * @fn make_cst
 * @brief Make constant value node function
 * @param val [in] value to put to node
 * @return pointer to created node
 */
INode *MemMan::make_cst(int val)
{
  pnodes_.push_front(new CNode{val});
  return pnodes_.front();
} /* End of 'make_cst' function */

/**
 * @fn make_op
 * @brief Create operator node function
 * @param l  [in] left node of operator
 * @param op [in] operator type
 * @param r  [in] right node of operator
 * @return pointer to created Node
 */
INode *MemMan::make_op(INode *l, Ops op, INode *r)
{
  switch (op)
  {
  case Ops::ADD:
    pnodes_.push_front(new PLNode{l, r});
    return pnodes_.front();
  case Ops::SUB:
    pnodes_.push_front(new SBNode{l, r});
    return pnodes_.front();
  case Ops::MUL:
    pnodes_.push_front(new MLNode{l, r});
    return pnodes_.front();
  case Ops::DIV:
    pnodes_.push_front(new DVNode{l, r});
    return pnodes_.front();
  case Ops::MOD:
    pnodes_.push_front(new MDNode{l, r});
    return pnodes_.front();
  case Ops::GREATER:
    pnodes_.push_front(new GNode{l, r});
    return pnodes_.front();
  case Ops::GR_EQ:
    pnodes_.push_front(new GENode{l, r});
    return pnodes_.front();
  case Ops::LESS:
    pnodes_.push_front(new LNode{l, r});
    return pnodes_.front();
  case Ops::LS_EQ:
    pnodes_.push_front(new LENode{l, r});
    return pnodes_.front();
  case Ops::IS_EQ:
    pnodes_.push_front(new EQNode{l, r});
    return pnodes_.front();
  case Ops::NOT_EQ:
    pnodes_.push_front(new NEQNode{l, r});
    return pnodes_.front();
  case Ops::AND:
    pnodes_.push_front(new ANDNode{l, r});
    return pnodes_.front();
  case Ops::OR:
    pnodes_.push_front(new ORNode{l, r});
    return pnodes_.front();
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
INode *MemMan::make_un(Ops op, INode *operand)
{
  switch (op)
  {
  case Ops::NEG:
    pnodes_.push_front(new NEGNode{operand});
    return pnodes_.front();
  case Ops::NOT:
    pnodes_.push_front(new NOTNode{operand});
    return pnodes_.front();
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
INode *MemMan::make_while(INode *cond, IScope *sc)
{
  pnodes_.push_front(new WHNode{cond, sc});
  return pnodes_.front();
} /* End of 'make_while' function */

/**
 * @fn make_if
 * @brief Create if node fucntion
 * @param cond ptr to condition
 * @param isc ptr to if scope
 * @param esc ptr to else scope (nullptr default)
 * @return pointer to created Node
 */
INode *MemMan::make_if(INode *cond, IScope *isc, IScope *esc /* = nullptr */)
{
  pnodes_.push_front(new IFNode{cond, isc, esc});
  return pnodes_.front();
} /* End of 'make_if' function */

/**
 * @fn make_scope
 * @brief Create scope function
 * @param par [in] - pointer to parent node
 * @return pointer to created Scope
 */
IScope *MemMan::make_scope(IScope *par /* = nullptr */)
{
  IScope *ptmp = new Scope{par};
  pnodes_.push_front(ptmp);
  return ptmp;
} /* End of 'create_scope' function */

/**
 * @fn make ref
 * @brief Make var node for expression
 * @param var_name name of a variable
 * @return pointer to created node
 */
INode *MemMan::make_ref(const std::string &var_name)
{
  auto it_bl = CUR_SCOPE->get_var(var_name);

  if (!it_bl.second)
  {
    std::string what = "Unknown variable '" + var_name + "'";
    throw std::runtime_error{what};
  }
  /* std::hdd::format(); */

  pnodes_.push_front(new VNode{it_bl.first});
  return pnodes_.front();
} /* End of 'make_ref' function */

/*!
 * @fn make_print
 * @brief Make print node function
 * @param[in] expr pointer to expression node
 * @return pointer to created node
 */
INode *MemMan::make_print(INode *expr)
{
  pnodes_.push_front(new PNode{expr});
  return pnodes_.front();
}

/**
 * @brief make scan node function
 * @return pointer to created node
 */
INode *MemMan::make_scan()
{
  pnodes_.push_front(new RNode);
  return pnodes_.front();
}

/**
 * @fn make_asgn
 * @brief Make assignment node function
 * @param var_name
 * @param expr
 * @return pointer to created node
 */
INode *MemMan::make_asgn(const std::string &var_name, INode *expr)
{
  auto it = CUR_SCOPE->check_n_insert(var_name);

  auto pvar = new VNode{it};
  pnodes_.push_front(pvar);

  pnodes_.push_front(new ASNode{pvar, expr});
  return pnodes_.front();
} /* End of 'make_ass' function */

} // namespace AST
