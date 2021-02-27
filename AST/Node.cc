#include "Node.hh"


////////////////////////// SCOPE METHODS /////////////////
AST::Scope::Scope(Scope *parent /* = nullptr */) : parent_(parent)
{
}

/**
 * @brief Add new branch to scope function
 * @param branch
 */
void AST::Scope::add_branch(INode *branch)
{
  nodes_.push_back(branch);
} /* End 'add_branch' function */

/**
 * @brief Interpret the scope function (claculate)
 * @return int 
 */
int AST::Scope::calc() const
{
  for (auto node : nodes_)
    node->calc();

  return 0;
} /* End of 'calc' function */

/**
 * @brief Add new var to scope function
 * @note reloading for variable node
 * @param branch
 * @param name - name of a variable to add
 */
void AST::Scope::add_var(const std::string &name)
{
  // find var in parent's scopes
  Scope *pscope = this;
  var_table::iterator it{};

  while (pscope != nullptr)
  {
    it = pscope->var_tbl_.find(name);
    if (it != pscope->var_tbl_.end())
      pscope->nodes_.push_back(new VNode{it});
    else /* variable wasn't found */
      pscope = pscope->parent_;
  }
  // It's completely new variable

  // add variable to current scope
  auto pair = var_tbl_.insert({name, {}});

  nodes_.push_back(new VNode{pair.first});
} /* End 'add_var' function */

/**
 * Scope class destructor
 */
AST::Scope::~Scope()
{
  for (const auto &node : nodes_)
    delete node;
}

//////////////END OF SCOPE METHODS ////////////////////////////////

////////////////VarNode METHODS/////////////////
/**
 * Varibale node ctor
 * @param name [in] name of a variable
 * @param loc
 */
AST::VNode::VNode( var_table::iterator loc ) : location_(loc)
{}

/**
 * Get variable name function
 * @return
 */
const std::string &AST::VNode::get_name() const
{
  return location_->first;
}

/**
 * Get variable location in table function
 * @return
 */
AST::var_table::iterator AST::VNode::get_loc() const
{
  return location_;
}

/**
 * Calculate value of a variable function
 * @return
 */
int AST::VNode::calc() const
{
  return location_->second;
}

/**
 * Set value of variable function
 * @param val
 */
void AST::VNode::set_val( int val )
{
  location_->second = val;
}
////////////////END OF VarNode METHODS/////////////////

////////////////CNode METHODS//////////////////////////

/**
 * Constant node ctor
 * @param val [in] - value of a node
 */
AST::CNode::CNode( int val ) : val_(val)
{}

/**
 * Calculate the value of node
 * @return value of a node
 */
int AST::CNode::calc() const
{
  return val_;
}

/////////END OF CNode METHODS//////////////////////////

////////////////OPNODE METHODS/////////////////////////

/**
 * Operator's node constructor
 * @param left  [in] - left node of operator
 * @param right [in] - right node of operator
 */
AST::OPNode::OPNode( INode *left, INode *right ) : left_ (left),
                                                   right_(right)
{}


/**
 * OPNode class destructor.
 * Deletes left ans right nodes
 */
AST::OPNode::~OPNode()
{
  delete left_;
  delete right_;
}



///////////END OF OPNODE METHODS/////////////////////////

/////////////WHNode METHODS//////////////////////////////

AST::WHNode::WHNode( IScope *scope, INode *cond ) : scope_(scope),
                                                    cond_(cond)
{}

/**
 * @brief Calculate while node function
 * @return int 
 */
int AST::WHNode::calc() const
{
  while (cond_->calc())
    scope_->calc();

  return 0;
}

/**
 * While node class destructor
 */
AST::WHNode::~WHNode()
{
  delete scope_;
  delete cond_;
}
//////////END OF WHNode METHODS//////////////////////////////

///////////////IFNode METHODS///////////////////////////////
AST::IFNode::IFNode(INode *cond, IScope *if_sc,
                    IScope *el_sc /* = nullptr */) : cond_(cond),
                                     if_scope_(if_sc),
                                     else_scope_(el_sc)
{}

/**
 * Interpret If node function
 * @return calculated result
 */
int AST::IFNode::calc() const
{
  if (cond_->calc())
    if_scope_->calc();
  else if (else_scope_ != nullptr)
    else_scope_->calc();

  return 0;
}

/**
 * If node dtor function
 */
AST::IFNode::~IFNode()
{
  delete cond_;
  delete if_scope_;
  delete else_scope_;
}
///////////END OF IFNode METHODS///////////////////////////////

////////////////////PNode METHODS//////////////////////////////
AST::PNode::PNode( INode *expr ) : expr_(expr)
{}

/**
 * Interpret print node function
 */
int AST::PNode::calc() const
{
  std::cout << expr_->calc() << std::endl;
  return 0;
}

AST::PNode::~PNode()
{
  delete expr_;
}
/////////////////END OF PNode METHODS//////////////////////////////
