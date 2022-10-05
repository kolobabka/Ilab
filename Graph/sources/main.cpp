#include "graph.hpp"
#include <FlexLexer.h>
#include <iostream>
#include <string>
#include <iterator>
#include <algorithm>
#include <memory>

#include "customLexer.hpp"


int main () try {

    //std::cout << "\t---Hello, graph!" << std::endl;

    // auto graphLexer = std::make_unique<GraphLexer>();
    // //std::cout << "Handle the graph's represenatation..." << std::endl;
    // while(graphLexer->yylex() != 0) {
        
    // }

    // std::cout << graphLexer->data_.size() << std::endl;
    // for (auto v : graphLexer->data_) {

    //     std::cout << v << std::endl;
    // }

    Combinatorics::KnuthsGraph graph{{1,2}, {3,4}, {6,5}};
#if 0
    auto graph = Combinatorics::readGraph();
    graph.dump();
    //graph.checkBipartite();
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

    Combinatorics::KnuthsGraph<std::string, int> graph{{{1, 2}, {1, 3}, {2, 3}, {2, 4}, {3, 4}}, {"Tommy", "Bob", "Frank", "DennisRedis"}, {1, 4, 3, 6, 7}}; 
    graph.dump();
#endif
}
catch (std::exception &err) {

    std::cout << err.what() << std::endl;
}