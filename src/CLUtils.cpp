#include "CLUtils.h"

#include <algorithm>

#include "CL.h"

std::vector<cl_device_id> devicesToDeviceIds(const std::vector<CLDevice>& devices)
{
    std::vector<cl_device_id> deviceIds(devices.size());
    std::transform(devices.cbegin(), devices.cend(), deviceIds.begin(),
        [](const CLDevice& device) { return device.deviceId; });
    return deviceIds;
}

CLPlatform platformFromId(cl_platform_id platformId)
{
    return CLPlatform(platformId);
}

std::vector<CLPlatform> getAllAvailableCLPlatforms()
{
    std::size_t numberOfPlatforms = numberOfAvailablePlatforms();
    std::vector<cl_platform_id> platformIds(numberOfPlatforms);
    CL(clGetPlatformIDs(static_cast<cl_uint>(numberOfPlatforms), &platformIds[0], nullptr));

    std::vector<CLPlatform> platforms;

    std::transform(platformIds.begin(), platformIds.end(), std::back_inserter(platforms), platformFromId);

    return platforms;
}

std::size_t numberOfAvailablePlatforms()
{
    cl_uint numPlatforms;
    CL(clGetPlatformIDs(0, nullptr, &numPlatforms));
    return numPlatforms;
}