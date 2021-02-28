#include "INode.hh"
#include "OPNode.hh"

/**
 * @brief Make constant calue node function
 * @param val [in] value to put to node
 * @return pointer to created node
 */
AST::INode *AST::make_cst(int val)
{
  IMMA_DOIN("CST");
  return new CNode{val};
} /* End of 'make_cst' function */

/**
 * @brief Create operator node function
 * @param l  [in] left node of operator
 * @param op [in] operator type
 * @param r  [in] right node of operator
 * @return pointer to created Node
 */
AST::INode *AST::make_op(INode *l, Ops op, INode *r)
{
  IMMA_DOIN("OP");
  switch (op)
  {
  case Ops::ADD:
    return new PLNode{l, r};
  case Ops::SUB:
    return new SBNode{l, r};
  case Ops::MUL:
    return new MLNode{l, r};
  case Ops::DIV:
    return new DVNode{l, r};
  default:
    std::cout << "Operator is not implemented\n";
    return nullptr;
  }
} /* End of 'make_op' function */

/**
 * @brief Make while node fucntion
 * @param o [in]
 * @param s []
 * @return pointer to created Node
 */
AST::INode *AST::make_while(INode *cond, IScope *sc)
{
  IMMA_DOIN("WHILE");
  return new WHNode{cond, sc};
} /* End of 'make_while' function */

/**
 * Create if node fucntion
 * @param o
 * @param s
 * @return pointer to created Node
 */
AST::INode *AST::make_if(INode *cond, IScope *isc, IScope *esc /* = nullptr */)
{
  IMMA_DOIN("IF");
  return new IFNode{cond, isc, esc};
} /* End of 'make_if' function */

/**
 * @brief Create scope function
 * @param par [in] - pointer to parent node
 * @return pointer to created Scope
 */
AST::IScope *AST::make_scope(IScope *par /* = nullptr */)
{
  IMMA_DOIN("SCOPE");
  return new Scope{par};

} /* End of 'create_scope' function */


/**
 * @brief Make var node for expression
 * @param var_name name of a variable
 * @warning DEBUG version - std::terminate!!!!!!!!!
 * @return pointer to created node
 */
AST::INode *AST::make_ref(const std::string &var_name)
{
  IMMA_DOIN("VAR USAGE");
  auto it_bl = CUR_SCOPE->check_var(var_name);

  // TODO: delete termination
  if (!it_bl.second)
    std::terminate();
    /* std::hdd::format(); */

  return new VNode{it_bl.first};
} /* End of 'make_ref' function */


//TODO: docs
AST::INode *AST::make_print(AST::INode *expr)
{
  return new PNode{expr};
}

/**
 * @brief Make assignment node function
 * @param var_name
 * @param expr
 * @return pointer to created node
 */
AST::INode *AST::make_ass(const std::string &var_name, INode *expr)
{
  IMMA_DOIN("ASS");
  auto it = CUR_SCOPE->check_n_insert(var_name);

  auto pvar = new VNode{it};

  return new ASNode{pvar, expr};
} /* End of 'make_ass' function */

/**
 * @brief debug function
 * @param doin_wha
 */
void AST::IMMA_DOIN(const char * doin_wha)
{
  std::cout << "I'MMA DOIN " << doin_wha << std::endl;
}