#include "../headers/cache.hpp"
//--------------------------------------------------------
//--------------------------------------------------------
int main () {

    size_t cacheSize = 0;
    size_t numPages  = 0;

    std::cin >> cacheSize >> numPages;
    assert(std::cin.good());

    caches::Cache_2Q<std::string, std::string> Cache{cacheSize};

    int result = Cache.CacheHit(numPages);

    std::cout << result << std::endl;
}
