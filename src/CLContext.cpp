#include "CLContext.h"

#include <exception>
#include <algorithm>

#include "CL.h"
#include "CLLog.h"
#include "CLUtils.h"

using namespace CL;

Context::Context(Context&& context)
{
    this->clHandle = context.clHandle;
    context.clHandle = nullptr;
}

Context::Context(const Platform& platform, const std::vector<Device>& devices) : Context(platform, devices, nullptr)
{}

Context::Context(const Platform& platform, const std::vector<Device>& devices, Context::ContextErrorCallback errorCallback)
{
    cl_int errCode;
    std::vector<cl_device_id> deviceIds = devicesToDeviceIds(devices);

    cl_context_properties properties[] = {
        CL_CONTEXT_PLATFORM, (cl_context_properties) platform.clHandle,
        0
    };

    clHandle = clCreateContext(properties, devices.size(), deviceIds.data(), errorCallback, nullptr, &errCode);
    if (errCode != CL_SUCCESS) {
        CLLog("Error while creating context ", errCode, " ", clErrorString(errCode));
        throw std::runtime_error(clErrorString(errCode));
    }
}

Context::~Context()
{
    if (clHandle) {
        clReleaseContext(clHandle);
    }
}