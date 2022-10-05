#pragma once
#include <iostream>
#include <string>
#include <vector>

#include "common/locator.hpp"

#include "llvm/IR/Value.h"

//*****************************************************************************
//************************* AST NODE DESCRIPTION HERE *************************
//*****************************************************************************
namespace AST {

    class Node;
    class VarNode;
    class NumNode;
    class ScopeNode;
    class OperNode;
    class CondNode;

    struct Visitor {
        // virtual llvm::Value* visit (Node *node) = 0;
        using ret_type = typename llvm::Value*;
        virtual llvm::Value* visit (VarNode *node) = 0;
        virtual llvm::Value* visit (NumNode *node) = 0;
        virtual llvm::Value* visit (OperNode *node) = 0;
        virtual llvm::Value* visit (CondNode *node) = 0;
        virtual llvm::Value* visit (ScopeNode *node) = 0;

        virtual ~Visitor() = default;
    };

    enum class NodeT {
        VARIABLE,
        NUMBER,
        OPERATOR,

        CONDITION,

        SCOPE,
    };

    class Node : public Locator {
        Node *parent_;
        std::vector<Node *> children_;
        const NodeT type_;

    public:
        Node (const NodeT type, Node *parent = nullptr) : type_ (type), parent_ (parent) {}

        Node (const NodeT type, yy::location loc, Node *parent = nullptr)
            : Locator (loc), type_ (type), parent_ (parent)
        {
        }

        virtual ~Node () = default;

        Node *operator[] (int num) { return children_[num]; }
        const Node *operator[] (int num) const { return children_[num]; }
        // Let's implement 0-rule
        Node (const Node &other) = delete;
        Node (Node &&other) = delete;
        Node &operator= (const Node &other) = delete;
        Node &operator= (Node &&other) = delete;

        virtual Visitor::ret_type accept(Visitor &v) = 0;

        // If you'd like to inherit from class Node you have to write NodeDump () func;
        virtual void nodeDump (std::ostream &out) const = 0;

        // Setters and getters

        NodeT getType () const { return type_; }

        template <typename It>
        void setChildren (It start, It fin)
        {
            children_.assign (start, fin);
        }

        std::vector<Node *>::const_iterator childBegin () const { return children_.cbegin (); }
        std::vector<Node *>::const_iterator childEnd () const { return children_.cend (); }
        std::vector<Node *>::reverse_iterator rChildBegin () { return children_.rbegin (); }
        std::vector<Node *>::reverse_iterator rChildEnd () { return children_.rend (); }

        Node *getLeftChild () const;

        Node *getRightChild () const;

        size_t getChildrenNum () const { return children_.size (); }

        void setParent (Node *parent) { parent_ = parent; }

        Node *getParent () const { return parent_; }

        void addChild (Node *child);

        std::vector<Node *>::iterator eraseChild (const int ind)
        {  //|!| THIS FUNCTION DOESN'T CHANGE parent_ FOR CHILD NODE
            return children_.erase (children_.begin () + ind);
        }
    };
}  // namespace AST
//*****************************************************************************
//**************************** AST Node inheritors ****************************
//*****************************************************************************
namespace AST {

    class VarNode final : public Node {
        std::string name_;

    public:
        VarNode (const std::string &name, yy::location loc, Node *parent = nullptr)
            : Node (NodeT::VARIABLE, loc, parent), name_ (name)
        {
        }

        auto accept(Visitor &v) -> decltype (v.visit(this)) override{
            return v.visit(this);
        }

        void nodeDump (std::ostream &out) const override { out << name_; }

        std::string getName () const { return name_; }
    };

    class OperNode final : public Node {
    public:
        enum class OperType;

    private:
        OperType opType_;

    public:
        enum class OperType {
            ADD,  // a + b
            SUB,  // a - b
            MUL,  // a * b
            DIV,  // a / b
            MOD,  // a % b

            UNARY_M,  // -a
            UNARY_P,  // +a

            ASSIGN,  // a = b

            MORE,  // a > b
            LESS,  // a < b
            EQ,    // a == b
            NEQ,   // a != b
            GTE,   // a >= b
            LTE,   // a <= b

            AND,  // a && b
            OR,   // a || b

            SCAN,   // a = ?
            PRINT,  // print (a)

            RETURN,  // return ...
            CALL
        };

        OperNode (const OperType opType, yy::location loc, Node *parent = nullptr)
            : Node (NodeT::OPERATOR, loc, parent), opType_ (opType)
        {
        }

        auto accept(Visitor &v) -> decltype (v.visit(this)) override{
            return v.visit(this);
        }

        OperNode (const OperType opType, Node *parent = nullptr) : Node (NodeT::OPERATOR, parent), opType_ (opType) {}

        OperType getOpType () const { return opType_; }

        void nodeDump (std::ostream &out) const override;
    };

    class NumNode final : public Node {
        int value_;

    public:
        NumNode (const int value = 0, Node *parent = nullptr) : Node (NodeT::NUMBER, parent), value_ (value) {}

        auto accept(Visitor &v) -> decltype (v.visit(this)) override{
            return v.visit(this);
        }

        void nodeDump (std::ostream &out) const override { out << value_; }

        int getValue () const { return value_; }
    };

    class ScopeNode final : public Node {
    public:
        ScopeNode (Node *parent = nullptr) : Node (NodeT::SCOPE, parent) {}
        ScopeNode (yy::location loc, Node *parent = nullptr) : Node (NodeT::SCOPE, loc, parent) {}

        auto accept(Visitor &v) -> decltype (v.visit(this)) override{
            return v.visit(this);
        }

        void nodeDump (std::ostream &out) const override { out << "SCOPE"; }
    };

    class CondNode final : public Node {
    public:
        enum class ConditionType {
            IF,
            WHILE
        };

    private:
        ConditionType condType_;

    public:
        CondNode (const ConditionType condType, yy::location loc, Node *parent = nullptr)
            : Node (NodeT::CONDITION, loc, parent), condType_ (condType)
        {
        }

        ConditionType getConditionType () const { return condType_; }

        auto accept(Visitor &v) -> decltype (v.visit(this)) override{
            return v.visit(this);
        }

        void nodeDump (std::ostream &out) const override;
    };


}  // namespace AST