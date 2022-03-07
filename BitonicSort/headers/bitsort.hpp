#ifndef BITSORT_HPP__
#define BITSORT_HPP__
//----------------------------------------
//----------------------------------------
#include <iostream>
#include <memory>
#include <string>
#include <vector>
//----------------------------------------
//----------------------------------------
template <typename Type>
class App final {

    std::vector<Type> sequence_;
    size_t size_ = 0;
    
    void readSequence () {

        size_t lenght;
        std::cin >> lenght;

        for (size_t i = 0; i < lenght; ++i) {
            
            Type val;
            std::cin >> val;
            sequence_.push_back(val);
        }
        size_ = lenght;
    }

    void bitonicSplit () {

        const size_t size = size_;
        const size_t middle = size / 2;

        for (size_t i = 0; i < middle; ++i) {

            if (sequence_[i] > sequence_[i + middle])
                std::swap (sequence_[i], sequence_[i + middle]);
        }
    }    

    void makeSubSeqBitonic (size_t step) {

        size_t cur  = 0;
        size_t stStep = step;
        size_t border = stStep;
        bool upORdown = true;   //true --> up

        while (step >= 1) {
            
            if (stStep == 0) {
                upORdown = !upORdown;
                stStep = border;
            }
            --stStep;

            if (cur + step >= size_) {

                cur = 0;
                step /= 2;
                stStep = border;
                upORdown = true;
                continue;
            }

            Type &curElem = sequence_[cur];
            Type &toSwap  = sequence_[cur + step];

            if ((curElem > toSwap) == upORdown)
                std::swap (curElem, toSwap);

            if ((cur + 1) % step == 0)                
                cur += step;

            ++cur;
        }
    }

    void bitonicSort () {        

        size_t step = 1;
        size_t middle = size_ / 2;

        while (step <= middle) {
            
            makeSubSeqBitonic (step); 
            step *= 2;
        }

        bitonicSplit ();
    }


public:
    App () {}

    void run () {

        readSequence ();
        bitonicSort ();
    }

    void inputSequence () const {

        for (auto v : sequence_)
            std::cout << v << " ";
        
        std::cout << std::endl;
    }
};

#endif