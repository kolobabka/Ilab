#include "matrix.hpp"
//--------------------------------------------------------
//--------------------------------------------------------
int main () {

    int size{};
    std::cin >> size;
    assert (std::cin.good());

    Matrix::Matrix <double> matrix{size};
    std::cin >> matrix;

    std::cout << matrix.determinant() << std::endl;

    return 0;
}
