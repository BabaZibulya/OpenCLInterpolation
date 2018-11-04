#include "CLCommandQueue.h"

#include "CL.h"

CLCommandQueue::CLCommandQueue(const CLContext& context, const CLDevice& device)
{
    cl_int errCode;
    commandQueue = clCreateCommandQueueWithProperties(context.clContext, device.deviceId, 0, &errCode);
    checkForCLError(errCode);
}

CLCommandQueue::~CLCommandQueue()
{
    clReleaseCommandQueue(commandQueue);
}

void CLCommandQueue::enqueueWriteBuffer(const CLBuffer& buffer, size_t bufferSize, const void *const rawBuffer)
{
    CL(clEnqueueWriteBuffer(commandQueue, buffer.buffer, CL_TRUE, 0, bufferSize, rawBuffer, 0, nullptr, nullptr));
}

void CLCommandQueue::enqueueReadBuffer(const CLBuffer& buffer, size_t bufferSize, void *const rawBuffer)
{
    CL(clEnqueueReadBuffer(commandQueue, buffer.buffer, CL_TRUE, 0, bufferSize, rawBuffer, 0, nullptr, nullptr));
}

void CLCommandQueue::enqueueNDRangeKernel(
    const CLProgram::CLKernel& kernel,
    size_t work_dim,
    size_t globalSize,
    size_t localWorkSize)
{
    CL(clEnqueueNDRangeKernel(commandQueue, kernel.kernel, 1, nullptr, &globalSize, &localWorkSize, 0, nullptr, nullptr));
}


void CLCommandQueue::finish()
{
    clFinish(commandQueue);
}