#pragma once

#include "CLPlatform.h"
#include "CLDevice.h"
#include "CLContext.h"
#include "CLCommandQueue.h"

#include "NDimContiniousArray.h"

class OpenCLSubtask {
public:
    OpenCLSubtask(const Interval& pkInterval,
                  unsigned Lmz,
                  unsigned Mmz,
                  unsigned Nmz,
                  const CL::Platform& platform);

    void solve();

    FourDimContiniousArray<float>& getResult();
private:
    Interval pkInterval;
    unsigned Lmz, Mmz, Nmz;

    FourDimContiniousArray<float> US, VS, HS, QS, TS, Qc;
    TwoDimContiniousArray<float> F_X;
    OneDimContiniousArray<float> Zmz;

    CL::Platform platform;
    CL::Device device;
    CL::Context context;
    CL::CommandQueue commandQueue;
};
