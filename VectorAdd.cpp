#include "VectorAdd.h"

#include <random>

#include "CLLog.h"
#include "CLContext.h"
#include "CLCommandQueue.h"
#include "CLDevice.h"
#include "CLPlatform.h"
#include "TimeUtils.h"
#include "NDimContiniousArray.h"
#include "CLUtils.h"

VectorAdd::VectorAdd(size_t size): size(size) {
}

void VectorAdd::solve() {
    auto platforms = CL::getAllAvailableCLPlatforms();
    CLLog("Found ", platforms.size(), " platforms");
    for (auto& platform : platforms) {
        CLLog(platform.getPlatformInfo());
    }
    CL::Platform platform = platforms[0];
    CL::Device dev = CL::Device::getAllAvailableCLDevices(platform)[0];
    auto platformName = platform.getPlatformInfoDetail(CL::Platform::CL_PLATFORM_NAME);
    CL::Context context(platform, { dev });
    CL::CommandQueue commandQueue(context, dev);

    CL::Program program = CL::Program::createProgram(context, { dev }, { "C:\\Users\\Bondarenko-PC\\Documents\\Projects\\OpenCLInterpolation\\kernels\\vecAdd.cl" }, CL::Program::ProgramType::SOURCE);
    //CL::Program program = CL::Program::createProgram(context, { dev }, { "kernels/hw/vecAdd.xclbin" }, CL::Program::ProgramType::BINARY);
    CL::Program::Kernel kernel = program.createKernel("vecAdd");

    std::random_device rd;
    std::mt19937 e2(rd());
    std::uniform_real_distribution<> dist(0, 10);

    OneDimContiniousArray<float> Vec1(size);
    Vec1.fill([&](unsigned i)
    {
        return dist(e2);
    });

    OneDimContiniousArray<float> Vec2(size);
    Vec2.fill([&](unsigned i)
    {
        return dist(e2);
    });

    OneDimContiniousArray<float> Res(size);

    CL::Buffer Vec1Buffer(context, CL::Buffer::BufferType::MEM_READ_ONLY, Vec1.getRawSize());
    CL::Buffer Vec2Buffer(context, CL::Buffer::BufferType::MEM_READ_ONLY, Vec2.getRawSize());
    CL::Buffer ResBuffer(context, CL::Buffer::BufferType::MEM_WRITE_ONLY, Res.getRawSize());

    try {
        measureTime(platformName + " OpenCL writing inputs ", [&]() {
            commandQueue.enqueueWriteBuffer(Vec1Buffer, Vec1.getRawSize(), Vec1.getData());
            commandQueue.enqueueWriteBuffer(Vec2Buffer, Vec2.getRawSize(), Vec2.getData());
        });

        /*
        __kernel void vecAdd(
            __global float *a,
            __global float *b,
            __global float *c,
            const unsigned int n
        )
        {
            int id = get_global_id(0);
            if (id < n)
                c[id] = a[id] + b[id];
        }
        */

        kernel.setKernelArg(0, Vec1Buffer);
        kernel.setKernelArg(1, Vec2Buffer);
        kernel.setKernelArg(2, ResBuffer);
        kernel.setKernelArg(3, (unsigned)size);
    }
    catch (std::exception & e) {
        CLLog("Got an exception: ", e.what());
        return;
    }

    measureTime(platformName + " OpenCL ", [&]() {
        commandQueue.enqueueNDRangeKernel(
            kernel, 1,
            CL::CommandQueue::GlobalSize{ {size} });
        commandQueue.finish();
    });

    measureTime(platformName + " OpenCL reading results ", [&]() {
        commandQueue.enqueueReadBuffer(ResBuffer, Res.getRawSize(), Res.getData());
    });

    float meanCpu = 0.f, meanOpenCL = 0.f;
    for (size_t i = 0; i < size; ++i) {
        meanCpu += Vec1.at(i) + Vec2.at(i);
        meanOpenCL += Res.at(i);
    }

    CLLog("Mean CPU = ", meanCpu);
    CLLog("Mean OpenCL = ", meanOpenCL);
}
