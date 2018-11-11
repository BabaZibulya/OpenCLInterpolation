#pragma once

#include <memory>

#include "CLContext.h"
#include "CLDevice.h"
#include "CLTypes.h"
#include "CLBuffer.h"
#include "CLProgram.h"


class CLCommandQueue {
public:
    CLCommandQueue(const CLContext& context, const CLDevice& device);
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

    void finish();
public:
    cl_command_queue commandQueue;
};