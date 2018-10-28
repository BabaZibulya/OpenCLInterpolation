#pragma once

#include <vector>
#include <string>

#include "CLContext.h"
#include "CLTypes.h"

class CLProgram
{
public:
    CLProgram(
        const CLContext& context,
        const std::vector<CLDevice>& devices,
        const std::vector<std::string>& sources);
    ~CLProgram();

    static CLProgram compileSources(
        const CLContext& context,
        const std::vector<CLDevice>& devices,
        const std::vector<std::string>& pathes);

    cl_program program;

private:
    std::string compilationLog(const CLDevice& device);
};