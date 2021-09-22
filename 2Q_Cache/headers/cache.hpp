#include <iostream>
#include <unordered_map>
#include <list>
#include <iterator>
#include <cassert>
//--------------------------------------------------------
//--------------------------------------------------------
enum whatList{

    IN,
    OUT,
    HOT
};
//--------------------------------------------------------
//--------------------------------------------------------
template <typename Data> struct ListNode {

    Data data;
    whatList place;
};
//--------------------------------------------------------
//--------------------------------------------------------
namespace caches {
    
    template <typename Key, typename Data>  
    struct QueueMap {

        const size_t sizeList;

        std::list<ListNode<Data>> List;

        QueueMap(const size_t size) : sizeList(size) {}

        bool IsFull () const {
            
            if (sizeList <= List.size ())
                return true;
            
            return false;
        }
    };
//--------------------------------------------------------
//--------------------------------------------------------
    template <typename Key, typename Data>  
    struct Cache_2Q {

        const size_t cacheSize;

        using ListIt = typename std::list<ListNode<Data>>::iterator;

        std::unordered_map<Key, ListIt> Map;

        Cache_2Q (const size_t size) : 
            cacheSize (size) {} 

        ~Cache_2Q () {}


        QueueMap<Key, Data> In {cacheSize - (3* (cacheSize / 5)) - (cacheSize / 5)};
        QueueMap<Key, Data> Out {3 * (cacheSize / 5)};
        QueueMap<Key, Data> Hot {cacheSize / 5};

        
//--------------------------------------------------------
//--------------------------------------------------------
        void InsertPage (ListNode<Data> node) {

            if (In.IsFull ()) {

                auto backList = Map.find(In.List.back().data);

                if (!Out.IsFull ()) {
                
                    backList->second->place = OUT;
                    Out.List.splice(Out.List.begin(), In.List, backList->second);

                }
                else {

                    if (Out.List.size () == 0) {

                        Map.erase (backList);
                        In.List.pop_back();
                        In.List.push_front (node);
                        Map.insert({node.data, In.List.begin()});

                        return;
                    }

                    auto backOut = Map.find(Out.List.back().data);
                    Map.erase(backOut);
                    Out.List.pop_back();

                    backList->second->place = OUT;
                    Out.List.splice(Out.List.begin(), In.List, backList->second);

                }

                In.List.push_front (node);
                Map.insert({node.data, In.List.begin()});

            }
            else {

                In.List.push_front (node);
                Map.insert({node.data, In.List.begin()});
            }

        }
//--------------------------------------------------------
//--------------------------------------------------------        
        void FindPage (typename std::unordered_map<Key, ListIt>::iterator page) {

            if (page->second->place == IN)     
                return;
                    
            if (page->second->place == OUT) {

                if (!Hot.IsFull ()) {

                    page->second->place = HOT;
                    Hot.List.splice(Hot.List.begin(), Out.List, page->second);

                    return;
                }
                else {

                    auto backHot = Map.find(Hot.List.back().data);
                    Map.erase(backHot);
                    Hot.List.pop_back();

                    page->second->place = HOT;
                    Hot.List.splice(Hot.List.begin(), Out.List, page->second);
 
                    return;
                }
            }
            if (page->second->place == HOT) {

                Hot.List.splice(Hot.List.begin(), Hot.List, page->second);
                return;
            }
        }
//--------------------------------------------------------
//--------------------------------------------------------
        int CacheHit (const size_t numPages) {

            int numHit = 0;

            for (size_t i = 0; i < numPages; i++) {

                Data data;
                std::cin >> data;
                assert (std::cin.good ());
                
                auto page = Map.find (data);

                if (page == Map.end ()) {
                    
                    ListNode<Data>node = {data, IN};
                    InsertPage (node);
                }
                else {

                    FindPage (page);
                    numHit++;
                }

            }

        return numHit;
        }
    };
}
//--------------------------------------------------------
//--------------------------------------------------------



