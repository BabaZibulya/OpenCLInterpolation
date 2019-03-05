#include "InterpolationProblem.h"

#include <thread>

#include "CLUtils.h"
#include "CL.h"
#include "CLContext.h"
#include "CLCommandQueue.h"

#include "InterpolationUtils.h"
#include "TimeUtils.h"

InterpolationProblem::InterpolationProblem(unsigned Pk, unsigned Lmz, unsigned Mmz, unsigned Nmz) :
    Pk(Pk), Lmz(Lmz), Mmz(Mmz), Nmz(Nmz), 
    US(createUS(Pk, Lmz, Mmz, Nmz)),
    VS(createVS(Pk, Lmz, Mmz, Nmz)),
    HS(createHS(Pk, Lmz, Mmz, Nmz)),
    QS(createQS(Pk, Lmz, Mmz, Nmz)),
    TS(createTS(Pk, Lmz, Mmz, Nmz)),
    Qc(createQc(Pk, Lmz, Mmz, Nmz)),
    F_X(createF_X(Mmz, Nmz)),
    Zmz(createZmz(Nmz))
{
}

void InterpolationProblem::solve(unsigned chunkSize)
{
    auto platforms = getAllAvailableCLPlatforms();
    CLLog("Found ", platforms.size(), " platforms");
    for (auto& platform : platforms) {
        CLLog(platform.getPlatformInfo());
    }
    std::vector<InterpolationTask> tasks;
    for (auto& platform : platforms) {
        tasks.push_back(InterpolationTask(platform, *this));
    }

    std::vector<std::thread> taskThreads;
    unsigned chunkNum = 0;
    // unsigned chunkSize = Pk / 10;
    for (auto& task : tasks) {
        taskThreads.push_back(std::thread(&InterpolationTask::solve, &task, chunkNum, chunkSize));
        chunkNum++;
    }

    for (auto& thread: taskThreads) {
        thread.join();
    }

    checkResults();
}

void InterpolationProblem::checkResults()
{
    //Pk, Lmz, Mmz, Nmz

    double sum = 0.0;

    for(size_t h = 0; h < Pk; h++)
        for(size_t k = 0; k < Lmz; k++)
            for(size_t j = 0; j < Mmz; j++)
                for(size_t i = 0; i < Nmz; i++) {
                    sum += Qc.at(h, k, j, i);
                }

    
    CLLog("Mean = ", sum / ((double) Pk * Lmz * Mmz * Nmz));
}

InterpolationProblem::InterpolationTask::InterpolationTask
    (const CLPlatform& platform, InterpolationProblem& problem)
        : problem(problem),
          platform(platform),
          devices(CLDevice::getAllAvailableCLDevices(platform)),
          context(platform, devices),
          commandQueue(context, devices[0]),
          platformName(platform.getPlatformInfo().name)
{
}

void InterpolationProblem::InterpolationTask::solve(unsigned chunkNum, unsigned chunkSize)
{
    try {
        CLProgram program = CLProgram::compileSources(context, devices, {"kernels/interpol.cl"});
        CLProgram::CLKernel kernel = program.createKernel("interpol");

        InterpolationData interpolationData(problem, context, kernel, commandQueue, platformName);

        measureTime(platformName + " OpenCL ", [&]() {
            if ((chunkSize == 0 && chunkNum == 0) || (chunkSize == problem.Pk && chunkNum == 1)) {
                CLLog(problem.platformName, " nothing to do");
            }
            else if ((chunkSize == 0 && chunkNum == 1) || (chunkSize == problem.Pk && chunkNum == 0)) {
                commandQueue.enqueueNDRangeKernel(
                    kernel, 3,
                    {problem.Pk, problem.Lmz, problem.Mmz});
            }
            else {
                commandQueue.enqueueNDRangeKernelWithOffset(
                    kernel, 3,
                    {(chunkNum == 0) ? chunkSize : problem.Pk, problem.Lmz, problem.Mmz},
                    {(chunkNum == 0) ? 0 : chunkSize, 0, 0});
            }
            commandQueue.finish();
        });

        interpolationData.readData();
    }
    catch (std::exception& e) {
        CLLog("Got an exception: ", e.what());
        return;
    }
}

InterpolationProblem::InterpolationData::InterpolationData(
    InterpolationProblem& problem,
    const CLContext& context,
    CLProgram::CLKernel& kernel,
    CLCommandQueue& commandQueue,
    const std::string platformName)
    : platformName(platformName),
      commandQueue(commandQueue),
      problem(problem),
      QcBuffer(context, CLBuffer::BufferType::MEM_WRITE_ONLY, problem.Qc.getRawSize()),
      USBuffer(context, CLBuffer::BufferType::MEM_READ_ONLY, problem.US.getRawSize()),
      VSBuffer(context, CLBuffer::BufferType::MEM_READ_ONLY, problem.VS.getRawSize()),
      HSBuffer(context, CLBuffer::BufferType::MEM_READ_ONLY, problem.HS.getRawSize()),
      QSBuffer(context, CLBuffer::BufferType::MEM_READ_ONLY, problem.QS.getRawSize()),
      TSBuffer(context, CLBuffer::BufferType::MEM_READ_ONLY, problem.TS.getRawSize()),
      F_XBuffer(context, CLBuffer::BufferType::MEM_READ_ONLY, problem.F_X.getRawSize()),
      ZmzBuffer(context, CLBuffer::BufferType::MEM_READ_ONLY, problem.Zmz.getRawSize())
{
    try {
        measureTime(platformName + " OpenCL writing inputs ", [&](){
            commandQueue.enqueueWriteBuffer(USBuffer, problem.US.getRawSize(), problem.US.getData());
            commandQueue.enqueueWriteBuffer(VSBuffer, problem.VS.getRawSize(), problem.VS.getData());
            commandQueue.enqueueWriteBuffer(HSBuffer, problem.HS.getRawSize(), problem.HS.getData());
            commandQueue.enqueueWriteBuffer(QSBuffer, problem.QS.getRawSize(), problem.QS.getData());
            commandQueue.enqueueWriteBuffer(TSBuffer, problem.TS.getRawSize(), problem.TS.getData());
            commandQueue.enqueueWriteBuffer(F_XBuffer, problem.F_X.getRawSize(), problem.F_X.getData());
            commandQueue.enqueueWriteBuffer(ZmzBuffer, problem.Zmz.getRawSize(), problem.Zmz.getData());
        });

        /*
            __global float *US,
            __global float *VS,
            __global float *HS,
            __global float *QS,
            __global float *TS,
            __global float *F_x,
            __global float *Zmz,
            __global float *Qc,
            const unsigned int Pk,
            const unsigned int Lmz,
            const unsigned int Mmz,
            const unsigned int Nmz
        */

        kernel.setKernelArg(0, USBuffer);
        kernel.setKernelArg(1, VSBuffer);
        kernel.setKernelArg(2, HSBuffer);
        kernel.setKernelArg(3, QSBuffer);
        kernel.setKernelArg(4, TSBuffer);
        kernel.setKernelArg(5, F_XBuffer);
        kernel.setKernelArg(6, ZmzBuffer);
        kernel.setKernelArg(7, QcBuffer);
        kernel.setKernelArg(8, problem.Pk);
        kernel.setKernelArg(9, problem.Lmz);
        kernel.setKernelArg(10, problem.Mmz);
        kernel.setKernelArg(11, problem.Nmz);
    }
    catch (std::exception& e) {
        CLLog("Got an exception: ", e.what());
        return;
    }
}

void InterpolationProblem::InterpolationData::readData()
{
    FourDimContiniousArray<float> result(problem.Pk, problem.Lmz, problem.Mmz, problem.Nmz);
    measureTime(platformName + " OpenCL reading results ", [&](){
        commandQueue.enqueueReadBuffer(QcBuffer, result.getRawSize(), result.getData());
    });

    // Temp hack
    // TODO load with offsets
    for (size_t i = 0; i < problem.Pk; ++i) {
        for (size_t j = 0; j < problem.Lmz; ++j) {
            for (size_t k = 0; k < problem.Mmz; ++k) {
                for (size_t l = 0; l < problem.Nmz; ++l) {
                    if (result.at(i, j, k, l) != 0.f) {
                        problem.Qc.at(i, j, k, l) = result.at(i, j, k, l);
                    }
                }
            }
        }
    }
}