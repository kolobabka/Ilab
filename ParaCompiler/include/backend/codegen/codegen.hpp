#pragma once

#include <string>
#include <memory>
#include <exception>
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

    llvm::Value *AddDeclRead(std::string Varname) {
        return Builder_.CreateLoad(getInt32Ty(), addConstantInt(42), Varname.c_str());
    }

    llvm::Value *printVar (llvm::Value* RightV) {
        auto *CalleeF = Module_.getFunction("__pcl_print");
        assert(CalleeF && "Driver shall create decl for __pcl_print");
        assert(RightV && "Print required non-null right arg");
        llvm::Value *ArgsV[] = {RightV};
        return Builder_.CreateCall(CalleeF, ArgsV);
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
        llvm::Value* visit(AST::VarNode *ref) override {
            std::cout << "Var Node: ";
            ref->nodeDump(std::cout);
            std::cout << std::endl;
            //gen_->AddDeclRead(ref->getName());d
        }
        llvm::Value* visit(AST::NumNode *ref) override {
            std::cout << "Num Node: ";
            ref->nodeDump(std::cout);
            std::cout << std::endl;
            gen_->addConstantInt (ref->getValue());
        }
        llvm::Value* visit(AST::ScopeNode *ref) override {
            std::cout << "Scope Node: ";
            ref->nodeDump(std::cout);
            std::cout << std::endl;
        }
        llvm::Value* visit(AST::OperNode *ref) override {
            std::cout << "OperNode Node: ";
            ref->nodeDump(std::cout);
            std::cout << std::endl;
            if (ref->getOpType() == AST::OperNode::OperType::PRINT) 
            {
                return gen_->printVar (ref->getLeftChild()->accept(*this));
            }
            // if (ref->getOpType() == AST::OperNode::OperType::ASSIGN) 
            // {
            //     gen_->assignment (30);
            // }
            // if (ref->getOpType() == AST::OperNode::OperType::ADD) 
            // {
            //     gen_->assignment (30);
            // }
        }
        llvm::Value* visit(AST::CondNode *ref) override {
            std::cout << "Cond Node: ";
            ref->nodeDump(std::cout);
            std::cout << std::endl;
        }
    };

    template <typename ASTNode_T> //TODO: Bad function -> make it better 
    void codegen (std::unique_ptr<Tree::NAryTree<ASTNode_T*>>& tree) {

        GetType getType{this};

        ASTNode_T* root = tree->getRoot();
        if (!root)
            return;
        ASTNode_T* curNode = root;

        std::stack<ASTNode_T*> stack;
        std::vector<ASTNode_T*> queueOnCodegen;
        stack.push (curNode);

        while (stack.size ()) {
            curNode = stack.top ();
            stack.pop ();
            queueOnCodegen.push_back (curNode);

            auto childrenSt = curNode->childBegin ();
            auto childrenFin = curNode->childEnd ();

            while (childrenSt != childrenFin) {
                stack.push (*childrenSt);
                childrenSt = std::next (childrenSt, 1);
            }
        }

        for (size_t i = 0; i < queueOnCodegen.size (); ++i) {
            queueOnCodegen[i]->accept(getType); 
        }
    }
};