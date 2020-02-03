#include "CLProgram.h"

#include <algorithm>
#include <exception>
#include <fstream>

#include "CL.h"
#include "CLUtils.h"

using namespace CL;

Program::Program(
    const Context& context,
    const std::vector<Device>& devices,
    const std::vector<std::string>& sources)
{
    std::vector<const unsigned char*> cSources(sources.size());
    std::transform(sources.cbegin(), sources.cend(), cSources.begin(), [](const std::string& source) { return (const unsigned char*)source.c_str(); });
    std::vector<size_t> lengths(sources.size());
    std::transform(sources.cbegin(), sources.cend(), lengths.begin(), [](const std::string& source) { return source.length(); });
    cl_int errCode, binStatus;
    program = clCreateProgramWithBinary(context.clHandle, devices.size(), &devicesToDeviceIds(devices)[0],&lengths[0], &cSources[0], &errCode, &binStatus);
    checkForCLError(errCode);

    auto result = clBuildProgram(program, devices.size(), &devicesToDeviceIds(devices)[0], "-w", nullptr, nullptr);
    if (result != CL_SUCCESS) {
        for (auto& device: devices) {
            std::cout << device.getDeviceInfoDetail(CL_DEVICE_NAME) << " program build log:" << '\n'
                      << compilationLog(device);
        }
    }
}

Program::Program(Program&& program)
{
    this->program = program.program;
    program.program = nullptr;
}

Program::~Program()
{
    if (program) {
        clReleaseProgram(program);
    }
}

Program Program::compileSources(
    const Context& context,
    const std::vector<Device>& devices,
    const std::vector<std::string>& pathes)
{
    std::vector<std::string> sources(pathes.size());
    std::transform(pathes.cbegin(), pathes.cend(), sources.begin(), [](const std::string& path) {
        std::ifstream file(path);
        if (file.is_open()) {
            std::string source((std::istreambuf_iterator<char>(file)),
                 std::istreambuf_iterator<char>());
            return source;
        }
        else {
            throw std::runtime_error("File " + path + " could not be opened");
        }
    });
    return Program(context, devices, sources);
}

std::string Program::compilationLog(const Device& device)
{
    size_t compilationLogSize;
    clGetProgramBuildInfo(program, device.deviceId, CL_PROGRAM_BUILD_LOG, 0, nullptr, &compilationLogSize);
    std::string log(compilationLogSize + 1, ' ');
    clGetProgramBuildInfo(program, device.deviceId, CL_PROGRAM_BUILD_LOG, compilationLogSize, &log[0], nullptr);
    return log;
}


Program::Kernel::Kernel(const Program& program, const std::string& kernelName)
{
    cl_int err;
    kernel = clCreateKernel(program.program, kernelName.c_str(), &err);
    checkForCLError(err);
}

Program::Kernel::Kernel(Kernel&& kernel)
{
    this->kernel = kernel.kernel;
    kernel.kernel = nullptr;
}

Program::Kernel::~Kernel()
{
    if (kernel) {
        clReleaseKernel(kernel);
    }
}

void Program::Kernel::setKernelArg(size_t argPosition, const Buffer& buffer)
{
    setKernelArg(argPosition, sizeof(buffer.clHandle), const_cast<cl_mem*>(&(buffer.clHandle)));
}

void Program::Kernel::setKernelArg(size_t argPosition, size_t size, void* arg)
{
    CL(clSetKernelArg(kernel, argPosition, size, arg));
}

Program::Kernel Program::createKernel(const std::string& kernelName)
{
    return Kernel(*this, kernelName);
}
