#pragma once

#include "CLContext.h"
#include "CLDevice.h"
#include "CLTypes.h"

class CLCommandQueue {
public:
    CLCommandQueue(const CLContext& context, const CLDevice& device);
    ~CLCommandQueue();
public:
    cl_command_queue commandQueue;
};