#include "bitsort.hpp"
//----------------------------------------
//----------------------------------------
int main () {
    size_t lenght;
    std::cin >> lenght;
    BitonicSort::App<int> app(lenght, "../sources/kernel.cl");
    std::cout << "Dead" << std::endl;
    app.run();
   //app.inputSequence(); 
    return 0;
}
