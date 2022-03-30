#ifndef __COMMON_HPP__
#define __COMMON_HPP__
//----------------------------------------
//----------------------------------------
#include <iostream>
#include <vector>
//----------------------------------------
//----------------------------------------
template <typename Type>
void outputSequence (const std::vector<Type> &seq) {
            
    for (size_t i = 0; i != seq.size(); ++i)
        std::cout << seq[i] << " ";            
    std::cout << std::endl;
}

template <typename Type>
void inputSequence (std::vector<Type> &seq) { 
            
    size_t lenght;
    std::cin >> lenght;
    if (lenght == 0)
        throw std::runtime_error ("Zero lenght of sequence (empty input)");

    seq.reserve(lenght);

    std::cout << "lenght = " << lenght << std::endl;

    for (size_t i = 0; i < lenght; ++i) {
        
        Type val;
        std::cin >> val;

        seq.push_back(val);
    }
}
#endif