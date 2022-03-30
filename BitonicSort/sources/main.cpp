#include "bitsort.hpp"
#include "common.hpp"
//----------------------------------------
//----------------------------------------
int main () try {

    std::vector<int> sequence;
    inputSequence<int> (sequence);
#ifdef GPU
    BitonicSort::GPUApp<int> GPUapp(sequence, "../kernels/kernel.cl");
    auto elapsed_seconds = GPUapp.run();

    outputSequence (GPUapp.getSeq()); 
    std::cout << "elapsed time: " << elapsed_seconds.count () << "s\n";  
#endif

#ifdef CPU
    BitonicSort::CPUApp<int> CPUapp(sequence);
    auto elapsed_seconds = CPUapp.run();

    outputSequence (CPUapp.getSeq()); 
#endif

#ifdef STD

    outputSequence (CPUapp.getSeq()); 
#endif
    return 0;
}

catch (std::exception &err) {

    std::cout << err.what() << std::endl;
}