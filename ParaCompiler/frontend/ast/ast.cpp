#include "ast.hpp"

//*****************************************************************************
//************************* AST NODE DESCRIPTION HERE *************************
//*****************************************************************************
namespace AST {

    Node* Node::getLeftChild () const {

        size_t vecSize = children_.size ();
        if (vecSize == 0)
            return nullptr;

        return children_[0];
    }

    Node* Node::getRightChild () const {

        size_t vecSize = children_.size ();
        if (vecSize == 0)
            return nullptr;

        return children_[children_.size () - 1];
    }

    void Node::addChild (Node *child) {

        children_.push_back (child);
        child->setParent (this);
    }

    void OperNode::nodeDump (std::ostream &out) const {

        switch (opType_) {
            case OperType::ADD: out << "ADD (+)"; break;
            case OperType::SUB: out << "SUB (-)"; break;
            case OperType::UNARY_M: out << "UNARY_M (-)"; break;
            case OperType::UNARY_P: out << "UNARY_P (+)"; break;
            case OperType::MUL: out << "MUL (*)"; break;
            case OperType::DIV: out << "DIV (/)"; break;
            case OperType::ASSIGN: out << "ASSIGN (=)"; break;
            case OperType::MORE: out << "MORE (>)"; break;
            case OperType::LESS: out << "LESS (<)"; break;
            case OperType::EQ: out << "EQ (==)"; break;
            case OperType::NEQ: out << "NEQ (!=)"; break;
            case OperType::GTE: out << "GTE (>=)"; break;
            case OperType::LTE: out << "LTE (<=)"; break;
            case OperType::SCAN: out << "SCAN (?)"; break;
            case OperType::OR: out << "OR (||)"; break;
            case OperType::AND: out << "AND (&&)"; break;
            case OperType::MOD: out << "MOD (%)"; break;
            case OperType::PRINT: out << "PRINT [print ()]"; break;
            case OperType::RETURN: out << "RETURN"; break;
            default: out << "Unexpected operator type!";
        }
    }

    void CondNode::nodeDump (std::ostream &out) const {

        switch (condType_) {
            case ConditionType::WHILE: out << "WHILE"; break;
            case ConditionType::IF: out << "IF"; break;
            default: out << "Unexpected condition type!";
        }
    }
}  // namespace AST
