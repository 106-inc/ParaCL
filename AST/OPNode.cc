//
// Created by andrey on 21.02.2021.
//

#include "OPNode.hh"

/////////////////////////PLNode METHODS //////////////////

AST::PLNode::PLNode(INode *left, INode *right) : OPNode(left, right)
{
}

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

AST::SBNode::SBNode(INode *left, INode *right) : OPNode(left, right)
{
}

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

AST::MLNode::MLNode(INode *left, INode *right) : OPNode(left, right)
{
}

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

AST::DVNode::DVNode(INode *left, INode *right) : OPNode(left, right)
{
}

/**
 * Calculate value of node function
 * @return calculation result
 */
int AST::DVNode::calc() const
{
  return left_->calc() / right_->calc();
}

//////////////////END OF PLNode METHODS //////////////////

////////////////////ASNode METHODS///////////////////////

AST::ASNode::ASNode(VNode *dst, INode *expr) : dst_(dst), expr_(expr)
{
}

/**
 * @brief Interpret the node function
 */
int AST::ASNode::calc() const
{
  int expr_res = expr_->calc();

  dst_->set_val(expr_res);

  return expr_res;
}

/**
 * @brief Assignment class destructor
 */
AST::ASNode::~ASNode()
{
  delete dst_;
  delete expr_;
}
////////////////END OF ASNode METHODS///////////////////
//////////////////////EQNode////////////////////////////

AST::EQNode::EQNode(INode *left, INode *right) : OPNode(left, right)
{
}

/**
 * @brief Calculate for equality
 * @return 1 if lhs is equal to rhs else 0
 */
int AST::EQNode::calc() const
{
  return left_->calc() == right_->calc();
}
///////////////END OF EQNode////////////////////////////
//////////////////////NEQNode////////////////////////////

AST::NEQNode::NEQNode(INode *left, INode *right) : OPNode(left, right)
{
}

/**
 * @brief Calculate for not equality
 * @return 1 if lhs is not equal to rhs else 0
 */
int AST::NEQNode::calc() const
{
  return left_->calc() != right_->calc();
}

///////////////END OF EQNode////////////////////////////
//////////////////////GNode////////////////////////////

AST::GNode::GNode(INode *left, INode *right) : OPNode(left, right)
{
}

/**
 * @brief Checking for greatness
 * @return 1 if lhs is greater then rhs
 */
int AST::GNode::calc() const
{
  return left_->calc() > right_->calc();
}

///////////////END OF GNode////////////////////////////
//////////////////////GENode////////////////////////////

AST::GENode::GENode(INode *left, INode *right) : OPNode(left, right)
{
}

/**
 * @brief Checking for greatness or equality
 * @return 1 if lhs is greater or equal then rhs
 */
int AST::GENode::calc() const
{
  return left_->calc() >= right_->calc();
}

///////////////END OF GENode////////////////////////////
//////////////////////LNode////////////////////////////

AST::LNode::LNode(INode *left, INode *right) : OPNode(left, right)
{
}

/**
 * @brief Checking for lessness
 * @return  1 if lhs is less then rhs
 */
int AST::LNode::calc() const
{
  return left_->calc() < right_->calc();
}
///////////////END OF LNode////////////////////////////
//////////////////////LENode////////////////////////////

AST::LENode::LENode(INode *left, INode *right) : OPNode(left, right)
{
}


/**
 * @brief Checking for lessness or equality
 * @return  1 if lhs is less or equal then rhs
 */
int AST::LENode::calc() const
{
  return left_->calc() <= right_->calc();
}

///////////////END OF LENode////////////////////////////
//////////////////////ANDNode////////////////////////////

AST::ANDNode::ANDNode(INode *left, INode *right) : OPNode(left, right)
{
}

/**
 * @brief Logical AND
 * @return 1 or 0
 */
int AST::ANDNode::calc() const
{
  return left_->calc() && right_->calc();
}

///////////////END OF ANDNode////////////////////////////
//////////////////////ORNode////////////////////////////

AST::ORNode::ORNode(INode *left, INode *right) : OPNode(left, right)
{
}

/**
 * @brief Logical OR
 * @return 1 or 0
 */
int AST::ORNode::calc() const
{
  return left_->calc() || right_->calc();
}

///////////////END OF ORNode////////////////////////////


















