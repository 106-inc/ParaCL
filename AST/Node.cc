#include "Node.hh"

namespace AST
{
////////////////////////// SCOPE METHODS /////////////////
Scope::Scope(IScope *parent /* = nullptr */) : parent_(parent)
{
}

/**
 * @brief Interpret the scope function (claculate)
 * @return int
 */
int Scope::calc() const
{
  for (auto node : nodes_)
    node->calc();

  return 0;
} /* End of 'calc' function */

/**
 * @brief Add node to scope function
 * @param node [in] node to add
 * @return none
 */
void Scope::push(INode *node)
{
  nodes_.push_back(node);
} /* End of 'push' function */

/**
 * @brief Check var in all available scopes function
 * @param var_name [in] name of a var to get access to
 * @return pair of iterator to var table and bool, which:
 * TRUE - iterator is valid, variable found,
 * FALSE - iterator is not valid (end()), variable was not found
 */
std::pair<var_table::iterator, bool> Scope::check_var(const std::string &var_name)
{
  // find var in parent's scopes
  IScope *pscope = this;
  std::pair<var_table::iterator, bool> it_n_bool{};

  while (pscope != nullptr)
  {
    it_n_bool = pscope->loc_check(var_name);

    if (it_n_bool.second)
      break;
    else /* variable wasn't found */
      pscope = pscope->reset_scope();
  }

  return it_n_bool;
} /* End of 'access' function */

/**
 * @brief Check variable in current scope function.
 * @param var_name [in] name of a var to find
 * @return pair of iterator to var table and bool, which:
 * TRUE - iterator is valid, variable found,
 * FALSE - iterator is not valid (end()), variable was not found
 */
Scope::it_bool Scope::loc_check(const std::string &var_name)
{
  it_bool itbl{};

  itbl.first = var_tbl_.find(var_name);
  itbl.second = (var_tbl_.end() != itbl.first);

  return itbl;
} /* End of 'loc_check' function */

/**
 *
 * @param var_name
 * @return
 */
var_table::iterator Scope::check_n_insert(const std::string &var_name)
{
  it_bool it_n_bool = check_var(var_name);

  if (!it_n_bool.second)
    it_n_bool.first = insert_var(var_name);

  return it_n_bool.first;
} /* End of 'check_n_insert' function */

/**
 * Scope class destructor
 */
Scope::~Scope()
{
  for (const auto &node : nodes_)
    delete node;
}

/**
 * @brief Insert variable to table of current scope
 * @warning NO VALIDATION OF UNIQUE!!!
 * @param var_name
 * @return iterator to inserted variable
 */
var_table::iterator Scope::insert_var(const std::string &var_name)
{
  auto it_bl = var_tbl_.insert({var_name, {}});

  return it_bl.first;
} /* End of 'insert_var' function */

//////////////END OF SCOPE METHODS ////////////////////////////////

////////////////VarNode METHODS/////////////////
/**
 * Varibale node ctor
 * @param name [in] name of a variable
 * @param loc
 */
VNode::VNode(var_table::iterator loc) : location_(loc)
{
}

/**
 * Get variable name function
 * @return
 */
const std::string &VNode::get_name() const
{
  return location_->first;
}

/**
 * Get variable location in table function
 * @return
 */
var_table::iterator VNode::get_loc() const
{
  return location_;
}

/**
 * Calculate value of a variable function
 * @return
 */
int VNode::calc() const
{
  return location_->second;
}

/**
 * Set value of variable function
 * @param val
 */
void VNode::set_val(int val)
{
  location_->second = val;
}
////////////////END OF VarNode METHODS/////////////////

////////////////CNode METHODS//////////////////////////

/**
 * Constant node ctor
 * @param val [in] - value of a node
 */
CNode::CNode(int val) : val_(val)
{
}

/**
 * Calculate the value of node
 * @return value of a node
 */
int CNode::calc() const
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
OPNode::OPNode(INode *left, INode *right) : left_(left), right_(right)
{
}

/**
 * OPNode class destructor.
 * Deletes left ans right nodes
 */
OPNode::~OPNode()
{
  delete left_;
  delete right_;
}

///////////END OF OPNODE METHODS/////////////////////////
////////////////UNOPNode METHODS/////////////////////////

/**
 * Operator's node constructor
 * @param left  [in] - left node of operator
 * @param right [in] - right node of operator
 */
UNOPNode::UNOPNode(INode *operand) : operand_(operand)
{
}

/**
 * UNOPNode class destructor.
 * Deletes left ans right nodes
 */
UNOPNode::~UNOPNode()
{
  delete operand_;
}

///////////END OF UNOPNode METHODS/////////////////////////
/////////////WHNode METHODS//////////////////////////////

WHNode::WHNode(INode *cond, IScope *scope) : cond_(cond), scope_(scope)
{
}

/**
 * @brief Calculate while node function
 * @return int
 */
int WHNode::calc() const
{
  while (cond_->calc())
    scope_->calc();

  return 0;
}

/**
 * While node class destructor
 */
WHNode::~WHNode()
{
  delete scope_;
  delete cond_;
}
//////////END OF WHNode METHODS//////////////////////////////

///////////////IFNode METHODS///////////////////////////////
IFNode::IFNode(INode *cond, IScope *if_sc, IScope *el_sc /* = nullptr */)
    : cond_(cond), if_scope_(if_sc), else_scope_(el_sc)
{
}

/**
 * Interpret If node function
 * @return calculated result
 */
int IFNode::calc() const
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
IFNode::~IFNode()
{
  delete cond_;
  delete if_scope_;
  delete else_scope_;
}
///////////END OF IFNode METHODS///////////////////////////////

////////////////////PNode METHODS//////////////////////////////
PNode::PNode(INode *expr) : expr_(expr)
{
}

/**
 * Interpret print node function
 */
int PNode::calc() const
{
  std::cout << expr_->calc() << std::endl;
  return 0;
}

PNode::~PNode()
{
  delete expr_;
}
/////////////////END OF PNode METHODS//////////////////////////////
/////////////////RNode METHODS/////////////////////////////////////

/**
 * @brief Interpret read node function
 * @return read value
 * @warning STD::TERMINATE CALL!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */
int RNode::calc() const
{
  int value{};

  std::cin >> value;
  if (!std::cin.good())
    std::terminate();

  return value;
} /* End of 'calc' function */
////////////////END OF RNode METHODS///////////////////////////////
} // namespace AST
