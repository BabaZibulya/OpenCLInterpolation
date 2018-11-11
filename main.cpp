#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include <cmath>
#include <memory>
#include <chrono>

#include "CLPlatform.h"
#include "CLBuffer.h"
#include "CLDevice.h"
#include "CLContext.h"
#include "CLCommandQueue.h"
#include "CLProgram.h"
#include "CLUtils.h"

#include "NDimContiniousArray.h"

#include "InterpolationUtils.h"

unsigned Pk = 40, Lmz = 101, Mmz = 102, Nmz = 103;

int main() {
    auto platforms = getAllAvailableCLPlatforms();

    std::vector<CLDevice> devices = CLDevice::getAllAvailableCLDevices(platforms[0]);
    CLContext context(devices);
    CLCommandQueue commandQueue(context, devices[0]);
    try {
        CLProgram program = CLProgram::compileSources(context, devices, {"kernels/interpol.cl"});
        CLProgram::CLKernel kernel = program.createKernel("interpol");

        auto US = createUS(Pk, Lmz, Mmz, Nmz);
        auto VS = createVS(Pk, Lmz, Mmz, Nmz);
        auto HS = createHS(Pk, Lmz, Mmz, Nmz);
        auto QS = createQS(Pk, Lmz, Mmz, Nmz);
        auto TS = createTS(Pk, Lmz, Mmz, Nmz);
        auto Qc = createQc(Pk, Lmz, Mmz, Nmz);

        auto F_X = createF_X(Mmz, Nmz);

        auto Zmz = createZmz(Nmz);

        CLBuffer USBuffer(context, CLBuffer::BufferType::CL_MEM_READ_ONLY, US.getRawSize());
        CLBuffer VSBuffer(context, CLBuffer::BufferType::CL_MEM_READ_ONLY, VS.getRawSize());
        CLBuffer HSBuffer(context, CLBuffer::BufferType::CL_MEM_READ_ONLY, HS.getRawSize());
        CLBuffer QSBuffer(context, CLBuffer::BufferType::CL_MEM_READ_ONLY, QS.getRawSize());
        CLBuffer TSBuffer(context, CLBuffer::BufferType::CL_MEM_READ_ONLY, TS.getRawSize());
        CLBuffer QcBuffer(context, CLBuffer::BufferType::CL_MEM_WRITE_ONLY, Qc.getRawSize());

        CLBuffer F_XBuffer(context, CLBuffer::BufferType::CL_MEM_READ_ONLY, F_X.getRawSize());
        CLBuffer ZmzBuffer(context, CLBuffer::BufferType::CL_MEM_READ_ONLY, Zmz.getRawSize());

        commandQueue.enqueueWriteBuffer(USBuffer, US.getRawSize(), US.getData());
        commandQueue.enqueueWriteBuffer(VSBuffer, VS.getRawSize(), VS.getData());
        commandQueue.enqueueWriteBuffer(HSBuffer, HS.getRawSize(), HS.getData());
        commandQueue.enqueueWriteBuffer(QSBuffer, QS.getRawSize(), QS.getData());
        commandQueue.enqueueWriteBuffer(TSBuffer, TS.getRawSize(), TS.getData());
        commandQueue.enqueueWriteBuffer(F_XBuffer, F_X.getRawSize(), F_X.getData());
        commandQueue.enqueueWriteBuffer(ZmzBuffer, Zmz.getRawSize(), Zmz.getData());

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
        kernel.setKernelArg(8, Pk);
        kernel.setKernelArg(9, Lmz);
        kernel.setKernelArg(10, Mmz);
        kernel.setKernelArg(11, Nmz);

        size_t localSize = 64;
        std::chrono::time_point<std::chrono::system_clock> start, end;
        start = std::chrono::system_clock::now();
        commandQueue.enqueueNDRangeKernel(
            kernel, 3,
            {Pk, Lmz, Mmz},
            {1, 1, 1});

        commandQueue.finish();
        end = std::chrono::system_clock::now();
        int elapsed_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>
                             (end-start).count();
 
        std::cout << "Execution time: " << elapsed_milliseconds << "ms\n";

        commandQueue.enqueueReadBuffer(QcBuffer, Qc.getRawSize(), Qc.getData());
        for (int i = 0; i < 10; i++) {
            std::cout << Qc.at(i, 0, 0, 0) << std::endl;
        }
    }
    catch (std::exception& e) {
        std::cout << e.what();
        return -1;
    }

    return 0;
}
