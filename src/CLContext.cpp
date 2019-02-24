#include "CLContext.h"

#include <exception>
#include <algorithm>

#include "CL.h"
#include "CLLog.h"
#include "CLUtils.h"

CLContext::CLContext(CLContext&& context)
{
    this->clContext = context.clContext;
    context.clContext = nullptr;
}

CLContext::CLContext(const CLPlatform& platform, const std::vector<CLDevice>& devices) : CLContext(platform, devices, nullptr)
{}

CLContext::CLContext(const CLPlatform& platform, const std::vector<CLDevice>& devices, CLContext::ErrorCallback errorCallback)
{
    cl_int errCode;
    std::vector<cl_device_id> deviceIds = devicesToDeviceIds(devices);

    cl_context_properties properties[] = {
        CL_CONTEXT_PLATFORM, (cl_context_properties) platform.platformId,
        0
    };

    clContext = clCreateContext(properties, devices.size(), deviceIds.data(), errorCallback, nullptr, &errCode);
    if (errCode != CL_SUCCESS) {
        CLLog("Error while creating context ", errCode, " ", clErrorString(errCode));
        throw std::runtime_error(clErrorString(errCode));
    }
}

CLContext::~CLContext()
{
    if (clContext) {
        clReleaseContext(clContext);
    }
}