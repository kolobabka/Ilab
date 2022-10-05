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
#include <map>
#include <stdexcept>
#include <iterator>
#include <memory>

#include "customLexer.hpp"

namespace Combinatorics {

using VecIt      = typename std::vector<int>::iterator;
using InitListIt = typename std::initializer_list<std::pair<int, int>>::iterator;

// using VecIt = typename std::vector<int>::iterator;

// class IGraph { 

//     virtual void dump () = 0;
// public:
//     virtual ~IGraph () = default;
// };

template <typename VLType = int, typename ELType = int>
class KnuthsGraph {
    
    size_t capVertex_;
    size_t numVertex_;
    size_t numEdges_; 
    std::vector<int> graph_;
    std::vector<VLType> VLinfo_;
    std::vector<ELType> ELinfo_;

    template <typename Iter>
    auto countUniqueVertex (Iter start, Iter fin) -> std::vector<typename Iter::value_type::first_type> {

        std::unordered_set<int> set;

        std::for_each (start, fin, [&set] (auto pair) {set.insert(pair.first); set.insert(pair.second);});
        return std::vector<typename Iter::value_type::first_type>(set.begin(), set.end());
    }

public: 
    //template <typename VertContainer>
    KnuthsGraph (const VertContainer &vertList, std::vector<VLType> && VLinfo = {}, std::vector<ELType> && ELinfo = {}) {

        std::cout << "vertList:" << std::endl;

        for (auto v : vertList)
            std::cout << v.first << ", " <<  v.second << std::endl;

        std::cout << "VLInfo" << std::endl;

        for (auto v : VLinfo)
            std::cout << v << std::endl;

        std::cout << "ELInfo" << std::endl;

        for (auto v : ELinfo)
            std::cout << v << std::endl;

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

        if (!VLinfo.empty() && VLinfo.size() != numVertex) 
            throw VLinfo.size() < numVertex ? std::logic_error ("Not all the verticies have additional information") : 
                                              std::logic_error ("Too many attributes for verticies!");
        
        if (!ELinfo.empty() && ELinfo.size() != vertList.size()) 
            throw ELinfo.size() < numVertex ? std::logic_error ("Not all the edges have additional information") : 
                                              std::logic_error ("Too many attributes for edges!");
         
    //  ---------------------------------------------------------------------- The Kalb's line

        graph_ = std::move(tmpGraph);
        VLinfo_ = std::move (VLinfo);
        ELinfo_ = std::move (ELinfo);
        capVertex_ = capVertex;
        numVertex_ = numVertex;
        numEdges_  = numEdges;
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

            // std::fill (graph_.begin(), std::next (graph_.begin(), numVertex_), 0);
        }

        return std::make_pair (true, colors);
    }

    void dump () {

        size_t i = 0, size = capVertex_ + numEdges_;

        size_t offset = 11;

        std::string eq;
        for (size_t j = 0; j < offset + 3; ++j)
            eq = eq + "=";

        std::cout << "| A |";
        for (size_t j = 0; j < size; ++j) 
            std::cout << std::setw(offset) << j << " | ";
        std::cout << std::endl;

        std::cout << "  ";

        for (size_t j = 0; j < size; ++j) 
            std::cout << std::setw(offset) << eq;
        std::cout << std::endl;

        std::cout << "| T |";
        for (i; i < size; ++i)
            std::cout << std::setw(offset) << graph_[i] << " | ";
        std::cout << std::endl; 

        std::cout << "  ";

        for (size_t j = 0; j < size; ++j) 
            std::cout << std::setw(offset) << "======";
        std::cout << std::endl;

        std::cout << "| N |";
        for (i; i < size * 2; ++i)
            std::cout << std::setw(offset) << graph_[i] << " | ";
        std::cout << std::endl;

        std::cout << "  ";

        for (size_t j = 0; j < size; ++j) 
            std::cout << std::setw(offset) << "======";
        std::cout << std::endl;

        std::cout << "| P |";
        for (i; i < size * 3; ++i)
            std::cout << std::setw(offset) << graph_[i] << " | ";
        std::cout << std::endl;

        std::cout << "  ";

        for (size_t j = 0; j < size; ++j) 
            std::cout << std::setw(offset) << "======";
        std::cout << std::endl;

        std::cout << "| VL|";
        for (i = 0; i < numVertex_; ++i)
            std::cout << std::setw(offset) << VLinfo_[i] << " | ";
        for (i; i < size; ++i)
            std::cout << std::setw(offset) << 0 << " | ";
        std::cout << std::endl;

        std::cout << "  ";

        for (size_t j = 0; j < size; ++j) 
            std::cout << std::setw(offset) << "======";
        std::cout << std::endl;

        std::cout << "| EL|";
        for (i = 0; i < capVertex_; ++i)
            std::cout << std::setw(offset) << 0 << " | ";
        for (i = 0; i < numEdges_ / 2; ++i)
            std::cout << std::setw(offset) << ELinfo_[i] << " | " << std::setw(offset) << ELinfo_[i] << " | ";
        std::cout << std::endl;
        
        std::cout << "  ";

        for (size_t j = 0; j < size; ++j) 
            std::cout << std::setw(offset) << "======";
        std::cout << std::endl;
    }

    ~KnuthsGraph () = default;
};

// template <typename dataType>
std::map<std::string, size_t> numerating_data (const std::vector<std::string> &data) {

    std::map<std::string, size_t> map;

    for (size_t i = 1, j = 0; auto v : data) {

        if ((j % 3) == 2) {
            ++j;
            continue;
        }
        //std::cout << "data[ " << j << "]: " << data[j] << ", i = " << i << std::endl; 
        if (map.find(v) == map.end())
            map.emplace (v, i++);
        ++j;
    }

    for (auto v : map)
        std::cout << "first " << v.first << ", sec " << v.second << std::endl; 

    std::cout << "SIZE: " << map.size() << std::endl;
    return map;
}

template< typename tPair >
struct first_t {
    typename tPair::first_type operator()( const tPair& p ) const { return p.first; }
};

template< typename tMap > 
first_t< typename tMap::value_type > first( const tMap& m ) { return first_t< typename tMap::value_type >(); }

KnuthsGraph<std::string, std::string> readGraph () {

    auto graphLexer = std::make_unique<GraphLexer>();
    std::cout << "Handle the graph's represenatation..." << std::endl;
    while(graphLexer->yylex() != 0) {}

    auto& data = graphLexer->data_;
    auto map = numerating_data(data);

    std::vector<std::pair<int, int>> edges;
    std::vector<std::string> weight;

    for (size_t i = 0; i < data.size(); ++i) {
        edges.emplace_back(map[data[i]], map[data[i + 1]]);
        weight.emplace_back(data[i + 2]);
        i += 2;
    }

    std::vector<std::string> verts;

    std::transform (map.begin(), map.end(), std::back_inserter(verts), first(map));

    KnuthsGraph<std::string, std::string> tmpGraph(edges, std::move(verts), std::move(weight));
    return tmpGraph;
} 
}

#endif