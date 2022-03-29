#include "bitsort.hpp"
#include <gtest/gtest.h>
//----------------------------------------
//----------------------------------------
#define CHANGE_STREAM(name) freopen (name, "r", stdin);\
    if (ftest == NULL) {                                \
        perror ("File open error:\n");                  \
        return;                                         \
    }       

#define CLOSE_FD(ftest) int error = fclose (ftest); \
    if (error != 0) {                            \
        perror ("File close error:\n");          \
        return;                                     \
    }

//----------------------------------------
//----------------------------------------
namespace {

#if 0
    void simpTestSorting (FILE* ftest) try {


        BitonicSort::App<int> testApp ("../kernels/kernel.cl");
        auto time = testApp.run();

        CLOSE_FD (ftest);

        std::vector<int>& seq = testApp.getSeq();

        int prev = seq[0];
        int cur  = 0;

        for (size_t i = 1; i < seq.size(); ++i) {
            
            cur = seq[i];
            ASSERT_GE (cur, prev);
        }
    }
    catch (std::exception &err) {

        CLOSE_FD (ftest);

        std::cout << "Caught exception: " << err.what() << std::endl;
    }
#endif

    void parTestSorting (FILE* ftest) try {


        BitonicSort::App<int> testApp ("../kernels/kernel.cl");
        auto time = testApp.run();

        CLOSE_FD (ftest);

        std::vector<int>& seq = testApp.getSeq();

        int prev = seq[0];
        int cur  = 0;

        for (size_t i = 1; i < seq.size(); ++i) {
            
            cur = seq[i];
            ASSERT_GE (cur, prev);
        }
    }
    catch (std::exception &err) {

        CLOSE_FD (ftest);

        std::cout << "Caught exception: " << err.what() << std::endl;
    }
}
//######################################################################################################
//######################################################################################################
//                                      Test of GPU_SORT
//######################################################################################################
//######################################################################################################

TEST( parTestSorting, test1 ) {

    FILE* ftest = CHANGE_STREAM("../tests/001.dat");
    parTestSorting (ftest);
}

TEST( parTestSorting, test2 ) {

    FILE* ftest = CHANGE_STREAM("../tests/002.dat");
    parTestSorting (ftest);
}

TEST( parTestSorting, test3 ) {

    FILE* ftest = CHANGE_STREAM("../tests/003.dat");
    parTestSorting (ftest);
}

TEST( parTestSorting, test4 ) {

    FILE* ftest = CHANGE_STREAM("../tests/004.dat");
    parTestSorting (ftest);
}

TEST( parTestSorting, test5 ) {

    FILE* ftest = CHANGE_STREAM("../tests/005.dat");
    parTestSorting (ftest);
}

TEST( parTestSorting, test6 ) {

    FILE* ftest = CHANGE_STREAM("../tests/006.dat");
    parTestSorting (ftest);
}

TEST( parTestSorting, test7 ) {

    FILE* ftest = CHANGE_STREAM("../tests/007.dat");
    parTestSorting (ftest);
}

TEST( parTestSorting, test8 ) {

    FILE* ftest = CHANGE_STREAM("../tests/008.dat");
    parTestSorting (ftest);
}
//----------------------------------------
//----------------------------------------
int main (int argc, char** argv) {

    testing::InitGoogleTest (&argc, argv);

    return RUN_ALL_TESTS ();
}