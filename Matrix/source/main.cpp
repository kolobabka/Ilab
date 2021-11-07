#include "matrix.hpp"
//--------------------------------------------------------
//--------------------------------------------------------
int main () {

    int size{};
    std::cin >> size;
    assert (std::cin.good());

    Matrix::Matrix<int> matrix{size};

    // std::cin >> matrix;
    // std::cout << "\t\t\t****DUMP" << std::endl;

    // std::cout << "SOURCE:" << std::endl;
    // std::cout << matrix;

    Matrix::Matrix<int> matrix_copy{matrix};

   
    // std::cout << "DEST:" << std::endl;
    // std::cout << matrix_copy;

    Matrix::Matrix<int> matrix_assignment{2};

    matrix_assignment = matrix_copy;

    Matrix::Matrix<int> copy_matrix_2 = matrix_copy;

    Matrix::Matrix<int> move_matrix{std::move (matrix_copy)};

    // std::cout << "ASS:" << std::endl;
    // std::cout << matrix_assignment;


    return 0;    
}
