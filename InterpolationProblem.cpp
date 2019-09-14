#include "InterpolationProblem.h"

#include <future>

#include "mpi.h"

#include "CLUtils.h"
#include "CL.h"
#include "CLContext.h"
#include "CLCommandQueue.h"

#include "InterpolationUtils.h"
#include "TimeUtils.h"
#include "OpenCLSubtask.h"

InterpolationProblem::InterpolationProblem(unsigned Pk, unsigned Lmz, unsigned Mmz, unsigned Nmz) :
    Pk(Pk), Lmz(Lmz), Mmz(Mmz), Nmz(Nmz)
{
    MPI_Init(nullptr, nullptr);
    MPI_Comm_size(MPI_COMM_WORLD, &worldSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &worldRank);
    CLLog("OpenMPI info: world size : ", worldSize, ", world rank : ", worldRank);
}

InterpolationProblem::~InterpolationProblem()
{
    MPI_Finalize();
}

void InterpolationProblem::solve()
{
	auto platforms = CL::getAllAvailableCLPlatforms();
	CLLog("Found ", platforms.size(), " platforms");
	for (auto& platform : platforms) {
		CLLog(platform.getPlatformInfo());
	}

    if (worldSize == 1)
    {
        OpenCLSubtask subtask({0, Pk}, Lmz, Mmz, Nmz, platforms[0]);
        subtask.solve();
        auto& res = subtask.getResult();
	    checkResults(res);
    }
}

void InterpolationProblem::solve(unsigned chunkSize)
{
    auto platforms = CL::getAllAvailableCLPlatforms();
    CLLog("Found ", platforms.size(), " platforms");
    for (auto& platform : platforms) {
        CLLog(platform.getPlatformInfo());
    }
    std::vector<OpenCLSubtask> tasks;
    int start = 0;
    int end = Pk / platforms.size();

    for (auto& platform : platforms) {
        tasks.push_back(OpenCLSubtask({start, end}, Lmz, Mmz, Nmz, platform));
        start = end;
        end = end + Pk / platforms.size();
    }

    std::vector<std::future<void>> futures;
    for (auto& task : tasks) {
        auto fut = std::async(std::launch::async, &OpenCLSubtask::solve, &task);
        futures.emplace_back(std::move(fut));
    }

    for (auto& future: futures) {
        future.get();
    }

    for (auto& task : tasks) {
        checkResults(task.getResult());
    }
}

void InterpolationProblem::checkResults(const FourDimContiniousArray<float>& Qc)
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
