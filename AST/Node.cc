#include "Node.hh"

AST::Scope::Scope(IScope *parent /* = nullptr */) : parent_(parent)
{
}

/**
 * @brief Add new branch to scope function
 * @param branch
 */
void AST::Scope::add_branch(INode *branch)
{
  nodes_.push_back(branch);
} /* Node 'add_branch' function */


/**
 * Varibale node ctor
 * @param name [in] name of a variable
 * @param loc
 */
AST::VNode::VNode( var_table::iterator loc ) : location_(loc)
{}

const std::string &AST::VNode::get_name() const
{
  return location_->first;
}


AST::var_table::iterator AST::VNode::get_loc() const
{
  return location_;
}

int AST::VNode::get_val() const
{
  return location_->second;
}

void AST::VNode::set_val( int val )
{
  location_->second = val;
}
