#include "CLBuffer.h"

#include "CL.h"

using namespace CL;

Buffer::Buffer(const Context& context, BufferType bufferType, size_t size)
{
    cl_int err;
    //cl_mem_ext_ptr_t d_bank0_ext;
    //d_bank0_ext.flags = XCL_MEM_DDR_BANK1;
    //d_bank0_ext.obj = nullptr;
    //d_bank0_ext.param = 0;
    //clHandle = clCreateBuffer(context.clHandle, static_cast<cl_mem_flags>(bufferType) | CL_MEM_EXT_PTR_XILINX, size, &d_bank0_ext, &err);
    clHandle = clCreateBuffer(context.clHandle, static_cast<cl_mem_flags>(bufferType) , size, nullptr, &err);
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
