#include <vector>
#include <string>
#include <iostream>
#include <memory>

#include "CLPlatform.h"
#include "CLDevice.h"

#include "CL/cl.h"

int main() {
    auto platforms = CLPlatform::getAllAvailableCLPlatforms();
    for (auto platform : platforms) {
        std::cout << platform.getPlatformInfo() << '\n';
        auto devices = CLDevice::getAllAvailableCLDevices(platform);
        for (auto device : devices) {
            auto result = device.getDeviceInfoDetail(CL_DEVICE_NAME);
            std::cout << "Device vendor: " << result << '\n';
        }
    }
    return 0;
}
