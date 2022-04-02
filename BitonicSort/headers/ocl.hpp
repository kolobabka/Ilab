#ifndef __OCL_HPP__
#define __OCL_HPP__
//----------------------------------------
//----------------------------------------
#include <string>
#include <vector>
#include <fstream>
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
#define ANALYZE 0
#endif

#ifdef DEBUG
#define ANALYZE 1
#endif

#define dbgs                                                                   \
  if (!ANALYZE) {                                                              \
  } else                                                                       \
    std::cout
//----------------------------------------
//----------------------------------------
namespace OCL {

    template <typename Type>
    const std::string getFunction ();

    template <> const std::string getFunction<char>             ()                   {return "bitonic_merge_char";  }
    template <> const std::string getFunction<int>              ()                   {return "bitonic_merge_int";   }
    template <> const std::string getFunction<float>            ()                   {return "bitonic_merge_float"; }
    template <> const std::string getFunction<double>            ()                   {return "bitonic_merge_float"; }

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
#endif
