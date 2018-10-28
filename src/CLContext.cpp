#include "CLContext.h"

#include <exception>
#include <algorithm>

#include "CL.h"
#include "CLUtils.h"

CLContext::CLContext(const std::vector<CLDevice>& devices)
{
    cl_int errCode;
    std::vector<cl_device_id> deviceIds = devicesToDeviceIds(devices); 
    clContext = clCreateContext(nullptr, devices.size(), deviceIds.data(), nullptr, nullptr, &errCode);
    if (errCode != CL_SUCCESS) {
        throw std::runtime_error(clErrorString(errCode));
    }
}

CLContext::CLContext(const std::vector<CLDevice>& devices, const CLContext::ErrorCallback& errorCallback)
{
    cl_int errCode;
    std::vector<cl_device_id> deviceIds = devicesToDeviceIds(devices);
    clContext = clCreateContext(nullptr, devices.size(), deviceIds.data(), errorCallback, nullptr, &errCode);
    if (errCode != CL_SUCCESS) {
        throw std::runtime_error(clErrorString(errCode));
    }
}

CLContext::~CLContext()
{
    clReleaseContext(clContext);
}