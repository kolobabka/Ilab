#include "../headers/cache.hpp"
#include <random>
#include <chrono>

void FillStrings (const char* nameTests[], std::string str[]);

const int numTests = 9;

double get_time()
{
    return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now().time_since_epoch()).count() / 1e6;
}

int main () {

    FILE* ftest;

    const char* nameTests[numTests] = {"../test/test1.txt", "../test/test2.txt", "../test/test3.txt", "../test/test4.txt", "../test/test5.txt",
                                       "../test/test6.txt", "../test/test7.txt", "../test/test8.txt", "../test/test9.txt"};

    std::string str[numTests];
    std::string answers[numTests];

    for (int i = 0; i < numTests; i++) {

        double start = 0, finish = 0;
        start = get_time();

        ftest = freopen (nameTests[i], "r", stdin);
        if (ftest == NULL) {

            perror ("File open error:");
            return -1;
        }

        size_t cacheSize = 0;
        size_t numPages  = 0;

        std::cin >> cacheSize >> numPages;
        assert(std::cin.good());

        caches::Cache_2Q<int, int> Cache{cacheSize};

        int result = Cache.CacheHit(numPages);

        std::cout <<"Result is : " << result << std::endl;
        finish = get_time();

        std::cout << "\t\tTest [" << i + 1 <<"] time: " << finish - start << std::endl;

        int trueResult = 0;

        std::cin >> trueResult;

        if (trueResult == result) {

            std::cout << "\t\t\t\tTest [" << i + 1<< "]: PASSED\n\n";
        }
        else {

            std::cout << "\t\tTest [" << i + 1 << "]: WRONG ANSWER\n" << "Result is " << trueResult <<" but expected one is " << result << "\n\n";
        }

        fclose (ftest);
    }
}
