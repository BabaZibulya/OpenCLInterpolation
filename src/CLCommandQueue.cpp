#include "CLCommandQueue.h"

#include "CL.h"

static cl_device_id deviceId; 

CLCommandQueue::CLCommandQueue(CLCommandQueue&& commandQueue)
{
    this->commandQueue = commandQueue.commandQueue;
    commandQueue.commandQueue = nullptr;
}

CLCommandQueue::CLCommandQueue(const CLContext& context, const CLDevice& device)
{
    cl_int errCode;
    commandQueue = clCreateCommandQueueWithProperties(context.clContext, device.deviceId, 0, &errCode);
    checkForCLError(errCode);
    deviceId = device.deviceId;
}

CLCommandQueue::~CLCommandQueue()
{
    if (commandQueue) {
        clReleaseCommandQueue(commandQueue);
    }
}

void CLCommandQueue::enqueueWriteBuffer(const CLBuffer& buffer, size_t bufferSize, const void *const rawBuffer)
{
    CL(clEnqueueWriteBuffer(commandQueue, buffer.clHandle, CL_TRUE, 0, bufferSize, rawBuffer, 0, nullptr, nullptr));
}

void CLCommandQueue::enqueueReadBuffer(const CLBuffer& buffer, size_t bufferSize, void *const rawBuffer)
{
    CL(clEnqueueReadBuffer(commandQueue, buffer.clHandle, CL_TRUE, 0, bufferSize, rawBuffer, 0, nullptr, nullptr));
}

void CLCommandQueue::enqueueNDRangeKernel(
    const CLProgram::CLKernel& kernel,
    size_t globalSize,
    size_t localWorkSize)
{
    CL(clEnqueueNDRangeKernel(commandQueue, kernel.kernel, 1, nullptr, &globalSize, &localWorkSize, 0, nullptr, nullptr));
}

void CLCommandQueue::enqueueNDRangeKernel(
    const CLProgram::CLKernel& kernel,
    size_t workDim,
    const std::vector<size_t>& globalSize,
    const std::vector<size_t>& localWorkSize)
{
    if (globalSize.size() != workDim || localWorkSize.size() != workDim) {
        throw std::runtime_error("Wrong dimensions of global or local work size");
    }
    CL(clEnqueueNDRangeKernel(commandQueue, kernel.kernel, workDim, nullptr, &globalSize[0], &localWorkSize[0], 0, nullptr, nullptr));
}

void CLCommandQueue::enqueueNDRangeKernel(
    const CLProgram::CLKernel& kernel,
    size_t workDim,
    const std::vector<size_t>& globalSize,
    const std::vector<size_t>& localWorkSize,
    const std::vector<size_t>& offsets
    )
{
    if (globalSize.size() != workDim || localWorkSize.size() != workDim || offsets.size() != workDim) {
        throw std::runtime_error("Wrong dimensions of global or local work size");
    }
    CL(clEnqueueNDRangeKernel(commandQueue, kernel.kernel, workDim, &offsets[0], &globalSize[0], &localWorkSize[0], 0, nullptr, nullptr));
}

void CLCommandQueue::enqueueNDRangeKernel(
    const CLProgram::CLKernel& kernel,
    size_t workDim,
    const std::vector<size_t>& globalSize)
{
    if (globalSize.size() != workDim) {
        throw std::runtime_error("Wrong dimensions of global or local work size");
    }
    CL(clEnqueueNDRangeKernel(commandQueue, kernel.kernel, workDim, nullptr, &globalSize[0], nullptr, 0, nullptr, nullptr));
}

void CLCommandQueue::enqueueNDRangeKernelWithOffset(
    const CLProgram::CLKernel& kernel,
    size_t workDim,
    const std::vector<size_t>& globalSize,
    const std::vector<size_t>& offset
    )
{
    if (globalSize.size() != workDim) {
        throw std::runtime_error("Wrong dimensions of global or local work size");
    }
    CL(clEnqueueNDRangeKernel(commandQueue, kernel.kernel, workDim, &offset[0], &globalSize[0], nullptr, 0, nullptr, nullptr));
}

void CLCommandQueue::finish()
{
    clFinish(commandQueue);
}