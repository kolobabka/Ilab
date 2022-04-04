#include "common.hpp"
#include "bitsort.hpp"
#include <string_view>
//----------------------------------------
//----------------------------------------
#define CHANGE_STREAM(name) freopen (name, "r", stdin);\
    if (ftest == NULL) {                                \
        perror ("File open error:\n");                  \
        return -1;                                         \
    }       

#define CLOSE_FD(ftest) int error = fclose (ftest); \
    if (error != 0) {                            \
        perror ("File close error:\n");          \
        return -1;                                     \
    }

#define CHECK() const std::vector<int>& sortSeq = testApp.getSeq(); \
        int prev = sortSeq[0]; \
        int cur  = 0;          \
        for (size_t i = 1; i < sortSeq.size(); ++i) { \
            cur = sortSeq[i]; \
            if (cur < sortSeq[i - 1]) \
                return -1; \
        }

const int numTests = 8;
//----------------------------------------
//----------------------------------------
namespace {
#if 1
    int GPUTestSorting (FILE* ftest) try {

        std::vector<int> seq;
        inputSequence<int> (seq);

        BitonicSort::GPUSortApp<int> testApp ("../kernels/kernel.cl");
        auto time = testApp.sort(seq.begin(), seq.end());

        CLOSE_FD (ftest);
        CHECK();
        return 0;
    }
    catch (cl::BuildError &err) {

    std::cerr << "OCL BUILD ERROR: " << err.err() << ":" << err.what()
            << std::endl;
    std::cerr << "-- Log --\n";
    for (auto e : err.getBuildLog())
    std::cerr << e.second;
    std::cerr << "-- End log --\n";
    return -1;
} 
catch (cl::Error &err) {

    std::cerr << "OCL ERROR: " << err.err() << ":" << err.what() << std::endl;
    return -1;
} 
catch (std::runtime_error &err) {

    std::cerr << "RUNTIME ERROR: " << err.what() << std::endl;
    return -1;
} 
catch (...) {

    std::cerr << "UNKNOWN ERROR\n";
    return -1;
}
#endif
}
//######################################################################################################
//######################################################################################################
//                                      Test of GPU_SORT
//######################################################################################################
//######################################################################################################
#if 0
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
//----------------------------------------
//----------------------------------------
int main () try {

    std::cout << "\t\t\t***Start end-to-end tests***" << std::endl <<
                 "\t\tChecking correctness of work:\n" << std::endl; 

    const char *tests[numTests]  = {

        "../tests/001.dat",
        "../tests/002.dat",
        "../tests/003.dat",
        "../tests/004.dat",
        "../tests/005.dat",
        "../tests/006.dat",
        "../tests/007.dat",
        "../tests/008.dat"
    };

    for (int i = 0; i < numTests; ++i) {

        std::cout << "TEST [" << i << "]: ";
        FILE* ftest = CHANGE_STREAM(tests[i]);
        if ((GPUTestSorting (ftest) == -1)) {

            std::cout << "WRONG!\n\tThe array is not sorted" << std::endl;
            return -1;
        }
        std::cout << "PASSED!" << std::endl;
    }

    std::cout << "\t\t\t***All tests passed, everything is correct***" << std::endl;
    return 0;
}
catch (cl::BuildError &err) {

    std::cerr << "OCL BUILD ERROR: " << err.err() << ":" << err.what()
            << std::endl;
    std::cerr << "-- Log --\n";
    for (auto e : err.getBuildLog())
    std::cerr << e.second;
    std::cerr << "-- End log --\n";
    return -1;
} 
catch (cl::Error &err) {

    std::cerr << "OCL ERROR: " << err.err() << ":" << err.what() << std::endl;
    return -1;
} 
catch (std::runtime_error &err) {

    std::cerr << "RUNTIME ERROR: " << err.what() << std::endl;
    return -1;
} 
catch (...) {

    std::cerr << "UNKNOWN ERROR\n";
    return -1;
}