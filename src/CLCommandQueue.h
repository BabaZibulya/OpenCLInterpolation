#pragma once

#include <memory>

#include "CLContext.h"
#include "CLDevice.h"
#include "CLTypes.h"
#include "CLBuffer.h"
#include "CLProgram.h"


class CLCommandQueue {
public:
    CLCommandQueue(CLCommandQueue&& commandQueue);
    CLCommandQueue(const CLContext& context, const CLDevice& device);
    CLCommandQueue(const CLCommandQueue&) = delete;
    CLCommandQueue& operator=(const CLCommandQueue&) = delete;
    ~CLCommandQueue();

    void enqueueWriteBuffer(const CLBuffer& buffer, size_t bufferSize, const void *const rawBuffer);
    void enqueueReadBuffer(const CLBuffer& buffer, size_t bufferSize, void *const rawBuffer);
    void enqueueNDRangeKernel(
        const CLProgram::CLKernel& kernel,
        size_t globalSize,
        size_t localWorkSize);
    void enqueueNDRangeKernel(
        const CLProgram::CLKernel& kernel,
        size_t workDim,
        const std::vector<size_t>& globalSize,
        const std::vector<size_t>& localWorkSize);
    void enqueueNDRangeKernel(
        const CLProgram::CLKernel& kernel,
        size_t workDim,
        const std::vector<size_t>& globalSize,
        const std::vector<size_t>& localWorkSize,
        const std::vector<size_t>& offsets);
    void enqueueNDRangeKernel(
        const CLProgram::CLKernel& kernel,
        size_t workDim,
        const std::vector<size_t>& globalSize
    );
    void enqueueNDRangeKernelWithOffset(
        const CLProgram::CLKernel& kernel,
        size_t workDim,
        const std::vector<size_t>& globalSize,
        const std::vector<size_t>& offset
    );

    void finish();
public:
    cl_command_queue commandQueue;
};