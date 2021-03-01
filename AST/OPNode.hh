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

  ~ASNode();
};
// TODO:
// ADD Logical operators classes
/////////////////////////
//
class EQNode final : public OPNode
{
public:
  EQNode(INode *left, INode *right);

  EQNode(const EQNode &) = delete;
  EQNode &operator=(const EQNode &) = delete;

  int calc() const override;
};


class NEQNode final : public OPNode
{
public:
  NEQNode(INode *left, INode *right);

  NEQNode(const NEQNode &) = delete;
  NEQNode &operator=(const NEQNode &) = delete;

  int calc() const override;
};


class GNode final : public OPNode
{
public:
  GNode(INode *left, INode *right);

  GNode(const GNode &) = delete;
  GNode &operator=(const GNode &) = delete;

  int calc() const override;
};


class GENode final : public OPNode
{
public:
  GENode(INode *left, INode *right);

  GENode(const GENode &) = delete;
  GENode &operator=(const GENode &) = delete;

  int calc() const override;
};


class LNode final : public OPNode
{
public:
  LNode(INode *left, INode *right);

  LNode(const LNode &) = delete;
  LNode &operator=(const LNode &) = delete;

  int calc() const override;
};

class LENode final : public OPNode
{
public:
  LENode(INode *left, INode *right);

  LENode(const LENode &) = delete;
  LENode &operator=(const LENode &) = delete;

  int calc() const override;
};


class ANDNode final : public OPNode
{
public:
  ANDNode(INode *left, INode *right);

  ANDNode(const ANDNode &) = delete;
  ANDNode &operator=(const ANDNode &) = delete;

  int calc() const override;
};

class ORNode final : public OPNode
{
public:
  ORNode(INode *left, INode *right);

  ORNode(const ORNode &) = delete;
  ORNode &operator=(const ORNode &) = delete;

  int calc() const override;
};


} // namespace AST

#endif // PARACL_OPNODE_HH
