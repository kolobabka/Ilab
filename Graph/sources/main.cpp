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

    Combinatorics::KnuthsGraph graph{{1, 2}, {1, 3}, {2, 3}, {2, 4}, {3, 4}}; 
    graph.dump();
#if 0
    graph.dump();
    auto res = graph.checkBipartite();
    if (res.first) {

        int blueColorBase = res.second[0];

        for (auto i = 1; auto color : res.second) {
            
            if (color == blueColorBase) 
                std::cout << i++ << " b ";
            else
                std::cout << i++ << " r ";
        }
        std::cout << std::endl;
    }
    else {
        
        std::cout << "Graph is not bipartite!" << std::endl;
    }
#endif
}