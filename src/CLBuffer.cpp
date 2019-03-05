#include "CLBuffer.h"

#include "CL.h"

using namespace CL;

Buffer::Buffer(const Context& context, BufferType bufferType, size_t size)
{
    cl_int err;
    clHandle = clCreateBuffer(context.clHandle, static_cast<cl_mem_flags>(bufferType), size, nullptr, &err);
    checkForCLError(err);
}

Buffer::Buffer(Buffer&& buffer)
{
    this->clHandle = buffer.clHandle;
    buffer.clHandle = nullptr;
}

Buffer::~Buffer()
{
    if (clHandle) {
        clReleaseMemObject(clHandle);
    }
}