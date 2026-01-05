#pragma once

#include "GenericMethods.hpp"
#include "HelperTypes.hpp"
#include "Operators.hpp"
#include "Platform.hpp"
#include "Vec4.hpp"

#include <bit>
#include <cmath>
#include <concepts>
#include <cstdint>
#include <limits>

namespace FSVF
{
/* notes/reminders on special float values:
 * - zero and infinity are signed; -0.f == 0.f always evaluates to true
 * - all ones exponent
 *   - all zeroes mantissa -> infinity
 *   - otherwise           -> NaN (we assume all NaNs as qNaNs, as per
 *                            -fno-trapping-math)
 * - all zeroes exponent
 *   - all zeroes mantissa -> zero
 *   - otherwise           -> denormals (the library will assume (and possibly
 *                            try to set) FTZ/DAZ, unless
 *                            FSVF_FSVF_FLOAT_DENORMAL_MATH is defined)
 */

// type constraint to prevent implicit type conversion
template<typename T>
concept Float = requires { requires std::same_as<T, float>; };

// type constraint to prevent implicit type conversion
template<typename T>
concept U32 = requires { requires std::same_as<T, uint32_t>; };

// bit cast, constrained to prevent implicit type conversion
constexpr FSVF_FORCEINLINE float bits_to_float(U32 auto bits)
{
  return std::bit_cast<float, uint32_t>(bits);
}

constexpr FSVF_FORCEINLINE uint32_t float_to_bits(Float auto f)
{
  return std::bit_cast<uint32_t, float>(f);
}

FSVF_FORCEINLINE __m128 bits_to_float(__m128i bits)
{
  return _mm_castsi128_ps(bits);
}

// bit cast utility for SSE types (compiles to no-op)
FSVF_FORCEINLINE __m128i float_to_bits(__m128 bits)
{
  return _mm_castps_si128(bits);
}

constexpr uint32_t f32_sign_mask            = 0x8000'0000u;
constexpr uint32_t f32_exponent_mask        = 0x7F80'0000u;
constexpr uint32_t f32_mantissa_mask        = 0x007F'FFFFu;
constexpr uint32_t f32_signed_exponent_mask = f32_sign_mask | f32_exponent_mask;

constexpr float negative_zero_float           = bits_to_float(f32_sign_mask);
constexpr float smallest_normal_float         = bits_to_float(0x0080'0000u);
constexpr float smallest_denormal_float       = bits_to_float(0x0000'0001u);

// clang-format off
// constrained to prevent implicit type conversion
constexpr FSVF_FORCEINLINE uint32_t mantissa        (Float auto x) { return float_to_bits(x) & f32_mantissa_mask; }
constexpr FSVF_FORCEINLINE uint32_t exponent        (Float auto x) { return float_to_bits(x) & f32_exponent_mask; }
constexpr FSVF_FORCEINLINE uint32_t signed_exponent (Float auto x) { return float_to_bits(x) & f32_signed_exponent_mask; }
constexpr FSVF_FORCEINLINE uint32_t sign_bit        (Float auto x) { return float_to_bits(x) & f32_sign_mask; }
constexpr FSVF_FORCEINLINE bool     is_inf_or_nan   (Float auto x) { return exponent(x) == f32_exponent_mask; }
constexpr FSVF_FORCEINLINE bool     is_finite       (Float auto x) { return !is_inf_or_nan(x); }
constexpr FSVF_FORCEINLINE bool     is_inf          (Float auto x) { return is_inf_or_nan(x) && mantissa(x) == 0; }
constexpr FSVF_FORCEINLINE bool     is_nan          (Float auto x) { return is_inf_or_nan(x) && mantissa(x) != 0; }

constexpr FSVF_FORCEINLINE bool is_denormal ([[maybe_unused]] Float auto x)
{
#ifdef FSVF_FLOAT_DENORMAL_MATH
  return x != 0 && exponent(x) == 0 && mantissa(x) != 0;
#else
  return false;
#endif
}
// clang-format on

template <typename T>
  requires(Float<T> || FastVector<T>)
T next_float_up(T x);

template <typename T>
  requires(Float<T> || FastVector<T>)
T next_float_down(T x);

static_assert(smallest_normal_float == std::numeric_limits<float>::min());

// TODO document "sticky" infinite
// TODO make branch-free?
template <>
constexpr float next_float_up(float x)
{
#if !defined(FSVF_FLOAT_DENORMAL_MATH)
  if (x == 0.f) return smallest_normal_float;
#else
  if (x == 0.f) return smallest_denormal_float;
#endif
#ifdef FSVF_ONE_SIDED_STICKY_INFINITY
  if (is_inf(x) && x > 0) return x;
#else
  if (is_inf(x)) return x;
#endif

  uint32_t bits = float_to_bits(x);
  const auto sign = bits & f32_sign_mask;
  bits &= ~f32_sign_mask;
  return sign ? bits_to_float(sign | (bits-1)) : bits_to_float(bits+1);
}

// TODO document "sticky" infinite
// TODO make branch-free?
template <>
constexpr float next_float_down(float x)
{
#if !defined(FSVF_FLOAT_DENORMAL_MATH)
  if (x == 0.f) return -smallest_normal_float;
#else
  if (x == 0.f) return -smallest_denormal_float;
#endif
#ifdef FSVF_ONE_SIDED_STICKY_INFINITY
  if (is_inf(x) && x < 0) return x;
#else
  if (is_inf(x)) return x;
#endif

  uint32_t bits = float_to_bits(x);
  const auto sign = bits & f32_sign_mask;
  bits &= ~f32_sign_mask;
  return sign ? bits_to_float(sign | (bits+1)) : bits_to_float(bits-1);
}

// Returns the ULP relative to the magnitude of the argument
constexpr FSVF_FORCEINLINE float ulp_for(Float auto x)
{
  return bits_to_float((float_to_bits(x) & f32_exponent_mask) | 1);
}

// Returns the ULP relative to the magnitude of the argument as a double
constexpr FSVF_FORCEINLINE double ulp_for_precise(Float auto x)
{
  return static_cast<double>(ulp_for(x));
}

// Count the number of floating point values between x and y,
// if their relative distance is small, i.e. the two values are
// less than 0x7F'FFFF (a full mantissa) floats apart,
// otherwise returns 0xFFFF'FFFF.
//
// Precondition: both arguments are finite
constexpr FSVF_FORCEINLINE uint32_t ulps_distance(Float auto x, Float auto y)
{
  // TODO optimize
#ifdef FSVF_FLOAT_DENORMAL_MATH
  // Handle sign in the case of denormals.

  // We only need to take care of the case where both
  // numbers are denormals or zero, the logic works correctly otherwise.
  const bool x_zero = x == 0.f;
  const bool y_zero = y == 0.f;

  if (x_zero && y_zero) return 0;

  if ((x_zero || is_denormal(x)) && (y_zero || is_denormal(y))) {
    // subtract as integers (two's complement)
    int32_t x_bits = std::bit_cast<int32_t>(float_to_bits(x)); // 0
    int32_t y_bits = std::bit_cast<int32_t>(float_to_bits(y)); // 0x80..01

    //auto x_bits = float_to_bits(x);
    //auto y_bits = float_to_bits(y);

    const bool x_sign = !!(x_bits & f32_sign_mask); // false
    const bool y_sign = !!(y_bits & f32_sign_mask); // true

    x_bits = x_bits & ~f32_sign_mask; // 0
    y_bits = y_bits & ~f32_sign_mask; // 1

    x_bits = x_sign ? -x_bits : x_bits; // 0
    y_bits = y_sign ? -y_bits : y_bits; // -1 (0xFFFF'FFFF)

    auto res = (x_bits > y_bits) ? (x_bits - y_bits) : (y_bits - x_bits);
    return res;
  }
#endif
  x = (x == 0.f) ? 0.f : x;
  y = (y == 0.f) ? 0.f : y;

  const uint32_t x_bits = float_to_bits(x);
  const uint32_t y_bits = float_to_bits(y);

  const auto exp_x = static_cast<int32_t>(x_bits >> 23);
  const auto exp_y = static_cast<int32_t>(y_bits >> 23);

  //const auto exp_d = (exp_x > exp_y) ? (exp_x - exp_y) : (exp_y - exp_x);
  const auto exp_d = std::abs(exp_x - exp_y);

  if (exp_d > 1) return 0xFFFF'FFFF;

  // TODO unnecessary (?)
  return (x_bits > y_bits) ? (x_bits - y_bits) : (y_bits - x_bits);
}

template<uint16_t UlpThreshold = 0xF>
constexpr FSVF_FORCEINLINE bool almost_equal_ulp(Float auto x, Float auto y)
{
#ifndef FSVF_FLOAT_FINITE_MATH
  // Handle special cases
  if (is_nan(x) || is_nan(y)) {
    return false;  // NaN is never equal to anything, including itself
  }

  if (is_inf(x) || is_inf(y)) {
    if (is_inf(x) && is_inf(y)) {
      // Both are infinity, check if they have the same sign
      return sign_bit(x) == sign_bit(y);
    } else {
      // One is infinity, the other is not
      return false;
    }
  }
#endif

  // For finite numbers, compare using ULP distance
  return ulps_distance(x, y) <= UlpThreshold;
}

template<float Epsilon = std::numeric_limits<float>::epsilon() / 2.f>
constexpr FSVF_FORCEINLINE bool almost_equal_eps(Float auto x, Float auto y)
{
#ifndef FSVF_FLOAT_FINITE_MATH
  // Handle special cases
  if (is_nan(x) || is_nan(y)) {
    return false;  // NaN is never equal to anything, including itself
  }

  if (is_inf(x) || is_inf(y)) {
    if (is_inf(x) && is_inf(y)) {
      // Both are infinity, check if they have the same sign
      return sign_bit(x) == sign_bit(y);
    } else {
      // One is infinity, the other is not
      return false;
    }
  }
#endif

  // For finite numbers, compare using absolute difference
  return std::fabs(y - x) <= Epsilon;
}

// TODO statically definable default values for UlpThreshold and Epsilon
template<uint16_t UlpThreshold = 0xF, float Epsilon = std::numeric_limits<float>::epsilon()>
constexpr FSVF_FORCEINLINE bool almost_equal_eps_ulp(Float auto x, Float auto y)
{
  return almost_equal_ulp<UlpThreshold>(x,y) || almost_equal_eps<Epsilon>(x,y);
}

constexpr FSVF_FORCEINLINE FSVF_VECCALL float clamp(auto value, auto min, auto max)
{
  // To date, this is the only way I've found to have all compilers
  // produce optimized code. If we nest the ternary operator or write
  // this as an if-else, only clang manages to output branchless code

  value = value < min ? min : value;
  value = value > max ? max : value;
  return value;
}

constexpr FSVF_FORCEINLINE float saturate(float value)
{
  return clamp(value, 0.0f, 1.0f);
}


constexpr FSVF_FORCEINLINE bool almost_equal(Float auto x, Float auto y)
{
    return almost_equal_eps_ulp<0xF, std::numeric_limits<float>::epsilon()>(x, y);
}

constexpr FSVF_FORCEINLINE __m128i ulps_distance(FastVector auto x, FastVector auto y)
{
#ifdef FSVF_FLOAT_DENORMAL_MATH
  // TODO
#endif

  // Mask away sign of zeroes
  auto nonzero_x = BitMasks::cmp_not_equal_f32(x, Vec4::zeroes());
  auto nonzero_y = BitMasks::cmp_not_equal_f32(y, Vec4::zeroes());

  x &= nonzero_x;
  y &= nonzero_y;

  const auto x_bits = float_to_bits(x);
  const auto y_bits = float_to_bits(y);

  // check distance of exponents
  const auto exp_x = _mm_srl_epi32(x_bits, _mm_set1_epi32(23));
  const auto exp_y = _mm_srl_epi32(y_bits, _mm_set1_epi32(23));

  const auto exp_d = abs(exp_x - exp_y);

  const auto mask_big = _mm_cmpgt_epi32_mask(exp_d, _mm_set1_epi32(1));
  const auto dist = _mm_abs_epi32(_mm_sub_epi32(x, y));

  return _mm_or_epi32(mask_big, dist);
}

template<FastVector V, uint16_t UlpThreshold = 0x4>
constexpr FSVF_FORCEINLINE bool almost_equal_ulp(V x, V y)
{
#ifndef FSVF_FLOAT_FINITE_MATH
  // Handle special cases
  if (any_is_nan(x) || any_is_nan(y)) {
    return false;  // NaN is never equal to anything, including itself
  }

  // Compare infinities first
  const auto infx = is_inf(x) & x;
  const auto infy = is_inf(y) & y;

  if (!all<4>(BitMasks::cmp_equal_bitwise(infx, infy))) {
    return false;
  }

  x &= ~is_inf(x);
  y &= ~is_inf(y);
#endif

  // For finite numbers, compare using ULP distance
  const auto res = _mm_cmple_epi32(ulps_distance(x, y), _mm_set1_epi32(UlpThreshold));
  return all<4>(bits_to_float(res));
}

template<FastVector V, float Epsilon = std::numeric_limits<float>::epsilon() / 2.f>
// TODO
/*constexpr*/ FSVF_FORCEINLINE bool almost_equal_eps(V x, V y)
{
#ifndef FSVF_FLOAT_FINITE_MATH
  // Handle special cases
  if (any_is_nan(x) || any_is_nan(y)) {
    return false;  // NaN is never equal to anything, including itself
  }

  // Compare infinities first
  const Vec4 infx = is_inf(x) & x;
  const Vec4 infy = is_inf(y) & y;

  if (!all<4>(BitMasks::cmp_equal_bitwise(infx, infy))) {
    return false;
  }

  const auto fx = x & ~is_inf(x);
  const auto fy = x & ~is_inf(y);

  return all<4>(Vec4{abs(fy - fx)} <= Vec4{Epsilon});
#else
  // For finite numbers, compare using absolute difference
  return all<4>(Vec4{abs(y - x)} <= Vec4{Epsilon});
#endif
}

template<FastVector V, uint16_t UlpThreshold = 0x4, float Epsilon = std::numeric_limits<float>::epsilon()>
// TODO
/*constexpr*/ FSVF_FORCEINLINE bool almost_equal_eps_ulp(V x, V y)
{
  return almost_equal_ulp<UlpThreshold>(x,y) || almost_equal_eps<Epsilon>(x,y);
}

// TODO
/*constexpr*/ FSVF_FORCEINLINE bool almost_equal(FastVector auto x, FastVector auto y)
{
    return almost_equal_eps_ulp<0x4, std::numeric_limits<float>::epsilon()>(x, y);
}

}    // namespace FSVF
