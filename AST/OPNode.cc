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

/////////////////////////SBNode METHODS //////////////////

AST::SBNode::SBNode( INode *left, INode *right ) : OPNode(left, right)
{}

/**
 * Calculate value of node function
 * @return
 */
int AST::SBNode::calc() const
{
  return left_->calc() - right_->calc();
}

//////////////////END OF SBNode METHODS //////////////////

/////////////////////////MLNode METHODS //////////////////

AST::MLNode::MLNode( INode *left, INode *right ) : OPNode(left, right)
{}

/**
 * Calculate value of node function
 * @return
 */
int AST::MLNode::calc() const
{
  return left_->calc() * right_->calc();
}

//////////////////END OF MLNode METHODS //////////////////

/////////////////////////DVNode METHODS //////////////////

AST::DVNode::DVNode( INode *left, INode *right ) : OPNode(left, right)
{}

/**
 * Calculate value of node function
 * @return
 */
int AST::DVNode::calc() const
{
  return left_->calc() / right_->calc();
}

//////////////////END OF PLNode METHODS //////////////////