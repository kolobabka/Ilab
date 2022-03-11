#ifndef BITSORT_HPP__
#define BITSORT_HPP__
//----------------------------------------
//----------------------------------------
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <string>
#include <fstream>
#include <chrono>
//----------------------------------------
//----------------------------------------
#ifndef CL_HPP_TARGET_OPENCL_VERSION
#define CL_HPP_MINIMUM_OPENCL_VERSION 120
#define CL_HPP_TARGET_OPENCL_VERSION 120
#endif

#define CL_HPP_CL_1_2_DEFAULT_BUILD
#define CL_HPP_ENABLE_EXCEPTIONS

#include "CL/opencl.hpp"

#ifndef ANALYZE
#define ANALYZE 1
#endif

#define dbgs                                                                   \
  if (!ANALYZE) {                                                              \
  } else                                                                       \
    std::cout

#define LOCAL_SIZE 128
//----------------------------------------
//----------------------------------------

template <typename Type> 
struct OclApp final {

    cl::Platform     platform_;
    cl::Context      context_;
    cl::CommandQueue queue_;
    std::string      kernel_;

    static cl::Platform select_platform ();
    static cl::Context  get_gpu_context (cl_platform_id);

    using split_t = cl::KernelFunctor<cl::Buffer, int>;
    using merge_t = cl::KernelFunctor<cl::Buffer, int, int, int>;

public:
    // size_t           bufSz_;   
    // cl::Buffer       seq_; 

    OclApp (std::string kernelPath) : platform_(select_platform()), context_(get_gpu_context(platform_())), 
                                                   queue_(context_) {
        
        std::fstream file (kernelPath, std::ios_base::in);

        while (file) {

            std::string newLine;
            std::getline (file, newLine);

            kernel_ += newLine + "\n";
        }

        cl::string name    = platform_.getInfo<CL_PLATFORM_NAME>();
        cl::string profile = platform_.getInfo<CL_PLATFORM_PROFILE>();

        dbgs << "Selected: " << name << ": " << profile << std::endl;
    }
    
    // void copy (Type *sequence, size_t size) {

    //     cl::copy (queue_, sequence, sequence + size, seq_);
    // }

    void bitonic_merge (cl::Buffer &seq_, size_t size, size_t subSeqSize, size_t step,  cl::Program &program, cl::EnqueueArgs &args) {

        // Type* seqData = sequence.data();
       
        // cl::Buffer seq_ (context_, CL_MEM_READ_WRITE, bufSz);

        // cl::copy(queue_, seqData, seqData + size, seq_);


        // cl::Program program (context_, kernel_, true); //think how to replace it to methods
        merge_t merge_seq (program, "bitonic_merge");

        // cl::NDRange globalRange (size);
        // cl::EnqueueArgs args (queue_, globalRange);

        // std::cout << "subSeqSize = " << subSeqSize << std::endl;
        // std::cout << "step = " << step << std::endl;

        cl::Event event = merge_seq (args, seq_, subSeqSize * 2, step, size);
        event.wait();

        // std::cout << "\t\t\t###BEFORE" << std::endl;
        // for (auto v : sequence)
        //     std::cout << v << " ";
        // std::cout << std::endl;

      

        // std::cout << "\t\t\t###AFTER" << std::endl;   
        // for (auto v : sequence)
        //     std::cout << v << " ";
        // std::cout << std::endl;
    }

    void bitonic_split (Type *sequence, size_t size) {

        size_t BufSz = size * sizeof (Type);

        cl::Buffer seq (context_, CL_MEM_READ_WRITE, BufSz);
        cl::copy (queue_, sequence, sequence + size, seq);

        cl::Program program (context_, kernel_, true);
        split_t split_seq (program, "bitonic_split");

        cl::NDRange globalRange (size / 2);
        cl::EnqueueArgs args (queue_, globalRange);

        auto start = std::chrono::steady_clock::now ();

        cl::Event event = split_seq (args, seq, size / 2);
        event.wait();

        auto end = std::chrono::steady_clock::now ();
        std::chrono::duration<double> elapsed_seconds = end - start;
        std::cout << "elapsed time: " << elapsed_seconds.count () << "s\n";

    }
};

namespace BitonicSort {

    template <typename Type>
    class App final {

        size_t size_ = 0;
        OclApp<Type> app_;
        std::vector<Type> sequence_;
        
        void readSequence () {

            for (size_t i = 0; i < size_; ++i) {
                
                Type val;
                std::cin >> val;
                sequence_.push_back(val);
            }
        }

        void bitonicSplit (std::string kernelPath) {
            
            OclApp<int> app (kernelPath, size_);

            app.bitonic_split (sequence_.data(), sequence_.size());
        }

        void bitonicSplit () {
        
            const size_t middle = size_ / 2;

            for (size_t i = 0; i < middle; ++i) {

                if (sequence_[i] > sequence_[i + middle])
                    std::swap (sequence_[i], sequence_[i + middle]);
            }
        }    

        void sort () {
            
            size_t subSeqSize   = 1;
            size_t middle = size_ / 2;

            Type* sequence = sequence_.data();
            size_t bufSz = (size_ * sizeof(Type)); 

            cl::Buffer seq_ (app_.context_, CL_MEM_READ_WRITE, bufSz);
            cl::copy(app_.queue_, sequence, sequence + size_, seq_);

            cl::Program program (app_.context_, app_.kernel_, true); //think how to replace it to methods
            cl::NDRange globalRange (size_);
            cl::EnqueueArgs args (app_.queue_, globalRange);
            
            for (; subSeqSize <= middle; subSeqSize *= 2) {
                for (size_t step = subSeqSize; step >= 1; step /= 2) {

                    app_.bitonic_merge(seq_, size_, subSeqSize, step, program, args);
                }
            }

            cl::copy (app_.queue_, seq_, sequence, sequence + size_);
        }

        void makeSubSeqBitonic (size_t step) {

            size_t cur  = 0;
            size_t stStep = step;
            size_t border = stStep;
            bool upORdown = true;   //true --> up  

            while (step >= 1) {
            
                if (!stStep) {
                    upORdown = !upORdown;
                    stStep = border;
                }
                --stStep;

                if (cur + step >= size_) {

                    cur = 0;
                    step /= 2;
                    stStep = border;
                    upORdown = true;
                    continue;
                }

                Type &curElem = sequence_[cur];
                Type &toSwap  = sequence_[cur + step];

                if ((curElem > toSwap) == upORdown)
                    std::swap (curElem, toSwap);

                if ((cur + 1) % step == 0)                
                    cur += step;

                ++cur;
            }
        }

        void bitonicSort () {        

            size_t step = 1;
            size_t middle = size_ / 2;

            while (step <= middle) {
                
                makeSubSeqBitonic (step); 
                step *= 2;
            }

            bitonicSplit ();
        }


    public:
        App (size_t lenght, std::string kernelPath) : size_(lenght), app_(kernelPath) {sequence_.reserve(size_);}

        void run () {

            readSequence ();
            auto start = std::chrono::steady_clock::now ();

            // bitonicSort();

            sort ();

            // std::sort (sequence_.begin(), sequence_.end());
            auto end = std::chrono::steady_clock::now ();
            std::chrono::duration<double> elapsed_seconds = end - start;
            std::cout << "elapsed time: " << elapsed_seconds.count () << "s\n";
        }

        void inputSequence () const {

            for (auto v : sequence_)
                std::cout << v << " ";
            
            std::cout << std::endl;
        }
    };
}

template <typename Type>
cl::Platform OclApp<Type>::select_platform () {

    cl::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);

    for (auto p : platforms) {

        cl_uint numDevices = 0;
        ::clGetDeviceIDs (p(), CL_DEVICE_TYPE_GPU, 0, NULL, &numDevices);

        if (numDevices > 0)
            return cl::Platform(p);
    }

    throw std::runtime_error ("No platform selected");
}

template <typename Type>
cl::Context OclApp<Type>::get_gpu_context (cl_platform_id PId) {

    cl_context_properties properties[] = {

        CL_CONTEXT_PLATFORM, reinterpret_cast<cl_context_properties>(PId), 0
    };

    return cl::Context (CL_DEVICE_TYPE_GPU, properties);
}

#endif
