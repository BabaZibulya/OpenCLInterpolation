#pragma once

#include "NDimContiniousArray.h"

#include "CLContext.h"
#include "CLProgram.h"
#include "CLCommandQueue.h"

struct InterpolationOpenCLData {
    InterpolationOpenCLData(
        const Interval& pkInterval,
        unsigned Lmz,
        unsigned Mmz, 
        unsigned Nmz,
        FourDimContiniousArray<float>& US,
        FourDimContiniousArray<float>& VS,
        FourDimContiniousArray<float>& HS,
        FourDimContiniousArray<float>& QS,
        FourDimContiniousArray<float>& TS,
        FourDimContiniousArray<float>& Qc,
        TwoDimContiniousArray<float>& F_X,
        OneDimContiniousArray<float>& Zmz,
        const CL::Context& context,
        CL::Program::Kernel& kernel,
        CL::CommandQueue& commandQueue,
        const std::string platformName = "");

    void readData();
private:
    Interval pkInterval;
    unsigned Lmz, Mmz, Nmz;
    FourDimContiniousArray<float>& Qc;
    CL::CommandQueue& commandQueue;
    std::string platformName;
    CL::Buffer QcBuffer, USBuffer, VSBuffer, HSBuffer, QSBuffer, TSBuffer, F_XBuffer, ZmzBuffer;
};