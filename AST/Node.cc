#include "llvm/ADT/APInt.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/Transforms/Utils.h"

#include "Interp.hh"
#include "Node.hh"

#include <functional>

extern llvm::LLVMContext *CUR_CONTEXT;
extern llvm::IRBuilder<> *BUILDER;
extern llvm::Function *CUR_FUNC;

namespace AST
{

std::unordered_map<std::string, llvm::Value *> NamedVals;

std::stack<int> ValStack{};

static llvm::AllocaInst *CreateEntryBlockAlloca(const std::string &varname)
{
  llvm::IRBuilder<> bldr(&CUR_FUNC->getEntryBlock(), CUR_FUNC->getEntryBlock().begin());
  return bldr.CreateAlloca(llvm::Type::getInt32Ty(*CUR_CONTEXT), nullptr, varname.c_str());
}

pINode make_cst(int val)
{
  return std::make_unique<CNode>(val);
}

pINode make_op(pINode &l, Ops op, pINode &r)
{
  return std::make_unique<OPNode>(l, op, r);
}

pINode make_un(Ops op, pINode &operand)
{
  return std::make_unique<UNOPNode>(op, operand);
}

pINode make_while(pINode &cond, pIScope &sc)
{
  return std::make_unique<WHNode>(cond, sc);
}

pINode make_if_else(pINode &cond, pIScope &isc, pIScope &esc)
{
  return std::make_unique<IFNode>(cond, isc, esc);
}

pINode make_if(pINode &cond, pIScope &isc)
{
  return std::make_unique<IFNode>(cond, isc);
}

pIScope make_scope(IScope *par /* = nullptr */)
{
  return std::make_unique<Scope>(par);
}

pINode make_ref(const std::string &var_name)
{
  auto it_bl = CUR_SCOPE->get_var(var_name);

  if (!it_bl.second)
  {
    std::string what = "Unknown variable '" + var_name + "'";
    throw std::runtime_error{what};
  }

  return std::make_unique<VNode>(it_bl.first);
}

pINode make_print(pINode &expr)
{
  return std::make_unique<PNode>(expr);
}

pINode make_scan()
{
  return std::make_unique<RNode>();
}

pINode make_asgn(const std::string &var_name, pINode &expr)
{
  auto it = CUR_SCOPE->check_n_insert(var_name);
  auto pvar = std::make_unique<VNode>(it);

#if (CODEGEN == 1)
  NamedVals[var_name] = CreateEntryBlockAlloca(var_name);
#endif

  return std::make_unique<ASNode>(pvar, expr);
}

/*pINode make_asgn_func(const std::string &var_name, pINode &func)
{
  auto it = CUR_SCOPE->check_n_insert(var_name);

  auto pvar = std::make_unique<VNode>(it);

  return std::make_unique<ASNode>(pvar, func);
}*/

////////////////////////// SCOPE METHODS /////////////////

std::pair<var_table::iterator, bool> Scope::get_var(const std::string &var_name)
{
  // find var in parent's scopes
  std::pair<var_table::iterator, bool> it_n_bool{loc_check(var_name)};

  if (it_n_bool.second)
    return it_n_bool;

  auto pscope = reset_scope();

  while (pscope != nullptr)
  {
    it_n_bool = pscope->loc_check(var_name);

    if (it_n_bool.second)
      break;
    else /* variable wasn't found */
      pscope = pscope->reset_scope();
  }

  return it_n_bool;
}

Scope::it_bool Scope::loc_check(const std::string &var_name)
{
  it_bool itbl{};

  itbl.first = var_tbl_.find(var_name);
  itbl.second = (var_tbl_.end() != itbl.first);

  return itbl;
}

var_table::iterator Scope::check_n_insert(const std::string &var_name)
{
  it_bool it_n_bool = get_var(var_name);

  if (!it_n_bool.second)
    it_n_bool.first = insert_var(var_name);

  return it_n_bool.first;
}

llvm::Value *Scope::codegen()
{
  for (auto &&nd : nodes_)
    nd->codegen();

  return nullptr;
}

llvm::Value *OPNode::codegen()
{
  auto L = left_->codegen();
  auto R = right_->codegen();

  if (L == nullptr || R == nullptr)
    return nullptr;

  switch (op_type_)
  {
  case Ops::ADD:
    return BUILDER->CreateAdd(L, R);
  case Ops::SUB:
    return BUILDER->CreateSub(L, R);
  case Ops::MUL:
    return BUILDER->CreateMul(L, R);
  case Ops::DIV:
    return BUILDER->CreateSDiv(L, R);
  case Ops::MOD:
    return BUILDER->CreateSRem(L, R);
  case Ops::GREATER:
    return BUILDER->CreateICmpSGT(L, R);
  case Ops::GR_EQ:
    return BUILDER->CreateICmpSGE(L, R);
  case Ops::LESS:
    return BUILDER->CreateICmpSLT(L, R);
  case Ops::LS_EQ:
    return BUILDER->CreateICmpSLE(L, R);
  case Ops::IS_EQ:
    return BUILDER->CreateICmpEQ(L, R);
  case Ops::NOT_EQ:
    return BUILDER->CreateICmpNE(L, R);
  case Ops::AND:
    return BUILDER->CreateAnd(L, R);
  case Ops::OR:
    return BUILDER->CreateOr(L, R);
  default:
    throw std::runtime_error("Unrecognized binary operator number\n");
  }
}

int OPNode::calc() const
{
  int right_val = ValStack.top();
  ValStack.pop();
  int left_val = ValStack.top();
  ValStack.pop();

  int res = 0;

  switch (op_type_)
  {
  case Ops::ADD:
    res = std::plus{}(left_val, right_val); // :)
    break;
  case Ops::SUB:
    res = std::minus{}(left_val, right_val); // :)
    break;
  case Ops::MUL:
    res = std::multiplies{}(left_val, right_val); // :)
    break;
  case Ops::DIV:
    if (std::equal_to{}(right_val, 0))
      throw std::runtime_error{"Dividing by ZERO!"};
    res = std::divides{}(left_val, right_val); // :)
    break;
  case Ops::MOD:
    if (right_val == 0)
      throw std::runtime_error{"Dividing by ZERO!"};
    res = left_val % right_val;
    break;
  case Ops::GREATER:
    res = left_val > right_val;
    break;
  case Ops::GR_EQ:
    res = left_val >= right_val;
    break;
  case Ops::LESS:
    res = left_val < right_val;
    break;
  case Ops::LS_EQ:
    res = left_val <= right_val;
    break;
  case Ops::IS_EQ:
    res = left_val == right_val;
    break;
  case Ops::NOT_EQ:
    res = left_val != right_val;
    break;
  case Ops::AND:
    res = left_val && right_val;
    break;
  case Ops::OR:
    res = left_val || right_val;
    break;
  default:
    throw std::runtime_error("Unrecognized binary operator number\n");
  }

  ValStack.push(res);

  return res;
}

llvm::Value *UNOPNode::codegen()
{
  auto V = operand_->codegen();
  if (V == nullptr)
    return nullptr;

  switch (op_type_)
  {
  case Ops::NEG:
    return BUILDER->CreateNeg(V);
  case Ops::NOT:
    return BUILDER->CreateNot(V);
  default:
    throw std::runtime_error("Unrecognized unary operator number\n");
  }
}

int UNOPNode::calc() const
{
  int val = ValStack.top();
  ValStack.pop();

  int res = 0;

  switch (op_type_)
  {
  case Ops::NEG:
    res = -val;
    break;
  case Ops::NOT:
    res = !val;
    break;
  default:
    throw std::runtime_error("Unrecognized unary operator number\n");
  }

  ValStack.push(res);

  return res;
}

int VNode::calc() const
{
  ValStack.push(location_->second.value);
  return location_->second.value;
}

llvm::Value *VNode::codegen()
{
  auto it = NamedVals.find(location_->first);
  if (it == NamedVals.end())
    throw std::runtime_error{"Unrecognized variable"};

  return BUILDER->CreateLoad(llvm::Type::getInt32Ty(*CUR_CONTEXT), it->second, it->first);
}

int CNode::calc() const
{
  ValStack.push(val_);
  return val_;
}

llvm::Value *CNode::codegen()
{
  return llvm::ConstantInt::get(llvm::Type::getInt32Ty(*CUR_CONTEXT), val_);
}

int ASNode::calc() const
{
  int expr_res = ValStack.top();
  ValStack.pop();

  ValStack.pop(); // delete dummy var value

  dst_->set_val(expr_res);

  ValStack.push(expr_res);
  return expr_res;
} /* End of 'calc' function */

llvm::Value *ASNode::codegen()
{
  auto it = NamedVals.find(dst_->get_name());
  if (it == NamedVals.end())
    throw std::runtime_error{"Unrecognized variable"};

  return BUILDER->CreateStore(expr_->codegen(), it->second);
}

INode *WHNode::get_i_child(size_t i) const
{
  i %= 2;

  if (i == 0)
    return cond_.get();

  int cond_calc = ValStack.top();
  ValStack.pop();

  if (cond_calc)
    return scope_.get();

  return nullptr;
}

llvm::Value *WHNode::codegen()
{
  return nullptr;
}

INode *IFNode::get_i_child(size_t i) const
{
  if (i >= childs_am_)
    throw std::runtime_error{"Incorrect children amount"};

  if (i == 0)
    return cond_.get();

  int calc_cond = ValStack.top();
  ValStack.pop();

  if (calc_cond)
    return if_scope_.get();
  else if (else_scope_ != nullptr)
    return else_scope_.get();

  return nullptr;
}

llvm::Value *IFNode::codegen()
{
  return nullptr;
}

int PNode::calc() const
{
  int val = ValStack.top();
  ValStack.pop();

  std::cout << val << std::endl;
  return 0;
}

llvm::Value *PNode::codegen()
{
  return nullptr;
}

int RNode::calc() const
{
  int value{};

  std::cin >> value;
  if (!std::cin.good())
    throw std::runtime_error{"Invalid symbol at stdin"};

  ValStack.push(value);

  return value;
}

llvm::Value *RNode::codegen()
{
  return nullptr;
}

//////////////END OF SCOPE METHODS ////////////////////////////////

} // namespace AST
