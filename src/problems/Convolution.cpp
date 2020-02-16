#include "Convolution.h"

#include <array>
#include <vector>
#include <cassert>
#include <cstring>

#include "CLLog.h"
#include "CLContext.h"
#include "CLCommandQueue.h"
#include "CLDevice.h"
#include "CLPlatform.h"
#include "TimeUtils.h"
#include "NDimContiniousArray.h"
#include "CLUtils.h"

Convolution::Convolution(const std::string &path) : image(path) {
}

void Convolution::solve() {
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

    //CL::Program program = CL::Program::createProgram(context, { dev }, { "C:\\Users\\Bondarenko-PC\\Documents\\Projects\\OpenCLInterpolation\\kernels\\convolution.cl" }, CL::Program::ProgramType::SOURCE);
    CL::Program program = CL::Program::createProgram(context, { dev }, { "kernels/hw/convolution.xclbin" }, CL::Program::ProgramType::BINARY);
    CL::Program::Kernel kernel = program.createKernel("convolution");

    size_t totalSize = 3 * image.getHeight() * image.getWidth();
    OneDimContiniousArray<unsigned char> Res(totalSize);

    CL::Buffer srcBuffer(context, CL::Buffer::BufferType::MEM_READ_ONLY, Res.getRawSize());
    CL::Buffer dstBuffer(context, CL::Buffer::BufferType::MEM_WRITE_ONLY, Res.getRawSize());

    constexpr size_t kernelSize = 5;

    constexpr size_t kernelSizeRaw = kernelSize * kernelSize * sizeof(float);
    CL::Buffer kernelBuffer(context, CL::Buffer::BufferType::MEM_READ_ONLY, kernelSizeRaw);
    //std::vector<float> kernelArg{ 0, -1, 0, -1, 5, -1, 0, -1, 0 };
    //std::vector<float> kernelArg{ -1.f, -1.f, -1.f, -1.f, 8.f, -1.f, -1.f, -1.f, -1.f };
    //std::vector<float> kernelArg{ 1.f / 16.f, 1.f / 8.f, 1.f / 16.f, 1.f / 8.f, 1.f / 4.f, 1.f / 8.f, 1.f / 16.f, 1.f / 8.f, 1.f / 16.f };
    //std::vector<float> kernelArg{ 0, 0, 0, 0, 1, 0, 0, 0, 0 };
    std::vector<float> kernelArg{ 1.f / 256.f, 4.f / 256.f, 6.f / 256.f, 4.f / 256.f, 1.f / 256.f,
                                  4.f / 256.f, 16.f / 256.f, 24.f / 256.f, 16.f / 256.f, 4.f / 256.f,
                                  6.f / 256.f, 24.f / 256.f, 36.f / 256.f, 24.f / 256.f, 6.f / 256.f,
                                  4.f / 256.f, 16.f / 256.f, 24.f / 256.f, 16.f / 256.f, 4.f / 256.f,
                                  1.f / 256.f, 4.f / 256.f, 6.f / 256.f, 4.f / 256.f, 1.f / 256.f };

    try {
        measureTime(platformName + " OpenCL writing inputs ", [&]() {
            commandQueue.enqueueWriteBuffer(srcBuffer, totalSize, image.getBuffer().get());
            commandQueue.enqueueWriteBuffer(kernelBuffer, kernelSizeRaw, kernelArg.data());
        });

        /*
        __kernel void convolution(
            __global unsigned char *src,
            __global unsigned char *dst,
            const unsigned int width,
            const unsigned int height,
            __global float *kern,
            const unsigned int kernel_size)
        */

        kernel.setKernelArg(0, srcBuffer);
        kernel.setKernelArg(1, dstBuffer);
        kernel.setKernelArg(2, image.getWidth());
        kernel.setKernelArg(3, image.getHeight());
        kernel.setKernelArg(4, kernelBuffer);
        kernel.setKernelArg(5, (unsigned)kernelSize);
    }
    catch (std::exception & e) {
        CLLog("Got an exception: ", e.what());
        return;
    }

    measureTime(platformName + " OpenCL ", [&]() {
        commandQueue.enqueueNDRangeKernel(
            kernel, 2,
            CL::CommandQueue::GlobalSize{ {image.getWidth(), image.getHeight()} });
        commandQueue.finish();
    });

    utils::BMPImage copy(image);
    measureTime(platformName + " OpenCL reading results ", [&]() {
        commandQueue.enqueueReadBuffer(dstBuffer, Res.getRawSize(), copy.getBuffer().get());
    });
    copy.save("C:\\Users\\Bondarenko-PC\\Documents\\Projects\\OpenCLInterpolation\\img\\airplane_gpu.bmp");

    solveCpu();
}

int coord(unsigned int i, unsigned int j, unsigned int width, int channel) {
    return 3 * (i + j * width) + channel;
}

int clamp(int val, int min, int max) {
    return std::min(std::max(val, min), max);
}

void convolutionImpl(
    unsigned char *src,
    unsigned char *dst,
    const unsigned int i,
    const unsigned int j,
    const unsigned int width,
    const unsigned int height,
    float *kern,
    const unsigned int kernel_size)
{
    char resultR = 0;
    char resultG = 0;
    char resultB = 0;

    unsigned char radius = kernel_size / 2;

    for (int ki = 0; ki < kernel_size; ++ki) {
        for (int kj = 0; kj < kernel_size; ++kj) {
            int kernel_el = ki + kj * kernel_size;

            int coordI = clamp(i + ki - radius, 0, (int)width);
            int coordJ = clamp(j + kj - radius, 0, (int)height);

            resultR += src[coord(coordI, coordJ, width, 0)] * kern[kernel_el];
            resultG += src[coord(coordI, coordJ, width, 1)] * kern[kernel_el];
            resultB += src[coord(coordI, coordJ, width, 2)] * kern[kernel_el];
        }
    }

    dst[coord(i, j, width, 0)] = resultR;
    dst[coord(i, j, width, 1)] = resultG;
    dst[coord(i, j, width, 2)] = resultB;
    auto c = coord(i, j, width, 2);
    assert(c < width * height * 3);
}

void Convolution::solveCpu() {
    utils::BMPImage copy(image);
    int width = copy.getWidth();
    int height = copy.getHeight();

    unsigned char *src = image.getBuffer().get();
    unsigned char *dst = copy.getBuffer().get();

    //constexpr size_t kernelSize = 3;
    //std::vector<float> kernelArg{ -1.f, -1.f, -1.f, -1.f, 8.f, -1.f, -1.f, -1.f, -1.f };
    //std::vector<float> kernelArg{ 1.f / 9.f, 1.f / 9.f, 1.f / 9.f, 1.f / 9.f, 1.f / 9.f, 1.f / 9.f, 1.f / 9.f, 1.f / 9.f, 1.f / 9.f };
    //std::vector<float> kernelArg{ 1.f / 16.f, 1.f / 8.f, 1.f / 16.f, 1.f / 8.f, 1.f / 4.f, 1.f / 8.f, 1.f / 16.f, 1.f / 8.f, 1.f / 16.f };

    constexpr size_t kernelSize = 5;
    std::vector<float> kernelArg{ 1.f / 256.f, 4.f / 256.f, 6.f / 256.f, 4.f / 256.f, 1.f / 256.f,
                                  4.f / 256.f, 16.f / 256.f, 24.f / 256.f, 16.f / 256.f, 4.f / 256.f, 
                                  6.f / 256.f, 24.f / 256.f, 36.f / 256.f, 24.f / 256.f, 6.f / 256.f, 
                                  4.f / 256.f, 16.f / 256.f, 24.f / 256.f, 16.f / 256.f, 4.f / 256.f,
                                  1.f / 256.f, 4.f / 256.f, 6.f / 256.f, 4.f / 256.f, 1.f / 256.f };

    measureTime(" CPU convolution time", [&]() {
        for (int j = 0; j < height; ++j) {
            for (int i = 0; i < width; ++i) {
                convolutionImpl(src, dst, i, j, width, height, kernelArg.data(), kernelSize);
            }
        }
    });

    std::memcpy(image.getBuffer().get(), copy.getBuffer().get(), width * height * 3);

    image.save("C:\\Users\\Bondarenko-PC\\Documents\\Projects\\OpenCLInterpolation\\img\\airplane_cpu.bmp");
}
