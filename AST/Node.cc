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
extern llvm::Module *CUR_MODULE;

namespace AST
{
std::unordered_map<std::string, llvm::Value *> NamedVals;

std::stack<IntT> ValStack{};

#if (CODEGEN == 1)
static llvm::AllocaInst *CreateEntryBlockAlloca(const std::string &varname)
{
  llvm::IRBuilder<> bldr(&CUR_FUNC->getEntryBlock(), CUR_FUNC->getEntryBlock().begin());
  return bldr.CreateAlloca(llvm::Type::getInt32Ty(*CUR_CONTEXT), nullptr, varname.c_str());
}
#endif

static llvm::Value *ToInt32(llvm::Value *val, bool is_signed = false)
{
  return BUILDER->CreateIntCast(val, BUILDER->getInt32Ty(), is_signed);
}

static llvm::Value *ZeroCmp(llvm::Value *val)
{
  return BUILDER->CreateICmpEQ(val, BUILDER->getInt32(0));
}

static llvm::Value *ZeroNEQ(llvm::Value *val)
{
  return BUILDER->CreateICmpNE(val, BUILDER->getInt32(0));
}

pINode make_cst(IntT val)
{
  return std::make_shared<CNode>(val);
}

pINode make_op(const pINode &l, Ops op, const pINode &r)
{
  return std::make_shared<OPNode>(l, op, r);
}

pINode make_un(Ops op, const pINode &operand)
{
  return std::make_shared<UNOPNode>(op, operand);
}

pINode make_while(const pINode &cond, const pIScope &sc)
{
  return std::make_shared<WHNode>(cond, sc);
}

pINode make_if_else(const pINode &cond, const pIScope &isc, const pIScope &esc)
{
  return std::make_shared<IFNode>(cond, isc, esc);
}

pINode make_if(const pINode &cond, const pIScope &isc)
{
  return std::make_shared<IFNode>(cond, isc);
}

pIScope make_scope(IScope *par /* = nullptr */)
{
  return std::make_shared<Scope>(par);
}

pINode make_ref(const std::string &var_name)
{
  auto it_bl = CUR_SCOPE->get_var(var_name);

  if (!it_bl.second)
  {
    std::string what = "Unknown variable '" + var_name + "'";
    throw std::runtime_error{what};
  }

  return std::make_shared<VNode>(it_bl.first);
}

pINode make_print(const pINode &expr)
{
  return std::make_shared<PNode>(expr);
}

pINode make_scan()
{
  return std::make_shared<RNode>();
}

pINode make_asgn(const std::string &var_name, const pINode &expr)
{
  auto it = CUR_SCOPE->check_n_insert(var_name);
  auto pvar = std::make_shared<VNode>(it);

#if (CODEGEN == 1)
  NamedVals[var_name] = CreateEntryBlockAlloca(var_name);
#endif

  return std::make_shared<ASNode>(pvar, expr);
}

/*pINode make_asgn_func(const std::string &var_name, pINode &func)
{
  auto it = CUR_SCOPE->check_n_insert(var_name);

  auto pvar = std::make_shared<VNode>(it);

  return std::make_unique<ASNode>(pvar, func);
}*/

////////////////////////// SCOPE METHODS /////////////////

std::pair<var_table::iterator, bool> Scope::get_var(const std::string &var_name)
{
  // find var in parent's scopes
  std::pair<var_table::iterator, bool> it_n_bool{loc_check(var_name)};

  if (it_n_bool.second)
    return it_n_bool;

  auto *pscope = reset_scope();

  while (pscope != nullptr)
  {
    it_n_bool = pscope->loc_check(var_name);

    if (!it_n_bool.second)
      pscope = pscope->reset_scope();
    else /* variable wasn't found */
      break;
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
  auto *L = left_->codegen();
  auto *R = right_->codegen();

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
    return ToInt32(BUILDER->CreateICmpSGT(L, R));
  case Ops::GR_EQ:
    return ToInt32(BUILDER->CreateICmpSGE(L, R));
  case Ops::LESS:
    return ToInt32(BUILDER->CreateICmpSLT(L, R));
  case Ops::LS_EQ:
    return ToInt32(BUILDER->CreateICmpSLE(L, R));
  case Ops::IS_EQ:
    return ToInt32(BUILDER->CreateICmpEQ(L, R));
  case Ops::NOT_EQ:
    return ToInt32(BUILDER->CreateICmpNE(L, R));
  case Ops::AND:
    return ToInt32(BUILDER->CreateAnd(ZeroCmp(L), ZeroCmp(R)));
  case Ops::OR:
    return ToInt32(BUILDER->CreateOr(ZeroCmp(L), ZeroCmp(R)));
  default:
    throw std::runtime_error("Unrecognized binary operator number\n");
  }
}

IntT OPNode::calc() const
{
  auto right_val = ValStack.top();
  ValStack.pop();
  auto left_val = ValStack.top();
  ValStack.pop();

  decltype(left_val) res = 0;

  switch (op_type_)
  {
  case Ops::ADD:
    res = left_val + right_val;
    break;
  case Ops::SUB:
    res = left_val - right_val;
    break;
  case Ops::MUL:
    res = left_val * right_val;
    break;
  case Ops::DIV:
    if (right_val == 0)
      throw std::runtime_error{"Dividing by ZERO!"};
    res = left_val / right_val;
    break;
  case Ops::MOD:
    if (right_val == 0)
      throw std::runtime_error{"Dividing by ZERO!"};
    res = left_val % right_val;
    break;
  case Ops::GREATER:
    res = static_cast<decltype(res)>(left_val > right_val);
    break;
  case Ops::GR_EQ:
    res = static_cast<decltype(res)>(left_val >= right_val);
    break;
  case Ops::LESS:
    res = static_cast<decltype(res)>(left_val < right_val);
    break;
  case Ops::LS_EQ:
    res = static_cast<decltype(res)>(left_val <= right_val);
    break;
  case Ops::IS_EQ:
    res = static_cast<decltype(res)>(left_val == right_val);
    break;
  case Ops::NOT_EQ:
    res = static_cast<decltype(res)>(left_val != right_val);
    break;
  case Ops::AND:
    res = static_cast<decltype(res)>(static_cast<bool>(left_val) && static_cast<bool>(right_val));
    break;
  case Ops::OR:
    res = static_cast<decltype(res)>(static_cast<bool>(left_val) || static_cast<bool>(right_val));
    break;
  default:
    throw std::runtime_error("Unrecognized binary operator number\n");
  }

  ValStack.push(res);

  return res;
}

llvm::Value *UNOPNode::codegen()
{
  auto *V = operand_->codegen();
  if (V == nullptr)
    return nullptr;

  switch (op_type_)
  {
  case Ops::NEG:
    return BUILDER->CreateNeg(V);
  case Ops::NOT:
    return ToInt32(BUILDER->CreateICmpEQ(V, BUILDER->getInt32(0)));
  default:
    throw std::runtime_error("Unrecognized unary operator number\n");
  }
}

IntT UNOPNode::calc() const
{
  auto val = ValStack.top();
  ValStack.pop();

  decltype(val) res = 0;

  switch (op_type_)
  {
  case Ops::NEG:
    res = -val;
    break;
  case Ops::NOT:
    res = static_cast<decltype(res)>(!static_cast<bool>(val));
    break;
  default:
    throw std::runtime_error("Unrecognized unary operator number\n");
  }

  ValStack.push(res);

  return res;
}

IntT VNode::calc() const
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

IntT CNode::calc() const
{
  ValStack.push(val_);
  return val_;
}

llvm::Value *CNode::codegen()
{
  return llvm::ConstantInt::get(llvm::Type::getInt32Ty(*CUR_CONTEXT), val_);
}

IntT ASNode::calc() const
{
  auto expr_res = ValStack.top();
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
  /*
   * i &= 1; -- more optimized way
   */

  if (i == 0)
    return cond_.get();

  IntT cond_calc = ValStack.top();
  ValStack.pop();

  if (cond_calc != 0)
    return scope_.get();

  return nullptr;
}

llvm::Value *WHNode::codegen()
{
  auto *condBB = llvm::BasicBlock::Create(*CUR_CONTEXT, "wh_cond", CUR_FUNC);
  auto *bodyBB = llvm::BasicBlock::Create(*CUR_CONTEXT, "wh_body", CUR_FUNC);
  auto *nextBB = llvm::BasicBlock::Create(*CUR_CONTEXT, "wh_next", CUR_FUNC);

  BUILDER->CreateBr(condBB);

  BUILDER->SetInsertPoint(condBB);
  auto *v_expr = cond_->codegen();
  if (v_expr == nullptr)
    return nullptr;

  auto *v_cond = ZeroNEQ(v_expr);

  BUILDER->CreateCondBr(v_cond, bodyBB, nextBB);

  BUILDER->SetInsertPoint(bodyBB);
  scope_->codegen();
  BUILDER->CreateBr(condBB);

  BUILDER->SetInsertPoint(nextBB);

  return nullptr;
}

INode *IFNode::get_i_child(size_t i) const
{
  if (i >= childs_am_)
    throw std::runtime_error{"Incorrect children amount"};

  if (i == 0)
    return cond_.get();

  IntT calc_cond = ValStack.top();
  ValStack.pop();

  if (calc_cond != 0)
    return if_scope_.get();

  if (else_scope_ != nullptr)
    return else_scope_.get();

  return nullptr;
}

llvm::Value *IFNode::codegen()
{
  auto *v_expr = cond_->codegen();
  if (v_expr == nullptr)
    return nullptr;

  auto *v_cond = ZeroNEQ(v_expr);

  auto &BBL = CUR_FUNC->getBasicBlockList();

  auto *trueBB = llvm::BasicBlock::Create(*CUR_CONTEXT, "if_true", CUR_FUNC);
  auto *nextBB = llvm::BasicBlock::Create(*CUR_CONTEXT, "if_next", CUR_FUNC);
  auto *falseBB = llvm::BasicBlock::Create(*CUR_CONTEXT, "if_false");

  BUILDER->CreateCondBr(v_cond, trueBB, else_scope_ == nullptr ? nextBB : falseBB);

  BUILDER->SetInsertPoint(trueBB);
  if_scope_->codegen();
  BUILDER->CreateBr(nextBB);

  if (else_scope_ != nullptr)
  {
    BBL.push_back(falseBB);
    BUILDER->SetInsertPoint(falseBB);
    else_scope_->codegen();
    BUILDER->CreateBr(nextBB);
  }

  BUILDER->SetInsertPoint(nextBB);

  return nullptr;
}

IntT PNode::calc() const
{
  auto val = ValStack.top();
  ValStack.pop();

  std::cout << val << std::endl;
  return 0;
}

llvm::Value *PNode::codegen()
{
  auto *V = expr_->codegen();

  auto *CallP = CUR_MODULE->getFunction("__pcl_print");

  return BUILDER->CreateCall(CallP, {V});
}

IntT RNode::calc() const
{
  IntT value{};

  std::cin >> value;
  if (!std::cin.good())
    throw std::runtime_error{"Invalid symbol at stdin"};

  ValStack.push(value);

  return value;
}

llvm::Value *RNode::codegen()
{
  auto *CallP = CUR_MODULE->getFunction("__pcl_scan");

  return BUILDER->CreateCall(CallP);
}

//////////////END OF SCOPE METHODS ////////////////////////////////

} // namespace AST
