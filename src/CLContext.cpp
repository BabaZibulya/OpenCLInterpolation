#include "CLContext.h"

#include <exception>
#include <algorithm>

#include "CL.h"
#include "CLLog.h"
#include "CLUtils.h"

static void logDevices(const std::vector<cl_device_id>& deviceIds)
{
    CLLog("Creating context with ids: ");
    for (const cl_device_id& id: deviceIds) {
        CLLog("\t\t", id);
    }
}

CLContext::CLContext(const CLPlatform& platform, const std::vector<CLDevice>& devices) : CLContext(platform, devices, nullptr)
{}

CLContext::CLContext(const CLPlatform& platform, const std::vector<CLDevice>& devices, CLContext::ErrorCallback errorCallback)
{
    cl_int errCode;
    std::vector<cl_device_id> deviceIds = devicesToDeviceIds(devices);
    logDevices(deviceIds);

    cl_context_properties properties[] = {
        CL_CONTEXT_PLATFORM, (cl_context_properties) platform.platformId,
        0
    };

    clContext = clCreateContext(properties, devices.size(), deviceIds.data(), errorCallback, nullptr, &errCode);
    if (errCode != CL_SUCCESS) {
        CLLog("Error while creating context ", clErrorString(errCode));
        throw std::runtime_error(clErrorString(errCode));
    }
}

CLContext::~CLContext()
{
    clReleaseContext(clContext);
}