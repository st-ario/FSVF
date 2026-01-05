#pragma once

//#define FSVF_FLOAT_FINITE_MATH
//#define FSVF_FLOAT_DENORMAL_MATH

#include "Config.hpp"
#include "FloatUtils.hpp"
#include "LMat3Impl.hpp"
#include "Mat4Impl.hpp"
#include "Vec4Impl.hpp"

#include <stdint.h>

// Fast Small Vectors of Floats

namespace FSVF
{

// Precondition: `is_nan(x) == false` for all components
// Note: The sign of zeroes is ignored.
// Note: In default and finite float arithmetic, we step up from zero to the
// smallest *normal* float. If FSVF_FLOAT_DENORMAL_MATH is defined, we step
// up to the actual next adjacent float, i.e. the smallest *denormal* float.
template <typename V>
  requires(Float<V> || FastVector<V>)
FSVF_FORCEINLINE
V next_float_up(V v)
{
    const Vec4 signs{bits_to_float(f32_sign_mask)};
    const Vec4 abs_v = Vec4{v} & ~signs;
    const auto negative_mask = v < V::zeroes();

    // step up/down
    const auto bits = float_to_bits(abs_v);
    const auto step_dn = bits_to_float(_mm_sub_epi32(bits, _mm_set1_epi32(1)));
    const auto step_up = bits_to_float(_mm_add_epi32(bits, _mm_set1_epi32(1)));

    auto merge = V{blend_with_vector_mask(step_up, step_dn, negative_mask)};
    merge |= (signs & v);

    // In default and finite float arithmetic, we step up from zero to the
    // closest *normal* float.
    const auto zero_mask = BitMasks::cmp_equal_f32(v, V::zeroes());
#if !defined(FSVF_FLOAT_DENORMAL_MATH)
    merge = blend_with_vector_mask(merge, V{smallest_normal_float}, zero_mask);
#else
    merge = blend_with_vector_mask(merge, V{smallest_denormal_float}, zero_mask);
#endif

  #ifdef FSVF_ONE_SIDED_STICKY_INFINITY
    const auto inf_mask = is_inf(v) & (v > V::zeroes());
  #else
    const auto inf_mask = is_inf(v);
  #endif

  merge = blend_with_vector_mask(merge, v, inf_mask);

  return merge;
}

// Precondition: `is_nan(x) == false` for all components
// Note: The sign of zeroes is ignored.
// Note: In default and finite float arithmetic, we step down from zero to the
// closes *normal* float. If FSVF_FLOAT_DENORMAL_MATH is defined, we step
// down to the actual next adjacent float, i.e. the negative of the
// smallest *denormal* float.
template <FSVF::FastVector V>
FSVF_FORCEINLINE
V next_float_down(V v)
{
  //const auto vv = Vec4{v};
  //const auto c = vv.components();
  //return V{Vec4{next_float_down(c[0]), next_float_down(c[1]), next_float_down(c[2]), next_float_down(c[3])}};
    const Vec4 signs{bits_to_float(f32_sign_mask)};
    const Vec4 abs_v = Vec4{v} & ~signs;
    const auto negative_mask = v < V::zeroes();

    // step up/down
    const auto bits = float_to_bits(abs_v);
    const auto step_dn = bits_to_float(_mm_sub_epi32(bits, _mm_set1_epi32(1)));
    const auto step_up = bits_to_float(_mm_add_epi32(bits, _mm_set1_epi32(1)));

    auto merge = V{blend_with_vector_mask(step_dn, step_up, negative_mask)};
    merge |= (signs & v);

    // zeroes:
    // In default and finite float arithmetic, we step down from zero to the
    // closest *normal* float.
    const auto zero_mask = BitMasks::cmp_equal_f32(v, V::zeroes());
#if !defined(FSVF_FLOAT_DENORMAL_MATH)
    merge = blend_with_vector_mask(merge, V{-smallest_normal_float}, zero_mask);
#else
    merge = blend_with_vector_mask(merge, V{-smallest_denormal_float}, zero_mask);
#endif

  #ifdef FSVF_ONE_SIDED_STICKY_INFINITY
    const auto inf_mask = is_inf(v) & (v < V::zeroes());
  #else
    const auto inf_mask = is_inf(v);
  #endif

  merge = blend_with_vector_mask(merge, v, inf_mask);

  return merge;
}

// TODO runtime shuffle

// TODO prefer unpackh/l to movhl/lh


// TODO USE _mm_insrt_epi32() and similar!!!!
// TODO add missing FSVF_VECCALLs
// TODO FSVF_FORCEINLINE almost everything
}    // namespace FSVF

// cleanup internal macros
#ifdef FSVF_RAW_
  #undef FSVF_RAW_
#endif
