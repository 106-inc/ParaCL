#include "INode.hh"

/**
 * @brief Create operator node function
 * @param l  [in] left node of operator
 * @param op [in] operator type
 * @param r  [in] right node of operator
 * @return pointer to created Node
 */
AST::INode *AST::make_op(INode *l, Ops op, INode *r)
{
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
AST::INode *AST::make_while(INode *o, INode *s)
{

} /* End of 'make_while' function */

/**
 * Create if node fucntion
 * @param o
 * @param s
 * @return pointer to created Node
 */
AST::INode *AST::make_if(INode *o, INode *s)
{
} /* End of 'make_if' function */

/**
 * Create scope function
 * @return pointer to created Scope
 */
AST::IScope *AST::create_scope()
{
} /* End of 'create_scope' function */