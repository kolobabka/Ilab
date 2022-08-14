#include "graph.hpp"
#include <FlexLexer.h>
#include <iostream>
#include <string>
#include <iterator>
#include <algorithm>
#include <memory>

#include "customLexer.hpp"


int main () {

    std::cout << "\t---Hello, graph!" << std::endl;

    auto lexer = std::make_unique<GraphLexer>();
    std::cout << "Handle the graph's represenatation..." << std::endl;
    while(lexer->yylex() != 0) {
        
    }

    Combinatorics::KnuthsGraph graph{lexer->nums_}; 
}