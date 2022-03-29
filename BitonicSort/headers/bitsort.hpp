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
#include <cmath>
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

#define PARALLEL
//----------------------------------------
//----------------------------------------
namespace OCL {

    template <typename Type>
    const std::string getFunction ();

    template <> const std::string getFunction<char>             ()                   {return "bitonic_merge_char";  }
    template <> const std::string getFunction<int>              ()                   {return "bitonic_merge_int";   }
    template <> const std::string getFunction<float>            ()                   {return "bitonic_merge_float"; }
    template <> const std::string getFunction<double>           ()                   {return "bitonic_merge_double";}

    template <typename Type> 
    struct OclApp final {

        cl::Platform     platform_;
        cl::Context      context_;
        cl::CommandQueue queue_;
        std::string      kernel_;

        static cl::Platform selectPlatform ();
        static cl::Context  getGPUContext (cl_platform_id);
        
        const cl::string name_;
        const cl::string profile_;

        OclApp (std::string kernelPath) : platform_(selectPlatform()), context_(getGPUContext(platform_())), queue_(context_), 
                                          name_(platform_.getInfo<CL_PLATFORM_NAME>()), profile_(platform_.getInfo<CL_PLATFORM_PROFILE>()) { //!TODO: Throw exception if kernel is unavailable 
            
            std::fstream file (kernelPath, std::ios_base::in);

            if (!file)
                throw std::runtime_error ("Disable to open a file");

            while (file) {

                std::string newLine;
                std::getline (file, newLine);

                kernel_ += newLine + "\n";
            };

            dbgs << "Selected: " << name_ << ": " << profile_ << std::endl;
        }
    };
    //----------------------------------------
    //----------------------------------------
    template <typename Type>
    cl::Platform OclApp<Type>::selectPlatform () {

        cl::vector<cl::Platform> platforms;
        cl::Platform::get (&platforms);

        for (auto p : platforms) {

            cl_uint numDevices = 0;
            ::clGetDeviceIDs (p(), CL_DEVICE_TYPE_GPU, 0, NULL, &numDevices);
	
	#ifdef NVIDIA
            if (p.getInfo<CL_PLATFORM_NAME> () == "NVIDIA CUDA")
                return cl::Platform(p);
	#else 
	    if (numDevices > 0)
		return cl::Platform(p);
	#endif
	 
        }

        throw std::runtime_error ("No platform selected");
    }

    template <typename Type>
    cl::Context OclApp<Type>::getGPUContext (cl_platform_id PId) {

        cl_context_properties properties[] = {

            CL_CONTEXT_PLATFORM, reinterpret_cast<cl_context_properties>(PId), 0
        };

        return cl::Context (CL_DEVICE_TYPE_GPU, properties);
    }
}
//----------------------------------------
//----------------------------------------
namespace BitonicSort {
    template <typename Type>
    class App final {

        size_t lenght_ = 0;
        OCL::OclApp<Type> app_;
        std::vector<Type> sequence_;

     	using merge_t = cl::KernelFunctor<cl::Buffer, cl_int, cl_int>;

    private:
        void possibleExtention (Type min) {

            double log_2 = log2(double (lenght_));
            if (fabs (log_2 - int (log2 (lenght_))) > 10E-15) {
                
                size_t newSize = pow (2, int(log_2) + 1);
                
                for (size_t i = lenght_; i < newSize; ++i)
                    sequence_.push_back (min);
            }
        }
   
        void readSequence () { 
            
            std::cin >> lenght_;
            if (lenght_ == 0)
                throw std::runtime_error ("Zero lenght of sequence (empty input)");

            sequence_.reserve(lenght_);

            Type min;
            std::cin >> min;
            sequence_.push_back(min);

            for (size_t i = 1; i < lenght_; ++i) {
                
                Type val;
                std::cin >> val;

                if (val < min)
                    min = val;
                sequence_.push_back(val);
            }

            possibleExtention (min);
        }
//######################################################################################################
//######################################################################################################
//                                           GPU_SORT
//######################################################################################################
//######################################################################################################
        void parallelSort () {
            
            size_t seqSize = sequence_.size ();
            size_t subSeqSize   = 1;
            size_t middle = seqSize / 2;

            Type* sequence = sequence_.data();

            cl::Buffer seq_ (app_.context_, CL_MEM_READ_WRITE, seqSize * sizeof(Type));
            cl::copy(app_.queue_, sequence, sequence + seqSize, seq_);

            cl::Program program (app_.context_, app_.kernel_, true); //TODO: think how to replace it to fields
            cl::NDRange globalRange (seqSize);
            cl::EnqueueArgs args (app_.queue_, globalRange);
            merge_t merge_seq (program, OCL::getFunction<Type>());

            for (; subSeqSize <= middle; subSeqSize *= 2) 
                for (size_t step = subSeqSize; step >= 1; step /= 2) 
                    bitonicMerge(seq_, subSeqSize, step, merge_seq, args);

            cl::copy (app_.queue_, seq_, sequence, sequence + seqSize);
        }

        void bitonicMerge (cl::Buffer &seq_, size_t subSeqSize, size_t step,  merge_t &merge_seq, cl::EnqueueArgs &args) {

            cl::Event event = merge_seq (args, seq_, subSeqSize * 2, step);
            event.wait();
        }
//######################################################################################################
//######################################################################################################
//                                          CPU_SORT
//######################################################################################################
//######################################################################################################
        void bitonicSort () {  

            size_t seqSize = sequence_.size ();
            size_t step = 1;
            size_t middle = seqSize / 2;

            while (step <= middle) {
                
                makeSubSeqBitonic (seqSize, step); 
                step *= 2;
            }

            bitonicSplit (seqSize);
        }

        void bitonicSplit (size_t seqSize) {
            
            const size_t middle = seqSize / 2;

            for (size_t i = 0; i < middle; ++i) {

                if (sequence_[i] > sequence_[i + middle])
                    std::swap (sequence_[i], sequence_[i + middle]);
            }
        }    

        void makeSubSeqBitonic (size_t seqSize, size_t step) { //TODO: Why is it so ugly? Maybe you should remove it?

            size_t cur    = 0;
            size_t stStep = step;
            size_t border = stStep;
            bool upORdown = true;   //true --> up  

            while (step >= 1) {
            
                if (!stStep) {

                    upORdown = !upORdown;
                    stStep = border;
                }
                --stStep;

                if (cur + step >= seqSize) {

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
    public:
        App (std::string kernelPath) : app_(kernelPath) {}

    #ifdef DEBUG

        std::vector<Type> & getSeq () { return sequence_; }
    
    #endif 

        std::chrono::duration<double> run () { 

            readSequence ();

            auto start = std::chrono::steady_clock::now ();

            #ifdef SIMPLE 
            #undef PARALLEL
            bitonicSort();
            #endif
            #ifdef STD
            #undef PARALLEL
            std::sort (sequence_.begin(), sequence_.end());
            #endif
            #ifdef PARALLEL
            parallelSort ();
            #endif

            auto end = std::chrono::steady_clock::now ();
            return end - start;      
        }

        void inputSequence () const {
            
            size_t size = sequence_.size();

            for (size_t i = size - lenght_; i != size; ++i)
                std::cout << sequence_[i] << " ";            
            std::cout << std::endl;
        }
    };
}


#endif
