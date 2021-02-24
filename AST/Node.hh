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

using var_table = std::unordered_map<std::string, int>;

// Scope structure
class Scope : public IScope // final(?)
{
  private:
    // vector with nodes of this scope
    std::vector<INode *> nodes_{};

    // Pointer to parent scope
    IScope *parent_{};

    var_table var_tbl_;

  public:
    // constructor by parent scope ptr
    explicit Scope(IScope *parent = nullptr);

    IScope *reset_scope() const override
    {
        return parent_;
    }

    void add_branch(INode *branch) override;

    ~Scope() override;
};

class VNode final : public INode
{
  private:
    // No ptrs, because this node will always be a leaf

    var_table::iterator location_;

  public:
    explicit VNode(var_table::iterator loc);

    const std::string &get_name() const;

    var_table::iterator get_loc() const;

    int calc() const override;

    void set_val(int val);
};

class CNode final : public INode
{
  private:
    const int val_;

  public:
    CNode(int val);

    int calc() const override;
};

class OPNode : public INode
{
  protected:
    INode *left_{};
    INode *right_{};

  public:
    OPNode(INode *left, INode *right);

    ~OPNode() override;
};

}; // namespace AST


#endif /* NODE_HH */