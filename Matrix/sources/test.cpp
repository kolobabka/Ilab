#include "matrix.hpp"
#include <gtest/gtest.h>

namespace {

    void testFunc (const char* nameTests, const char* nameAnsws) {

        FILE* ftest;

        ftest = freopen (nameTests, "r", stdin);
        if (ftest == NULL) {

            perror ("File open error:");
            return;
        }

        int size{};

        std::cin >> size;
        assert (std::cin.good());

        Matrix::Matrix<double> testMatrix {size};

        std::cin >> testMatrix;
        assert (std::cin.good());

        int error = fclose (ftest);
        if (error != 0) {

            perror ("File close error:\n");
            return;
        }

        ftest = freopen (nameAnsws, "r", ftest);
        if (ftest == NULL) {

            perror ("File open error:\n");
            return;
        }

        double result{};
        std::cin >> result;
        error = fclose (ftest);
        if (error != 0) {

            perror ("File close error:\n");
            return;
        }

        EXPECT_DOUBLE_EQ (result, testMatrix.determinant());
    }
}

TEST( detTest, test1 ) {

    testFunc ("../tests/test1.txt", "../tests/answ1.txt");
}
TEST( detTest, test2 ) {

    testFunc ("../tests/test2.txt", "../tests/answ2.txt");
}
TEST( detTest, test3 ) {

    testFunc ("../tests/test3.txt", "../tests/answ3.txt");
}
TEST( detTest, test4 ) {

    testFunc ("../tests/test4.txt", "../tests/answ4.txt");
}
TEST( detTest, test5 ) {

    testFunc ("../tests/test5.txt", "../tests/answ5.txt");
}
TEST( detTest, test6 ) {

    testFunc ("../tests/test6.txt", "../tests/answ6.txt");
}
TEST( detTest, test7 ) {

    testFunc ("../tests/test7.txt", "../tests/answ7.txt");
}
TEST( detTest, test8 ) {

    testFunc ("../tests/test8.txt", "../tests/answ8.txt");
}
int main (int argc, char** argv) {

    testing::InitGoogleTest (&argc, argv);

    return RUN_ALL_TESTS ();
}