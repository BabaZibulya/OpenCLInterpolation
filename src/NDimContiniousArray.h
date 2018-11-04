#pragma once

#include <type_traits>
#include <initializer_list>

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

template<
    typename T,
    unsigned dimSizeFirst,
    unsigned dimSizeSecond,
    unsigned dimSizeThird,
    unsigned dimSizeForth>
class FourDimContiniousArray
{
public:
    FourDimContiniousArray() {
        size_t size = ArraySize<1, dimSizeFirst, dimSizeSecond, dimSizeThird, dimSizeForth>::value;
        data = std::make_unique<T[]>(size);
    }

    T& at(
        unsigned dimFirst,
        unsigned dimSecond,
        unsigned dimThird,
        unsigned dimForth
    ) {
        size_t offset = dimFirst + dimSizeFirst * dimSecond +
            dimSizeFirst * dimSizeSecond * dimThird +
            dimSizeFirst * dimSizeSecond * dimSizeThird * dimForth;
        
        return data[offset];
    }

private:
    std::unique_ptr<T[]> data;
};