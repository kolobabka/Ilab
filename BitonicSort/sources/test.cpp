#include <iostream> 
#include <vector>


int main () {

    int step = 1;
    int subSeqSize = 4;

    std::vector<int> vec;
    int size;
    std::cin >> size;

    for (int i = 0; i < size; ++i) {

        int tmp;
        std::cin >> tmp;
        vec.push_back(tmp);
    }

    std::cout << "\t\t\t###BEFORE" << std::endl;

    for (auto v : vec) 
        std::cout << v << " ";
    std::cout << std::endl;

    int num = size / subSeqSize;

    std::cout << "\t\t##NUM = " << num << std::endl;


    for (int i = 0; i < size; ++i) {

    
        if (i % (step * 2) < step) {
            
            int upORdown = 1;
            if ((i / subSeqSize) % 2 == 1)
			    upORdown = 0;
            
            std::cout << "i = " << i << ", i + step = " << i + step << std::endl;
            std::cout << " i / num = " <<  i / subSeqSize << std::endl;
            std::cout << "upORdown = " << upORdown << std::endl;

            if (i + step >= size)
                std::cout << "Gay has been found" << std::endl;

            int first  = vec[i];
            int second = vec[i + step];

            if ((first > second) && upORdown) {

                vec[i] = second;
                vec[i + step] = first;
            }
            else if ((first < second) && !upORdown) {

                vec[i] = second;
                vec[i + step] = first;
            }
        }
    }

    std::cout << "\t\t\t###AFTER" << std::endl;

    for (auto v : vec) 
        std::cout << v << " ";
    std::cout << std::endl;
}