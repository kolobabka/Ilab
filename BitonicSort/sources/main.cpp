#include "bitsort.hpp"
//----------------------------------------
//----------------------------------------
int main () try {

    BitonicSort::App<int> app("../kernels/kernel.cl");
    auto elapsed_seconds = app.run();

    app.inputSequence(); 
    std::cout << "elapsed time: " << elapsed_seconds.count () << "s\n";  

    return 0;
}

catch (std::exception &err) {

    std::cout << err.what() << std::endl;
}