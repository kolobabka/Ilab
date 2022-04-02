#include "common.hpp"
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

#define CHECK() const std::vector<int>& sortSeq = testApp.getSeq(); \
        int prev = sortSeq[0]; \
        int cur  = 0;          \
        for (size_t i = 1; i < sortSeq.size(); ++i) { \
            cur = sortSeq[i]; \
            ASSERT_GE (cur, prev); \
        }
//----------------------------------------
//----------------------------------------
namespace {

#if 1
    void CPUTestSorting (FILE* ftest) try {

        std::vector<int> seq;
        inputSequence<int> (seq);

        BitonicSort::CPUSortApp<int> testApp (seq);
        auto time = testApp.run();

        CLOSE_FD (ftest);
        CHECK();
	//return 0;
    }
    catch (cl::BuildError &err) {

    std::cerr << "OCL BUILD ERROR: " << err.err() << ":" << err.what()
            << std::endl;
    std::cerr << "-- Log --\n";
    for (auto e : err.getBuildLog())
    std::cerr << e.second;
    std::cerr << "-- End log --\n";
    //return -1;
} 
catch (cl::Error &err) {

    std::cerr << "OCL ERROR: " << err.err() << ":" << err.what() << std::endl;
    //return -1;
} 
catch (std::runtime_error &err) {

    std::cerr << "RUNTIME ERROR: " << err.what() << std::endl;
    //return -1;
} 
catch (...) {

    std::cerr << "UNKNOWN ERROR\n";
    //return -1;
}
#endif

#if 1
    void GPUTestSorting (FILE* ftest) try {

        std::vector<int> seq;
        inputSequence<int> (seq);

        BitonicSort::GPUSortApp<int> testApp (seq, "../kernels/kernel.cl");
        auto time = testApp.run();

        CLOSE_FD (ftest);
        CHECK();
    }
    catch (cl::BuildError &err) {

    std::cerr << "OCL BUILD ERROR: " << err.err() << ":" << err.what()
            << std::endl;
    std::cerr << "-- Log --\n";
    for (auto e : err.getBuildLog())
    std::cerr << e.second;
    std::cerr << "-- End log --\n";
    //return -1;
} 
catch (cl::Error &err) {

    std::cerr << "OCL ERROR: " << err.err() << ":" << err.what() << std::endl;
    //return -1;
} 
catch (std::runtime_error &err) {

    std::cerr << "RUNTIME ERROR: " << err.what() << std::endl;
    //return -1;
} 
catch (...) {

    std::cerr << "UNKNOWN ERROR\n";
    //return -1;
}
#endif
}
//######################################################################################################
//######################################################################################################
//                                      Test of GPU_SORT
//######################################################################################################
//######################################################################################################
#if 1
TEST( GPUTestSorting, test1 ) {

    FILE* ftest = CHANGE_STREAM("../tests/001.dat");
    GPUTestSorting (ftest);
}

TEST( GPUTestSorting, test2 ) {

    FILE* ftest = CHANGE_STREAM("../tests/002.dat");
    GPUTestSorting (ftest);
}

TEST( GPUTestSorting, test3 ) {

    FILE* ftest = CHANGE_STREAM("../tests/003.dat");
    GPUTestSorting (ftest);
}

TEST( GPUTestSorting, test4 ) {

    FILE* ftest = CHANGE_STREAM("../tests/004.dat");
    GPUTestSorting (ftest);
}

TEST( GPUTestSorting, test5 ) {

    FILE* ftest = CHANGE_STREAM("../tests/005.dat");
    GPUTestSorting (ftest);
}

TEST( GPUTestSorting, test6 ) {

    FILE* ftest = CHANGE_STREAM("../tests/006.dat");
    GPUTestSorting (ftest);
}

TEST( GPUTestSorting, test7 ) {

    FILE* ftest = CHANGE_STREAM("../tests/007.dat");
    GPUTestSorting (ftest);
}

TEST( GPUTestSorting, test8 ) {

    FILE* ftest = CHANGE_STREAM("../tests/008.dat");
    GPUTestSorting (ftest);
}
#endif
//######################################################################################################
//######################################################################################################
//                                      Test of CPU_SORT
//######################################################################################################
//######################################################################################################
#if 1
TEST( CPUTestSorting, test1 ) {

    FILE* ftest = CHANGE_STREAM("../tests/001.dat");
    CPUTestSorting (ftest);
}

TEST( CPUTestSorting, test2 ) {

    FILE* ftest = CHANGE_STREAM("../tests/002.dat");
    CPUTestSorting (ftest);
}

TEST( CPUTestSorting, test3 ) {

    FILE* ftest = CHANGE_STREAM("../tests/003.dat");
    CPUTestSorting (ftest);
}

TEST( CPUTestSorting, test4 ) {

    FILE* ftest = CHANGE_STREAM("../tests/004.dat");
    CPUTestSorting (ftest);
}

TEST( CPUTestSorting, test5 ) {

    FILE* ftest = CHANGE_STREAM("../tests/005.dat");
    CPUTestSorting (ftest);
}

TEST( CPUTestSorting, test6 ) {

    FILE* ftest = CHANGE_STREAM("../tests/006.dat");
    CPUTestSorting (ftest);
}

TEST( CPUTestSorting, test7 ) {

    FILE* ftest = CHANGE_STREAM("../tests/007.dat");
    CPUTestSorting (ftest);
}

TEST( CPUTestSorting, test8 ) {

    FILE* ftest = CHANGE_STREAM("../tests/008.dat");
    CPUTestSorting (ftest);
}
#endif
//----------------------------------------
//----------------------------------------
int main (int argc, char** argv) {

    testing::InitGoogleTest (&argc, argv);

    return RUN_ALL_TESTS ();
}
