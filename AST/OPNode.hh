//
// Created by andrey on 21.02.2021.
//

#ifndef PARACL_OPNODE_HH
#define PARACL_OPNODE_HH

#include "Node.hh"

namespace AST
{
class PLNode final : public OPNode
{
public:
  PLNode(INode *left, INode *right);

  PLNode(const PLNode &) = delete;
  PLNode &operator=(const PLNode &) = delete;

  int calc() const override;
};

class SBNode final : public OPNode
{
public:
  SBNode(INode *left, INode *right);

  SBNode(const SBNode &) = delete;
  SBNode &operator=(const SBNode &) = delete;

  int calc() const override;
};

class MLNode final : public OPNode
{
public:
  MLNode(INode *left, INode *right);

  MLNode(const MLNode &) = delete;
  MLNode &operator=(const MLNode &) = delete;

  int calc() const override;
};

class DVNode final : public OPNode
{
public:
  DVNode(INode *left, INode *right);

  DVNode(const DVNode &) = delete;
  DVNode &operator=(const DVNode &) = delete;

  int calc() const override;
};

/**
 * @brief Assignment operator's node
 */
class ASNode final : public INode
{
private:
  VNode *dst_;  // variable to assign
  INode *expr_; // expression
public:
  ASNode(VNode *dst, INode *expr);

  ASNode(const ASNode &) = delete;
  ASNode &operator=(const ASNode &) = delete;

  int calc() const override;
};
// TODO:
// ADD Logical operators classes
/////////////////////////
} // namespace AST

#endif // PARACL_OPNODE_HH
