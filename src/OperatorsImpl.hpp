#pragma once

#include "FloatUtils.hpp"
#include "GenericMethods.hpp"
#include "Operators.hpp"
#include "VecInternals.hpp"

namespace FSVF
{

template<FastVector T1, FastVector T2>
FSVF_FORCEINLINE auto FSVF_VECCALL operator|(T1 v, T2 w)
  -> LargestDimensionType<T1, T2>
{
  using Res = LargestDimensionType<T1, T2>;
  return FSVF_RAW_(Res, _mm_or_ps(m_sse(v), m_sse(w)));
}

template<FastVector T1, FastVector T2>
FSVF_FORCEINLINE auto FSVF_VECCALL operator&(T1 v, T2 w)
  -> SmallestDimensionType<T1, T2>
{
  using Res = SmallestDimensionType<T1, T2>;
  return FSVF_RAW_(Res, _mm_and_ps(m_sse(v), m_sse(w)));
}

template<FastVector T1, FastVector T2>
FSVF_FORCEINLINE auto FSVF_VECCALL operator^(T1 v, T2 w)
  -> LargestDimensionType<T1, T2>
{
  using Res = LargestDimensionType<T1, T2>;

  // cleanup of last component might be needed for XOR
  return Res{ Vec4{ _mm_xor_ps(m_sse(v), m_sse(w)) } };
}

template<FastVector T>
FSVF_FORCEINLINE T FSVF_VECCALL operator~(T v)
{
  if constexpr (std::is_base_of_v<LVec3, T>)
  {
    static_assert(
      Internals::always_false_v<T>,
      "Using operator~() on a 3D vector is ambiguous in terms of what should "
      "happen in the (hidden) fourth component; please use this operator on a "
      "4D vector, making all intended casts explicit"
    );
  }

  auto ones = bits_to_float(
    _mm_cmpeq_epi32(float_to_bits(m_sse(v)), float_to_bits(m_sse(v)))
  );

  return v ^ (FSVF_RAW_(T, ones));
}

template<FastVector T1, FastVector T2>
  requires(SameDimension<T1, T2>)
FSVF_FORCEINLINE T1& FSVF_VECCALL operator&=(T1& v, T2 w)
{
  v = v & w;
  return v;
}

template<FastVector T1, FastVector T2>
  requires(SameDimension<T1, T2>)
FSVF_FORCEINLINE T1& FSVF_VECCALL operator|=(T1& v, T2 w)
{
  v = v | w;
  return v;
}

template<FastVector T1, FastVector T2>
  requires(SameDimension<T1, T2>)
FSVF_FORCEINLINE T1& FSVF_VECCALL operator^=(T1& v, T2 w)
{
  v = v ^ w;
  return v;
}

template<FastVector T>
FSVF_FORCEINLINE T FSVF_VECCALL operator-(T v)
{
  if constexpr (!std::is_base_of_v<LVec3, T>)
  {
    const __m128i vb        = float_to_bits(m_sse(v));
    const __m128i ones      = _mm_cmpeq_epi32(vb, vb);
    const __m128  sign_mask = bits_to_float(_mm_slli_epi32(ones, 31));
    return FSVF_RAW_(T, _mm_xor_ps(m_sse(v), sign_mask));
  }
  else { return LVec3(-Vec4(v)); }
}

template<FastVector T1, FastVector T2>
FSVF_FORCEINLINE bool FSVF_VECCALL operator==(T1 v, T2 w)
{
  __m128i res = _mm_castps_si128(_mm_cmpneq_ps(m_sse(v), m_sse(w)));
  return static_cast<bool>(_mm_testz_si128(res, res));
}

template<FastVector T1, FastVector T2>
FSVF_FORCEINLINE bool FSVF_VECCALL operator!=(T1 v, T2 w)
{
  return !(v == w);
}

template<FastVector T1, FastVector T2>
FSVF_FORCEINLINE auto FSVF_VECCALL operator+(T1 v, T2 w)
  -> LargestDimensionType<T1, T2>
{
  using Res = LargestDimensionType<T1, T2>;
  return FSVF_RAW_(Res, _mm_add_ps(m_sse(v), m_sse(w)));
}

template<FastVector T1, FastVector T2>
FSVF_FORCEINLINE auto FSVF_VECCALL operator-(T1 v, T2 w)
  -> LargestDimensionType<T1, T2>
{
  using Res = LargestDimensionType<T1, T2>;
  return FSVF_RAW_(Res, _mm_sub_ps(m_sse(v), m_sse(w)));
}

// TODO document on declaration
template<FastVector T1, FastVector T2>
FSVF_FORCEINLINE auto FSVF_VECCALL operator/(T1 v, T2 x) -> decltype(v * Vec4{1.f})
{
    Vec4 divisor = x;
    if constexpr (std::derived_from<T2, LVec3>)
    {
        static_assert(SameDimension<T1, T2>, "Invalid use of operator/(): can't divide a 4D vector by a 3D vector");
        // avoid dividing by zero
        divisor |= Vec4{0.f, 0.f, 0.f, 1.f};
    }

#if defined(FSVF_NO_RECIPROCAL_MATH)
  return Res{ Vec4{ _mm_div_ps(m_sse(v), m_sse(w)) } };
#else
  Vec4 rcp = _mm_rcp_ps(divisor);
  // TODO statically configurable number of Newton-Raphson steps
  const auto nr = fnmadd(divisor, rcp, Vec4{2.f});
  const Vec4 mul = nr * rcp;
  // strip NaNs
  // TODO stripping NaNs should depend on a preprocessor flag
  const Vec4 mask = _mm_cmpord_ps(nr, Vec4{0.f});
  rcp = _mm_blendv_ps(rcp, mul, mask);
  return v * rcp;
#endif
}

template<FastVector T1, FastVector T2>
FSVF_FORCEINLINE auto FSVF_VECCALL operator*(T1 v, T2 w)
  -> SmallestDimensionType<T1, T2>
{
  using Res = SmallestDimensionType<T1, T2>;
  return FSVF_RAW_(Res, _mm_mul_ps(m_sse(v), m_sse(w)));
}

template<FastVector T>
FSVF_FORCEINLINE T FSVF_VECCALL operator*(float x, T v)
{
  return FSVF_RAW_(T, _mm_mul_ps(_mm_set_ps1(x), m_sse(v)));
}

template<FastVector T>
FSVF_FORCEINLINE T FSVF_VECCALL operator*(T v, float x)
{
  return x * v;
}

// TODO document on declaration
template<FastVector T>
FSVF_FORCEINLINE T FSVF_VECCALL operator/(float x, T v)
{
  return T{ x } / v;
}

// TODO document on declaration
template<FastVector T>
FSVF_FORCEINLINE T FSVF_VECCALL operator/(T v, float x)
{
  return T{ v / Vec4{ x } };
}

template<FastVector T1, FastVector T2>
FSVF_FORCEINLINE T1& FSVF_VECCALL operator-=(T1& v, T2 w)
{
  if constexpr (std::derived_from<T1, LVec3>)
    {
        static_assert(SameDimension<T1, T2>, "Invalid use of operator-=() to alter a 3D vector by a 4D vector");
    }
  v = v - w;
  return v;
}

template<FastVector T1, FastVector T2>
FSVF_FORCEINLINE T1& FSVF_VECCALL operator+=(T1& v, T2 w)
{
  if constexpr (std::derived_from<T1, LVec3>)
    {
        static_assert(SameDimension<T1, T2>, "Invalid use of operator+=() to decrement a 3D vector by a 4D vector");
    }
  v = v + w;
  return v;
}

template<FastVector T1, FastVector T2>
FSVF_FORCEINLINE T1& FSVF_VECCALL operator/=(T1& v, T2 w)
{
  if constexpr (std::derived_from<T2, LVec3>)
    {
        static_assert(SameDimension<T1, T2>, "Invalid use of operator/=() to scale a 4D vector by a 3D vector");
    }
  v = T1{v / w};
  return v;
}    // TODO document on declaration

template<FastVector T1, FastVector T2>
FSVF_FORCEINLINE T1& FSVF_VECCALL operator*=(T1& v, T2 w)
{
  v = v * w;
  return v;
}

template<FastVector T>
FSVF_FORCEINLINE T& FSVF_VECCALL operator/=(T& v, float x)
{
  v = v / x;
  return v;
}    // TODO specialize to avoid dividing by 0 in LVec3s

template<FastVector T>
FSVF_FORCEINLINE T& FSVF_VECCALL operator*=(T& v, float x)
{
  v = v * x;
  return v;
}

template<FastVector T1, FastVector T2>
requires(SameDimension<T1, T2>)
FSVF_FORCEINLINE Vec4 FSVF_VECCALL operator<=(T1 v, T2 w)
{
  return _mm_cmple_ps(m_sse(v), m_sse(w));
}

template<FastVector T1, FastVector T2>
requires(SameDimension<T1, T2>)
FSVF_FORCEINLINE Vec4 FSVF_VECCALL operator>=(T1 v, T2 w)
{
  return _mm_cmpge_ps(m_sse(v), m_sse(w));
}

template<FastVector T1, FastVector T2>
requires(SameDimension<T1, T2>)
FSVF_FORCEINLINE Vec4 FSVF_VECCALL operator<(T1 v, T2 w)
{
  return _mm_cmplt_ps(m_sse(v), m_sse(w));
}

template<FastVector T1, FastVector T2>
requires(SameDimension<T1, T2>)
FSVF_FORCEINLINE Vec4 FSVF_VECCALL operator>(T1 v, T2 w)
{
  return _mm_cmpgt_ps(m_sse(v), m_sse(w));
}

}    // namespace FSVF
