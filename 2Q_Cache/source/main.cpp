#include "../headers/cache.hpp"
//--------------------------------------------------------
//--------------------------------------------------------
int main () {

    size_t cacheSize = 0;
    size_t numPages  = 0;

    std::cin >> cacheSize >> numPages;
    assert(std::cin.good());

    caches::Cache_2Q<int, int> Cache{cacheSize};

    int result = CacheHit(Cache, numPages);

    std::cout << result << std::endl;
}
