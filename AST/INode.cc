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
INode *make_cst(int val)
{
  IMMA_DOIN("CST");
  return new CNode{val};
} /* End of 'make_cst' function */

/**
 * @fn make_op
 * @brief Create operator node function
 * @param l  [in] left node of operator
 * @param op [in] operator type
 * @param r  [in] right node of operator
 * @return pointer to created Node
 */
INode *make_op(INode *l, Ops op, INode *r)
{
  IMMA_DOIN("OP");
  switch (op)
  {
  case Ops::ADD:
    IMMA_DOIN("+");
    return new PLNode{l, r};
  case Ops::SUB:
    IMMA_DOIN("-");
    return new SBNode{l, r};
  case Ops::MUL:
    IMMA_DOIN("*");
    return new MLNode{l, r};
  case Ops::DIV:
    IMMA_DOIN("/");
    return new DVNode{l, r};
  case Ops::GREATER:
    return new GNode{l, r};
  case Ops::GR_EQ:
    return new GENode{l, r};
  case Ops::LESS:
    return new LNode{l, r};
  case Ops::LS_EQ:
    return new LENode{l, r};
  case Ops::IS_EQ:
    return new EQNode{l, r};
  case Ops::NOT_EQ:
    return new NEQNode{l, r};
  case Ops::AND:
    return new ANDNode{l, r};
  case Ops::OR:
    return new ORNode{l, r};
  default:
    throw std::runtime_error("Operator is not implemented\n");
  }
} /* End of 'make_op' function */

INode *make_un(Ops op, INode *operand)
{
  switch (op)
  {
  case Ops::NEG:
    return new NEGNode{operand};
  case Ops::NOT:
    return new NOTNode{operand};
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
INode *make_while(INode *cond, IScope *sc)
{
  IMMA_DOIN("WHILE");
  return new WHNode{cond, sc};
} /* End of 'make_while' function */

/**
 * @fn make_if
 * @brief Create if node fucntion
 * @param cond ptr to condition
 * @param isc ptr to if scope
 * @param esc ptr to else scope (nullptr default)
 * @return pointer to created Node
 */
INode *make_if(INode *cond, IScope *isc, IScope *esc /* = nullptr */)
{
  IMMA_DOIN("IF");
  return new IFNode{cond, isc, esc};
} /* End of 'make_if' function */

/**
 * @fn make_scope
 * @brief Create scope function
 * @param par [in] - pointer to parent node
 * @return pointer to created Scope
 */
IScope *make_scope(IScope *par /* = nullptr */)
{
  IMMA_DOIN("SCOPE");
  return new Scope{par};

} /* End of 'create_scope' function */

/**
 * @fn make ref
 * @brief Make var node for expression
 * @param var_name name of a variable
 * @warning DEBUG version - std::terminate!!!!!!!!!
 * @return pointer to created node
 */
INode *make_ref(const std::string &var_name)
{
  IMMA_DOIN("VAR USAGE");
  auto it_bl = CUR_SCOPE->get_var(var_name);

  // TODO: delete termination
  if (!it_bl.second)
    std::terminate();
  /* std::hdd::format(); */

  return new VNode{it_bl.first};
} /* End of 'make_ref' function */

/*!
 * @fn make_print
 * @brief Make print node function
 * @param[in] expr pointer to expression node
 * @return pointer to created node
 */
INode *make_print(INode *expr)
{
  return new PNode{expr};
}

/**
 * @brief make scan node function
 * @return pointer to created node
 */
INode *make_scan()
{
  return new RNode;
}

/**
 * @fn make_asgn
 * @brief Make assignment node function
 * @param var_name
 * @param expr
 * @return pointer to created node
 */
INode *make_asgn(const std::string &var_name, INode *expr)
{
  IMMA_DOIN("ASS");
  auto it = CUR_SCOPE->check_n_insert(var_name);

  IMMA_DOIN("VAR CREATE");
  auto pvar = new VNode{it};

  return new ASNode{pvar, expr};
} /* End of 'make_ass' function */

/**
 * @fn IMMA_DOIN
 * @brief debug function
 * @param doin_wha
 */
void IMMA_DOIN(const char *doin_wha)
{
  // std::cout << "I'MMA DOIN " << doin_wha << std::endl;
}

} // namespace AST
