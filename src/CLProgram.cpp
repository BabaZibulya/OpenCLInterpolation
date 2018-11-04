#include "CLProgram.h"

#include <algorithm>
#include <exception>
#include <fstream>

#include "CL.h"
#include "CLUtils.h"

CLProgram::CLProgram(
    const CLContext& context,
    const std::vector<CLDevice>& devices,
    const std::vector<std::string>& sources)
{
    std::vector<const char*> cSources(sources.size());
    std::transform(sources.cbegin(), sources.cend(), cSources.begin(), [](const std::string& source) { return source.c_str(); });
    std::vector<size_t> lengths(sources.size());
    std::transform(sources.cbegin(), sources.cend(), lengths.begin(), [](const std::string& source) { return source.length(); });
    cl_int errCode;
    program = clCreateProgramWithSource(context.clContext, cSources.size(), &cSources[0], &lengths[0], &errCode);
    checkForCLError(errCode);

    auto result = clBuildProgram(program, devices.size(), &devicesToDeviceIds(devices)[0], "-w", nullptr, nullptr);
    if (result != CL_SUCCESS) {
        for (auto& device: devices) {
            std::cout << device.getDeviceInfoDetail(CL_DEVICE_NAME) << " program build log:" << '\n'
                      << compilationLog(device);
        }
    }
}

CLProgram::~CLProgram()
{
    clReleaseProgram(program);
}

CLProgram CLProgram::compileSources(
    const CLContext& context,
    const std::vector<CLDevice>& devices,
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
    return CLProgram(context, devices, sources);
}

std::string CLProgram::compilationLog(const CLDevice& device)
{
    size_t compilationLogSize;
    clGetProgramBuildInfo(program, device.deviceId, CL_PROGRAM_BUILD_LOG, 0, nullptr, &compilationLogSize);
    std::string log(compilationLogSize + 1, ' ');
    clGetProgramBuildInfo(program, device.deviceId, CL_PROGRAM_BUILD_LOG, compilationLogSize, &log[0], nullptr);
    return log;
}


CLProgram::CLKernel::CLKernel(const CLProgram& program, const std::string& kernelName)
{
    cl_int err;
    kernel = clCreateKernel(program.program, kernelName.c_str(), &err);
    checkForCLError(err);
}

CLProgram::CLKernel::~CLKernel()
{
    clReleaseKernel(kernel);
}

void CLProgram::CLKernel::setKernelArg(size_t argPosition, const CLBuffer& buffer)
{
    setKernelArg(argPosition, sizeof(buffer.buffer), const_cast<cl_mem*>(&(buffer.buffer)));
}

void CLProgram::CLKernel::setKernelArg(size_t argPosition, size_t size, void* arg)
{
    CL(clSetKernelArg(kernel, argPosition, size, arg));
}

CLProgram::CLKernel CLProgram::createKernel(const std::string& kernelName)
{
    return CLKernel(*this, kernelName);
}