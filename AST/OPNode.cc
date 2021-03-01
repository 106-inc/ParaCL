//
// Created by andrey on 21.02.2021.
//

#include "OPNode.hh"

namespace AST
{
/////////////////////////PLNode METHODS //////////////////

PLNode::PLNode(INode *left, INode *right) : OPNode(left, right)
{
}

/**
 * Calculate value of node function
 * @return
 */
int PLNode::calc() const
{
  return left_->calc() + right_->calc();
}

//////////////////END OF PLNode METHODS //////////////////

/////////////////////////SBNode METHODS //////////////////

SBNode::SBNode(INode *left, INode *right) : OPNode(left, right)
{
}

/**
 * Calculate value of node function
 * @return
 */
int SBNode::calc() const
{
  return left_->calc() - right_->calc();
}

//////////////////END OF SBNode METHODS //////////////////

/////////////////////////MLNode METHODS //////////////////

MLNode::MLNode(INode *left, INode *right) : OPNode(left, right)
{
}

/**
 * Calculate value of node function
 * @return
 */
int MLNode::calc() const
{
  return left_->calc() * right_->calc();
}

//////////////////END OF MLNode METHODS //////////////////

/////////////////////////DVNode METHODS //////////////////

DVNode::DVNode(INode *left, INode *right) : OPNode(left, right)
{
}

/**
 * Calculate value of node function
 * @return calculation result
 */
int DVNode::calc() const
{
  return left_->calc() / right_->calc();
}

//////////////////END OF PLNode METHODS //////////////////

////////////////////ASNode METHODS///////////////////////

ASNode::ASNode(VNode *dst, INode *expr) : dst_(dst), expr_(expr)
{
}

/**
 * @brief Interpret the node function
 */
int ASNode::calc() const
{
  int expr_res = expr_->calc();

  dst_->set_val(expr_res);

  return expr_res;
}

/**
 * @brief Assignment class destructor
 */
ASNode::~ASNode()
{
  delete dst_;
  delete expr_;
}
////////////////END OF ASNode METHODS///////////////////
//////////////////////EQNode////////////////////////////

EQNode::EQNode(INode *left, INode *right) : OPNode(left, right)
{
}

/**
 * @brief Calculate for equality
 * @return 1 if lhs is equal to rhs else 0
 */
int EQNode::calc() const
{
  return left_->calc() == right_->calc();
}
///////////////END OF EQNode////////////////////////////
//////////////////////NEQNode////////////////////////////

NEQNode::NEQNode(INode *left, INode *right) : OPNode(left, right)
{
}

/**
 * @brief Calculate for not equality
 * @return 1 if lhs is not equal to rhs else 0
 */
int NEQNode::calc() const
{
  return left_->calc() != right_->calc();
}

///////////////END OF EQNode////////////////////////////
//////////////////////GNode////////////////////////////

GNode::GNode(INode *left, INode *right) : OPNode(left, right)
{
}

/**
 * @brief Checking for greatness
 * @return 1 if lhs is greater then rhs
 */
int GNode::calc() const
{
  return left_->calc() > right_->calc();
}

///////////////END OF GNode////////////////////////////
//////////////////////GENode////////////////////////////

GENode::GENode(INode *left, INode *right) : OPNode(left, right)
{
}

/**
 * @brief Checking for greatness or equality
 * @return 1 if lhs is greater or equal then rhs
 */
int GENode::calc() const
{
  return left_->calc() >= right_->calc();
}

///////////////END OF GENode////////////////////////////
//////////////////////LNode////////////////////////////

LNode::LNode(INode *left, INode *right) : OPNode(left, right)
{
}

/**
 * @brief Checking for lessness
 * @return  1 if lhs is less then rhs
 */
int LNode::calc() const
{
  return left_->calc() < right_->calc();
}
///////////////END OF LNode////////////////////////////
//////////////////////LENode////////////////////////////

LENode::LENode(INode *left, INode *right) : OPNode(left, right)
{
}

/**
 * @brief Checking for lessness or equality
 * @return  1 if lhs is less or equal then rhs
 */
int LENode::calc() const
{
  return left_->calc() <= right_->calc();
}

///////////////END OF LENode////////////////////////////
//////////////////////ANDNode////////////////////////////

ANDNode::ANDNode(INode *left, INode *right) : OPNode(left, right)
{
}

/**
 * @brief Logical AND
 * @return 1 or 0
 */
int ANDNode::calc() const
{
  return left_->calc() && right_->calc();
}

///////////////END OF ANDNode////////////////////////////
//////////////////////ORNode////////////////////////////

ORNode::ORNode(INode *left, INode *right) : OPNode(left, right)
{
}

/**
 * @brief Logical OR
 * @return 1 or 0
 */
int ORNode::calc() const
{
  return left_->calc() || right_->calc();
}

///////////////END OF ORNode////////////////////////////
} // namespace AST
