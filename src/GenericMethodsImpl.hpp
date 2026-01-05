#pragma once

#include "OperatorsImpl.hpp"

#include <cmath>
#include <utility>

namespace FSVF
{
template<FastVector T1, FastVector T2>
FSVF_FORCEINLINE T1 FSVF_VECCALL and_not(T1 v, T2 w)
{
  // we want v AND (NOT w), so the arguments are flipped in the intrinsic call
  return
  FSVF_RAW_(T1, _mm_andnot_ps(m_sse(w), m_sse(v)));
}

namespace Internals
{
// bitwise magic to reverse a 4-bit number in binary representation
// requires: x <= 0xF
FSVF_FORCEINLINE constexpr uint8_t magic_flip(uint8_t x)
{
  // understanding how this works is left as an exercise for the reader :)
  int32_t res  = x;
  res         *= 0x222u;
  res         &= 0x1284u;

  uint32_t magic   = res * 0x2083;
  magic          >>= 19;
  magic           *= 0xC1;

  res += magic;
  return static_cast<uint8_t>(res);
}
}    // namespace Internals

template<FastVector T>
FSVF_FORCEINLINE bool FSVF_VECCALL bitwise_equal(T v, T w)
{
  // bitwise equality <=> a ^ b == 0
  __m128i xorRes = _mm_castps_si128(_mm_xor_ps(m_sse(v), m_sse(w)));
  return _mm_testz_si128(xorRes, xorRes);
}

template<uint8_t BinaryMask, FastVector T1, FastVector T2>
FSVF_FORCEINLINE auto FSVF_VECCALL blend_with_mask(T1 v, T2 w)
  -> LargestDimensionType<T1, T2>
  requires(BinaryMask <= 0b1111)
{
  using Res = LargestDimensionType<T1, T2>;
  const __m128 blended
    = _mm_blend_ps(m_sse(v), m_sse(w), Internals::magic_flip(BinaryMask));
  return FSVF_RAW_(Res, blended);
}

// TODO rename?
// TODO invert order of arguments?
template<FastVector T1, FastVector T2, FastVector T3>
FSVF_FORCEINLINE auto FSVF_VECCALL blend_with_vector_mask(T1 v, T2 w, T3 mask)
  -> LargestDimensionType<T1, T2>
{
  using Res = LargestDimensionType<T1, T2>;
  const __m128 blended = _mm_blendv_ps(m_sse(v), m_sse(w), m_sse(mask));
  return FSVF_RAW_(Res, blended);
}

namespace BitMasks
{
template<FastVector T1, FastVector T2>
FSVF_FORCEINLINE Vec4 FSVF_VECCALL cmp_equal_f32(T1 v, T2 w)
{
  return Vec4{ _mm_cmpeq_ps(m_sse(v), m_sse(w)) };
}

template<FastVector T1, FastVector T2>
FSVF_FORCEINLINE auto FSVF_VECCALL cmp_not_equal_f32(T1 v, T2 w)
  -> LargestDimensionType<T1, T2>
{
  using Res = LargestDimensionType<T1, T2>;
  return FSVF_RAW_(Res, _mm_cmpneq_ps(m_sse(v), m_sse(w)));
}

template<FastVector T1, FastVector T2>
FSVF_FORCEINLINE Vec4 FSVF_VECCALL cmp_equal_bitwise(T1 v, T2 w)
{
  if constexpr (std::is_base_of_v<LVec3, T1> || std::is_base_of_v<LVec3, T2>)
  {
    static_assert(
      Internals::always_false_v<T1>,
      "Using a bitwise comparison on a 3D vector is bug-prone due to the "
      "(hidden) fourth component; please use this operator on 4D vectors, "
      "making all intended casts explicit"
    );
  }
  return bits_to_float(
    _mm_cmpeq_epi32(float_to_bits(m_sse(v)), float_to_bits(m_sse(w)))
  );
}

template<FastVector T1, FastVector T2>
FSVF_FORCEINLINE Vec4 FSVF_VECCALL cmp_not_equal_bitwise(T1 v, T2 w)
{
  return ~cmp_equal_bitwise(v, w);
}

FSVF_FORCEINLINE bool all_ones_bitwise(Vec4 v)
{
  const auto s    = float_to_bits(m_sse(v));
  const auto ones = _mm_cmpeq_epi32(s, s);
  return _mm_testc_si128(s, ones);
}

FSVF_FORCEINLINE bool all_zeroes_bitwise(Vec4 v)
{
  // bit-wise zero <=> x & x == 0
  const __m128i bits = float_to_bits(m_sse(v));
  return _mm_testz_si128(bits, bits);
}

// isolate and pack bits of index 32j-1 in Vec4 for 1 <= j <= 4
// can be interpreted as the sign-bits of its components
FSVF_FORCEINLINE uint8_t hi_bits(Vec4 v) { return Internals::magic_flip(_mm_movemask_ps(m_sse(v))); }
}    // namespace BitMasks

template<Shuf4 SHUFFLE, FastVector T>
FSVF_FORCEINLINE auto FSVF_VECCALL shuffle(T v) -> LargestDimensionType<T, Vec4>
{
  using Res = LargestDimensionType<T, Vec4>;
  return Res{ _mm_shuffle_ps(m_sse(v), m_sse(v), std::to_underlying(SHUFFLE)) };
}

template<Shuf3 SHUFFLE, FastVector T>
FSVF_FORCEINLINE auto FSVF_VECCALL shuffle(T v) -> T
{
  return FSVF_RAW_(
    T,
    _mm_shuffle_ps(m_sse(v), m_sse(v), std::to_underlying(SHUFFLE))
  );
}

template<Shuf4 SHUFFLE, FastVector T1, FastVector T2>
FSVF_FORCEINLINE auto FSVF_VECCALL shuffle_blend(T1 v, T2 w) -> LargestDimensionType<T1, T2>
{
  using Res = LargestDimensionType<T1, T2>;
  return FSVF_RAW_(
    Res,
    _mm_shuffle_ps(m_sse(v), m_sse(w), std::to_underlying(SHUFFLE))
  );
}

template<Shuf3 SHUFFLE, FastVector T1, FastVector T2>
FSVF_FORCEINLINE auto FSVF_VECCALL shuffle_blend(T1 v, T2 w) -> LargestDimensionType<T1, T2>
{
  using Res = LargestDimensionType<T1, T2>;
  return FSVF_RAW_(
    Res,
    _mm_shuffle_ps(m_sse(v), m_sse(w), std::to_underlying(SHUFFLE))
  );
}

template<uint8_t VectorDimensions>
  //requires(VectorDimensions == 3 || VectorDimensions == 4)
FSVF_FORCEINLINE bool FSVF_VECCALL all(Vec4 bitmask)
{
  if constexpr (VectorDimensions == 3)
  {
    bitmask |= Vec4{ 0.f, 0.f, 0.f, bits_to_float(0xFFFF'FFFFu) };
  }

  return BitMasks::all_ones_bitwise(bitmask);
}

template<uint8_t VectorDimensions>
  //requires(VectorDimensions == 3 || VectorDimensions == 4)
FSVF_FORCEINLINE bool FSVF_VECCALL none(Vec4 bitmask)
{
  if constexpr (VectorDimensions == 3) { bitmask = LVec3(bitmask); }

  return BitMasks::all_zeroes_bitwise(bitmask);
}

template<uint8_t VectorDimensions>
FSVF_FORCEINLINE bool FSVF_VECCALL any(Vec4 bitmask)
{
  return !none<VectorDimensions>(bitmask);
}

template bool FSVF_VECCALL all<3>(Vec4);
template bool FSVF_VECCALL all<4>(Vec4);
template bool FSVF_VECCALL none<3>(Vec4);
template bool FSVF_VECCALL none<4>(Vec4);
template bool FSVF_VECCALL any<3>(Vec4);
template bool FSVF_VECCALL any<4>(Vec4);

namespace Internals
{
template<typename Fn>
concept Cumulable = std::is_invocable_r_v<__m128, Fn, __m128, __m128>;

// For v = (x y z w)
// Returns a 4-vector whose first component is f(f(x,y), f(z,w))
template<Cumulable Fn>
__m128 FSVF_FORCEINLINE FSVF_VECCALL reduce0(Fn f, __m128 v)
{
  // see https://stackoverflow.com/a/35270026
  const __m128 shuf    = _mm_movehdup_ps(v);             // y y w w
  const __m128 couple  = f(v, shuf);                     // fxy fyy fzw fww
  const __m128 partial = _mm_movehl_ps(shuf, couple);    // fzw fww w w
  return f(couple, partial);    // f(fxy fzw) f(fyy fww) f(fzw w) f(fww w)
}

// Reduces the four arguments horizontally.
// For vN = (xN yN zN wN), each component of the resulting vector will be
// f(f(xN,zN), f(zN,wN))
template<Cumulable Fn>
__m128 FSVF_FORCEINLINE FSVF_VECCALL
  reduceAll(Fn f, __m128 v0, __m128 v1, __m128 v2, __m128 v3)
{
  // see Intel(R) 64 and IA-32 Architectures Optimization Reference Manual, 7.5.1.4

  // v0 = a b c d
  // v1 = x y z w
  // v2 = i j k l
  // v3 = m n o p
  const __m128 t0 = _mm_movelh_ps(v0, v1); // a b x y
  const __m128 t1 = _mm_movehl_ps(v1, v0); // c d z w
  const __m128 t2 = _mm_movelh_ps(v2, v3); // i j m n
  const __m128 t3 = _mm_movehl_ps(v3, v2); // k l o p

  const __m128 a0 = f(t0, t1); // fac fbd fxz fyw
  const __m128 a1 = f(t2, t3); // fik fjl fmo fnp

  const __m128 s0 = _mm_shuffle_ps(a0, a1, std::to_underlying(Shuf4::xzxz)); // fac fxz fik fmo
  const __m128 s1 = _mm_shuffle_ps(a0, a1, std::to_underlying(Shuf4::ywyw)); // fbd fyw fjo fnp

  return f(s0, s1); // f(fac fbd) f(fxz fyw) f(fik fjo) f(fmo fnp)
}
}    // namespace Internals

// TODO add tests

template<FastVector T1, FastVector T2>
FSVF_FORCEINLINE auto FSVF_VECCALL addsub(T1 v1, T2 v2) -> LargestDimensionType<T1, T2>
{
  using Res = LargestDimensionType<T1, T2>;
  return FSVF_RAW_(Res, _mm_subadd_ps(m_sse(v1), m_sse(v2)));
}

template<FastVector T1, FastVector T2>
FSVF_FORCEINLINE auto FSVF_VECCALL subadd(T1 v1, T2 v2) -> LargestDimensionType<T1, T2>
{
  using Res = LargestDimensionType<T1, T2>;
  return FSVF_RAW_(Res, _mm_addsub_ps(m_sse(v1), m_sse(v2)));
}

template<FastVector T1, FastVector T2, FastVector T3>
FSVF_FORCEINLINE auto FSVF_VECCALL fmadd(T1 v1, T2 v2, T3 v3) -> LargestDimensionType<SmallestDimensionType<T1, T2>, T3>
{
  using Res = LargestDimensionType<SmallestDimensionType<T1, T2>, T3>;
  return FSVF_RAW_(Res, _mm_fmadd_ps(m_sse(v1), m_sse(v2), m_sse(v3)));
}

template<FastVector T1, FastVector T2, FastVector T3>
FSVF_FORCEINLINE auto FSVF_VECCALL fmsub(T1 v1, T2 v2, T3 v3) -> LargestDimensionType<SmallestDimensionType<T1, T2>, T3>
{
  using Res = LargestDimensionType<SmallestDimensionType<T1, T2>, T3>;
  return FSVF_RAW_(Res, _mm_fmsub_ps(m_sse(v1), m_sse(v2), m_sse(v3)));
}

template<FastVector T1, FastVector T2, FastVector T3>
FSVF_FORCEINLINE auto FSVF_VECCALL fmaddsub(T1 v1, T2 v2, T3 v3) -> LargestDimensionType<SmallestDimensionType<T1, T2>, T3>
{
  using Res = LargestDimensionType<SmallestDimensionType<T1, T2>, T3>;
  return FSVF_RAW_(Res, _mm_fmsubadd_ps(m_sse(v1), m_sse(v2), m_sse(v3)));
}

// TODO add test
template<FastVector T1, FastVector T2, FastVector T3>
FSVF_FORCEINLINE auto FSVF_VECCALL fmsubadd(T1 v1, T2 v2, T3 v3) -> LargestDimensionType<SmallestDimensionType<T1, T2>, T3>
{
  using Res = LargestDimensionType<SmallestDimensionType<T1, T2>, T3>;
  return FSVF_RAW_(Res, _mm_fmaddsub_ps(m_sse(v1), m_sse(v2), m_sse(v3)));
}

template<FastVector T1, FastVector T2, FastVector T3>
FSVF_FORCEINLINE auto FSVF_VECCALL fnmadd(T1 v1, T2 v2, T3 v3) -> LargestDimensionType<SmallestDimensionType<T1, T2>, T3>
{
  using Res = LargestDimensionType<SmallestDimensionType<T1, T2>, T3>;
  return FSVF_RAW_(Res, _mm_fnmadd_ps(m_sse(v1), m_sse(v2), m_sse(v3)));
}

template<FastVector T1, FastVector T2, FastVector T3>
FSVF_FORCEINLINE auto FSVF_VECCALL fnmsub(T1 v1, T2 v2, T3 v3) -> LargestDimensionType<SmallestDimensionType<T1, T2>, T3>
{
  using Res = LargestDimensionType<SmallestDimensionType<T1, T2>, T3>;
  return FSVF_RAW_(Res, _mm_fnmsub_ps(m_sse(v1), m_sse(v2), m_sse(v3)));
}

template<FastVector T>
FSVF_FORCEINLINE float FSVF_VECCALL sum(T v)
{
  auto sums = Internals::reduce0(INTR_WRAP(_mm_add_ps), m_sse(v));
  return _mm_cvtss_f32(sums);
}

template<FastVector T>
FSVF_FORCEINLINE float FSVF_VECCALL prod(T v)
{
  if constexpr (std::is_base_of_v<LVec3, T>)
  {
    const Vec4 adjusted = v | Vec4(0, 0, 0, 1.0f);
    auto prods = Internals::reduce0(INTR_WRAP(_mm_mul_ps), m_sse(adjusted));
    return _mm_cvtss_f32(prods);
  }

  auto prods = Internals::reduce0(INTR_WRAP(_mm_mul_ps), m_sse(v));
  return _mm_cvtss_f32(prods);
}

// TODO test
template<FastVector T1, FastVector T2, FastVector T3, FastVector T4>
FSVF_FORCEINLINE Vec4 FSVF_VECCALL parallel_sum(T1 v1, T2 v2, T3 v3, T4 v4)
{
  auto sums = Internals::reduceAll(INTR_WRAP(_mm_add_ps), m_sse(v1), m_sse(v2), m_sse(v3), m_sse(v4));
  return Vec4{sums};
}

template<FastVector T1, FastVector T2>
FSVF_FORCEINLINE float FSVF_VECCALL dot(T1 v, T2 w)
{
  __m128 res = _mm_mul_ps(m_sse(v), m_sse(w));
  return sum(Vec4{ res });
}

template<FastVector T1, FastVector T2>
FSVF_FORCEINLINE LVec3 FSVF_VECCALL cross(T1 v, T2 w)
{
  // v = x y z n
  // w = a b c m
  const auto wp = shuffle<Shuf3::zxy>(w);    // c a b m
  const auto vp = shuffle<Shuf3::zxy>(v);    // z x y n
  const auto m1 = v * wp;                    // cx ay bz mn

  auto s = fmsub(w, vp, m1);           // az-cx bx-ay cy-bz 0

#ifdef FSVF_EXTRA_PRECISION_CROSS
  auto error = fnmadd(v, wp, m1);
  s += error;
#endif

  auto sh = shuffle<Shuf3::zxy>(s);    // cy-bz az-cx bx-ay 0

  //TODO check that RAW preserves the invariant
  return FSVF_RAW_(LVec3, m_sse(sh));
}

template<FastVector T>
T FSVF_VECCALL abs(T v)
{
  constexpr uint32_t f32_sign_mask = 0x8000'0000u;
  __m128             abs_mask      = _mm_set_ps1(bits_to_float(~f32_sign_mask));
  return FSVF_RAW_(T, _mm_and_ps(abs_mask, m_sse(v)));
}

template<FastVector T>
FSVF_FORCEINLINE float FSVF_VECCALL len_squared(T v)
{
  return dot(v, v);
}

template<FastVector T>
FSVF_FORCEINLINE auto FSVF_VECCALL sqrt(T v)
{
  return FSVF_RAW_(T, _mm_sqrt_ps(m_sse(v)));
}

template<FastVector T>
FSVF_FORCEINLINE auto FSVF_VECCALL rsqrt(T v)
{
  Vec4 res = FSVF_RAW_(Vec4, _mm_rsqrt_ps(m_sse(v)));

  // for types deriving from LVec3
  // the constructor will mask out possible junk coming from the last component
  return T{ res };
}

template<FastVector T>
float FSVF_VECCALL len(T v)
{
  // TODO the workaround is needed only if we disabled denormals
  //
  // using that len(v) = abs(max) * len(v/abs(max))
  // TODO adjust max_component and fix accordingly
  //auto maxVal = abs(Vec4{v});
  //maxVal = merge_max(maxVal, shuffle<Shuf4::wxyz>(maxVal));
  //maxVal = merge_max(maxVal, shuffle<Shuf4::zwxy>(maxVal));

  //Vec4 maxVal = max_component_value(Vec4{abs(v)});
  Vec4 maxVal = max_component_value(abs(v));

  // avoid dividing by zero if the vector is zero
  // TODO early return? Benchmark
  maxVal = blend_with_vector_mask(maxVal, Vec4{1.f}, BitMasks::cmp_equal_f32(maxVal, Vec4::zeroes()));

  // TODO temp
    if (any_is_inf(maxVal)) return std::numeric_limits<float>::infinity();
  Vec4 scaled = v / maxVal;

  // handle the case where maxVal is zero
  scaled &= BitMasks::cmp_not_equal_f32(Vec4::zeroes(), maxVal);

  // TODO benchmark against DPPS
  // TODO exploit the mask to work with 3D vectors (and benchmark that too)

  //const Vec4 lenSquaredScaled = _mm_dp_ps(scaled, scaled, 0xFF);
  //const Vec4 lenScaled = _mm_sqrt_ps(m_sse(lenSquaredScaled));
  const float lenScaled = sqrtf(len_squared(scaled));

  const auto res = maxVal * lenScaled;

  return _mm_cvtss_f32(m_sse(res));
}

template<FastVector T>
T FSVF_VECCALL normalize(T v)
{
  // TODO consider DPPS, benchmark, optimize 3D case
  // using that norm(v) = norm(v/abs(max)) to improve numerical stability
  const Vec4 maxVal = max_component_value(Vec4{abs(v)});

  Vec4 scaled = v / maxVal;

  // handle the case maxVal == 0
  const Vec4 nonzeroMask  = BitMasks::cmp_not_equal_f32(Vec4::zeroes(), maxVal);
  scaled                 &= nonzeroMask;

  Vec4 lenScaledSquared{ _mm_dp_ps(scaled, scaled, 0xFF) };

#if defined(FSVF_RSQRT_NORMALIZE)
  const Vec4 invLenScaled = rsqrt(lenScaledSquared);
  Vec4       res          = scaled * invLenScaled;
#else
  const Vec4 lenScaled = sqrt(lenScaledSquared);
  Vec4       res       = scaled / lenScaled;
#endif

  // handle the case lenScaled == 0
  res &= nonzeroMask;

  //return FSVF_RAW_(T, res);
  return T{res};
}

template<FastVector T1, FastVector T2>
FSVF_FORCEINLINE auto FSVF_VECCALL merge_max(T1 v, T2 w)
  -> LargestDimensionType<T1, T2>
{
  using Res = LargestDimensionType<T1, T2>;
  return FSVF_RAW_(Res, _mm_max_ps(m_sse(v), m_sse(w)));
}

template<FastVector T1, FastVector T2>
FSVF_FORCEINLINE auto FSVF_VECCALL merge_min(T1 v, T2 w)
  -> LargestDimensionType<T1, T2>
{
  using Res = LargestDimensionType<T1, T2>;
  return FSVF_RAW_(Res, _mm_min_ps(m_sse(v), m_sse(w)));
}

template<FastVector T>
FSVF_FORCEINLINE uint8_t FSVF_VECCALL min_component_index(T v)
{
  if constexpr (!std::is_base_of_v<LVec3, T>)
  {
    auto const wshuf
      = _mm_shuffle_ps(m_sse(v), m_sse(v), static_cast<uint8_t>(Shuf4::wwxy));
    auto const zshuf
      = _mm_shuffle_ps(m_sse(v), m_sse(v), static_cast<uint8_t>(Shuf4::zzxy));

    // check if we're gonna keep z or w (w means adding 1 if later we pick "Z")
    auto const wz = _mm_ucomilt_ss(wshuf, zshuf);

#ifdef FSVF_FLOAT_FINITE_MATH
    // the rest of the function can operates on integer lanes, saving a few cycles and possibly
    // with higher throughput; worth it even on CPUs with domain transition penalties
    auto izshuf = bits_to_float(zshuf);
    auto iwshuf = bits_to_float(wshuf);
    auto w      = _mm_min_epi32(iwshuf, izshuf);
#else
    auto w = _mm_min_ps(wshuf, zshuf);
#endif

    // from this point on, Z refers to whichever member survived between m_sse[2] and m_sse[3]

    // w = ZZxy, so with respect to m_sse, the permutations are
    // a = xxyy
    // b = yZZZ
#ifdef FSVF_FLOAT_FINITE_MATH
    auto const a   = _mm_shuffle_epi32(w, static_cast<uint8_t>(Shuf4::zzww));
    auto const b   = _mm_shuffle_epi32(w, static_cast<uint8_t>(Shuf4::wxxx));
    auto const cmp = _mm_cmplt_epi32(a, b);
#else
    auto const a   = _mm_shuffle_ps(w, w, static_cast<uint8_t>(Shuf4::zzww));
    auto const b   = _mm_shuffle_ps(w, w, static_cast<uint8_t>(Shuf4::wxxx));
    auto const cmp = _mm_cmplt_ps(a, b);
#endif

    std::array<uint32_t, 4> arr;
    std::memcpy(&arr, &cmp, sizeof(arr));

    /* arr[0] = 0xFFFF'FFFF * (x < y)
    *  arr[1] = 0xFFFF'FFFF * (x < Z)
    *  arr[2] = 0xFFFF'FFFF * (y < Z) */

    /* branch-free implementation of:
    return (x < y) ?
      (x < Z) ? 0 : (2 + wz) :
      (y < Z) ? 1 : (2 + wz);
    */

    // clang-format off
    return /* x < y ? */ ( arr[0] &
           /* x < Z ? */ (!arr[1] * (2 + wz)) ) /* ":" */ | ( ~arr[0] &
           /* y < Z ? */ (!arr[2] * (1 + wz) + 1));
    // clang-format on
  }
  else
  {
    // 3D case
    // TODO finish optimizing for the FINITE_MATH case

    // make all desired comparisons at once:
    auto const a
      = _mm_shuffle_ps(m_sse(v), m_sse(v), std::to_underlying(Shuf3::xxy));
    auto const b
      = _mm_shuffle_ps(m_sse(v), m_sse(v), std::to_underlying(Shuf3::yzz));
    auto const cmp = _mm_cmplt_ps(a, b);

    std::array<uint32_t, 4> arr;
    std::memcpy(&arr, &cmp, sizeof(arr));


    // the branch-free logic is the same as the 4-dimensional case
    // without the extra complexity of having to deal with `wz`

    // clang-format off
    return /* x < y ? */ ( arr[0] &
           /* x < z ? */ (!arr[1] * 2) ) /* ":" */ | ( ~arr[0] &
           /* y < z ? */ (!arr[2] + 1) );
    // clang-format on
  }
}

template<FastVector T>
inline uint8_t FSVF_VECCALL max_component_index(T v)
{
  // see min_component_index() for documentation about the implementation

  if constexpr (!std::is_base_of_v<LVec3, T>)
  {
    auto const wshuf
      = _mm_shuffle_ps(m_sse(v), m_sse(v), static_cast<uint8_t>(Shuf4::wwxy));
    auto const zshuf
      = _mm_shuffle_ps(m_sse(v), m_sse(v), static_cast<uint8_t>(Shuf4::zzxy));

    auto const wz = _mm_ucomigt_ss(wshuf, zshuf);

#ifdef FSVF_FLOAT_FINITE_MATH
    auto izshuf = _mm_castps_si128(zshuf);
    auto iwshuf = _mm_castps_si128(wshuf);
    auto w      = _mm_max_epi32(iwshuf, izshuf);

    auto const a   = _mm_shuffle_epi32(w, static_cast<uint8_t>(Shuf4::zzww));
    auto const b   = _mm_shuffle_epi32(w, static_cast<uint8_t>(Shuf4::wxxx));
    auto const cmp = _mm_cmpgt_epi32(a, b);
#else
    auto w = _mm_max_ps(wshuf, zshuf);

    auto const a   = _mm_shuffle_ps(w, w, static_cast<uint8_t>(Shuf4::zzww));
    auto const b   = _mm_shuffle_ps(w, w, static_cast<uint8_t>(Shuf4::wxxx));
    auto const cmp = _mm_cmpgt_ps(a, b);
#endif

    std::array<uint32_t, 4> arr;
    std::memcpy(&arr, &cmp, sizeof(arr));

    return (arr[0] & (!arr[1] * (2 + wz)))
         | (~arr[0] & (!arr[2] * (1 + wz) + 1));
  }
  else
  {
    // 3D case
    // TODO finish optimizing for the FINITE_MATH case

    auto const a
      = _mm_shuffle_ps(m_sse(v), m_sse(v), static_cast<uint8_t>(Shuf3::xxy));
    auto const b
      = _mm_shuffle_ps(m_sse(v), m_sse(v), static_cast<uint8_t>(Shuf3::yzz));
    auto const cmp = _mm_cmpgt_ps(a, b);

    std::array<uint32_t, 4> arr;
    std::memcpy(&arr, &cmp, sizeof(arr));

    return (arr[0] & (!arr[1] * 2)) | (~arr[0] & (!arr[2] + 1));
  }
}
// TODO floor, ceil

template<FastVector T>
FSVF_FORCEINLINE T FSVF_VECCALL min_component_value(T v)
{
  if constexpr (std::is_base_of_v<LVec3, T>)
  {
    v = merge_min(v, shuffle<Shuf3::yzx>(v));
    return merge_min(v, shuffle<Shuf3::zxy>(v));
  }
  else
  {
    // do the first two merges separately
    T vms0 = shuffle<Shuf4::zwxy>(v);
    T vms1 = shuffle<Shuf4::wxyz>(v);

    v = merge_min(v, shuffle<Shuf4::yzwx>(v));
    vms0 = merge_min(vms0, vms1);

    return merge_min(v, vms0);
  }
}

template<FastVector T>
FSVF_FORCEINLINE T FSVF_VECCALL max_component_value(T v)
{
  if constexpr (std::is_base_of_v<LVec3, T>)
  {
    v = merge_max(v, shuffle<Shuf3::yzx>(v));
    return merge_max(v, shuffle<Shuf3::zxy>(v));
  }
  else
  {
    T vms0 = shuffle<Shuf4::zwxy>(v);
    T vms1 = shuffle<Shuf4::wxyz>(v);

    v = merge_max(v, shuffle<Shuf4::yzwx>(v));
    vms0 = merge_max(vms0, vms1);

    return merge_max(v, vms0);
  }
}

template<FastVector T>
FSVF_FORCEINLINE float FSVF_VECCALL min_component_value_f32(T v)
{
  return _mm_cvtss_f32(m_sse(min_component_value(v)));
}

template<FastVector T>
FSVF_FORCEINLINE float FSVF_VECCALL max_component_value_f32(T v)
{
  return _mm_cvtss_f32(m_sse(max_component_value(v)));
}

template<FastVector T1, FastVector T2>
FSVF_FORCEINLINE T1 FSVF_VECCALL clamp(T1 v, T2 min, T2 max)
{
  auto min_mask    = Vec4{ v } < Vec4{ min };
  auto max_mask    = Vec4{ v } > Vec4{ max };
  auto change_mask = min_mask | max_mask;

  auto max_filler = max_mask & max;
  auto min_filler = min_mask & min;

  auto res = blend_with_vector_mask(v, (max_filler | min_filler), change_mask);

  return res;
}

template<FastVector T>
FSVF_FORCEINLINE T FSVF_VECCALL saturate(T v)
{
  // TODO optimize for 3D
  return clamp<T, T>(v, T::zeroes(), T{1.f});
}

template<FastVector T>
FSVF_FORCEINLINE T FSVF_VECCALL sort(T v)
{
  if constexpr (!std::is_base_of_v<LVec3, T>)
  {
    // bitonic sort

    /* sort upper and lower half */

    auto yyww = _mm_movehdup_ps(m_sse(v));
    auto xxzz = _mm_moveldup_ps(m_sse(v));
    auto min  = _mm_min_ps(m_sse(v), yyww);    // (min(x,y), y, min(z,w), w)
    auto max  = _mm_max_ps(m_sse(v), xxzz);    // (x, max(x,y), z, max(z,w))
    auto upup = _mm_blend_ps(
      min,
      max,
      0b10'10
    );    // (min(x,y), max(x,y), min(z,w), max(z,w))

    /* swap last two entries to make the sequence bitonic */

    auto bitonic = _mm_shuffle_ps(
      upup,
      upup,
      static_cast<uint8_t>(Shuf4::xywz)
    );    // (min(x,y), max(x,y), max(z,w), min(z,w))

    /* conditionally swap odd/even entries if they're not ordered */

    auto shuf = _mm_shuffle_ps(upup, upup, static_cast<uint8_t>(Shuf4::wzxy));
    auto swap = _mm_cmpgt_ps(bitonic, shuf);
    // repeat the upper half to make the swapper/keeper trick work (the second half is already the negation of the first)
    swap = _mm_shuffle_ps(swap, swap, static_cast<uint8_t>(Shuf4::xyxy));
    auto swapper = _mm_and_ps(swap, shuf);
    auto keeper  = _mm_andnot_ps(swap, bitonic);
    auto merger  = _mm_or_ps(swapper, keeper);

    /* sort again lower and upper half */

    yyww = _mm_movehdup_ps(merger);
    xxzz = _mm_moveldup_ps(merger);
    min  = _mm_min_ps(merger, yyww);
    max  = _mm_max_ps(merger, xxzz);

    return FSVF_RAW_(Vec4, _mm_blend_ps(min, max, 0b10'10));
  }
  else
  {
    // 3D case

    /* sort upper elements */
    auto yyz
      = _mm_shuffle_ps(m_sse(v), m_sse(v), static_cast<uint8_t>(Shuf3::yyz));
    auto xxz
      = _mm_shuffle_ps(m_sse(v), m_sse(v), static_cast<uint8_t>(Shuf3::xxz));
    auto min = _mm_min_ps(m_sse(v), yyz);          // (mxy, y, z)
    auto max = _mm_max_ps(m_sse(v), xxz);          // (x, Mxy, z)
    auto mxy = _mm_blend_ps(min, max, 0b00'10);    // (mxy, Mxy, z)

    /* sort lower elements */
    // clang-format off
    auto sh1 = _mm_shuffle_ps(mxy, mxy, static_cast<uint8_t>(Shuf3::xzz)); // (mxy, z, z)
    auto sh2 = _mm_shuffle_ps(mxy, mxy, static_cast<uint8_t>(Shuf3::xyy)); // (mxy, Mxy, Mxy)
    min = _mm_min_ps(mxy, sh1); // (mxy, min(Mxy,z), z)
    max = _mm_max_ps(mxy, sh2); // (mxy, Mxy, max(Mxy,z))
    auto myz = _mm_blend_ps(min, max, 0b11'00); // (mxy, min(z,Mxy), max(z,Mxy))
    // clang-format on

    /* sort upper elements again */
    yyz = _mm_shuffle_ps(myz, myz, static_cast<uint8_t>(Shuf3::yyz));
    xxz = _mm_shuffle_ps(myz, myz, static_cast<uint8_t>(Shuf3::xxz));
    min = _mm_min_ps(myz, yyz);
    max = _mm_max_ps(myz, xxz);

    return FSVF_RAW_(LVec3, _mm_blend_ps(min, max, 0b00'10));
  }
}

template<FastVector T>
FSVF_FORCEINLINE T FSVF_VECCALL reverse_sort(T v)
{
  if constexpr (!std::is_base_of_v<LVec3, T>)
  {
    // bitonic sort, same logic as sort()

    /* sort upper and lower half */

    auto yyww = _mm_movehdup_ps(m_sse(v));
    auto xxzz = _mm_moveldup_ps(m_sse(v));
    auto max  = _mm_max_ps(m_sse(v), yyww);
    auto min  = _mm_min_ps(m_sse(v), xxzz);
    auto upup = _mm_blend_ps(max, min, 0b10'10);

    /* swap last two entries to make the sequence bitonic */

    auto bitonic
      = _mm_shuffle_ps(upup, upup, static_cast<uint8_t>(Shuf4::xywz));

    /* conditionally swap odd/even entries if they're not ordered */

    auto shuf = _mm_shuffle_ps(upup, upup, static_cast<uint8_t>(Shuf4::wzxy));
    auto swap = _mm_cmplt_ps(bitonic, shuf);
    swap      = _mm_shuffle_ps(swap, swap, static_cast<uint8_t>(Shuf4::xyxy));
    auto swapper = _mm_and_ps(swap, shuf);
    auto keeper  = _mm_andnot_ps(swap, bitonic);
    auto merger  = _mm_or_ps(swapper, keeper);

    /* sort again lower and upper half */

    yyww = _mm_movehdup_ps(merger);
    xxzz = _mm_moveldup_ps(merger);
    max  = _mm_max_ps(merger, yyww);
    min  = _mm_min_ps(merger, xxzz);

    return FSVF_RAW_(Vec4, _mm_blend_ps(max, min, 0b10'10));
  }
  else
  {
    // 3D case

    /* sort upper elements */
    auto yyz
      = _mm_shuffle_ps(m_sse(v), m_sse(v), static_cast<uint8_t>(Shuf3::yyz));
    auto xxz
      = _mm_shuffle_ps(m_sse(v), m_sse(v), static_cast<uint8_t>(Shuf3::xxz));
    auto min = _mm_max_ps(m_sse(v), yyz);          // (Mxy, y, z)
    auto max = _mm_min_ps(m_sse(v), xxz);          // (x, mxy, z)
    auto mxy = _mm_blend_ps(min, max, 0b00'10);    // (Mxy, mxy, z)

    /* sort lower elements */
    // clang-format off
    auto sh1 = _mm_shuffle_ps(mxy, mxy, static_cast<uint8_t>(Shuf3::xzz)); // (Mxy, z, z)
    auto sh2 = _mm_shuffle_ps(mxy, mxy, static_cast<uint8_t>(Shuf3::xyy)); // (Mxy, mxy, mxy)
    min = _mm_max_ps(mxy, sh1); // (Mxy, max(mxy,z), z)
    max = _mm_min_ps(mxy, sh2); // (Mxy, mxy, min(mxy,z))
    auto myz = _mm_blend_ps(min, max, 0b11'00); // (Mxy, max(mxy,z), min(mxy,z))
    // clang-format on

    /* sort upper elements again */
    yyz = _mm_shuffle_ps(myz, myz, static_cast<uint8_t>(Shuf3::yyz));
    xxz = _mm_shuffle_ps(myz, myz, static_cast<uint8_t>(Shuf3::xxz));
    min = _mm_max_ps(myz, yyz);
    max = _mm_min_ps(myz, xxz);

    return FSVF_RAW_(LVec3, _mm_blend_ps(min, max, 0b00'10));
  }
}

template<FastVector T>
T FSVF_FORCEINLINE FSVF_VECCALL floor(T v)
{
  return FSVF_RAW_(T, _mm_floor_ps(m_sse(v)));
}

template<FastVector T>
T FSVF_FORCEINLINE FSVF_VECCALL ceil(T v)
{
  return FSVF_RAW_(T, _mm_ceil_ps(m_sse(v)));
}

FSVF_FORCEINLINE __m128 LVec3::precondition(__m128 sse)
{
  return blend_with_mask<0b1110>(Vec4::zeroes(), Vec4(sse));
}

// TODO move to float utils
FSVF_FORCEINLINE FSVF_VECCALL Vec4 mantissa            (FastVector auto x) { return x & Vec4{bits_to_float(f32_mantissa_mask)}; }
FSVF_FORCEINLINE FSVF_VECCALL Vec4 exponent            (FastVector auto x) { return x & Vec4{bits_to_float(f32_exponent_mask)}; }
FSVF_FORCEINLINE FSVF_VECCALL Vec4 signed_exponent     (FastVector auto x) { return x & Vec4{bits_to_float(f32_signed_exponent_mask)}; }
FSVF_FORCEINLINE FSVF_VECCALL Vec4 sign_bit            (FastVector auto x) { return x & Vec4{bits_to_float(f32_sign_mask)}; }
FSVF_FORCEINLINE FSVF_VECCALL Vec4 is_inf_or_nan       (FastVector auto x) { return BitMasks::cmp_equal_bitwise(exponent(x), Vec4{bits_to_float(f32_exponent_mask)}); }
FSVF_FORCEINLINE FSVF_VECCALL Vec4 is_finite           (FastVector auto x) { return ~is_inf_or_nan(x); }
FSVF_FORCEINLINE FSVF_VECCALL Vec4 is_inf              (FastVector auto x) { return is_inf_or_nan(x) & BitMasks::cmp_equal_bitwise(mantissa(x), Vec4::zeroes()); }
FSVF_FORCEINLINE FSVF_VECCALL Vec4 is_nan              (FastVector auto x) { return is_inf_or_nan(x) & BitMasks::cmp_not_equal_bitwise(mantissa(x), Vec4::zeroes()); }
FSVF_FORCEINLINE FSVF_VECCALL bool any_is_inf_or_nan   (FastVector auto x) { return any<4>(is_inf_or_nan(x)); }
FSVF_FORCEINLINE FSVF_VECCALL bool any_is_finite       (FastVector auto x) { return any<4>(is_finite(x)); }
FSVF_FORCEINLINE FSVF_VECCALL bool any_is_inf          (FastVector auto x) { return any<4>(is_inf(x)); }
FSVF_FORCEINLINE FSVF_VECCALL bool any_is_nan          (FastVector auto x) { return any<4>(is_nan(x)); }
}    // namespace FSVF
