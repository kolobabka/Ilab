#pragma once

#include <fstream>
#include <iostream>

#include "parser.hpp"

class Frontend {

    int argc_ = 0;
    char** argv_ = nullptr; 
    std::fstream code_;
    std::string msg_;

public:
    Frontend (const int argc = 0, char **argv_ = nullptr);

    std::unique_ptr<Tree::NAryTree<AST::Node*>> build_ast ();
};