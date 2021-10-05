#include "triangles.hpp"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
int main () {
    
    long long result = IntersectCount<double> ();
    std::cout << "result =" << result << std::endl;

    return 0;
}