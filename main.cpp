#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include <cmath>
#include <memory>

#include "CLPlatform.h"
#include "CLBuffer.h"
#include "CLDevice.h"
#include "CLContext.h"
#include "CLCommandQueue.h"
#include "CLProgram.h"
#include "CLUtils.h"

size_t lengthOfArr = 10000;
size_t sizeOfArr = lengthOfArr * sizeof(float);

std::unique_ptr<float[]> bufferA() {
    auto p = std::make_unique<float[]>(lengthOfArr);
    for (size_t i = 0; i < lengthOfArr; ++i) {
        p[i] = static_cast<float>(i);
    }
    return p;
}


std::unique_ptr<float[]> bufferB() {
    auto p = std::make_unique<float[]>(lengthOfArr);
    for (size_t i = 0; i < lengthOfArr; ++i) {
        p[i] = -static_cast<float>(i);
    }
    return p;
}

std::unique_ptr<float[]> bufferC() {
    return std::make_unique<float[]>(lengthOfArr);
}

int main() {
    auto platforms = getAllAvailableCLPlatforms();

    std::vector<CLDevice> devices = CLDevice::getAllAvailableCLDevices(platforms[0]);
    CLContext context(devices);
    CLCommandQueue commandQueue(context, devices[0]);
    try {
        CLProgram program = CLProgram::compileSources(context, devices, {"kernels/vecAdd.cl"});
        CLProgram::CLKernel kernel = program.createKernel("vecAdd");

        CLBuffer a(context, CLBuffer::BufferType::CL_MEM_READ_ONLY, sizeOfArr);
        CLBuffer b(context, CLBuffer::BufferType::CL_MEM_READ_ONLY, sizeOfArr);
        CLBuffer c(context, CLBuffer::BufferType::CL_MEM_WRITE_ONLY, sizeOfArr);

        auto rawA = bufferA();
        auto rawB = bufferB();
        commandQueue.enqueueWriteBuffer(a, sizeOfArr, rawA.get());
        commandQueue.enqueueWriteBuffer(b, sizeOfArr, rawB.get());

        kernel.setKernelArg(0, a);
        kernel.setKernelArg(1, b);
        kernel.setKernelArg(2, c);
        kernel.setKernelArg(3, static_cast<unsigned int>(lengthOfArr));

        size_t localSize = 64;
        commandQueue.enqueueNDRangeKernel(kernel, 1, ceil(lengthOfArr/(float)localSize)*localSize, localSize);

        commandQueue.finish();

        auto rawC = bufferC();
        commandQueue.enqueueReadBuffer(c, sizeOfArr, rawC.get());
        for (size_t i = 0; i < lengthOfArr; ++i) {
            if (rawC[i] != 0.0) {
                std::cout << "Error: " << rawC[i] << '\n';
            }
        }
    }
    catch (std::exception& e) {
        std::cout << e.what();
        return -1;
    }

    return 0;
}
