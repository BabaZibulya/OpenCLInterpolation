#include "CLUtils.h"

#include <algorithm>

#include "CL.h"

using namespace CL;

std::vector<cl_device_id> CL::devicesToDeviceIds(const std::vector<Device>& devices)
{
    std::vector<cl_device_id> deviceIds(devices.size());
    std::transform(devices.cbegin(), devices.cend(), deviceIds.begin(),
        [](const Device& device) { return device.deviceId; });
    return deviceIds;
}

Platform platformFromId(cl_platform_id platformId)
{
    return CL::Platform(platformId);
}

std::vector<Platform> CL::getAllAvailableCLPlatforms()
{
    std::size_t numberOfPlatforms = numberOfAvailablePlatforms();
    std::vector<cl_platform_id> platformIds(numberOfPlatforms);
    CL(clGetPlatformIDs(static_cast<cl_uint>(numberOfPlatforms), &platformIds[0], nullptr));

    std::vector<Platform> platforms;

    std::transform(platformIds.begin(), platformIds.end(), std::back_inserter(platforms), platformFromId);

    return platforms;
}

std::size_t CL::numberOfAvailablePlatforms()
{
    cl_uint numPlatforms;
    CL(clGetPlatformIDs(0, nullptr, &numPlatforms));
    return numPlatforms;
}