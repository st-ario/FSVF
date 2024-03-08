#pragma once

#include "Platform.hpp"

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
concept Float = requires { std::same_as<T, float>; };

// type constraint to prevent implicit type conversion
template<typename T>
concept U32 = requires { std::same_as<T, uint32_t>; };

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
constexpr float largest_negative_normal_float = bits_to_float(0x8080'0000u);

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
constexpr FSVF_FORCEINLINE bool     is_denormal     (Float auto x) { return x != 0 && is_finite(x) && mantissa(x) != 0; }

// clang-format on

constexpr float next_float_up(Float auto x)
{
#if defined(FSVF_FLOAT_FINITE_MATH) || defined(FSVF_FLOAT_DEFAULT_MATH)
    if (x == 0.f) return smallest_normal_float;
#endif

    return bits_to_float(float_to_bits(x) + 1);
    //uint32_t bits = float_to_bits(x);
    //const auto sign = bits & f32_sign_mask;
    //bits &= ~f32_sign_mask;
    //return sign ? bits_to_float(sign | (bits-1)) : bits_to_float(bits+1);
}

constexpr float next_float_down(Float auto x)
{
#if defined(FSVF_FLOAT_FINITE_MATH) || defined(FSVF_FLOAT_DEFAULT_MATH)
    if (x == 0.f) return -smallest_normal_float;
#endif

    return bits_to_float(float_to_bits(x) - 1);
    //if (x == 0.f) return -smallest_normal_float;
    //uint32_t bits = float_to_bits(x);
    //const auto sign = bits & f32_sign_mask;
    //bits &= ~f32_sign_mask;
    //return sign ? bits_to_float(sign | (bits+1)) : bits_to_float(bits-1);
}

// Implementation of ULP-related functions
constexpr FSVF_FORCEINLINE float ulp_value(Float auto x)
{
#if defined(FSVF_FLOAT_FINITE_MATH) || defined(FSVF_FLOAT_DEFAULT_MATH)
  if (x == 0.0f) {
    return smallest_normal_float;
  }
#endif
  return bits_to_float((float_to_bits(x) & f32_exponent_mask) | 1);
}

constexpr FSVF_FORCEINLINE double ulp_value_precise(Float auto x)
{
  return static_cast<double>(ulp_value(x));
}

constexpr FSVF_FORCEINLINE uint32_t ulps_distance(Float auto x, Float auto y)
{
  // TODO behavior on 0, NaNs and denormals depends on preprocessor definitions
  if (is_nan(x) || is_nan(y)) {
    return std::numeric_limits<uint32_t>::max();
  }

  // Handle signed zero comparison
  if (x == 0.0f && y == 0.0f) {
    return 0;
  }

  // Convert to bit representation for comparison
  uint32_t x_bits = float_to_bits(x);
  uint32_t y_bits = float_to_bits(y);

  // TODO double-check

  // For proper comparison, we need to handle negative numbers correctly
  // IEEE 754 uses sign-magnitude representation, so we need to convert to two's complement
  // for proper ordering of negative numbers
  if (sign_bit(x) != 0) {
    x_bits = 0xFFFF'FFFFu - ((x_bits & ~f32_sign_mask) - 1);
  }
  if (sign_bit(y) != 0) {
    y_bits = 0xFFFF'FFFFu - ((y_bits & ~f32_sign_mask) - 1);
  }

  // Calculate the absolute difference
  return (x_bits > y_bits) ? (x_bits - y_bits) : (y_bits - x_bits);
}

template<uint8_t UlpThreshold>
constexpr FSVF_FORCEINLINE bool almost_equal_ulp(Float auto x, Float auto y)
{
  // TODO behavior on 0, NaNs and denormals depends on preprocessor definitions

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

  // Handle signed zero comparison
  if (x == 0.0f && y == 0.0f) {
    return true;
  }

  // For finite numbers, compare using ULP distance
  return ulps_distance(x, y) <= UlpThreshold;
}

template<float Epsilon = std::numeric_limits<float>::epsilon() / 2.f>
constexpr FSVF_FORCEINLINE bool almost_equal_eps(Float auto x, Float auto y)
{
  // TODO behavior on 0, NaNs and denormals depends on preprocessor definitions

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

  // Handle signed zero comparison
  if (x == 0.0f && y == 0.0f) {
    return true;
  }

  // For finite numbers, compare using absolute difference
  return std::fabs(y - x) <= Epsilon;
}

template<uint8_t UlpThreshold, float Epsilon = std::numeric_limits<float>::epsilon()>
constexpr FSVF_FORCEINLINE bool almost_equal_eps_ulp(Float auto x, Float auto y)
{
  return almost_equal_ulp<UlpThreshold>(x,y) || almost_equal_eps<Epsilon>(x,y);
}

// TODO document NaN handling
constexpr FSVF_FORCEINLINE float clamp(float value, float min, float max)
{
  // clang-format off
  return (value < min) ? min :
    (value > max) ? max : value;
  // clang-format on
}

constexpr FSVF_FORCEINLINE float saturate(float value)
{
  return clamp(value, 0.0f, 1.0f);
}

// Explicit template instantiations for common ULP thresholds
template bool almost_equal_ulp<0>(float x, float y);
template bool almost_equal_ulp<1>(float x, float y);
template bool almost_equal_ulp<2>(float x, float y);
template bool almost_equal_ulp<4>(float x, float y);
template bool almost_equal_ulp<8>(float x, float y);

template bool almost_equal_eps_ulp<0>(float x, float y);
template bool almost_equal_eps_ulp<1>(float x, float y);
template bool almost_equal_eps_ulp<2>(float x, float y);
template bool almost_equal_eps_ulp<4>(float x, float y);
template bool almost_equal_eps_ulp<8>(float x, float y);

constexpr FSVF_FORCEINLINE bool almost_equal(Float auto x, Float auto y)
{
    return almost_equal_eps_ulp<2, std::numeric_limits<float>::epsilon()>(x, y);
}

}    // namespace FSVF
