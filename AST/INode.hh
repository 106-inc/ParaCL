#ifndef INODE_HH
#define INODE_HH

#include <string>

using std::string;

// node interface
struct INode {
    virtual int calc() = 0;
    virtual void dump() = 0;
    virtual ~INode() {}
};

// scope interface
struct IScope : public INode {
    virtual IScope *push() = 0;
    virtual IScope *reset_scope() const = 0;
    virtual void add_branch(INode *branch) = 0;
    virtual INode *access(string const &var_name) = 0;
    virtual INode *visible(string const &var_name) = 0;
};

enum class Ops {
    // TODO: WRITE OPS
};

INode *make_value(int val);
INode *make_op(INode *l, Ops o, INode *r);
INode *make_while(INode *o, INode *s);
INode *make_if(INode *o, INode *s);
IScope *create_scope();

#endif