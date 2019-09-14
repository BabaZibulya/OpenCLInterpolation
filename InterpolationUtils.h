#pragma once

#include <memory>

#include "NDimContiniousArray.h"

FourDimContiniousArray<float> createUS(unsigned Pk, unsigned Lmz, unsigned Mmz, unsigned Nmz);
FourDimContiniousArray<float> createVS(unsigned Pk, unsigned Lmz, unsigned Mmz, unsigned Nmz);
FourDimContiniousArray<float> createHS(unsigned Pk, unsigned Lmz, unsigned Mmz, unsigned Nmz);
FourDimContiniousArray<float> createQS(unsigned Pk, unsigned Lmz, unsigned Mmz, unsigned Nmz);
FourDimContiniousArray<float> createTS(unsigned Pk, unsigned Lmz, unsigned Mmz, unsigned Nmz);
FourDimContiniousArray<float> createQc(unsigned Pk, unsigned Lmz, unsigned Mmz, unsigned Nmz);

FourDimContiniousArray<float> createUS(const Interval& pkInterval, unsigned Lmz, unsigned Mmz, unsigned Nmz);
FourDimContiniousArray<float> createVS(const Interval& pkInterval, unsigned Lmz, unsigned Mmz, unsigned Nmz);
FourDimContiniousArray<float> createHS(const Interval& pkInterval, unsigned Lmz, unsigned Mmz, unsigned Nmz);
FourDimContiniousArray<float> createQS(const Interval& pkInterval, unsigned Lmz, unsigned Mmz, unsigned Nmz);
FourDimContiniousArray<float> createTS(const Interval& pkInterval, unsigned Lmz, unsigned Mmz, unsigned Nmz);
FourDimContiniousArray<float> createQc(const Interval& pkInterval, unsigned Lmz, unsigned Mmz, unsigned Nmz);

TwoDimContiniousArray<float> createF_X(unsigned Mmz, unsigned Nmz);

OneDimContiniousArray<float> createZmz(unsigned Nmz);