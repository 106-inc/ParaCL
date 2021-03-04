//
// Created by andrey on 21.02.2021.
//

#ifndef PARACL_OPNODE_HH
#define PARACL_OPNODE_HH

#include "Node.hh"

namespace AST
{
/**
 * @brief Plus node struct
 */
struct PLNode final : public OPNode
{
  /**
   * @brief PLus node struct ctor
   * @param left left node ptr
   * @param right right node ptr
   */
  PLNode(INode *left, INode *right) : OPNode(left, right)
  {
  }

  /**
   * @brief Calculate value of plus node function
   * @return calculated sum value
   */
  int calc() const override
  {
    return left_->calc() + right_->calc();
  } /* End of 'calc' function */
};

/**
 * @brief Sub node struct
 */
struct SBNode final : public OPNode
{
  /**
   * @brief Sub node struct ctor
   * @param left left node ptr
   * @param right right node ptr
   */
  SBNode(INode *left, INode *right) : OPNode(left, right)
  {
  }

  /**
   * @brief Calculate value of subtract node function
   * @return calculated subtract value
   */
  int calc() const override
  {
    return left_->calc() - right_->calc();
  } /* End of 'calc' function */
};

/**
 * @brief Mul node struct
 */
struct MLNode final : public OPNode
{
  /**
   * @brief Mul node struct ctor
   * @param left left node ptr
   * @param right right node ptr
   */
  MLNode(INode *left, INode *right) : OPNode(left, right)
  {
  }

  /**
   * @brief Calculate value of multiply node function
   * @return calculated product value
   */
  int calc() const override
  {
    return left_->calc() * right_->calc();
  } /* End of 'calc' function */
};

/**
 * @brief Division node struct
 */
struct DVNode final : public OPNode
{
  /**
   * @brief Division node struct ctor
   * @param left left node ptr
   * @param right right node ptr
   */
  DVNode(INode *left, INode *right) : OPNode(left, right)
  {
  }

  /**
   * @brief Calculate value of division node function
   * @return calculated divided value
   */
  int calc() const override
  {
    int rcalc = right_->calc();
    
    if (rcalc == 0)
      throw std::runtime_error{"Dividing by ZERO!"};

    return left_->calc() / rcalc;
  } /* End of 'calc' function */
};

/**
 * @brief Assignment operator's node class
 */
class ASNode final : public INode
{
private:
  VNode *dst_;  // variable to assign
  INode *expr_; // expression
public:
  /**
   * @brief Assignment node class ctor
   * @param dst pointer to destination variable node
   * @param expr pointer to expression node(-s)
   */
  ASNode(VNode *dst, INode *expr) : dst_(dst), expr_(expr)
  {
  }

  /**
   * @brief Calculate an assignment function
   * @return calculated assigned value
   */
  int calc() const override
  {
    int expr_res = expr_->calc();

    dst_->set_val(expr_res);

    return expr_res;
  } /* End of 'calc' function */
};

/**
 * @brief Equality node struct
 */
struct EQNode final : public OPNode
{
  /**
   * @brief Equality node struct ctor
   * @param left left node ptr
   * @param right right node ptr
   */
  EQNode(INode *left, INode *right) : OPNode(left, right)
  {
  }

  /**
   * @brief Calculate value of equality node function
   * @return calculated is equality value
   */
  int calc() const override
  {
    return left_->calc() == right_->calc();
  } /* End of 'calc' function */
};

/**
 * Not equality node struct
 */
struct NEQNode final : public OPNode
{
  /**
   * @brief Not equality node struct ctor
   * @param left left node ptr
   * @param right right node ptr
   */
  NEQNode(INode *left, INode *right) : OPNode(left, right)
  {
  }

  /**
   * @brief Calculate value of not equality node function
   * @return calculated not equality checking value
   */
  int calc() const override
  {
    return left_->calc() != right_->calc();
  } /* End of 'calc' function */
};

/**
 * @brief Greater node struct
 */
struct GNode final : public OPNode
{
  /**
   * @brief Greater node struct ctor
   * @param left left node ptr
   * @param right right node ptr
   */
  GNode(INode *left, INode *right) : OPNode(left, right)
  {
  }

  /**
   * @brief Calculate value of greater node function
   * @return calculated value
   */
  int calc() const override
  {
    return left_->calc() > right_->calc();
  } /* End of 'calc' function */
};

/**
 * @brief Greater-or-equal node struct
 */
struct GENode final : public OPNode
{
  /**
   * @brief Greater-or-equal node struct ctor
   * @param left left node ptr
   * @param right right node ptr
   */
  GENode(INode *left, INode *right) : OPNode(left, right)
  {
  }

  /**
   * @brief Calculate value of Greater-or-equal node function
   * @return calculated value
   */
  int calc() const override
  {
    return left_->calc() >= right_->calc();
  } /* End of 'calc' function */
};

/**
 * @brief Less node struct
 */
struct LNode final : public OPNode
{
  /**
   * @brief Less node struct ctor
   * @param left left node ptr
   * @param right right node ptr
   */
  LNode(INode *left, INode *right) : OPNode(left, right)
  {
  }

  /**
   * @brief Calculate value of less node function
   * @return calculated value
   */
  int calc() const override
  {
    return left_->calc() < right_->calc();
  } /* End of 'calc' function */
};

/**
 * @brief Less-or-equal node struct
 */
struct LENode final : public OPNode
{
  /**
   * @brief Less-or-equal node struct ctor
   * @param left left node ptr
   * @param right right node ptr
   */
  LENode(INode *left, INode *right) : OPNode(left, right)
  {
  }

  /**
   * @brief Calculate value of Less-or-equal node function
   * @return calculated value
   */
  int calc() const override
  {
    return left_->calc() <= right_->calc();
  } /* End of 'calc' function */
};

/**
 * @brief Logical AND node struct
 */
struct ANDNode final : public OPNode
{
  /**
   * @brief Logical AND node struct ctor
   * @param left left node ptr
   * @param right right node ptr
   */
  ANDNode(INode *left, INode *right) : OPNode(left, right)
  {
  }

  /**
   * @brief Calculate value of logical AND node function
   * @return calculated conjuncted value
   */
  int calc() const override
  {
    return left_->calc() && right_->calc();
  } /* End of 'calc' function */
};

/**
 * @brief Logical OR node struct
 */
struct ORNode final : public OPNode
{
  /**
   * @brief Logical OR node struct ctor
   * @param[in] left left node ptr
   * @param[in] right right node ptr
   */
  ORNode(INode *left, INode *right) : OPNode(left, right)
  {
  }

  /**
   * @brief Calculate value of Logical OR node function
   * @return calculated disjuncted value
   */
  int calc() const override
  {
    return left_->calc() || right_->calc();
  } /* End of 'calc' function */
};

/**
 * @brief Mod node struct
 * @struct MDNode
 */
struct MDNode final : public OPNode
{
  /**
   * @brief Construct a new MDNode object
   * @param[in] left  pointer to left node
   * @param[in] right pointer to right node
   */
  MDNode(INode *left, INode *right) : OPNode(left, right)
  {
  }

  /**
   * @brief calculate value of mod node function
   * @return calculated value
   */
  int calc() const override
  {
    int rcalc = right_->calc();

    if (!rcalc)
      throw std::runtime_error{"Dividing by ZERO!"};

    return left_->calc() % rcalc;
  }
};

/**
 * @struct NEGNode
 * @brief Negative node struct
 */
struct NEGNode final : public UNOPNode
{
  /**
   * @brief Negative node struct ctor
   * @param[in] operand operand node ptr
   */
  NEGNode(INode *operand) : UNOPNode(operand)
  {
  }

  /**
   * @brief Calculate value of negative node function
   * @return calculated negatiated value
   */
  int calc() const override
  {
    return -operand_->calc();
  } /* End of 'calc' function */
};

/**
 * @struct NOTNode
 * @brief Logical NOT node struct
 */
struct NOTNode final : public UNOPNode
{
  /**
   * @brief Logical NOT node struct ctor
   * @param[in] operand operand node ptr
   */
  NOTNode(INode *operand) : UNOPNode(operand)
  {
  }

  /**
   * @brief Calculate value of Logical NOT node function
   * @return calculated inversed value
   */
  int calc() const override
  {
    return !operand_->calc();
  } /* End of 'calc' function */
};

} // namespace AST

#endif // PARACL_OPNODE_HH
