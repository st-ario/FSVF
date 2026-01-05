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

// TODO test

// alternately add and subtract entries: + - + -
// NOTE opposite convention than the one used for intrinsic instruction names
template<FastVector T1, FastVector T2>
auto FSVF_VECCALL addsub(T1 v1, T2 v2) -> LargestDimensionType<T1, T2>;

// alternately subtract and add entries: - + - +
// NOTE opposite convention than the one used for intrinsic instruction names
template<FastVector T1, FastVector T2>
auto FSVF_VECCALL subadd(T1 v1, T2 v2) -> LargestDimensionType<T1, T2>;

// TODO document

// IMPORTANT: addsub and subadd have opposite conventions wrt
//            instruction names

template<FastVector T1, FastVector T2, FastVector T3>
auto FSVF_VECCALL fmadd(T1 v1, T2 v2, T3 v3) -> LargestDimensionType<SmallestDimensionType<T1, T2>, T3>;

template<FastVector T1, FastVector T2, FastVector T3>
auto FSVF_VECCALL fmsub(T1 v1, T2 v2, T3 v3) -> LargestDimensionType<SmallestDimensionType<T1, T2>, T3>;

template<FastVector T1, FastVector T2, FastVector T3>
auto FSVF_VECCALL fmaddsub(T1 v1, T2 v2, T3 v3) -> LargestDimensionType<SmallestDimensionType<T1, T2>, T3>;

template<FastVector T1, FastVector T2, FastVector T3>
auto FSVF_VECCALL fmsubadd(T1 v1, T2 v2, T3 v3) -> LargestDimensionType<SmallestDimensionType<T1, T2>, T3>;

template<FastVector T1, FastVector T2, FastVector T3>
auto FSVF_VECCALL fnmadd(T1 v1, T2 v2, T3 v3) -> LargestDimensionType<SmallestDimensionType<T1, T2>, T3>;

template<FastVector T1, FastVector T2, FastVector T3>
auto FSVF_VECCALL fnmsub(T1 v1, T2 v2, T3 v3) -> LargestDimensionType<SmallestDimensionType<T1, T2>, T3>;

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
T FSVF_VECCALL max_component_value(T v);

// returns a vector whose components are all the smallest value in the argument
template<FastVector T>
T FSVF_VECCALL min_component_value(T v);

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

// compile-time blend (generates a single instruction)
// returns a vector obtained by blending the two arguments according to `BinaryMask`
// 0 -> keep corresponding first's argument component
// 1 -> keep corresponding second's argument component
// e.g. blend_with_mask<0b1011>(a, b) == {b.x, a.y, b.z, b.w}
// (note for the experts: unlike the corresponding intrinsic, this function processes the mask going
// most -> least significant bit)
template<uint8_t BinaryMask, FastVector T1, FastVector T2>
auto FSVF_VECCALL blend_with_mask(T1 v, T2 w) -> LargestDimensionType<T1, T2>;

// TODO document
// run-time blend
template<FastVector T1, FastVector T2, FastVector T3>
auto FSVF_VECCALL blend_with_vector_mask(T1 v, T2 w, T3 mask) -> LargestDimensionType<T1, T2>;

// component-wise clamp
template<FastVector T1, FastVector T2>
T1 FSVF_VECCALL clamp(T1 v, T2 min, T2 max);

// component-wise saturate
template<FastVector T1>
T1 FSVF_VECCALL saturate(T1 v);

// returns true if all [VectorDimensions] components of the argument are 0xFFFF'FFFF
// valid values for `VectorDimensions` are 3 and 4
template<uint8_t VectorDimensions>
bool FSVF_VECCALL all(Vec4 bitmask);

// returns true if any of the first [VectorDimensions] components of the argument are not 0
// valid values for `VectorDimensions` are 3 and 4
// NOTE: any non-zero *bit* anywhere in the argument will result in a `true` value.
template<uint8_t VectorDimensions>
bool FSVF_VECCALL any(Vec4 bitmask);

// returns true if all of the first [VectorDimensions] components of the argument are 0
template<uint8_t VectorDimensions>
bool FSVF_VECCALL none(Vec4 bitmask);

template<FastVector T1, FastVector T2>
auto FSVF_VECCALL and_not(T1 v, T2 w) -> T1;

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
auto FSVF_VECCALL shuffle(T v) -> T;

// shuffles and blends the two arguments;
// components 0 and 1 of the resulting vector will be taken from `v`
// components 2 and 3 of the resulting vector will be taken from `w`
// the shuffle argument is templated because `_mm_shuffle_ps()` expects a compile-time constant
// (each shuffle instruction has a different opcode)
// shuffling a 3-dimensional vector with a Shuf4 will return a 4D vector (the 4th component of the
// argument being == 0.f by type contract)
template<Shuf4 SHUFFLE, FastVector T1, FastVector T2>
auto FSVF_VECCALL shuffle_blend(T1 v, T2 w) -> LargestDimensionType<T1, T2>;

// shuffles and blends the two arguments;
// components 0 and 1 of the resulting vector will be taken from `v`
// component 3 (and the implicit component 4) of the resulting vector will be taken from `w`
// the shuffle argument is templated because `_mm_shuffle_ps()` expects a compile-time constant
// (each shuffle instruction has a different opcode)
// shuffling 4-dimensional vector with a Shuf3 will keep the last component in place (i.e.
// equal to the last component of `w`)
template<Shuf3 SHUFFLE, FastVector T1, FastVector T2>
auto FSVF_VECCALL shuffle_blend(T1 v, T2 w) -> LargestDimensionType<T1, T2>;

template<FastVector T>
FSVF_FORCEINLINE bool FSVF_VECCALL bitwise_equal(T v, T w);

namespace BitMasks
{
// TODO near-equality (epsilon and ULP based)

template<FastVector T1, FastVector T2>
Vec4 FSVF_VECCALL cmp_equal_f32(T1 v, T2 w);

template<FastVector T1, FastVector T2>
auto FSVF_VECCALL cmp_not_equal_f32(T1 v, T2 w) -> LargestDimensionType<T1, T2>;

template<FastVector T1, FastVector T2>
Vec4 FSVF_VECCALL cmp_equal_bitwise(T1 v, T2 w);

template<FastVector T1, FastVector T2>
Vec4 FSVF_VECCALL cmp_not_equal_bitwise(T1 v, T2 w);
}

bool FSVF_VECCALL all_ones_bitwise(Vec4 v);
bool FSVF_VECCALL all_zeroes_bitwise(Vec4 v);

uint8_t FSVF_VECCALL hi_bits(Vec4 v);

// Float properties utility functions
Vec4 mantissa            FSVF_VECCALL (FastVector auto x);
Vec4 exponent            FSVF_VECCALL (FastVector auto x);
Vec4 signed_exponent     FSVF_VECCALL (FastVector auto x);
Vec4 sign_bit            FSVF_VECCALL (FastVector auto x);
Vec4 is_inf_or_nan       FSVF_VECCALL (FastVector auto x);
Vec4 is_finite           FSVF_VECCALL (FastVector auto x);
Vec4 is_inf              FSVF_VECCALL (FastVector auto x);
Vec4 is_nan              FSVF_VECCALL (FastVector auto x);
bool any_is_inf_or_nan   FSVF_VECCALL (FastVector auto x);
bool any_is_finite       FSVF_VECCALL (FastVector auto x);
bool any_is_inf          FSVF_VECCALL (FastVector auto x);
bool any_is_nan          FSVF_VECCALL (FastVector auto x);
}    // namespace FSVF
