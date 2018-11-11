#pragma once

#include <type_traits>
#include <initializer_list>
#include <functional>

template<unsigned size, unsigned dimSizeFirst, unsigned... others>
struct ArraySize
{
    static const size_t value = ArraySize<size * dimSizeFirst, others...>::value;
};

template<unsigned size, unsigned dimLast>
struct ArraySize<size, dimLast>
{
    static const size_t value = size * dimLast;
};

template<unsigned n, unsigned dimSizeFirst, unsigned... others>
struct nthDimension
{
    static const unsigned value = nthDimension<n - 1, others...>::value;
};

template<unsigned dimSizeFirst, unsigned... others>
struct nthDimension<0, dimSizeFirst, others...>
{
    static const unsigned value = dimSizeFirst;
};

template<unsigned dimSizeFirst, unsigned... others>
struct Dimensions : public Dimensions<others...>
{
    template<unsigned n, typename std::enable_if<(n > 0)>::type* = nullptr>
    static unsigned get()
    {
        return Dimensions<others...>::template get<n - 1>();
    }
    
    template<unsigned n, typename std::enable_if<n == 0>::type* = nullptr>
    static unsigned get()
    {
        return dimSizeFirst;
    }
};

template<unsigned last>
struct Dimensions<last>
{
    template<unsigned n>
    static unsigned get()
    {
        static_assert(n == 0, "Index is larger than dimensions list");
        return last;
    }
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

    void fill(std::function<T(unsigned)> f) {
        for (unsigned i = 0; i < size; ++i) {
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

    void fill(std::function<T(unsigned, unsigned)> f) {
        for (int j = 0; j < dimSizeFirst; j++)
            for (int i = 0; i < dimSizeSecond; i++) {
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
    FourDimContiniousArray(unsigned dimSizeFirst,
                           unsigned dimSizeSecond,
                           unsigned dimSizeThird,
                           unsigned dimSizeFourth) :
                           dimSizeFirst(dimSizeFirst),
                           dimSizeSecond(dimSizeSecond),
                           dimSizeThird(dimSizeThird),
                           dimSizeFourth(dimSizeFourth)
    {
        numberOfElements = dimSizeFirst * dimSizeSecond * dimSizeThird * dimSizeFourth;
        data = std::make_unique<T[]>(numberOfElements);
    }

    T& at(
        unsigned dimFirst,
        unsigned dimSecond,
        unsigned dimThird,
        unsigned dimFourth
    ) {
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

    unsigned getDimSizeFirst() {
        return dimSizeFirst; 
    }

    unsigned getDimSizeSecond() {
        return dimSizeSecond; 
    }

    unsigned getDimSizeThird() {
        return dimSizeThird; 
    }

    unsigned getDimSizeFourth() {
        return dimSizeFourth; 
    }

    void fill(std::function<T(unsigned, unsigned, unsigned, unsigned)> fillFunc) {
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
