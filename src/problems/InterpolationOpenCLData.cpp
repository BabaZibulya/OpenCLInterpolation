#include "InterpolationOpenCLData.h"

#include "TimeUtils.h"

#include "CLLog.h"

InterpolationOpenCLData::InterpolationOpenCLData(const Interval& pkInterval,
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
                                                 const std::string platformName)
    : pkInterval(pkInterval),
      Lmz(Lmz),
      Mmz(Mmz),
      Nmz(Nmz),
      Qc(Qc),
      commandQueue(commandQueue),
      platformName(platformName),
      QcBuffer(context, CL::Buffer::BufferType::MEM_WRITE_ONLY, Qc.getRawSize()),
      USBuffer(context, CL::Buffer::BufferType::MEM_READ_ONLY, US.getRawSize()),
      VSBuffer(context, CL::Buffer::BufferType::MEM_READ_ONLY, VS.getRawSize()),
      HSBuffer(context, CL::Buffer::BufferType::MEM_READ_ONLY, HS.getRawSize()),
      QSBuffer(context, CL::Buffer::BufferType::MEM_READ_ONLY, QS.getRawSize()),
      TSBuffer(context, CL::Buffer::BufferType::MEM_READ_ONLY, TS.getRawSize()),
      F_XBuffer(context, CL::Buffer::BufferType::MEM_READ_ONLY, F_X.getRawSize()),
      ZmzBuffer(context, CL::Buffer::BufferType::MEM_READ_ONLY, Zmz.getRawSize())
{
    try {
        measureTime(platformName + " OpenCL writing inputs ", [&](){
            commandQueue.enqueueWriteBuffer(USBuffer, US.getRawSize(), US.getData());
            commandQueue.enqueueWriteBuffer(VSBuffer, VS.getRawSize(), VS.getData());
            commandQueue.enqueueWriteBuffer(HSBuffer, HS.getRawSize(), HS.getData());
            commandQueue.enqueueWriteBuffer(QSBuffer, QS.getRawSize(), QS.getData());
            commandQueue.enqueueWriteBuffer(TSBuffer, TS.getRawSize(), TS.getData());
            commandQueue.enqueueWriteBuffer(F_XBuffer, F_X.getRawSize(), F_X.getData());
            commandQueue.enqueueWriteBuffer(ZmzBuffer, Zmz.getRawSize(), Zmz.getData());
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
        kernel.setKernelArg(8, pkInterval.right - pkInterval.left);
        kernel.setKernelArg(9, Lmz);
        kernel.setKernelArg(10, Mmz);
        kernel.setKernelArg(11, Nmz);
    }
    catch (std::exception& e) {
        CLLog("Got an exception: ", e.what());
        return;
    }
}

void InterpolationOpenCLData::readData()
{
    FourDimContiniousArray<float> result(pkInterval.right - pkInterval.left, Lmz, Mmz, Nmz);
    measureTime(platformName + " OpenCL reading results ", [&](){
        commandQueue.enqueueReadBuffer(QcBuffer, result.getRawSize(), result.getData());
    });

    // Temp hack
    // TODO load with offsets
    for (size_t i = 0; i < pkInterval.right - pkInterval.left; ++i) {
        for (size_t j = 0; j < Lmz; ++j) {
            for (size_t k = 0; k < Mmz; ++k) {
                for (size_t l = 0; l < Nmz; ++l) {
                    if (result.at(i, j, k, l) != 0.f) {
                        Qc.at(i, j, k, l) = result.at(i, j, k, l);
                    }
                }
            }
        }
    }
}