#pragma once

#include <string>
#include <vector>
#include <memory>

#include "CLContext.h"
#include "CLCommandQueue.h"
#include "CLDevice.h"

#include "NDimContiniousArray.h"

class InterpolationProblem
{
public:
    InterpolationProblem(unsigned Pk = 40, unsigned Lmz = 100, unsigned Mmz = 102, unsigned Nmz = 103);
	void solve();
    void solve(unsigned chunkSize);
    void checkResults();
private:
    struct InterpolationData {
        InterpolationData(
            InterpolationProblem& problem,
            const CL::Context& context,
			CL::Program::Kernel& kernel,
            CL::CommandQueue& commandQueue,
            const std::string platformName = "");

        void readData();
    private:
        std::string platformName;
        CL::CommandQueue& commandQueue;
        InterpolationProblem& problem;
		CL::Buffer QcBuffer, USBuffer, VSBuffer, HSBuffer, QSBuffer, TSBuffer, F_XBuffer, ZmzBuffer;
    };
    struct InterpolationTask {
        InterpolationTask(const CL::Platform& platform, InterpolationProblem& problem);
        void solve(unsigned chunkNum, unsigned chunkSize);
    private:
        InterpolationProblem& problem;
		CL::Platform platform;
        std::vector<CL::Device> devices;
		CL::Context context;
		CL::CommandQueue commandQueue;
        std::string platformName;
    };
    friend struct InterpolationTask;
    friend struct InterpolationData;
private:
    unsigned Pk, Lmz, Mmz, Nmz;
    
    FourDimContiniousArray<float> US, VS, HS, QS, TS, Qc;
    TwoDimContiniousArray<float> F_X;
    OneDimContiniousArray<float> Zmz;

    std::string platformName;
};
