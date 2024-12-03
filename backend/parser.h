#include "lex.cc"
#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
using namespace llvm;
//===----------------------------------------------------------------------===//
// Abstract Syntax Tree (aka Parse Tree)
//===----------------------------------------------------------------------===//

// 使用面向对象的类封装节点
// ExprAST - Base cass for all expression nodes
class ExprAST {
    public:
    virtual ~ExprAST() = default;
    virtual Value* codegen() = 0; // 生成IR,返回一个Value*指针

};

// Number ExprAST - Expression class for numeric literals like '1.0'
class NumberExprAST :public ExprAST {
    double Val;
    public:
    NumberExprAST(double _val) :Val(_val) {}
    Value* codegen() override;

};
// VariableExprAST - Expression class for varialbes like 'a'
class VariableExprAST : public ExprAST {
    std::string Name;
    public:
    VariableExprAST(const std::string& Name) : Name(Name) {}
    Value* codegen() override;
};

// Value*   

// BinaryExprAST - Expression class for a binary operator
class BinaryExprAST :public ExprAST {
    char Op;
    std::unique_ptr<ExprAST>LHS, RHS;
    public:
    BinaryExprAST(char op,
        std::unique_ptr<ExprAST> lhs,
        std::unique_ptr<ExprAST> rhs
    ) :Op(op), LHS(std::move(lhs)), RHS(std::move(rhs)) {}
    // 使用move转移unique_ptr的使用权，unique_ptr本身是独占的禁止拷贝构造
    Value* codegen() override;
    // 这里没有写析构函数的原因是unique_ptr是自动析构

};

// CallExprAST - Expression class for function calls
class CallExprAST :public ExprAST {
    std::string Callee;
    std::vector<std::unique_ptr<ExprAST>>Args;
    public:
    CallExprAST(const std::string& Callee, std::vector<std::unique_ptr<ExprAST>>Args) :
        Callee(Callee), Args(std::move(Args)) {}
    Value* codegen() override;
};

// PrototypeAST - This class represents the "prototype" for a function,
class PrototypeAST {
    std::string Name;
    std::vector<std::string> Args;
    public:
    PrototypeAST(const std::string& Name, std::vector<std::string> Args) :Name(Name), Args(std::move(Args)) {}
    Function* codegen();
    const std::string& getName() const { return Name; }
};

// FunctionAST - This class represents a function definition itself 
class FunctionAST {
    std::unique_ptr<PrototypeAST>Proto;
    std::unique_ptr <ExprAST> Body;

    public:
    FunctionAST(std::unique_ptr<PrototypeAST> Proto, std::unique_ptr<ExprAST>Body) :
        Proto(std::move(Proto)), Body(std::move(Body)) {}
    Function* codegen();

};
