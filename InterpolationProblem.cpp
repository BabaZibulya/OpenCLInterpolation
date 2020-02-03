#include "InterpolationProblem.h"

#include <future>

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
    worldSize = 1;
}

InterpolationProblem::~InterpolationProblem()
{
    //MPI_Finalize();
}

void InterpolationProblem::solve()
{
	auto platforms = CL::getAllAvailableCLPlatforms();
	CLLog("Found ", platforms.size(), " platforms");
	for (auto& platform : platforms) {
		CLLog(platform.getPlatformInfo());
	}

    if (worldSize == 1) {
        CLLog("Solving with Pk = ", Pk);
        OpenCLSubtask subtask({0, Pk}, Lmz, Mmz, Nmz, platforms[0]);
        subtask.solve();
        auto& res = subtask.getResult();
	    checkResults(res);
    } else {
        unsigned nodeChunkSize = Pk / worldSize;
        if (worldRank == 0) {
            // Server
            auto platform = platforms[0];
            auto numberOfDevices = CL::Device::getAllAvailableCLDevices(platform).size();
            auto deviceChunk = nodeChunkSize / numberOfDevices;
            std::vector<OpenCLSubtask> subtasks;
            std::vector<std::future<void>> subtaskFinishFuture;
            for (size_t i = 0; i < numberOfDevices; ++i) {
                subtasks.emplace_back(OpenCLSubtask({deviceChunk * i, deviceChunk * (i + 1)}, Lmz, Mmz, Nmz, platform, i));
                auto& subtask = subtasks[i];
                subtaskFinishFuture.emplace_back(std::async(std::launch::async,
                                                 [&subtask]() mutable { subtask.solve(); } ));
            }
            for (auto& fut : subtaskFinishFuture) {
                fut.get();
            }
            // OpenCLSubtask subtask({deviceChunk * i, deviceChunk * (i + 1)}, Lmz, Mmz, Nmz, platform);
            // subtask.solve();
            auto& res = subtasks[0].getResult();

            std::vector<std::future<void>> futures;
            std::vector<FourDimContiniousArray<float>> results;
            for (int otherNode = 1; otherNode < worldSize; ++otherNode)
            {
                results.emplace_back(res.getDimSizeFirst() * numberOfDevices,
                                     res.getDimSizeSecond(),
                                     res.getDimSizeThird(),
                                     res.getDimSizeFourth());
                CLLog("Allocated result buf with ", results.back().getNumberOfElements());
            }
            for (int otherNode = 1; otherNode < worldSize; ++otherNode)
            {
                //auto fut = std::async(std::launch::async,
                //                      [resBuf = results[otherNode - 1].getData(),
                //                       numberOfEl = results[otherNode - 1].getNumberOfElements()](unsigned otherNode) {
                //                          MEASURE_TIME("Time to receive ",
                //                          CLLog("Receiving data from node = ", otherNode,
                //                                " rest buf size = ", numberOfEl);
                //                          MPI_Recv(resBuf,
                //                                   numberOfEl, // All chuncks are same size
                //                                   MPI_FLOAT,
                //                                   otherNode,
                //                                   0,
                //                                   MPI_COMM_WORLD,
                //                                   MPI_STATUS_IGNORE);
                //                          CLLog("Received data from node = ", otherNode);
                //                          );
                //                      },
                //                      otherNode);
                //futures.emplace_back(std::move(fut));
            }


            for (auto& fut : futures) {
                CLLog("Waiting for future");
                fut.get();
            }

            for (unsigned worker = 0; worker < worldSize - 1; worker++) {
		        CLLog("First el = ", results[worker].at(0, 0, 0, 0));
		        CLLog("Worker ", worker + 1, " mean");
                checkResults(results[worker]);
            }
	        CLLog("Server means");
            for (size_t i = 0; i < numberOfDevices; ++i) {
                checkResults(subtasks[i].getResult());
            }
            // checkResults(res);
        } else {
            // Workers
            unsigned left = nodeChunkSize * worldRank;
            unsigned right = nodeChunkSize * (worldRank + 1);
            CLLog("Worker = ", worldRank, " left = ", left, " right = ", right);
            OpenCLSubtask subtask({left, right},
                                   Lmz, Mmz, Nmz, platforms[0]);
            subtask.solve();
            auto& res = subtask.getResult();

            CLLog("Worker = ", worldRank, " rest buf size = ", res.getNumberOfElements());
            //MPI_Send(res.getData(),
            //         res.getNumberOfElements(),
            //         MPI_FLOAT,
            //         0,
            //         0,
            //         MPI_COMM_WORLD);
            CLLog("Worker mean");
            checkResults(res);
        }
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
    auto Pk = Qc.getDimSizeFirst();
    auto Lmz = Qc.getDimSizeSecond();
    auto Mmz = Qc.getDimSizeThird();
    auto Nmz = Qc.getDimSizeFourth();

    double sum = 0.0;

    for(size_t h = 0; h < Pk; h++)
        for(size_t k = 0; k < Lmz; k++)
            for(size_t j = 0; j < Mmz; j++)
                for(size_t i = 0; i < Nmz; i++) {
                    sum += Qc.at(h, k, j, i);
                }

    
    CLLog("Mean = ", sum / ((double) Pk * Lmz * Mmz * Nmz));
}
