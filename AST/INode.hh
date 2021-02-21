#ifndef INODE_HH
#define INODE_HH

#include <string>

namespace AST
{
// node interface
struct INode
{
    virtual int calc() const = 0;

    virtual void dump() = 0;

    virtual ~INode() = default;
};

// scope interface
struct IScope : public INode
{
  // TODO: can't understand the purpose of this func
    virtual IScope *push() = 0;

    virtual IScope *reset_scope() const = 0;

    virtual void add_branch(INode *branch) = 0;

    virtual INode *access(std::string const &var_name) = 0;

    virtual INode *visible(std::string const &var_name) = 0;
};

enum class Ops
{
    // TODO: WRITE OPS
    // TODO: Idk is it necessary?
};

INode *make_value(int val);

INode *make_op(INode *l, Ops o, INode *r);

INode *make_while(INode *o, INode *s);

INode *make_if(INode *o, INode *s);

IScope *create_scope();

////////////////// TYPES OF NODES ////////////////////////
/*
 * 1. Variable -> just iterator to var table OK
 * 2. Infix operator ->
 * 3. While -> condition + scope
 * 4. if -> condition + scope
 * 5. else -> scope only
 * 5. I think it's all fir now
 */
//////////////////////////////////////////////////////////
} // namespace AST

#endif /* INODE_HH */