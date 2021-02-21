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
    PLNode( INode *left, INode *right );

    int calc() const override;
  };
}

#endif //PARACL_OPNODE_HH
