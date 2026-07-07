/*
Function to document major highlights from reading a random book on 
performant C++
*/


#include <iostream>
#include <cstdlib>
#include <assert.h>
#include <array>
#include <cmath>
#include <vector>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <type_traits>

/**
This is calculated in compile time ...
Arrays are more performant than vectors because the size of the array is known at compile time
*/
template<size_t N>
constexpr std::array<std::array<int, N>, N>
matmul (const std::array<std::array<int, N>, N> &A, 
        const std::array<std::array<int, N>, N> &B)
        {
            std::array<std::array<int, N>, N> R{};
            for(size_t i = 0; i < N; ++i)
            {
                for(size_t j = 0; j < N; ++j)
                {
                    for(size_t k = 0; k < N; ++k)
                    {
                        R[i][j] += A[i][k] * B[k][j];
                    }
                }
            }
            return R;
        }

/**

*/
template<size_t N>
constexpr std::array<int, N> TwoArrSum(std::array<int, N>& x, std::array<int, N>& y)
{
    std::array<int, N> result {};
    for(size_t i = 0; i < N; ++i)
    result[i] = x[i] + y[i];
    return result;
}

template<size_t N>
constexpr void LookupTable(std::array<int, N>& lookup_t)
{
    for(int i = 0; i < N; ++i)
    {
        lookup_t[i] = i * i;
    }
}

template<size_t N>
constexpr void Print(std::array<int, N>& lt)
{
    for(size_t i = 0; i < N ; ++i)
    {
        std::cout << "i = " << i  << "; value = " << lt[i] << "\n";
    }
}

/**
Struct of Arrays(SoA) better than Array of structs (Aos)
//////////////// SOA ///////////////////////
std::tuple<int, int, int, int> quaternion;
struct SoA{
    vector<quaternion> qs;
};

//////////////// AOS //////////////////////
struct Aos {
    int x, y, z, r;
};
std::vector<Aos> qs; 

SoA is always better for performance
*/



/**
alignas(32) float data[8]; --> This is telling the compiler to align with 32 byte
*/
int main()
{
    std::array<int, 5> lt;
    LookupTable<5>(lt);
    Print<5>(lt);
    alignas(32) float data[8];
    std::pmr::vector<float> buf { }; // -> this buffer is a vector over the memory stored in stack;


    auto* p = new(std::align_val_t(32)) float[256];
    delete[] p;

    std::cout << "numeric limits " << std::numeric_limits<double>::epsilon << "\n";

    int src= 10;
    int dst = 0;
    asm("movl %1, %0\n\t"
        "addl $5, %0"
        : "=r" (dst)
        : "r" (src)
        :
        );
    std::cout << "Result = " << dst << std::endl;


    return 0;
}

/**
Have to look at the following
std::vector<int, MyAllocator<int>> fastvec;
std::pmr::vector<float> buf { &my_resource_pool };
std::span


*/
