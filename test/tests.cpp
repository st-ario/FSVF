#include <FSFV_Export.h>
#include <iostream>
#include <catch2/catch_test_macros.hpp>
#include <random>
#include <limits>
#include <algorithm>

using namespace FSVF;

#define FSFV_CHECK_COMPONENTS(vec, x, y, z, w) \
{                                              \
  auto components = vec.components();          \
  CHECK(components[0] == x);                   \
  CHECK(components[1] == y);                   \
  CHECK(components[2] == z);                   \
  CHECK(components[3] == w);                   \
}                                              \

#define FSFV_CHECK_COMP3(vec, x, y, z) \
{                                      \
  auto components = vec.components();  \
  CHECK(components[0] == x);           \
  CHECK(components[1] == y);           \
  CHECK(components[2] == z);           \
}                                      \

static std::random_device rd;
static std::mt19937 gen(rd());

static std::vector<float> gen_params(uint16_t n)
{
  std::uniform_int_distribution<int> ranb(0,1);
  std::uniform_real_distribution<float> ranf(1.0f, 2.0f);
    //std::numeric_limits<float>::max());

  std::vector<float> params{};

  for (uint16_t i = 0; i < n; ++i)
  {
    const float rn = (ranf(gen) - 1.f) * (std::numeric_limits<float>::max() - 1.f);
    params.push_back((-1 * ranb(gen)) * rn);
  }

  return params;
}

// makes 0 appear in tests more often than with the uniform distribution of
// gen_params()
// any value in the result will be 0 with probability 1/bias
// calling this function with a bias of 0 is equivalent to calling gen_params()
static std::vector<float> biased_params(uint16_t n, uint16_t bias)
{
  auto params = gen_params(n);

  if (bias == 0)
  {
    return params;
  }

  std::uniform_int_distribution<> zero_bias(1, bias);

  for (auto& val : params)
  {
    if (zero_bias(gen) == bias)
    {
      val = 0.0f;
    }
  }

  return params;
}

TEST_CASE("Vec4ConstructorsAssignments", "[core]")
{
  const auto params = biased_params(10, 10);

  // single-value constructor
  auto v0 = Vec4(params[0]);
  FSFV_CHECK_COMPONENTS(v0, params[0], params[0], params[0], params[0]);

  // components constructor
  auto v1 = Vec4(params[1], params[2], params[3], params[4]);
  FSFV_CHECK_COMPONENTS(v1, params[1], params[2], params[3], params[4]);

  // pointer constructor
  auto v2 = Vec4(&params[3]);
  FSFV_CHECK_COMPONENTS(v2, params[3], params[4], params[5], params[6]);
  std::array<float, 4> arr{params[9], params[6], params[8], params[7]};
  auto v3 = Vec4(arr.data());
  FSFV_CHECK_COMPONENTS(v3, arr[0], arr[1], arr[2], arr[3]);

  const LVec3 vec3 = LVec3(params[6]);

  // LVec3 conversion
  auto v4 = Vec4(vec3);
  const auto cmp = vec3.components();
  FSFV_CHECK_COMPONENTS(v4, cmp[0], cmp[1], cmp[2], 0);

  // (LVec3, float) constructor
  auto v5 = Vec4(vec3, params[5]);
  const auto cmp1 = vec3.components();
  FSFV_CHECK_COMPONENTS(v5, cmp1[0], cmp1[1], cmp1[2], params[5]);

  // copy constructor
  auto v6 = Vec4(v0);
  const auto cmpv0 = v0.components();
  FSFV_CHECK_COMPONENTS(v6, cmpv0[0], cmpv0[1], cmpv0[2], cmpv0[3]);

  // move constructor
  const auto cmpv1 = v1.components();
  auto v7 = Vec4(std::move(v1));
  FSFV_CHECK_COMPONENTS(v7, cmpv1[0], cmpv1[1], cmpv1[2], cmpv1[3]);

  // copy assignment
  v6 = v2;
  FSFV_CHECK_COMPONENTS(v6, params[3], params[4], params[5], params[6]);

  // move assingment
  v7 = std::move(v3);
  FSFV_CHECK_COMPONENTS(v7, arr[0], arr[1], arr[2], arr[3]);
}

TEST_CASE("Vec3ConstructorsAssignments", "[core]")
{
  const auto params = biased_params(10, 10);

  // single-value constructor
  auto v0 = LVec3(params[0]);
  FSFV_CHECK_COMP3(v0, params[0], params[0], params[0]);
  FSFV_CHECK_COMPONENTS(Vec4{v0}, params[0], params[0], params[0], 0);

  // components constructor
  auto v1 = LVec3(params[1], params[3], params[5]);
  FSFV_CHECK_COMP3(v1, params[1], params[3], params[5]);
  FSFV_CHECK_COMPONENTS(Vec4{v1}, params[1], params[3], params[5], 0);

  // pointer constructor
  auto v2 = LVec3(&params[6]);
  FSFV_CHECK_COMP3(v2, params[6], params[7], params[8]);
  FSFV_CHECK_COMPONENTS(Vec4{v2}, params[6], params[7], params[8], 0);
  auto arr = std::array<float,3>{params[9], params[7], params[3]};
  auto v3 = LVec3(arr.data());
  FSFV_CHECK_COMP3(v3, arr[0], arr[1], arr[2]);
  FSFV_CHECK_COMPONENTS(Vec4{v3}, arr[0], arr[1], arr[2], 0);

  // Vec4 conversion
  auto vec4 = Vec4{params[5], params[7], params[1], params[4]};
  auto v4 = LVec3(vec4);
  const auto cmp = vec4.components();
  FSFV_CHECK_COMP3(v4, cmp[0], cmp[1], cmp[2]);
  FSFV_CHECK_COMPONENTS(Vec4{v4}, cmp[0], cmp[1], cmp[2], 0);

  // copy constructor
  auto v6 = LVec3(v1);
  FSFV_CHECK_COMP3(v6, params[1], params[3], params[5]);
  FSFV_CHECK_COMPONENTS(Vec4{v6}, params[1], params[3], params[5], 0);

  // move constructor
  auto v7 = LVec3(std::move(v2));
  FSFV_CHECK_COMP3(v7, params[6], params[7], params[8]);
  FSFV_CHECK_COMPONENTS(Vec4{v7}, params[6], params[7], params[8], 0);

  // copy assignment
  v6 = v3;
  FSFV_CHECK_COMP3(v6, arr[0], arr[1], arr[2]);
  FSFV_CHECK_COMPONENTS(Vec4{v6}, arr[0], arr[1], arr[2], 0);

  // move assignment
  v7 = std::move(v4);
  FSFV_CHECK_COMP3(v7, cmp[0], cmp[1], cmp[2]);
  FSFV_CHECK_COMPONENTS(Vec4{v7}, cmp[0], cmp[1], cmp[2], 0);
}

#include "test_shuffles.inl"

template<typename T, size_t N>
void cartesian_helper(
  std::vector<std::array<T,N>>& power,
  std::array<T,N> current,
  const std::array<T,N>& vals,
  size_t current_pos = 0)
{
  if (current_pos == N-1)
  {
    for (auto i : vals)
    {
      current[current_pos] = i;
      power.push_back(current);
    }

    return;
  }

  for (auto i : vals)
  {
    current[current_pos] = i;
    cartesian_helper(power, current, vals, current_pos + 1);
  }
}

template<typename T, size_t N>
std::vector<std::array<T,N>> cartesian_power(const std::array<T,N>& arr)
{
  std::vector<std::array<T,N>> res{};
  res.reserve(std::pow(N,N));

  cartesian_helper(res, std::array<T,N>{}, arr);

  return res;
}

TEST_CASE("Sort_and_MinMax4", "[methods]")
{
  const auto params = gen_params(4);

  std::array<float, 4> vals;
  vals[0] = params[0];
  vals[1] = params[1];
  vals[2] = params[2];
  vals[3] = params[3];

  const auto cartesian = cartesian_power(vals);

  for (auto combination : cartesian)
  {
    Vec4 test{combination.data()};

    auto smin = std::ranges::min(combination);
    auto smax = std::ranges::max(combination);

    auto mindim = min_component_index(test);
    auto maxdim = max_component_index(test);

    CHECK(combination[mindim] == smin);
    CHECK(min_component_value_f32(test) == smin);
    CHECK(min_component_value(test).components() == Vec4(smin).components());

    CHECK(combination[maxdim] == smax);
    CHECK(max_component_value_f32(test) == smax);
    CHECK(max_component_value(test).components() == Vec4(smax).components());

    std::ranges::sort(combination);
    FSFV_CHECK_COMPONENTS(sort(test),
      combination[0], combination[1], combination[2], combination[3]);

    std::ranges::sort(combination, std::greater<float>{});
    FSFV_CHECK_COMPONENTS(reverse_sort(test),
      combination[0], combination[1], combination[2], combination[3]);
  }
}

TEST_CASE("Sort_and_MinMax3", "[methods]")
{
  const auto params = gen_params(3);

  std::array<float,3> vals;
  vals[0] = params[0];
  vals[1] = params[1];
  vals[2] = params[2];

  const auto cartesian = cartesian_power(vals);

  for (auto combination : cartesian)
  {
    LVec3 test{combination.data()};

    auto smin = std::ranges::min(combination);
    auto smax = std::ranges::max(combination);

    auto mindim = min_component_index(test);
    auto maxdim = max_component_index(test);

    CHECK(combination[mindim] == smin);
    CHECK(min_component_value_f32(test) == smin);
    CHECK(min_component_value(test) == Vec4(smin));

    CHECK(combination[maxdim] == smax);
    CHECK(max_component_value_f32(test) == smax);
    CHECK(max_component_value(test) == Vec4(smax));

    std::ranges::sort(combination);
    auto sorted = sort(test);
    FSFV_CHECK_COMP3(sorted, combination[0], combination[1], combination[2]);
    FSFV_CHECK_COMPONENTS(Vec4{sorted},
      combination[0], combination[1], combination[2], 0);

    std::ranges::sort(combination, std::greater<float>{});
    sorted = reverse_sort(test);
    FSFV_CHECK_COMP3(sorted,
      combination[0], combination[1], combination[2]);
    FSFV_CHECK_COMPONENTS(Vec4{sorted},
      combination[0], combination[1], combination[2], 0);
  }
}

TEST_CASE("DotProduct", "[linear_algebra]")
{
  const auto params = biased_params(8, 8);

  Vec4 v(&params[0]);
  Vec4 w(&params[4]);

  {
    float res = dot(v, w);

    const auto av = v.components();
    const auto aw = w.components();

    float check = av[0] * aw[0]
                + av[1] * aw[1]
                + av[2] * aw[2]
                + av[3] * aw[3];

    CHECK(res == check);
  }

  LVec3 lv{v};
  LVec3 lw{w};

  {
    float res = dot(lv, lw);

    const auto av = lv.components();
    const auto aw = lw.components();

    float check = av[0] * aw[0]
                + av[1] * aw[1]
                + av[2] * aw[2];

    CHECK(res == check);
  }
}

TEST_CASE("Constructors 3x3 Matrices", "[basics]")
{
  LMat3 m(1,2,1,-2,0,5,3,3,4);
  CHECK(m == LMat3(LVec3(1, 2, 1), LVec3(-2, 0, 5), LVec3(3, 3, 4)));
}

TEST_CASE("Constructors 4x4 Matrices", "[basics]")
{
  Mat4 m(1,2,1,0,-2,0,5,0,3,3,4,0,0,0,0,3);
  CHECK(m == Mat4(Vec4(1, 2, 1, 0), Vec4(-2, 0, 5, 0), Vec4(3, 3, 4, 0), Vec4(0,0,0,3)));
}

TEST_CASE("Determinant 3x3", "[linear_algebra]")
{
  LMat3 m{1.f};
  CHECK(m.determinant() == 1.f);

  m = LMat3{2.f};
  CHECK(m.determinant() == 8.f);

  m = LMat3(1,2,1,-2,0,5,3,3,4);
  CHECK(m.determinant() == 25.f);
}

TEST_CASE("Determinant 4x4", "[linear_algebra]")
{
  Mat4 m{1.f};
  CHECK(m.determinant() == 1.f);
  CHECK(m.affine_determinant() == 1.f);

  m = Mat4{2.f};
  CHECK(m.determinant() == 16.f);
  CHECK(m.affine_determinant() == 8.f);

  m = Mat4(Vec4(1, 2, 1, 0), Vec4(-2, 0, 5, 0), Vec4(3, 3, 4, 0), Vec4(0,0,0,3));
  CHECK(m.determinant() == 75.f);
  CHECK(m.affine_determinant() == 25.f);
}

