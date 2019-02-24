#include "CLBuffer.h"

#include "CL.h"

CLBuffer::CLBuffer(const CLContext& context, BufferType bufferType, size_t size)
{
    cl_int err;
    clHandle = clCreateBuffer(context.clContext, static_cast<cl_mem_flags>(bufferType), size, nullptr, &err);
    checkForCLError(err);
}

CLBuffer::CLBuffer(CLBuffer&& buffer)
{
    this->clHandle = buffer.clHandle;
    buffer.clHandle = nullptr;
}

CLBuffer::~CLBuffer()
{
    if (clHandle) {
        clReleaseMemObject(clHandle);
    }
}