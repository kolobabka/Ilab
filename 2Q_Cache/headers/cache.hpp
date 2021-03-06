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
const int multPart = 3;
const int divPart  = 5;
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

            return (sizeList <= List.size ());
        }
    };
//--------------------------------------------------------
//--------------------------------------------------------
    template <typename Key, typename Data>
    class Cache_2Q {

    private: 
        const size_t cacheSize;
        using ListIt = typename std::list<ListNode<Data>>::iterator;
        using MapIt  = typename std::unordered_map<Key, ListIt>::iterator;

        std::unordered_map<Key, ListIt> Map;

        QueueMap<Key, Data> In {cacheSize - (multPart * (cacheSize / divPart)) - (cacheSize / divPart)};
        QueueMap<Key, Data> Out {multPart * (cacheSize / divPart)};
        QueueMap<Key, Data> Hot {cacheSize / divPart};


        void replace (QueueMap<Key, Data> &Cur, QueueMap<Key, Data> &Prev, MapIt page, whatList place) {

            auto back = Map.find(Cur.List.back().data);
            Map.erase(back);
            Cur.List.pop_back();

            page->second->place = whatList (place);
            Cur.List.splice(Cur.List.begin(), Prev.List, page->second);
        }


        void InCache (MapIt page) {

            if (page->second->place == IN)
                return;

            if (page->second->place == OUT) {

                if (!Hot.IsFull ()) {

                    page->second->place = HOT;
                    Hot.List.splice(Hot.List.begin(), Out.List, page->second);

                    return;
                }
                else {

                    replace (Hot, Out, page, HOT);
                    return;
                }
            }
            
            if (page->second->place == HOT) {

                Hot.List.splice(Hot.List.begin(), Hot.List, page->second);
                return;
            }
        }


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
                    }
                    else {
                        
                        replace (Out, In, backList, OUT);
                    }
                }
            }

            In.List.push_front (node);
            Map.insert({node.data, In.List.begin()});
        }

    public:
        Cache_2Q (const size_t size) :
            cacheSize (size) {}

        bool LookUp (Data data) {

          auto page = Map.find (data);

          if (page == Map.end ()) {

              ListNode<Data>node = {data, IN};
              InsertPage (node);
              return false;
          }

          else {

              InCache (page);
              return true;
          }
        }        
    };
}
//--------------------------------------------------------
//--------------------------------------------------------
template <typename Key, typename Data>
int CacheHit (caches::Cache_2Q<Key, Data> Cache, const size_t numPages) {

    int numHit = 0;

    for (size_t i = 0; i < numPages; i++) {

        Data data;
        std::cin >> data;
        assert (std::cin.good ());

        if (Cache.LookUp(data))
            ++numHit;
    }

    return numHit;
}
//--------------------------------------------------------
//--------------------------------------------------------
