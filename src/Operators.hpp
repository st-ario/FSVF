#pragma once

#include "Vec4.hpp"

namespace FSVF
{

// clang-format off

// comparisons returning 4 bitmasks
// 0xFFFF'FFFF if the condition holds, 0x0 otherwise
template<FastVector T> Vec4 FSVF_VECCALL operator<=(T v, T w);
template<FastVector T> Vec4 FSVF_VECCALL operator>=(T v, T w);
template<FastVector T> Vec4 FSVF_VECCALL operator< (T v, T w);
template<FastVector T> Vec4 FSVF_VECCALL operator> (T v, T w);

// comparison returning a boolean
// use BitMasks::cmp_equal_f32() for the 4 bitmasks equivalent
template<FastVector T1, FastVector T2>
bool FSVF_VECCALL operator==(T1 v, T2 w);

// comparison returning a boolean
// use BitMasks::cmp_not_equal_f32() for the 4 bitmasks equivalent
template<FastVector T1, FastVector T2>
bool FSVF_VECCALL operator!=(T1 v, T2 w);

template<FastVector T> T FSVF_VECCALL operator*(float x, T v);
template<FastVector T> T FSVF_VECCALL operator/(float x, T v);
template<FastVector T> T FSVF_VECCALL operator*(T v, float x);
template<FastVector T> T FSVF_VECCALL operator/(T v, float x);

template<FastVector T> T FSVF_VECCALL operator-(T v);
template<FastVector T> T FSVF_VECCALL operator~(T v);

template<FastVector T1, FastVector T2> auto FSVF_VECCALL operator^(T1 v, T2 w) -> LargestDimensionType<T1, T2>;
template<FastVector T1, FastVector T2> auto FSVF_VECCALL operator|(T1 v, T2 w) -> LargestDimensionType<T1, T2>;
template<FastVector T1, FastVector T2> auto FSVF_VECCALL operator&(T1 v, T2 w) -> SmallestDimensionType<T1, T2>;

template<FastVector T1, FastVector T2> auto FSVF_VECCALL operator+(T1 v, T2 w) -> LargestDimensionType<T1, T2>;
template<FastVector T1, FastVector T2> auto FSVF_VECCALL operator-(T1 v, T2 w) -> LargestDimensionType<T1, T2>;
template<FastVector T1, FastVector T2> auto FSVF_VECCALL operator/(T1 v, T2 w) -> LargestDimensionType<T1, T2>;
template<FastVector T1, FastVector T2> auto FSVF_VECCALL operator*(T1 v, T2 w) -> SmallestDimensionType<T1, T2>;

template<FastVector T1, FastVector T2> T1& FSVF_VECCALL operator-=(T1& v, T2 w);
template<FastVector T1, FastVector T2> T1& FSVF_VECCALL operator+=(T1& v, T2 w);
template<FastVector T1, FastVector T2> T1& FSVF_VECCALL operator/=(T1& v, T2 w);
template<FastVector T1, FastVector T2> T1& FSVF_VECCALL operator*=(T1& v, T2 w);

template<FastVector T> T& FSVF_VECCALL operator/=(T& v, float x);
template<FastVector T> T& FSVF_VECCALL operator*=(T& v, float x);

template<FastVector T1, FastVector T2>
  requires(SameDimension<T1, T2>) T1& FSVF_VECCALL operator&=(T1& v, T2 w);

template<FastVector T1, FastVector T2>
  requires(SameDimension<T1, T2>) T1& FSVF_VECCALL operator|=(T1& v, T2 w);

template<FastVector T1, FastVector T2>
  requires(SameDimension<T1, T2>) T1& FSVF_VECCALL operator^=(T1& v, T2 w);

// clang-format on
}    // namespace FSVF
