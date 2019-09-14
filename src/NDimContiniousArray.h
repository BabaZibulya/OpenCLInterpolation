#pragma once

#include <functional>
#include <memory>

struct Interval {
    unsigned left, right;
};

template<typename T>
class OneDimContiniousArray
{
public:
    OneDimContiniousArray(size_t size) : size(size) {
        data = std::make_unique<T[]>(size);
    }

    T& at(size_t ind) {
        return data[ind];
    }

    void fill(std::function<T(size_t)> f) {
        for (size_t i = 0; i < size; ++i) {
            data[i] = f(i);
        }
    }

    size_t numberOfElements() {
        return size;
    }

    size_t getRawSize() {
        return size * sizeof(T);
    }

    void *getData() {
        return data.get();
    }
private:
    std::unique_ptr<T[]> data;
    size_t size;
};

template<typename T>
class TwoDimContiniousArray
{
public:
    TwoDimContiniousArray(size_t dimSizeFirst, size_t dimSizeSecond) :
        dimSizeFirst(dimSizeFirst), dimSizeSecond(dimSizeSecond)
    {
        data = std::make_unique<T[]>(dimSizeFirst * dimSizeSecond);
    }

    T& at(size_t indFirst, size_t indSecond) {
        return data[indFirst + dimSizeFirst * indSecond];
    }

    void fill(std::function<T(size_t, size_t)> f) {
        for (size_t j = 0; j < dimSizeFirst; j++)
            for (size_t i = 0; i < dimSizeSecond; i++) {
                at(j, i) = f(j, i);
            }
    }

    size_t getRawSize() {
        return dimSizeFirst * dimSizeSecond * sizeof(T);
    }

    void *getData() {
        return data.get();
    }
private:
    std::unique_ptr<T[]> data;
    size_t dimSizeFirst, dimSizeSecond;
};

template<typename T>
class FourDimContiniousArray
{
public:
    FourDimContiniousArray(size_t dimSizeFirst,
                           size_t dimSizeSecond,
                           size_t dimSizeThird,
                           size_t dimSizeFourth) :
                           dimSizeFirst(dimSizeFirst),
                           dimSizeSecond(dimSizeSecond),
                           dimSizeThird(dimSizeThird),
                           dimSizeFourth(dimSizeFourth)
    {
        numberOfElements = dimSizeFirst * dimSizeSecond * dimSizeThird * dimSizeFourth;
        data = std::make_unique<T[]>(numberOfElements);
    }

    T& at(
        size_t dimFirst,
        size_t dimSecond,
        size_t dimThird,
        size_t dimFourth
    ) {
        size_t offset = dimFirst + dimSizeFirst * dimSecond +
            dimSizeFirst * dimSizeSecond * dimThird +
            dimSizeFirst * dimSizeSecond * dimSizeThird * dimFourth;
        
        return data[offset];
    }

    const T& at(
        size_t dimFirst,
        size_t dimSecond,
        size_t dimThird,
        size_t dimFourth
    ) const {
        size_t offset = dimFirst + dimSizeFirst * dimSecond +
            dimSizeFirst * dimSizeSecond * dimThird +
            dimSizeFirst * dimSizeSecond * dimSizeThird * dimFourth;
        
        return data[offset];
    }

    size_t getNumberOfElements() {
        return numberOfElements;
    }

    size_t getRawSize() {
        return numberOfElements * sizeof(float);
    }

    size_t getDimSizeFirst() {
        return dimSizeFirst; 
    }

    size_t getDimSizeSecond() {
        return dimSizeSecond; 
    }

    size_t getDimSizeThird() {
        return dimSizeThird; 
    }

    size_t getDimSizeFourth() {
        return dimSizeFourth; 
    }

    void fill(std::function<T(size_t, size_t, size_t, size_t)> fillFunc) {
        for (size_t h = 0; h < dimSizeFirst; h++)
            for (size_t k = 0; k < dimSizeSecond; k++)
                for (size_t j = 0; j < dimSizeThird; j++)
                    for (size_t i = 0; i < dimSizeFourth; i++) {
                        at(h, k, j, i) = fillFunc(h, k, j, i);
                    }
    }
    
    void *getData() {
        return data.get();
    }

private:
    unsigned dimSizeFirst, dimSizeSecond, dimSizeThird, dimSizeFourth;
    std::unique_ptr<T[]> data;
    size_t numberOfElements;
};
