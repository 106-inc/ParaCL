#include "INode.hh"

/**
 * @brief Create value node function
 * @param val value of a node
 * @return pointer to created Node
 */
AST::INode *AST::make_value(int val)
{
} /* End of 'make_value' function */

/**
 * @brief Create operator node function
 * @param l [in] left node of operator
 * @param o [in] operator type
 * @param r [in] right node of operator
 * @return pointer to created Node
 */
AST::INode *AST::make_op(INode *l, Ops o, INode *r)
{

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