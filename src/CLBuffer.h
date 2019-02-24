#pragma once

#include "CLTypes.h"
#include "CLContext.h"

class CLBuffer
{
public:
    /*
     * #define CL_MEM_READ_WRITE                           (1 << 0)
     * #define CL_MEM_WRITE_ONLY                           (1 << 1)
     * #define CL_MEM_READ_ONLY                            (1 << 2)
     * #define CL_MEM_USE_HOST_PTR                         (1 << 3)
     * #define CL_MEM_ALLOC_HOST_PTR                       (1 << 4)
     * #define CL_MEM_COPY_HOST_PTR                        (1 << 5)
     */
    enum class BufferType {
         MEM_READ_WRITE                            = (1 << 0),
         MEM_WRITE_ONLY                            = (1 << 1),
         MEM_READ_ONLY                             = (1 << 2),
         MEM_USE_HOST_PTR                          = (1 << 3),
         MEM_ALLOC_HOST_PTR                        = (1 << 4),
         MEM_COPY_HOST_PTR                         = (1 << 5)
    };
public:
    CLBuffer(const CLContext& context, BufferType bufferType, size_t size);
    CLBuffer(CLBuffer&& buffer);
    CLBuffer(const CLBuffer&) = delete;
    CLBuffer& operator=(const CLBuffer&) = delete;
    ~CLBuffer();

public:
    cl_mem clHandle;
};