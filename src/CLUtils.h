#pragma once

#include <vector>

#include "CLDevice.h"
#include "CLPlatform.h"

namespace CL {
	std::vector<cl_device_id> devicesToDeviceIds(const std::vector<Device>& devices);
	std::vector<Device> getAllDevices();
	std::vector<Platform> getAllAvailableCLPlatforms();
	std::size_t numberOfAvailablePlatforms();
}