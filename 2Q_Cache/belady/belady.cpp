#include <iostream>
#include <cassert>
#include <cstdlib>
#include <new>
#include <unordered_map>

void FillArr (int* arr, int size);
void PrintArr (int* arr, int size);
int Cache (int* cache, int cacheSize, int* pages, int numPages);
bool InsertPage (int* cache, int* pages, int cacheSize, int numPages, int* size, int page, int index);

int main () {

    int numPages  = 0;
    int cacheSize = 0;

    std::cin >> cacheSize >> numPages;
    assert (std::cin.good()); 

    int* pages = new int [numPages];
    assert (pages);

    FillArr (pages, numPages); 

    PrintArr (pages, numPages); 

    int* cache = new int [cacheSize];

    int result = Cache (cache, cacheSize, pages, numPages);

    std::cout << result << std::endl;

    delete[] pages;  
    delete[] cache;
}

void FillArr (int* arr, int size) {

    assert (arr);

    for (int index = 0; index < size; ++index) {

        std::cin >> arr[index];
        assert (std::cin.good ());
    }
}

void PrintArr (int* arr, int size) {

    assert (arr);

    for (int index = 0; index < size; ++index) 
        std::cout << arr[index] << ' ';    
    
    std::cout << std::endl;
}

int Cache (int* cache, int cacheSize, int* pages, int numPages) {

    int page   = 0;
    int result = 0;
    int size   = 0;
    assert (pages);

    for (int index = 0; index < cacheSize; ++index) {

        cache[index] = -1;
    }

    for (int index = 0; index < numPages; ++index) {

        page = pages[index];
        assert (std::cin.good());

        std::cout << "The " << index << " iteration of the circle\n";

        if (InsertPage (cache, pages, cacheSize, numPages, &size, page, index))  
            result++;
        
        PrintArr (cache, cacheSize);

    }

    return result;
}

bool InsertPage (int* cache, int* pages, int cacheSize, int numPages, int* size, int page, int index) {

    assert (cache);

    int maxLenght    = 0;
    int indMaxLenght = 0;
    int isFound      = 0; 

    // std::cout << page <<" Page!\n";

    for (int i = 0; i < cacheSize; ++i) {

        // std::cout << "page = " << page << "\n";
        // std::cout << "cache[" << i << "] = " << cache[i] << std::endl;
        if (cache[i] == page) 
            return true;
    }

    
    if (*size < cacheSize) {

        cache[*size] = page;
        ++(*size);
        // std::cout << "here!\n";
        return false;
    }

    for (int i = 0; i < cacheSize; ++i)  {

        for (int j = index; j < numPages; ++j) {

            // std::cout << "cache[" << i <<"] = " << cache[i] << ", pages[" << j <<"] = " << pages[j] << std::endl;
            if (cache[i] == pages[j]) {
                
                // std::cout << "Here!!!\n";
                isFound = 1;
                if (j > maxLenght) {

                    maxLenght    = j;
                    indMaxLenght = i;
                }

                break;
            }
        }

        // std::cout << "maxLenght = " << maxLenght << ", indMaxLenght = " << indMaxLenght << "\n";

        if (isFound == 0) {

            // std::cout << "And I'm here!\n";
            cache[i] = page; 
            return false;
        }
        else {

            isFound = 0;
            
        }

        
    }   

    cache[indMaxLenght] = page;

    return false;
}

