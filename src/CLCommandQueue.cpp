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