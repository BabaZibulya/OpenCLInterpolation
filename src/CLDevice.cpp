#include "CLDevice.h"

#include <algorithm>

#include "CL.h"

CLDevice::CLDevice(cl_device_id deviceId) :
    deviceId(deviceId)
{}

std::size_t CLDevice::numberOfAvailableCLDevices(const CLPlatform& platform, DeviceType deviceType)
{
    cl_uint numberOfDevices;
    CL(clGetDeviceIDs(platform.platformId, static_cast<unsigned>(deviceType), 0, nullptr, &numberOfDevices));
    return numberOfDevices;
}

std::vector<CLDevice> CLDevice::getAllAvailableCLDevices(const CLPlatform& platform, DeviceType deviceType)
{
    std::size_t numberOfDevices = numberOfAvailableCLDevices(platform, deviceType);
    std::vector<cl_device_id> deviceIds(numberOfDevices);
    CL(clGetDeviceIDs(platform.platformId, static_cast<unsigned>(deviceType), static_cast<cl_uint>(numberOfDevices), &deviceIds[0], nullptr));
    std::vector<CLDevice> devices;
    std::transform(deviceIds.begin(), deviceIds.end(), std::back_inserter(devices), [](cl_device_id deviceId) { return CLDevice(deviceId); });
    return devices;
}

std::size_t CLDevice::getDeviceInfoDetailSize(unsigned int detailInd) const
{
    std::size_t sizeOfDetail;
    CL(clGetDeviceInfo(deviceId, detailInd, 0, nullptr, &sizeOfDetail));
    return sizeOfDetail;
}

void CLDevice::getDeviceInfoDetail(unsigned int detailInd, std::size_t sizeOfOutput, void *output) const
{
    clGetDeviceInfo(deviceId, detailInd, sizeOfOutput, output, nullptr);
}

std::string CLDevice::getDeviceInfoDetail(unsigned int detailInd) const
{
    std::size_t detailSize = getDeviceInfoDetailSize(detailInd);
    std::string result(detailSize, '\0');
    getDeviceInfoDetail(detailInd, detailSize, &result[0]);
    return result;
}
