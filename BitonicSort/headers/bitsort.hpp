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
    class IApp {

    protected:
        std::vector<Type> sequence_;
        size_t lenght_ = 0;

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

    public:
        IApp (const std::vector<Type> &seq) : sequence_(seq), lenght_(sequence_.size()) {} 
        const std::vector<Type> & getSeq () const { return sequence_; }

        virtual std::chrono::duration<double> run () = 0;       
        virtual ~IApp () {}
    };

    template <typename Type>
    class GPUApp final : public IApp<Type> {

        OCL::OclApp<Type> app_;
        using IApp<Type>::sequence_;
        using IApp<Type>::lenght_;
        using IApp<Type>::possibleErase;
        using IApp<Type>::possibleExtention;
     	using merge_t = cl::KernelFunctor<cl::Buffer, cl_int, cl_int>;
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

            cl::Program program (app_.context_, app_.kernel_, true); 
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
    public:
        GPUApp (const std::vector<Type> &seq, std::string kernelPath) : IApp<Type>(seq), app_(kernelPath) {}    

        std::chrono::duration<double> run () override { 

            possibleExtention ();

            auto start = std::chrono::steady_clock::now ();
            parallelSort ();
            auto end = std::chrono::steady_clock::now ();

            possibleErase ();
            return end - start;      
        }
    };

    template <typename Type> 
    class CPUApp final : public IApp<Type> {

        using IApp<Type>::sequence_;
        using IApp<Type>::lenght_;
        using IApp<Type>::possibleErase;
        using IApp<Type>::possibleExtention;

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

        void makeSubSeqBitonic (size_t seqSize, size_t step) { 

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

        CPUApp (const std::vector<Type> &seq) : IApp<Type>(seq) {}  

        std::chrono::duration<double> run () override { 

            possibleExtention ();

            auto start = std::chrono::steady_clock::now ();
            bitonicSort();
            auto end = std::chrono::steady_clock::now ();
            
            possibleErase ();
            return end - start;      
        } 
    };
}

#endif
