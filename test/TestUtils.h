#pragma once

#include <FSVF.h>

#define CATCH_CONFIG_FAST_COMPILE
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include <array>
#include <random>
#include <set>

using Catch::Approx;

#define CHECK_BITMASK_TRUE(component) \
  CHECK(std::bit_cast<uint32_t>(component) == 0xFFFFFFFF)

#define CHECK_BITMASK_FALSE(component) \
  CHECK(std::bit_cast<uint32_t>(component) == 0x00000000)

#define CHECK_BITWISE_EQ(x, y) \
    { \
    CHECK(std::bit_cast<uint32_t>(x) == std::bit_cast<uint32_t>(y)); \
    }

#define CHECK_BITWISE_NEQ(x, y) \
    { \
    CHECK(std::bit_cast<uint32_t>(x) != std::bit_cast<uint32_t>(y)); \
    }

#define CHECK_VEC4_COMPONENTS(vec, x, y, z, w) \
    { \
        auto c_macro = (vec).components(); \
        CHECK(c_macro[0] == Approx(x)); \
        CHECK(c_macro[1] == Approx(y)); \
        CHECK(c_macro[2] == Approx(z)); \
        CHECK(c_macro[3] == Approx(w)); \
    }

#define CHECK_LVEC3_COMPONENTS(vec, x, y, z) \
    { \
        auto c_macro = (Vec4{vec}).components(); \
        CHECK(c_macro[0] == Approx(x)); \
        CHECK(c_macro[1] == Approx(y)); \
        CHECK(c_macro[2] == Approx(z)); \
        CHECK(c_macro[3] == 0.f); \
    }

#define CHECK_VEC4_BITWISE_EQ(vec, x, y, z, w) \
    { \
        auto c_macro = (vec).components(); \
        CHECK_BITWISE_EQ(c_macro[0], x) \
        CHECK_BITWISE_EQ(c_macro[1], y) \
        CHECK_BITWISE_EQ(c_macro[2], z) \
        CHECK_BITWISE_EQ(c_macro[3], w) \
    }

#define CHECK_LVEC3_BITWISE_EQ(vec, x, y, z) \
    { \
        auto c_macro = (Vec4{vec}).components(); \
        CHECK_BITWISE_EQ(c_macro[0], x) \
        CHECK_BITWISE_EQ(c_macro[1], y) \
        CHECK_BITWISE_EQ(c_macro[2], z) \
        CHECK_BITWISE_EQ(c_macro[3], 0) \
    }

#define CHECK_VEC4_BITWISE_NEQ(vec, x, y, z, w) \
    { \
        auto c_macro = (vec).components(); \
        CHECK_BITWISE_NEQ(c_macro[0], x) \
        CHECK_BITWISE_NEQ(c_macro[1], y) \
        CHECK_BITWISE_NEQ(c_macro[2], z) \
        CHECK_BITWISE_NEQ(c_macro[3], w) \
    }

#define CHECK_LVEC3_BITWISE_NEQ(vec, x, y, z) \
    { \
        auto c_macro = (Vec4{vec}).components(); \
        CHECK_BITWISE_NEQ(c_macro[0], x) \
        CHECK_BITWISE_NEQ(c_macro[1], y) \
        CHECK_BITWISE_NEQ(c_macro[2], z) \
        CHECK_BITWISE_EQ(c_macro[3], 0) \
    }
#define CHECK_VEC4_ALMOST_EQ(v1, v2) \
    { \
        auto c_macro = (Vec4{v2}).components(); \
        CHECK_VEC4_COMPONENTS(v1, c_macro[0], c_macro[1], c_macro[2], c_macro[3]) \
    }
#define CHECK_LVEC3_ALMOST_EQ(v1, v2) \
    { \
        auto c_macro = (Vec4{v2}).components(); \
        CHECK_LVEC3_COMPONENTS(v1, c_macro[0], c_macro[1], c_macro[2]) \
        CHECK(c_macro[3] == 0.f); \
    }

namespace Catch
{
template<FSVF::FastVector V>
struct StringMaker<V>
{
  static std::string convert(const V& v)
  {
    return StringMaker<std::array<float, 4>>::convert(FSVF::Vec4{v}.components());
  }
};

template<>
struct StringMaker<FSVF::Mat4>
{
  static std::string convert(const FSVF::Mat4& m)
  {
    return StringMaker<std::array<FSVF::Vec4, 4>>::convert(m.cols);
  }
};
}

static inline std::random_device rd;
static inline std::mt19937       gen(rd());

inline float random_float(float min = -1000.0f, float max = 1000.0f)
{
  std::uniform_real_distribution<float> dist(min, max);
  return dist(gen);
}

inline bool coin_flip()
{
  std::uniform_int_distribution<int> dist(0, 1);
  return dist(gen);
}

// makes it more likely to produce a 0
// any value in the result will be 0 with probability 1/bias
// calling this function with a bias of 0 is equivalent to calling random_float()
inline float biased_random_float(uint16_t bias, float min = -1000.0f, float max = 1000.0f)
{
  std::uniform_int_distribution<> zero_bias(1, bias);

  if (zero_bias(gen) == bias) return 0.f;

  std::uniform_real_distribution<float> dist(min, max);

  return dist(gen);
}

inline float create_nan(uint32_t payload = 0x12345)
{
  return FSVF::bits_to_float(FSVF::f32_exponent_mask | 0x40'0000 | payload);
}

inline uint32_t random_bits()
{
  std::uniform_int_distribution<uint32_t> dist;
  return dist(gen);
}

inline FSVF::Vec4 rnd4(float min = -1000.f, float max = 1000.f)
{
  return FSVF::Vec4{random_float(min, max), random_float(min, max), random_float(min, max), random_float(min, max)};
}

inline FSVF::LVec3 rnd3(float min = -1000.f, float max = 1000.f)
{
  return FSVF::LVec3{rnd4(min, max)};
}

namespace internal {
template<typename T, size_t N>
void cartesian_helper(
  std::vector<std::array<T, N>>& power,
  std::array<T, N>               current,
  const std::array<T, N>&        vals,
  size_t                         current_pos = 0
)
{
  if (current_pos == N - 1)
  {
    for (auto i: vals)
    {
      current[current_pos] = i;
      power.push_back(current);
    }

    return;
  }

  for (auto i: vals)
  {
    current[current_pos] = i;
    cartesian_helper(power, current, vals, current_pos + 1);
  }
}

template<size_t N>
inline void permutations_helper(std::vector<std::array<size_t, N>>& perms, std::array<size_t, N>& arr, size_t depth, std::set<size_t>& visited)
{
  if (depth == N)
  {
    perms.push_back(arr);
    return;
  }

  for (size_t i = 0; i < N; ++i) {
    if (visited.contains(i)) continue;

    auto arr_copy = arr;
    arr_copy[depth] = i;
    visited.insert(i);
    permutations_helper(perms, arr_copy, depth + 1, visited);
    visited.erase(i);
  }

  return;
}
} // namespace internal

template<typename T, size_t N>
inline std::vector<std::array<T, N>> cartesian_power(const std::array<T, N>& arr)
{
  std::vector<std::array<T, N>> res{};
  res.reserve(std::pow(N, N));

  internal::cartesian_helper(res, std::array<T, N>{}, arr);

  return res;
}

inline constexpr size_t factorial(size_t n) {
  size_t res = 1;
  while (n > 1) {
    res *= n--;
  }
  return res;
}

template<size_t N>
std::vector<std::array<size_t, N>> create_permutations()
{
  std::vector<std::array<size_t, N>> res{};
  res.reserve(factorial(N));

  std::array<size_t, N> arr;

  for (size_t i = 0; i < N; ++i)
    arr[i] = i;

  std::set<size_t> visited{};

  internal::permutations_helper(res, arr, 0, visited);

  return res;
}

