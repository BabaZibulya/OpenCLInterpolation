#include "OpenCLSubtask.h"

#include "InterpolationUtils.h"
#include "InterpolationOpenCLData.h"

#include "TimeUtils.h"

#include "CLLog.h"

OpenCLSubtask::OpenCLSubtask(const Interval& pkInterval,
                             unsigned Lmz,
                             unsigned Mmz,
                             unsigned Nmz,
                             const CL::Platform& platform,
                             size_t deviceNum)
    : pkInterval(pkInterval),
      Lmz(Lmz),
      Mmz(Mmz),
      Nmz(Nmz),

      US(createUS(pkInterval, Lmz, Mmz, Nmz)),
      VS(createVS(pkInterval, Lmz, Mmz, Nmz)),
      HS(createHS(pkInterval, Lmz, Mmz, Nmz)),
      QS(createQS(pkInterval, Lmz, Mmz, Nmz)),
      TS(createTS(pkInterval, Lmz, Mmz, Nmz)),
      Qc(createQc(pkInterval, Lmz, Mmz, Nmz)),
      F_X(createF_X(Mmz, Nmz)),
      Zmz(createZmz(Nmz)),

      platform(platform),
      device(CL::Device::getAllAvailableCLDevices(platform)[deviceNum]),
      context(platform, { device }),
      commandQueue(context, device)
{
	size_t size = US.getRawSize() + VS.getRawSize() + HS.getRawSize() + QS.getRawSize() + TS.getRawSize() + Qc.getRawSize() + F_X.getRawSize() + Zmz.getRawSize();
	CLLog("Allocated ", size / (1024  * 1024), " Mb of data");
}

void OpenCLSubtask::solve()
{
    try {
        CL::Program program = CL::Program::createProgram(context, {device}, {"kernels/hw/interpol.xclbin"}, CL::Program::ProgramType::BINARY);
        CL::Program::Kernel kernel = program.createKernel("interpol");

        auto platformName = platform.getPlatformInfoDetail(CL::Platform::CL_PLATFORM_NAME);

        InterpolationOpenCLData interpolationData(pkInterval,
                                                  Lmz,
                                                  Mmz,
                                                  Nmz,
                                                  US,
                                                  VS,
                                                  HS,
                                                  QS,
                                                  TS,
                                                  Qc,
                                                  F_X,
                                                  Zmz,
                                                  context,
                                                  kernel,
                                                  commandQueue,
                                                  platformName);

        measureTime(platformName + " OpenCL ", [&]() {
            commandQueue.enqueueNDRangeKernel(
                kernel, 3,
                CL::CommandQueue::GlobalSize{ {pkInterval.right - pkInterval.left,
                                               Lmz,
                                               Mmz} },
                CL::CommandQueue::LocalSize{{16, 4, 4}});
            commandQueue.finish();
        });

        interpolationData.readData();
    }
    catch (std::exception& e) {
        CLLog("Got an exception: ", e.what());
        return;
    }
}

FourDimContiniousArray<float>& OpenCLSubtask::getResult()
{
    return Qc;
}
