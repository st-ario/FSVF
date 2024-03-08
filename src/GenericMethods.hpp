#pragma once

#include "HelperTypes.hpp"
#include "Platform.hpp"
#include "Shuffles.hpp"

#include <cstdint>

namespace FSVF
{
// returns the sum of all its argument's components
template<FastVector T>
float FSVF_VECCALL sum(T v);

// returns the product of all its argument's components
template<FastVector T>
float FSVF_VECCALL prod(T v);

// returns the unit vector of the argument
template<FastVector T>
T FSVF_VECCALL normalize(T v);

// returns the (Euclidean) length of the argument
template<FastVector T>
float FSVF_VECCALL len(T v);

// returns the square of the (Euclidean) length of the argument
// effectively equivalent to dot(v,v)
template<FastVector T>
float FSVF_VECCALL len_squared(T v);

template<FastVector T>
uint8_t FSVF_VECCALL max_component_index(T v);

template<FastVector T>
uint8_t FSVF_VECCALL min_component_index(T v);

// returns a vector whose components are all the largest value in the argument
template<FastVector T>
Vec4 FSVF_VECCALL max_component_value(T v);

// returns a vector whose components are all the smallest value in the argument
template<FastVector T>
Vec4 FSVF_VECCALL min_component_value(T v);

// returns the largest component of the argument
template<FastVector T>
float FSVF_VECCALL max_component_value_f32(T v);

// returns the smallest component of the argument
template<FastVector T>
float FSVF_VECCALL min_component_value_f32(T v);

// sort components (smaller to larger)
template<FastVector T>
T FSVF_VECCALL sort(T v);

// reverse-sort components (larger to smaller)
template<FastVector T>
T FSVF_VECCALL reverse_sort(T v);

// component-wise operations
template<FastVector T>
T FSVF_VECCALL floor(T v);
template<FastVector T>
T FSVF_VECCALL ceil(T v);
template<FastVector T>
T FSVF_VECCALL abs(T v);
template<FastVector T>
auto FSVF_VECCALL sqrt(T v);

// TODO should depend on RECPR_MATH
// component-wise reciprocal of the square root; faster but less precise than doing `1.f/sqrt(v)`
template<FastVector T>
auto FSVF_VECCALL rsqrt(T v);

// returns a vector with the component-wise maximum of the two arguments
template<FastVector T1, FastVector T2>
auto FSVF_VECCALL merge_max(T1 v, T2 w) -> LargestDimensionType<T1, T2>;

// returns a vector with the component-wise minimum of the two arguments
template<FastVector T1, FastVector T2>
auto FSVF_VECCALL merge_min(T1 v, T2 w) -> LargestDimensionType<T1, T2>;

template<FastVector T1, FastVector T2>
float FSVF_VECCALL dot(T1 v, T2 w);

template<FastVector T1, FastVector T2>
LVec3 FSVF_VECCALL cross(T1 v, T2 w);

// returns a vector whose i-th component is the horizontal sum of the i-th argument
// logically equivalent to (but faster than) Vec4(sum(v1), sum(v2), sum(v3), sum(v4))
template<FastVector T1, FastVector T2, FastVector T3, FastVector T4>
Vec4 FSVF_VECCALL parallel_sum(T1 v1, T2 v2, T3 v3, T4 v4);

// returns a vector whose i-th component is the horizontal product of the i-th argument
// logically equivalent to (but faster than) Vec4(prod(v1), prod(v2), prod(v3), prod(v4))
template<FastVector T1, FastVector T2, FastVector T3, FastVector T4>
Vec4 FSVF_VECCALL parallel_prod(T1 v1, T2 v2, T3 v3, T4 v4);

// returns a vector obtained by blending the two arguments according to `BinaryMask`
// 0 -> keep corresponding first's argument component
// 1 -> keep corresponding second's argument component
// e.g. blend_with_mask<0b1011>(a, b) == {b.x, a.y, b.z, b.w}
// (note for the experts: unlike the corresponding intrinsic, this function processes the mask going
// most -> least significant bit)
template<uint8_t BinaryMask, FastVector T1, FastVector T2>
auto FSVF_VECCALL blend_with_mask(T1 v, T2 w) -> LargestDimensionType<T1, T2>;

// component-wise clamp
template<FastVector T1, FastVector T2>
T1 FSVF_VECCALL clamp(T1 v, T2 min, T2 max);

// returns true if all [VectorDimensions] components of the argument are 0xFFFF'FFFF
// valid values for `VectorDimensions` are 3 and 4
template<uint8_t VectorDimensions>
bool FSVF_VECCALL all(Vec4 bitmask);

// returns true if any of the first [VectorDimensions] components of the argument are 0xFFFF'FFFF
// valid values for `VectorDimensions` are 3 and 4
template<uint8_t VectorDimensions>
bool FSVF_VECCALL any(Vec4 bitmask);

// returns true if none of the first [VectorDimensions] components of the argument are 0xFFFF'FFFF
template<uint8_t VectorDimensions>
bool FSVF_VECCALL none(Vec4 bitmask);

template<FastVector T1, FastVector T2>
Vec4 FSVF_VECCALL and_not(T1 v, T2 w);

// shuffles the vector according to the shuffle passed as template argument;
// the shuffle argument is templated because `_mm_shuffle_ps()` expects a compile-time constant
// (each shuffle instruction has a different opcode)
// shuffling a 3-dimensional vector with a Shuf4 will return a 4D vector (the 4th component of the
// argument being == 0.f by type contract)
template<Shuf4 SHUFFLE, FastVector T>
auto FSVF_VECCALL shuffle(T v) -> LargestDimensionType<T, Vec4>;

// shuffles the vector according to the shuffle passed as template argument;
// the shuffle argument is templated because `_mm_shuffle_ps()` expects a compile-time constant
// (each shuffle instruction has a different opcode)
// shuffling 4-dimensional vector with a Shuf3 will keep the last component in place
template<Shuf3 SHUFFLE, FastVector T>
T FSVF_VECCALL shuffle(T v);

template<FastVector T>
FSVF_FORCEINLINE bool FSVF_VECCALL bitwise_equal(T v, T w);

namespace BitMasks
{
template<FastVector T1, FastVector T2>
Vec4 FSVF_VECCALL cmp_equal_f32(T1 v, T2 w);

template<FastVector T1, FastVector T2>
auto FSVF_VECCALL cmp_not_equal_f32(T1 v, T2 w) -> LargestDimensionType<T1, T2>;

template<FastVector T1, FastVector T2>
Vec4 FSVF_VECCALL cmp_equal_bitwise(T1 v, T2 w);
}

FSVF_FORCEINLINE bool FSVF_VECCALL all_ones_bitwise(Vec4 v);
FSVF_FORCEINLINE bool FSVF_VECCALL all_zeroes_bitwise(Vec4 v);

FSVF_FORCEINLINE uint8_t FSVF_VECCALL hi_bits(Vec4 v);

// TODO benchmark against `normalize()` and remove the slower one
template<FastVector T>
T FSVF_VECCALL normalize_alt(T v);
}    // namespace FSVF
