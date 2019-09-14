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
    ~InterpolationProblem();
	void solve();
    void solve(unsigned chunkSize);
    void checkResults(const FourDimContiniousArray<float>& Qc);
private:
    unsigned Pk, Lmz, Mmz, Nmz;

    int worldSize, worldRank;

    std::string platformName;
};
