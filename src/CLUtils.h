#pragma once

#include <vector>

#include "CLDevice.h"
#include "CLPlatform.h"

std::vector<cl_device_id> devicesToDeviceIds(const std::vector<CLDevice>& devices);
std::vector<CLDevice> getAllDevices();
std::vector<CLPlatform> getAllAvailableCLPlatforms();
std::size_t numberOfAvailablePlatforms();
