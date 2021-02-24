#ifndef NODE_HH
#define NODE_HH

/////////////////////////////////////
///// STL containers ///////////////
#include <unordered_map>
#include <vector>
/////////////////////////////////////

////// OUR HEADERS //////////////////
#include "INode.hh"
/////////////////////////////////////

namespace AST
{
// Scope structure
class Scope : public IScope // final(?)
{
  private:
    // vector with nodes of this scope
    std::vector<INode *> nodes_{};

    // Pointer to parent scope
    IScope *parent_{};

    std::unordered_map<std::string, int> var_tbl_;

  public:
    // constructor by parent scope ptr
    explicit Scope(IScope *parent = nullptr);

    void push(INode *node) override;

    ~Scope() = default;
};
} // namespace AST

#endif /* NODE_HH */