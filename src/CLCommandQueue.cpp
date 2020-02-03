#include "CLCommandQueue.h"

#include "CL.h"

using namespace CL;

CommandQueue::CommandQueue(CommandQueue&& commandQueue)
{
    this->clHandle = commandQueue.clHandle;
    commandQueue.clHandle = nullptr;
}

CommandQueue::CommandQueue(const Context& context, const Device& device)
{
    cl_int errCode;
    clHandle = clCreateCommandQueue(context.clHandle, device.deviceId,0 & CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE, &errCode);
    checkForCLError(errCode);
}

CommandQueue::~CommandQueue()
{
    if (clHandle) {
        clReleaseCommandQueue(clHandle);
    }
}

void CommandQueue::enqueueWriteBuffer(const Buffer& buffer, size_t bufferSize, const void *const rawBuffer)
{
    CL(clEnqueueWriteBuffer(clHandle, buffer.clHandle, CL_TRUE, 0, bufferSize, rawBuffer, 0, nullptr, nullptr));
}

void CommandQueue::enqueueReadBuffer(const Buffer& buffer, size_t bufferSize, void *const rawBuffer)
{
    CL(clEnqueueReadBuffer(clHandle, buffer.clHandle, CL_TRUE, 0, bufferSize, rawBuffer, 0, nullptr, nullptr));
}

void CommandQueue::enqueueNDRangeKernel(
    const Program::Kernel& kernel,
    size_t globalSize,
    size_t localWorkSize)
{
    CL(clEnqueueNDRangeKernel(clHandle, kernel.kernel, 1, nullptr, &globalSize, &localWorkSize, 0, nullptr, nullptr));
}

void CommandQueue::enqueueNDRangeKernel(
    const Program::Kernel& kernel,
    size_t workDim,
    const GlobalSize& globalSize,
    const LocalSize& localWorkSize)
{
    if (globalSize.get().size() != workDim || localWorkSize.get().size() != workDim) {
        throw std::runtime_error("Wrong dimensions of global or local work size");
    }
    CL(clEnqueueNDRangeKernel(clHandle, kernel.kernel, workDim, nullptr, &globalSize.get()[0], &localWorkSize.get()[0], 0, nullptr, nullptr));
}

void CommandQueue::enqueueNDRangeKernel(
    const Program::Kernel& kernel,
    size_t workDim,
    const GlobalSize& globalSize,
    const LocalSize& localWorkSize,
    const Offset& offsets
    )
{
    if (globalSize.get().size() != workDim || localWorkSize.get().size() != workDim || offsets.get().size() != workDim) {
        throw std::runtime_error("Wrong dimensions of global or local work size");
    }
    CL(clEnqueueNDRangeKernel(clHandle, kernel.kernel, workDim, &offsets.get()[0], &globalSize.get()[0], &localWorkSize.get()[0], 0, nullptr, nullptr));
}

void CommandQueue::enqueueNDRangeKernel(
    const Program::Kernel& kernel,
    size_t workDim,
    const GlobalSize& globalSize)
{
    if (globalSize.get().size() != workDim) {
        throw std::runtime_error("Wrong dimensions of global or local work size");
    }
    CL(clEnqueueNDRangeKernel(clHandle, kernel.kernel, workDim, nullptr, &globalSize.get()[0], nullptr, 0, nullptr, nullptr));
}

void CommandQueue::enqueueNDRangeKernel(
    const Program::Kernel& kernel,
    size_t workDim,
    const GlobalSize& globalSize,
    const Offset& offset
    )
{
    if (globalSize.get().size() != workDim) {
        throw std::runtime_error("Wrong dimensions of global or local work size");
    }
    CL(clEnqueueNDRangeKernel(clHandle, kernel.kernel, workDim, &offset.get()[0], &globalSize.get()[0], nullptr, 0, nullptr, nullptr));
}

void CommandQueue::enqueueTask(const Program::Kernel& kernel)
{
    CL(clEnqueueTask(clHandle, kernel.kernel, 0, nullptr, nullptr));
}
void CommandQueue::finish()
{
    clFinish(clHandle);
}
