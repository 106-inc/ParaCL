#include "Node.hh"

AST::Scope::Scope(IScope *parent /* = nullptr */) : parent_(parent)
{
}

/**
 * push node to scope function
 * @param node to push
 * @return Nothing
 */
void AST::Scope::push(INode *node)
{

} /* End of 'push' function */
