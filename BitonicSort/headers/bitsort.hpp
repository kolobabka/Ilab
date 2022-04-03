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
        void kernelSort () {
            
            size_t seqSize = sequence_.size();
            size_t subSeqSize   = 1;
            size_t middle = seqSize / 2;

            Type* sequence = sequence_.data();

            cl::Buffer seq_ (app_.context_, CL_MEM_READ_WRITE, seqSize * sizeof(Type));
            cl::copy(app_.queue_, sequence, sequence + seqSize, seq_);

            cl::Program program (app_.context_, app_.kernel_, true); 
            cl::NDRange globalRange = seqSize;
            cl::EnqueueArgs args (app_.queue_, globalRange);
            merge_t merge_seq (program, OCL::getFunction<Type>());

            for (; subSeqSize <= middle; subSeqSize *= 2) 
                for (size_t step = subSeqSize; step >= 1; step /= 2) 
                    bitonicMerge(seq_, subSeqSize, step, merge_seq, args);

            cl::copy (app_.queue_, seq_, sequence, sequence + seqSize);
        }

        void bitonicMerge (cl::Buffer &seq_, size_t subSeqSize, size_t step,  merge_t &merge_seq, cl::EnqueueArgs &args) {
            
            cl_ulong GPUTimeStart, GPUTimeFin;
            long Dur, GDur;

            std::cout << "GPU wall time measured: " << Dur << " ms" << std::endl;
            cl::Event event = merge_seq (args, seq_, subSeqSize * 2, step);
            GPUTimeStart = event.getProfilingInfo<CL_PROFILING_COMMAND_START>();
            GPUTimeFin = event.getProfilingInfo<CL_PROFILING_COMMAND_END>();
            GDur = (GPUTimeFin - GPUTimeStart) / 1000000; // ns -> ms
            std::cout << "GPU pure time measured: " << GDur << " ms" << std::endl;
            event.wait();
        }
    public:
        GPUSortApp (std::string kernelPath) : app_(kernelPath) {}    

        const std::vector<Type> & getSeq () const { return sequence_; }

        template <typename RandomIt> 
        std::chrono::duration<double> sort (RandomIt begin, RandomIt end) {     

            sequence_ = std::vector(begin, end);       
            lenght_ = sequence_.size();

            auto startTime = std::chrono::steady_clock::now ();
            possibleExtention ();
            kernelSort ();
            possibleErase ();
            auto endTime = std::chrono::steady_clock::now ();

            return endTime - startTime;      
        }
    };
}

#endif
