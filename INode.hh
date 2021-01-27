#ifndef INODE_HH
#define INODE_HH

#include <string>

// node interface
struct INode
{
  virtual int calc ( ) = 0;
  virtual void dump ( ) = 0;
  virtual ~INode ( ) {}
};

// scope interface
struct IScope : public INode
{

};

#endif