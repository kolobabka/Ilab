#include "triangles.hpp"

int main () {

    objects::Vector<double> vec1{};
    objects::Vector<double> vec2{}; 
    
    objects::Triangle<double> triangle{};

    std::cin >> triangle;

    objects::Vector<double> vec = 4.0 * triangle.GetVec1();

    vec =  2* (-vec);

    vec = (-vec) / 2;

    std::cout << triangle;

    std::cout << vec;

    return 0;
}