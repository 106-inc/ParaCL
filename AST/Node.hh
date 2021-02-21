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

    IScope *reset_scope() const override { return parent_; }

    void add_branch(INode *branch) override;

    ~Scope() = default;
};

struct VNode final : public INode
{
private:
  var_table::iterator location_;

public:

  VNode( var_table::iterator loc );

  const std::string &get_name() const;

  var_table::iterator get_loc() const;

  int get_val() const;

  void set_val( int val );

};
} // namespace AST

#endif /* NODE_HH */