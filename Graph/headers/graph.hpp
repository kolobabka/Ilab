#ifndef __GRAPH_HPP__
#define __GRAPH_HPP__

#include <vector>
#include <iostream>


namespace Combinatorics {

    class IGraph { 

        virtual void dump () = 0;

    public:
        ~IGraph () = default;
    };

    class KnuthsGraph : public IGraph {

        std::vector<int> graph_;

    public: 
        void dump () override {}
        KnuthsGraph (const std::vector<int> &graph) {

            size_t size = graph.size();
            std::cout << "size: " << graph.size() << std::endl;
            size_t i = 0;
            for (i; i < size / 3; ++i)
                graph_.push_back (0);
            
            for (size_t j = i; j <= size + 1; j += 3) {

                graph_.push_back (graph[j - i]);
                graph_.push_back (graph[j - i + 1]);
            }

            std::cout << "a: ";
            for (auto v : graph_)
                std::cout << v << " ";
            std::cout << std::endl;
        }

        ~KnuthsGraph () = default;
    };
}

#endif