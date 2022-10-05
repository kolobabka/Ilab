#pragma once

#include <string>
#include <memory>
#include <exception>
#include <unordered_map>
#include <deque>
#include <stack>
#include <iostream> //TODO:Remove it

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

#include "common/n_ary_tree.hpp"

class Codegen final {

    llvm::LLVMContext Context_; //static?
    llvm::IRBuilder<> Builder_;
    llvm::Module Module_;
    llvm::Function* Function_;

    std::unordered_map<std::string, llvm::Value*> names_;

public:
    Codegen(std::string Name) : Builder_(Context_),
                                Module_(Name, Context_) {}

    void StartFunction(std::string Name) {
        // single __pcl_start function for void module
        llvm::FunctionType *FT = llvm::FunctionType::get(
            llvm::Type::getVoidTy(Context_), /* va args? */ false);

        Function_ = llvm::Function::Create(FT, llvm::Function::ExternalLinkage, Name,
                                            Module_);

        // basic block to start instruction insertion
        llvm::BasicBlock *BB = llvm::BasicBlock::Create(Context_, "entry", Function_);
        Builder_.SetInsertPoint(BB);
    }

    using WhileBlocksTy = std::pair<llvm::BasicBlock *, llvm::BasicBlock *>;

    WhileBlocksTy StartWhile(llvm::Value *CondV) {
        llvm::BasicBlock *BodyBB = llvm::BasicBlock::Create(Context_, "body");
        llvm::BasicBlock *MergeBB = llvm::BasicBlock::Create(Context_, "endwhile");
        Builder_.CreateCondBr(CondV, BodyBB, MergeBB);
        Function_->getBasicBlockList().push_back(BodyBB);
        Builder_.SetInsertPoint(BodyBB);
        return std::make_pair(BodyBB, MergeBB);
    }

    void EndWhile(llvm::Value *CondV, WhileBlocksTy BBs) {
        // assume we are now inside body
        Function_->getBasicBlockList().push_back(BBs.second);
        Builder_.CreateCondBr(CondV, BBs.first, BBs.second);
        Builder_.SetInsertPoint(BBs.second);
    }

    llvm::BasicBlock *StartIf(llvm::Value *CondV) {
        llvm::BasicBlock *ThenBB = llvm::BasicBlock::Create(Context_, "then");
        llvm::BasicBlock *MergeBB = llvm::BasicBlock::Create(Context_, "endif");
        Builder_.CreateCondBr(CondV, ThenBB, MergeBB);
        Function_->getBasicBlockList().push_back(ThenBB);
        Builder_.SetInsertPoint(ThenBB);
        return MergeBB;
    }

    void EndIf(llvm::BasicBlock *MergeBB) {
        // assume we are now in ThenBB
        Function_->getBasicBlockList().push_back(MergeBB);
        Builder_.CreateBr(MergeBB);
        Builder_.SetInsertPoint(MergeBB);
    }

    void EndCurrentFunction() { Builder_.CreateRetVoid(); }

    void SaveModule(std::string ModuleName) {
        std::error_code EC;
        llvm::raw_fd_ostream Outfile{ModuleName, EC};
        if (EC)
            llvm::errs() << EC.message().c_str() << "\n";

        Module_.print(Outfile, nullptr);
        Outfile.close();

        if (Outfile.has_error()) {
            std::cerr << Outfile.error().message() << std::endl;
            throw std::runtime_error("Error printing to file: ");
        }
    }

    llvm::Type *getInt32Ty() { return llvm::Type::getInt32Ty(Context_); }

    llvm::Type *getVoidTy() { return llvm::Type::getVoidTy(Context_); }

    void createFnDecl(llvm::FunctionType *FT, std::string Name) {
        llvm::Function::Create(FT, llvm::Function::ExternalLinkage, Name, Module_);
    }

    llvm::Value *addConstantInt (int val) {
        auto *Ty = getInt32Ty();
        return llvm::ConstantInt::get(Ty,val);
    }

    void addDecl (std::string varname) {

        auto &&BB = Function_->getEntryBlock();
        auto Ty = llvm::Type::getInt32Ty(Context_);
        llvm::IRBuilder<> TmpB(&BB, BB.begin());
        auto *Alloca = TmpB.CreateAlloca(Ty, 0, varname.c_str());
        names_[varname] = Alloca;
    }

    llvm::Value *AddDeclRead(std::string Varname) {
        return Builder_.CreateLoad(getInt32Ty(), names_[Varname], Varname.c_str());
    }

    llvm::Value *genAssign(std::string varname, llvm::Value *rhs) {
        std::cout << "names var: " << names_[varname] << std::endl;
        return Builder_.CreateStore(rhs, names_[varname]);
    }

    llvm::Value *genAdd(llvm::Value* first, llvm::Value* second) {
        return Builder_.CreateAdd(first, second);
    }

    llvm::Value *genSub(llvm::Value* first, llvm::Value* second) {
        return Builder_.CreateSub(first, second);
    }

    llvm::Value *genMul(llvm::Value* first, llvm::Value* second) {
        return Builder_.CreateMul(first, second);
    }

    llvm::Value *genEq(llvm::Value* first, llvm::Value* second) {
        return Builder_.CreateICmpEQ(first, second);
    }

    llvm::Value *genNeq(llvm::Value* first, llvm::Value* second) {
        return Builder_.CreateICmpNE(first, second);
    }
    
    llvm::Value *genLess(llvm::Value* first, llvm::Value* second) {
        return Builder_.CreateICmpSLT(first, second);
    }

    llvm::Value *genMore(llvm::Value* first, llvm::Value* second) {
        return Builder_.CreateICmpSGT(first, second);
    }

    llvm::Value *genLTE(llvm::Value* first, llvm::Value* second) {
        return Builder_.CreateICmpSLE(first, second);
    }

    llvm::Value *genGTE(llvm::Value* first, llvm::Value* second) {
        return Builder_.CreateICmpSGE(first, second);
    }


    llvm::Value *printVar (llvm::Value* RightV) {
        auto *CalleeF = Module_.getFunction("__pcl_print");
        assert(CalleeF && "Driver shall create decl for __pcl_print");
        assert(RightV && "Print required non-null right arg");
        llvm::Value *ArgsV[] = {RightV};
        return Builder_.CreateCall(CalleeF, ArgsV);
    }
    llvm::Value *scanVar () {
        auto *CalleeF = Module_.getFunction("__pcl_scan");
        assert(CalleeF && "Driver shall create decl for __pcl_scan");
        return Builder_.CreateCall(CalleeF);
    }

    static Codegen* createCodeGen(std::string Name) {
        // Codegen *Gen = new Codegen(Name);
        Codegen* Gen{new Codegen{Name}};
        

        // prototype for print and scan functions
        llvm::Type *Tys[] = {Gen->getInt32Ty()};
        llvm::FunctionType *FTPrint =
        llvm::FunctionType::get(Gen->getVoidTy(), Tys, /* va args? */ false);
        llvm::FunctionType *FTScan =
        llvm::FunctionType::get(Gen->getInt32Ty(), /* va args? */ false);

        // creating decls for modules
        Gen->createFnDecl(FTPrint, "__pcl_print");
        Gen->createFnDecl(FTScan, "__pcl_scan");
        return Gen;
    }

    struct GetType : public AST::Visitor {

        Codegen *gen_;
        GetType (Codegen *gen) : gen_(gen) {}
        llvm::Value* visit(AST::ScopeNode *ref) override {
            std::cout << "Scope Node: ";
            ref->nodeDump(std::cout);
            std::cout << std::endl;
            
            auto beg = ref->rChildBegin();
            auto end = ref->rChildEnd();

            while (beg != end) {
                (*beg)->accept(*this);
                ++beg;
            }
            return nullptr;
        }
        llvm::Value* visit(AST::VarNode *ref) override {
            std::cout << "Var Node: ";
            ref->nodeDump(std::cout);
            std::cout << std::endl;
            return gen_->AddDeclRead(ref->getName());
        }
        llvm::Value* visit(AST::NumNode *ref) override {
            std::cout << "Num Node: ";
            ref->nodeDump(std::cout);
            std::cout << std::endl;
            return gen_->addConstantInt (ref->getValue());
        }
        llvm::Value* visit(AST::OperNode *ref) override {
            std::cout << "OperNode Node: ";
            ref->nodeDump(std::cout);
            std::cout << std::endl;
            if (ref->getOpType() == AST::OperNode::OperType::ASSIGN)
            {
                auto *varNode = static_cast<AST::VarNode*>(ref->getLeftChild());
                if (!gen_->names_[varNode->getName()])
                    gen_->addDecl(varNode->getName());
                if (ref->getRightChild()->getType() == AST::NodeT::OPERATOR) {
                    auto *rhsOperNode = static_cast<AST::OperNode*>(ref->getRightChild());
                    if (rhsOperNode->getOpType() == AST::OperNode::OperType::ASSIGN) {
                        auto *storeInstBase = rhsOperNode->accept(*this);
                        auto *storeInst = static_cast<llvm::StoreInst*>(storeInstBase);
                        return gen_->genAssign (varNode->getName(), storeInst->getValueOperand());
                    }
                }
                return gen_->genAssign (varNode->getName(), ref->getRightChild()->accept(*this));
            }
            if (ref->getOpType() == AST::OperNode::OperType::PRINT) 
            {
                return gen_->printVar (ref->getLeftChild()->accept(*this));
            }
            if (ref->getOpType() == AST::OperNode::OperType::SCAN) 
            {
                return gen_->scanVar ();
            }
            if (ref->getOpType() == AST::OperNode::OperType::ADD) 
            {
                return gen_->genAdd (ref->getLeftChild()->accept(*this), ref->getRightChild()->accept(*this));
            }
            if (ref->getOpType() == AST::OperNode::OperType::SUB) 
            {
                return gen_->genSub (ref->getLeftChild()->accept(*this), ref->getRightChild()->accept(*this));
            }
            if (ref->getOpType() == AST::OperNode::OperType::MUL) 
            {
                return gen_->genMul (ref->getLeftChild()->accept(*this), ref->getRightChild()->accept(*this));
            }
            if (ref->getOpType() == AST::OperNode::OperType::EQ) 
            {
                return gen_->genEq(ref->getLeftChild()->accept(*this), ref->getRightChild()->accept(*this));
            }
            if (ref->getOpType() == AST::OperNode::OperType::NEQ) 
            {
                return gen_->genNeq(ref->getLeftChild()->accept(*this), ref->getRightChild()->accept(*this));
            }
            if (ref->getOpType() == AST::OperNode::OperType::LESS) 
            {
                return gen_->genLess(ref->getLeftChild()->accept(*this), ref->getRightChild()->accept(*this));
            }
            if (ref->getOpType() == AST::OperNode::OperType::MORE) 
            {
                return gen_->genMore(ref->getLeftChild()->accept(*this), ref->getRightChild()->accept(*this));
            }
            if (ref->getOpType() == AST::OperNode::OperType::LTE) 
            {
                return gen_->genLTE(ref->getLeftChild()->accept(*this), ref->getRightChild()->accept(*this));
            }
            if (ref->getOpType() == AST::OperNode::OperType::GTE) 
            {
                return gen_->genGTE(ref->getLeftChild()->accept(*this), ref->getRightChild()->accept(*this));
            }
            throw std::runtime_error ("Unknown operation node");
        }
        llvm::Value* visit(AST::CondNode *ref) override {
            std::cout << "Cond Node: ";
            ref->nodeDump(std::cout);
            std::cout << std::endl;

            auto *condVal = ref->getLeftChild()->accept(*this);

            if (ref->getConditionType() == AST::CondNode::ConditionType::IF) 
            {
                auto *MergeBB = gen_->StartIf(condVal);
                gen_->EndIf(MergeBB);
                auto *scopeVal = ref->getRightChild()->accept(*this);
            }
            if (ref->getConditionType() == AST::CondNode::ConditionType::WHILE)
            {
                auto BBs = gen_->StartWhile(condVal);
                auto *scopeVal = ref->getRightChild()->accept(*this);
                llvm::Value *condValNew = ref->getLeftChild()->accept(*this);
                gen_->EndWhile(condValNew, BBs);
            }
            return nullptr;   
        }
    };

    template <typename ASTNode_T> //TODO: Bad function -> make it better 
    void codegen (std::unique_ptr<Tree::NAryTree<ASTNode_T*>>& tree) {

        GetType getType{this};
        // tree->getRoot()->accept(this);

        ASTNode_T* root = tree->getRoot();
        root->accept(getType);
    }
};