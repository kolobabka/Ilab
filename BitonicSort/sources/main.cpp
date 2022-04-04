#include "bitsort.hpp"
#include "common.hpp"
#include <algorithm>
//----------------------------------------
//----------------------------------------
int main () try {

    std::vector<int> sequence;
    inputSequence<int> (sequence);


    std::cout << "\t\t\t###GPU-SORT" << std::endl;
    
#ifdef STD
    std::cout << "\t\t\t###STD-SORT" << std::endl;

    auto start = std::chrono::steady_clock::now ();
    std::sort(sequence.begin(), sequence.end());
    auto end = std::chrono::steady_clock::now ();

    std::chrono::duration<double> elapsed_seconds = end - start;
    outputSequence (sequence);
    std::cout << "elapsed time: " << elapsed_seconds.count () << "s\n";  
    return 0;

#endif

#ifdef KERNEL

    std::cout << KERNEL << std::endl;
    BitonicSort::GPUSortApp<int> GPUapp(KERNEL);
#else
    BitonicSort::GPUSortApp<int> GPUapp("../kernels/kernel.cl");
#endif


    BitonicSort::TotalTime seconds = GPUapp.sort(sequence.begin(), sequence.end());

    outputSequence (GPUapp.getSeq()); 
    std::cout << "CPU time: " << seconds.CPUTime << "ms\n";
    std::cout << "GPU time: " << seconds.GPUTime << "ms\n";

    return 0;
}
catch (cl::BuildError &err) {

    std::cerr << "OCL BUILD ERROR: " << err.err() << ":" << err.what()
            << std::endl;
    std::cerr << "-- Log --\n";
    for (auto e : err.getBuildLog())
    std::cerr << e.second;
    std::cerr << "-- End log --\n";
    return -1;
} 
catch (cl::Error &err) {

    std::cerr << "OCL ERROR: " << err.err() << ":" << err.what() << std::endl;
    return -1;
} 
catch (std::runtime_error &err) {

    std::cerr << "RUNTIME ERROR: " << err.what() << std::endl;
    return -1;
} 
catch (...) {

    std::cerr << "UNKNOWN ERROR\n";
    return -1;
}
