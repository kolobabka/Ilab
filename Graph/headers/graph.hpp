#ifndef __GRAPH_HPP__
#define __GRAPH_HPP__

#include <queue>
#include <vector>
#include <list>
#include <iostream>
#include <bit>
#include <algorithm>
#include <cstring>
#include <numeric>
#include <unordered_set>
#include <cmath>
#include <initializer_list>
#include <iomanip>
#include <iterator>

namespace Combinatorics {

using VecIt      = typename std::vector<int>::iterator;
using InitListIt = typename std::initializer_list<std::pair<int, int>>::iterator;

// using VecIt = typename std::vector<int>::iterator;

class IGraph { 

    virtual void dump () = 0;
public:
    virtual ~IGraph () = default;
};

class KnuthsGraph : public IGraph {
    
    size_t capVertex_;
    size_t numVertex_;
    size_t numEdges_; 
    std::vector<int> graph_;

    size_t countNumOfUniqueVertex (VecIt start, VecIt end) {

        std::unordered_set<int> set{start, end};
        return set.size();
    }

    std::vector<int> countUniqueVertex (InitListIt start, InitListIt fin) {

        std::unordered_set<int> set;

        std::for_each (start, fin, [&set] (auto pair) {set.insert(pair.first); set.insert(pair.second);});
        return std::vector<int>(set.begin(), set.end());
    }

public: 
    KnuthsGraph (const std::initializer_list<std::pair<int, int>> &vertList) {

        std::vector<int> uniqueVertex = countUniqueVertex (vertList.begin(), vertList.end());
        size_t numVertex  = uniqueVertex.size();

        size_t capVertex  = std::popcount(numVertex) == 1 ? numVertex : 
                            pow (2, static_cast<size_t>(log2(numVertex)) + 1);
        size_t numEdges   = vertList.size() * 2; // mult 2 because one edge --> two vertices
        size_t size       = capVertex + numEdges;

        std::vector<int> tmpGraph (size * 3); // 3 lines in the graph

        std::fill (tmpGraph.begin(), std::next (tmpGraph.begin(), capVertex), 0);

        std::for_each (vertList.begin(), vertList.end(), 
                       [&tmpGraph, offset = capVertex] (auto pair) mutable {

                            tmpGraph[offset++] = pair.first;
                            tmpGraph[offset++] = pair.second;
                        });

        for (size_t i = 0; i < numVertex; ++i) {
            
            size_t prevId = i;

            for (size_t j = 0; j < size; ++j) {    
                if (tmpGraph[j] == i + 1) {

                    tmpGraph[size + prevId] = j;
                    tmpGraph[2 * size + j] = prevId;
                    prevId = j;
                }
            }
            tmpGraph[size + prevId] = i;
            tmpGraph[2 * size + i] = prevId;
        }   

    //  ---------------------------------------------------------------------- The Kalb's line

        graph_ = std::move(tmpGraph);
        capVertex_ = capVertex;
        numVertex_ = numVertex;
        numEdges_  = numEdges;
    }

    // KnuthsGraph (const std::initializer_list<std::pair<int, int>> &vertList, const std::vector<ELType> &ELinfo) :
    //              KnuthsGraph (vertList), 

    KnuthsGraph (const std::vector<int> &graph) {
        
        using cVecIt = typename std::vector<int>::const_iterator;

        std::vector<int> vertexBuf;
        std::copy_if (graph.begin(), graph.end(), std::back_inserter(vertexBuf), [i = 1] (int x) mutable {return (i++) % 3 != 0;});

        numVertex_ = countNumOfUniqueVertex(vertexBuf.begin(), vertexBuf.end()); //non exception-safety
        
        capVertex_ = std::popcount(numVertex_) == 1 ? numVertex_ : 
                     pow (2, static_cast<size_t>(log2(numVertex_)) + 1); 

        numEdges_  = (graph.size() * 2 / 3); //not safe 
        size_t size = capVertex_ + numEdges_; 

        std::vector<int> tmpGraph(size * 3);            
        
        std::fill (tmpGraph.begin(), tmpGraph.begin() + capVertex_, 0);
        std::copy (vertexBuf.begin(), vertexBuf.end(), tmpGraph.begin() + capVertex_);

        for (size_t i = 0; i < numVertex_; ++i) {
            
            size_t prevId = i;

            for (size_t j = 0; j < size; ++j) {    
                if (tmpGraph[j] == i + 1) {

                    tmpGraph[size + prevId] = j;
                    tmpGraph[2 * size + j] = prevId;
                    prevId = j;
                }
            }
                tmpGraph[size + prevId] = i;
                tmpGraph[2 * size + i] = prevId;
        }   

        graph_.reserve (size * 3);
        graph_ = std::move(tmpGraph);
    }

    std::pair<bool, std::vector<int>> checkBipartite () {
        
        std::vector<int> colors(numVertex_, 0);
        size_t size = capVertex_ + numEdges_;
        colors[0] = 2;

        for (size_t i = 0; i < numVertex_; i++) {

            std::list<int> queue;
            queue.push_back (i + 1);

            while (!(queue.empty())) {
            
                size_t curVert = queue.back() - 1;
                queue.pop_back();
                
                if (graph_[curVert])
                    continue;

                graph_[curVert] = 1;
                size_t curEdge = graph_[size + curVert];

                while (curEdge != curVert) {
                    
                    size_t pairEdge = curEdge ^ 1;
                    size_t pairVert = graph_[pairEdge] - 1; 

                    if (colors[pairVert] == colors[curVert]) 
                        return std::make_pair (false, std::vector<int>{});
                    
                    if (graph_[pairVert]) {

                        curEdge = graph_[size + curEdge];
                        continue;
                    }
                
                    colors[pairVert] = colors[curVert] ^ 1;
                    
                    queue.push_back(pairVert + 1);
                    curEdge = graph_[size + curEdge];
                }
            }
        }

        return std::make_pair (true, colors);
    }

    void dump () override {

        size_t i = 0, size = capVertex_ + numEdges_;

        std::cout << "| A |";
        for (size_t j = 0; j < size; ++j) 
            std::cout << std::setw(3) << j << " | ";
        std::cout << std::endl;

        std::cout << "  ";

        for (size_t j = 0; j < size; ++j) 
            std::cout << std::setw(3) << "======";
        std::cout << std::endl;

        std::cout << "| T |";
        for (i; i < size; ++i)
            std::cout << std::setw(3) << graph_[i] << " | ";
        std::cout << std::endl; 

        std::cout << "  ";

        for (size_t j = 0; j < size; ++j) 
            std::cout << std::setw(4) << "======";
        std::cout << std::endl;

        std::cout << "| N |";
        for (i; i < size * 2; ++i)
            std::cout << std::setw(3) << graph_[i] << " | ";
        std::cout << std::endl;

        std::cout << "  ";

        for (size_t j = 0; j < size; ++j) 
            std::cout << std::setw(4) << "======";
        std::cout << std::endl;

        std::cout << "| P |";
        for (i; i < size * 3; ++i)
            std::cout << std::setw(3) << graph_[i] << " | ";
        std::cout << std::endl;

        std::cout << "  ";

        for (size_t j = 0; j < size; ++j) 
            std::cout << std::setw(4) << "======";
        std::cout << std::endl;
    }

    ~KnuthsGraph () = default;
};
}

#endif