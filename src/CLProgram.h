#pragma once

#include <vector>
#include <string>
#include <type_traits>

#include "CLContext.h"
#include "CLTypes.h"
#include "CLBuffer.h"

class CLProgram
{
public:
    CLProgram(
        const CLContext& context,
        const std::vector<CLDevice>& devices,
        const std::vector<std::string>& sources);
    ~CLProgram();

    static CLProgram compileSources(
        const CLContext& context,
        const std::vector<CLDevice>& devices,
        const std::vector<std::string>& pathes);

public:
    cl_program program;

public:
    struct CLKernel {
        CLKernel(const CLProgram& program, const std::string& kernelName);
        ~CLKernel();

        template<typename ArgType>
        void setKernelArg(size_t argPosition, const ArgType& arg, typename std::enable_if<std::is_arithmetic<ArgType>::value>::type* = 0)
        {
            setKernelArg(argPosition, sizeof(arg), const_cast<ArgType*>(&arg));
        }
        void setKernelArg(size_t argPosition, const CLBuffer& arg);
        void setKernelArg(size_t argPosition, size_t size, void* arg);

        cl_kernel kernel;
    };

    CLKernel createKernel(const std::string& kernelName);

private:
    std::string compilationLog(const CLDevice& device);
};