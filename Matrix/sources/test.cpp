#include "matrix.hpp"
#include <gtest/gtest.h>

namespace {

    Matrix::Matrix<double> inputMatrix (const char* nameTests) {

        FILE* ftest;

        ftest = freopen (nameTests, "r", stdin);
        if (ftest == NULL) {

            perror ("File open error:");
            assert (false);
        }

        int size{};

        std::cin >> size;
        assert (std::cin.good());

        Matrix::Matrix<double> matrix {size};

        std::cin >> matrix;
        assert (std::cin.good());

        int error = fclose (ftest);
        if (error != 0) {

            perror ("File close error:\n");
            assert (false);
        }

        return matrix;
    }
    void testDetFunc (const char* nameTests, const char* nameAnsws) {

        Matrix::Matrix<double> testMatrix = inputMatrix (nameTests);

        FILE* ftest = freopen (nameAnsws, "r", stdin);
        if (ftest == NULL) {

            perror ("File open error:\n");
            return;
        }

        double result{};
        std::cin >> result;

        int error = fclose (ftest);
        if (error != 0) {

            perror ("File close error:\n");
            return;
        }

        EXPECT_DOUBLE_EQ (result, testMatrix.determinant());
    }
}
//######################################################################################################
//######################################################################################################
//                                      Test of Determinant
//######################################################################################################
//######################################################################################################

TEST( detTest, test1 ) {

    testDetFunc ("../tests/test1.txt", "../tests/answ1.txt");
}
TEST( detTest, test2 ) {

    testDetFunc ("../tests/test2.txt", "../tests/answ2.txt");
}
TEST( detTest, test3 ) {

    testDetFunc ("../tests/test3.txt", "../tests/answ3.txt");
}
TEST( detTest, test4 ) {

    testDetFunc ("../tests/test4.txt", "../tests/answ4.txt");
}
TEST( detTest, test5 ) {

    testDetFunc ("../tests/test5.txt", "../tests/answ5.txt");
}
TEST( detTest, test6 ) {

    testDetFunc ("../tests/test6.txt", "../tests/answ6.txt");
}
TEST( detTest, test7 ) {

    testDetFunc ("../tests/test7.txt", "../tests/answ7.txt");
}
TEST( detTest, test8 ) {

    testDetFunc ("../tests/test8.txt", "../tests/answ8.txt");
}
TEST( detTest, test9 ) {

    testDetFunc ("../tests/test9.txt", "../tests/answ9.txt");
}
TEST( detTest, test10 ) {

    testDetFunc ("../tests/test10.txt", "../tests/answ10.txt");
}
//######################################################################################################
//######################################################################################################
//                                      Test of Copy Ctor
//######################################################################################################
//######################################################################################################
TEST( copyCtorTest, test1 ) {

    Matrix::Matrix<double> matrix = inputMatrix ("../tests/test1.txt");
    Matrix::Matrix<double> testMatrix{matrix};
    ASSERT_EQ(matrix, testMatrix);
}
TEST( copyCtorTest, test2 ) {

    Matrix::Matrix<double> matrix = inputMatrix ("../tests/test2.txt");
    Matrix::Matrix<double> testMatrix{matrix};
    ASSERT_EQ(matrix, testMatrix);
}
TEST( copyCtorTest, test3 ) {

    Matrix::Matrix<double> matrix = inputMatrix ("../tests/test3.txt");
    Matrix::Matrix<double> testMatrix = matrix;
    ASSERT_EQ(matrix, testMatrix);
}
TEST( copyCtorTest, test4 ) {

    Matrix::Matrix<double> matrix = inputMatrix ("../tests/test8.txt");
    Matrix::Matrix<double> testMatrix = matrix;
    ASSERT_EQ(matrix, testMatrix);
}
//######################################################################################################
//######################################################################################################
//                                       Test of Move Cotr
//######################################################################################################
//######################################################################################################
TEST( MoveCtorTest, test1 ) {

    Matrix::Matrix<double> matrix = inputMatrix ("../tests/test1.txt");
    Matrix::Matrix<double> testMatrix{std::move (matrix)};
    
    ASSERT_EQ(matrix, testMatrix);
}
TEST( MoveCtorTest, test2 ) {

    Matrix::Matrix<double> matrix = inputMatrix ("../tests/test2.txt");
    Matrix::Matrix<double> testMatrix{std::move (matrix)};
    
    ASSERT_EQ(matrix, testMatrix);
}
TEST( MoveCtorTest, test3 ) {

    Matrix::Matrix<double> matrix = inputMatrix ("../tests/test3.txt");
    Matrix::Matrix<double> testMatrix{std::move (matrix)};
    
    ASSERT_EQ(matrix, testMatrix);
}
TEST( MoveCtorTest, test4 ) {

    Matrix::Matrix<double> matrix = inputMatrix ("../tests/test8.txt");
    Matrix::Matrix<double> testMatrix{std::move (matrix)};
    
    ASSERT_EQ(matrix, testMatrix);
}
//######################################################################################################
//######################################################################################################
//                                    Test of Assignment Operatot
//######################################################################################################
//######################################################################################################
TEST( AssignmentTest, test1 ) {

    Matrix::Matrix<double> firstMatrix  = inputMatrix ("../tests/test1.txt");
    Matrix::Matrix<double> secondMatrix = {};

    firstMatrix = secondMatrix; 
    
    ASSERT_EQ(firstMatrix, secondMatrix);
}
TEST( AssignmentTest, test2 ) {

    Matrix::Matrix<double> firstMatrix  = inputMatrix ("../tests/test2.txt");
    Matrix::Matrix<double> secondMatrix = {};

    firstMatrix = secondMatrix; 
    
    ASSERT_EQ(firstMatrix, secondMatrix);
}
TEST( AssignmentTest, test3 ) {

    Matrix::Matrix<double> firstMatrix  = inputMatrix ("../tests/test3.txt");
    Matrix::Matrix<double> secondMatrix = {};

    firstMatrix = secondMatrix; 
    
    ASSERT_EQ(firstMatrix, secondMatrix);
}
TEST( AssignmentTest, test4 ) {

    Matrix::Matrix<double> firstMatrix  = inputMatrix ("../tests/test8.txt");
    Matrix::Matrix<double> secondMatrix = {};

    firstMatrix = secondMatrix; 
    
    ASSERT_EQ(firstMatrix, secondMatrix);
}

int main (int argc, char** argv) {

    testing::InitGoogleTest (&argc, argv);

    return RUN_ALL_TESTS ();
}