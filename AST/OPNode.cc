//
// Created by andrey on 21.02.2021.
//

#include "OPNode.hh"

/////////////////////////PLNode METHODS //////////////////

AST::PLNode::PLNode( INode *left, INode *right ) : OPNode(left, right)
{}

/**
 * Calculate value of node function
 * @return
 */
int AST::PLNode::calc() const
{
  return left_->calc() + right_->calc();
}

//////////////////END OF PLNode METHODS //////////////////