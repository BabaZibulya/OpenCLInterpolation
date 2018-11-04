#include "CLBuffer.h"

#include "CL.h"

CLBuffer::CLBuffer(const CLContext& context, BufferType bufferType, size_t size)
{
    cl_int err;
    buffer = clCreateBuffer(context.clContext, static_cast<cl_mem_flags>(bufferType), size, nullptr, &err);
    checkForCLError(err);
}

CLBuffer::~CLBuffer()
{
    clReleaseMemObject(buffer);
}