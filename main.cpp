#include <vector>
#include <string>
#include <iostream>
#include <memory>

#include "CLPlatform.h"
#include "CLDevice.h"
#include "CLContext.h"
#include "CLCommandQueue.h"
#include "CLProgram.h"
#include "CLUtils.h"

#include "CL/cl.h"

int main() {
    auto platforms = getAllAvailableCLPlatforms();
    for (auto platform : platforms) {
        std::cout << platform.getPlatformInfo() << '\n';
        auto devices = CLDevice::getAllAvailableCLDevices(platform);
        for (auto device : devices) {
            auto result = device.getDeviceInfoDetail(CL_DEVICE_NAME);
            std::cout << "Device vendor: " << result << '\n';
        }
    }

    std::vector<CLDevice> devices = CLDevice::getAllAvailableCLDevices(platforms[0]);
    CLContext context(devices);
    CLCommandQueue commandQueue(context, devices[0]);
    try {
        CLProgram program = CLProgram::compileSources(context, devices, {"kernels/vecAdd.cl"});
    }
    catch (std::exception& e) {
        std::cout << e.what();
    }
    return 0;
}
