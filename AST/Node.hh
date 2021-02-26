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
    Scope *parent_{};

    var_table var_tbl_;

  public:
    // constructor by parent scope ptr
    explicit Scope(Scope *parent = nullptr);

    Scope( const Scope &sc ) = delete;

    Scope &operator =( const Scope &sc ) = delete;

    IScope *reset_scope() const override { return parent_; }

    void add_branch(INode *branch) override;

    int calc() const override;

    void add_var(const std::string &name) override;

    ~Scope() override;
};

/**
 * @brief Variable node class
 */
class VNode final : public INode
{
private:
  // No ptrs, because this node will always be a leaf


  var_table::iterator location_{};

public:

  explicit VNode( var_table::iterator loc );

  VNode( const VNode & ) = delete;
  VNode &operator =( const VNode & ) = delete;

  const std::string &get_name() const;

  var_table::iterator get_loc() const;

  int calc() const override;

  void set_val( int val );
};


/**
 * @brief Constant node class
 */
class CNode final : public INode
{
private:
  const int val_;
public:
  CNode( int val );

  CNode( const CNode & ) = delete;
  CNode &operator =( const CNode & ) = delete;

  int calc() const override;
};

/**
 * @brief Operator node class
 */
class OPNode : public INode
{
protected:
  INode *left_ {};
  INode *right_{};
public:

  OPNode( INode *left, INode *right );

  ~OPNode() override;
};

/**
 * @brief While node class
 */
class WHNode final : public INode
{
private:
  IScope *scope_;

  INode *cond_;
public:

  WHNode( IScope *scope, INode *cond );

  WHNode( const WHNode & ) = delete;
  WHNode &operator =( const WHNode & ) = delete;

  int calc() const override;

  ~WHNode();
};


/**
 * @brief If node class
 */
// TODO: 
// REALIZATION!!!

} // namespace AST

#endif /* NODE_HH */