#include "bitsort.hpp"
#include "common.hpp"
#include <algorithm>
//----------------------------------------
//----------------------------------------
int main () try {

    std::vector<int> sequence;
    inputSequence<int> (sequence);
#ifdef GPU
    std::cout << "\t\t\t###GPU-SORT" << std::endl;

    BitonicSort::GPUSortApp<int> GPUapp(sequence, "../kernels/kernel.cl");
    auto elapsed_seconds = GPUapp.run();

    outputSequence (GPUapp.getSeq()); 
    std::cout << "elapsed time: " << elapsed_seconds.count () << "s\n";  
#endif

#ifdef CPU
    std::cout << "\t\t\t###CPU-SORT" << std::endl;

    BitonicSort::CPUSortApp<int> CPUapp(sequence);
    auto elapsed_seconds = CPUapp.run();

    outputSequence (CPUapp.getSeq()); 
    std::cout << "elapsed time: " << elapsed_seconds.count () << "s\n";  
#endif

#ifdef STD
    std::cout << "\t\t\t###STD-SORT" << std::endl;

    auto start = std::chrono::steady_clock::now ();
    std::sort(sequence.begin(), sequence.end());
    auto end = std::chrono::steady_clock::now ();

    std::chrono::duration<double> elapsed_seconds = end - start;
    outputSequence (sequence);
    std::cout << "elapsed time: " << elapsed_seconds.count () << "s\n";  

#endif
    return 0;
}

catch (std::exception &err) {

    std::cout << err.what() << std::endl;
}