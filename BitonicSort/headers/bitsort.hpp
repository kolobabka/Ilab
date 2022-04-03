#ifndef __BITSORT_HPP__
#define __BITSORT_HPP__
//----------------------------------------
//----------------------------------------
#include <iostream>
#include <iterator>
#include <memory>
#include <string>
#include <vector>
#include <chrono>
#include <cmath>
#include "ocl.hpp"
//----------------------------------------
//----------------------------------------
namespace BitonicSort {

    struct TotalTime {

        long CPUTime;
        size_t GPUTime;
    };

    template <typename Type>
    class GPUSortApp final {
        
        std::vector<Type> sequence_;
        size_t lenght_ = 0;
        OCL::OclApp<Type> app_;
     	using merge_t = cl::KernelFunctor<cl::Buffer, cl_int, cl_int>;

        void possibleErase () {

            sequence_.erase(sequence_.begin() + lenght_, sequence_.end());
        }

        void possibleExtention () {

            Type max = std::numeric_limits<Type>::max ();

            double log_2 = log2(double (lenght_));
            if (fabs (log_2 - int (log2 (lenght_))) > 10E-15) {
                
                size_t newSize = pow (2, int(log_2) + 1);
                
                for (size_t i = lenght_; i < newSize; ++i)
                    sequence_.push_back (max);
            }
        }
//######################################################################################################
//######################################################################################################
//                                           GPU_SORT
//######################################################################################################
//######################################################################################################
        size_t kernelSort () {
            
            size_t seqSize = sequence_.size();
            size_t subSeqSize   = 1;
            size_t middle = seqSize / 2;
            size_t GPUTime = 0; 

            Type* sequence = sequence_.data();

            cl::Buffer seq_ (app_.context_, CL_MEM_READ_WRITE, seqSize * sizeof(Type));
            cl::copy(app_.queue_, sequence, sequence + seqSize, seq_);

            cl::Program program (app_.context_, app_.kernel_, true); 
            cl::NDRange globalRange = seqSize;
            cl::EnqueueArgs args (app_.queue_, globalRange);
            merge_t merge_seq (program, OCL::getFunction<Type>());

            for (; subSeqSize <= middle; subSeqSize *= 2) 
                for (size_t step = subSeqSize; step >= 1; step /= 2) 
                    GPUTime += bitonicMerge(seq_, subSeqSize, step, merge_seq, args);

            cl::copy (app_.queue_, seq_, sequence, sequence + seqSize);
            return GPUTime;
        }

        size_t bitonicMerge (cl::Buffer &seq_, size_t subSeqSize, size_t step,  merge_t &merge_seq, cl::EnqueueArgs &args) {
            
            cl_ulong GPUTimeStart, GPUTimeFin;
            long GPUTime;

            cl::Event event = merge_seq (args, seq_, subSeqSize * 2, step);
            event.wait();

            GPUTimeStart = event.getProfilingInfo<CL_PROFILING_COMMAND_START>();
            GPUTimeFin = event.getProfilingInfo<CL_PROFILING_COMMAND_END>();
            GPUTime = (GPUTimeFin - GPUTimeStart); // ns -> ms
            return GPUTime;
        }
    public:
        GPUSortApp (std::string kernelPath) : app_(kernelPath) {}    

        const std::vector<Type> & getSeq () const { return sequence_; }

        template <typename RandomIt> 
        TotalTime sort (RandomIt begin, RandomIt end) {     

            sequence_ = std::vector(begin, end);       
            lenght_ = sequence_.size();

            auto startTime = std::chrono::high_resolution_clock::now();
            possibleExtention ();
            size_t GPUTime = kernelSort ();
            possibleErase ();
            auto endTime = std::chrono::high_resolution_clock::now();
        
            TotalTime time {std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count(), GPUTime / 1000000};
            // std::cout << "GPU pure time measured: " << GPUTime << " ns" << std::endl;

            return time;      
        }
    };
}

#endif
