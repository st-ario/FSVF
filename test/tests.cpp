#include <FSVF.h>

#define CATCH_CONFIG_FAST_COMPILE
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <algorithm>
#include <cmath>
#include <limits>
#include <random>

// TODO!!!
// Most of the tests are AI-generated and require manual double-checking and cleanup

using namespace FSVF;
using Catch::Approx;

namespace Catch
{
template<FastVector V>
struct StringMaker<V>
{
  static std::string convert(const V& v)
  {
    return StringMaker<std::array<float, 4>>::convert(v.components());
  }
};
}

#define FSFV_CHECK_COMPONENTS(vec, x, y, z, w) \
{                                              \
  auto components = vec.components();          \
  CHECK(components[0] == x);                   \
  CHECK(components[1] == y);                   \
  CHECK(components[2] == z);                   \
  CHECK(components[3] == w);                   \
}

#define FSFV_CHECK_COMP3(vec, x, y, z) \
{                                      \
  auto components = vec.components();  \
  CHECK(components[0] == x);           \
  CHECK(components[1] == y);           \
  CHECK(components[2] == z);           \
}

static std::random_device rd;
static std::mt19937       gen(rd());

static std::vector<float> gen_params(
  uint16_t n,
  float    interval_boundary = std::numeric_limits<float>::max() - 1.f
)
{
  std::uniform_int_distribution<int>    ranb(0, 1);
  std::uniform_real_distribution<float> ranf(1.0f, 2.0f);
  //std::numeric_limits<float>::max());

  std::vector<float> params{};

  for (uint16_t i = 0; i < n; ++i)
  {
    const float rn = (ranf(gen) - 1.f) * (interval_boundary - 1.f);
    params.push_back((-1 * ranb(gen)) * rn);
  }

  return params;
}

// makes 0 appear in tests more often than with the uniform distribution of
// gen_params()
// any value in the result will be 0 with probability 1/bias
// calling this function with a bias of 0 is equivalent to calling gen_params()
static std::vector<float> biased_params(
  uint16_t n,
  uint16_t bias,
  float    interval_boundary = std::numeric_limits<float>::max() - 1.f
)
{
  auto params = gen_params(n, interval_boundary);

  if (bias == 0) { return params; }

  std::uniform_int_distribution<> zero_bias(1, bias);

  for (auto& val: params)
  {
    if (zero_bias(gen) == bias) { val = 0.0f; }
  }

  return params;
}

TEST_CASE("Vec4 Constructors and Assignments", "[core]")
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
  std::array<float, 4> arr{ params[9], params[6], params[8], params[7] };
  auto                 v3 = Vec4(arr.data());
  FSFV_CHECK_COMPONENTS(v3, arr[0], arr[1], arr[2], arr[3]);

  const LVec3 vec3 = LVec3(&params[6]);

  // LVec3 conversion
  auto       v4  = Vec4(vec3);
  const auto cmp = vec3.components();
  FSFV_CHECK_COMPONENTS(v4, cmp[0], cmp[1], cmp[2], 0);

  // (LVec3, float) constructor
  auto       v5   = Vec4(vec3, params[5]);
  const auto cmp1 = vec3.components();
  FSFV_CHECK_COMPONENTS(v5, cmp1[0], cmp1[1], cmp1[2], params[5]);

  // copy constructor
  auto       v6    = Vec4(v0);
  const auto cmpv0 = v0.components();
  FSFV_CHECK_COMPONENTS(v6, cmpv0[0], cmpv0[1], cmpv0[2], cmpv0[3]);

  // move constructor
  const auto cmpv1 = v1.components();
  auto       v7    = Vec4(std::move(v1));
  FSFV_CHECK_COMPONENTS(v7, cmpv1[0], cmpv1[1], cmpv1[2], cmpv1[3]);

  // copy assignment
  v6 = v2;
  FSFV_CHECK_COMPONENTS(v6, params[3], params[4], params[5], params[6]);

  // move assingment
  v7 = std::move(v3);
  FSFV_CHECK_COMPONENTS(v7, arr[0], arr[1], arr[2], arr[3]);
}

TEST_CASE("Vec3 Constructors and Assignments", "[core]")
{
  const auto params = biased_params(10, 10);

  // single-value constructor
  auto v0 = LVec3(params[0]);
  FSFV_CHECK_COMP3(v0, params[0], params[0], params[0]);
  FSFV_CHECK_COMPONENTS(Vec4{ v0 }, params[0], params[0], params[0], 0);

  // components constructor
  auto v1 = LVec3(params[1], params[3], params[5]);
  FSFV_CHECK_COMP3(v1, params[1], params[3], params[5]);
  FSFV_CHECK_COMPONENTS(Vec4{ v1 }, params[1], params[3], params[5], 0);

  // pointer constructor
  auto v2 = LVec3(&params[6]);
  FSFV_CHECK_COMP3(v2, params[6], params[7], params[8]);
  FSFV_CHECK_COMPONENTS(Vec4{ v2 }, params[6], params[7], params[8], 0);
  auto arr = std::array<float, 3>{ params[9], params[7], params[3] };
  auto v3  = LVec3(arr.data());
  FSFV_CHECK_COMP3(v3, arr[0], arr[1], arr[2]);
  FSFV_CHECK_COMPONENTS(Vec4{ v3 }, arr[0], arr[1], arr[2], 0);

  // Vec4 conversion
  auto       vec4 = Vec4{ params[5], params[7], params[1], params[4] };
  auto       v4   = LVec3(vec4);
  const auto cmp  = vec4.components();
  FSFV_CHECK_COMP3(v4, cmp[0], cmp[1], cmp[2]);
  FSFV_CHECK_COMPONENTS(Vec4{ v4 }, cmp[0], cmp[1], cmp[2], 0);

  // copy constructor
  auto v6 = LVec3(v1);
  FSFV_CHECK_COMP3(v6, params[1], params[3], params[5]);
  FSFV_CHECK_COMPONENTS(Vec4{ v6 }, params[1], params[3], params[5], 0);

  // move constructor
  auto v7 = LVec3(std::move(v2));
  FSFV_CHECK_COMP3(v7, params[6], params[7], params[8]);
  FSFV_CHECK_COMPONENTS(Vec4{ v7 }, params[6], params[7], params[8], 0);

  // copy assignment
  v6 = v3;
  FSFV_CHECK_COMP3(v6, arr[0], arr[1], arr[2]);
  FSFV_CHECK_COMPONENTS(Vec4{ v6 }, arr[0], arr[1], arr[2], 0);

  // move assignment
  v7 = std::move(v4);
  FSFV_CHECK_COMP3(v7, cmp[0], cmp[1], cmp[2]);
  FSFV_CHECK_COMPONENTS(Vec4{ v7 }, cmp[0], cmp[1], cmp[2], 0);
}

#include "test_shuffles.inl"

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

template<typename T, size_t N>
std::vector<std::array<T, N>> cartesian_power(const std::array<T, N>& arr)
{
  std::vector<std::array<T, N>> res{};
  res.reserve(std::pow(N, N));

  cartesian_helper(res, std::array<T, N>{}, arr);

  return res;
}

TEST_CASE("Sort and MinMax4", "[methods]")
{
  const auto params = gen_params(4);

  std::array<float, 4> vals;
  vals[0] = params[0];
  vals[1] = params[1];
  vals[2] = params[2];
  vals[3] = params[3];

  const auto cartesian = cartesian_power(vals);

  for (auto combination: cartesian)
  {
    Vec4 test{ combination.data() };

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
    FSFV_CHECK_COMPONENTS(
      sort(test),
      combination[0],
      combination[1],
      combination[2],
      combination[3]
    );

    std::ranges::sort(combination, std::greater<float>{});
    FSFV_CHECK_COMPONENTS(
      reverse_sort(test),
      combination[0],
      combination[1],
      combination[2],
      combination[3]
    );
  }
}

TEST_CASE("Sort and MinMax3", "[methods]")
{
  const auto params = gen_params(3);

  std::array<float, 3> vals;
  vals[0] = params[0];
  vals[1] = params[1];
  vals[2] = params[2];

  const auto cartesian = cartesian_power(vals);

  for (auto combination: cartesian)
  {
    LVec3 test{ combination.data() };

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
    FSFV_CHECK_COMPONENTS(
      Vec4{ sorted },
      combination[0],
      combination[1],
      combination[2],
      0
    );

    std::ranges::sort(combination, std::greater<float>{});
    sorted = reverse_sort(test);
    FSFV_CHECK_COMP3(sorted, combination[0], combination[1], combination[2]);
    FSFV_CHECK_COMPONENTS(
      Vec4{ sorted },
      combination[0],
      combination[1],
      combination[2],
      0
    );
  }
}

TEST_CASE("Dot Product", "[linear_algebra]")
{
  const auto params = biased_params(8, 8);

  Vec4 v(&params[0]);
  Vec4 w(&params[4]);

  {
    float res = dot(v, w);

    const auto av = v.components();
    const auto aw = w.components();

    float check = av[0] * aw[0] + av[1] * aw[1] + av[2] * aw[2] + av[3] * aw[3];

    CHECK(res == check);
  }

  LVec3 lv{ v };
  LVec3 lw{ w };

  {
    float res = dot(lv, lw);

    const auto av = lv.components();
    const auto aw = lw.components();

    float check = av[0] * aw[0] + av[1] * aw[1] + av[2] * aw[2];

    CHECK(res == check);
  }
}

TEST_CASE("Length And Norm", "[linear_algebra]")
{
  for (int i = 0; i < 1000; ++i)
  {
    const Vec4 v{ gen_params(4, 10'000).data() };
    const auto norm_online = normalize(v);
    const auto norm_pbrt   = normalize_alt(v);

    UNSCOPED_INFO("For: " << Catch::StringMaker<Vec4>::convert(v));
    CHECK(norm_online == norm_pbrt);
  }

  for (int i = 0; i < 1000; ++i)
  {
    const Vec4 v{ biased_params(4, 8, 10'000).data() };
    const auto norm_online = normalize(v);
    const auto norm_pbrt   = normalize_alt(v);

    UNSCOPED_INFO("For: " << Catch::StringMaker<Vec4>::convert(v));
    CHECK(norm_online == norm_pbrt);
  }
}

TEST_CASE("Constructors 3x3 Matrices", "[basics]")
{
  LMat3 m(1, 2, 1, -2, 0, 5, 3, 3, 4);
  CHECK(m == LMat3(LVec3(1, 2, 1), LVec3(-2, 0, 5), LVec3(3, 3, 4)));
}

TEST_CASE("Constructors 4x4 Matrices", "[basics]")
{
  Mat4 m(1, 2, 1, 0, -2, 0, 5, 0, 3, 3, 4, 0, 0, 0, 0, 3);
  CHECK(
    m
    == Mat4(
      Vec4(1, 2, 1, 0),
      Vec4(-2, 0, 5, 0),
      Vec4(3, 3, 4, 0),
      Vec4(0, 0, 0, 3)
    )
  );
}

TEST_CASE("Determinant 3x3", "[linear_algebra]")
{
  LMat3 m{ 1.f };
  CHECK(m.determinant() == 1.f);

  m = LMat3{ 2.f };
  CHECK(m.determinant() == 8.f);

  m = LMat3(1, 2, 1, -2, 0, 5, 3, 3, 4);
  CHECK(m.determinant() == 25.f);
}

TEST_CASE("Determinant 4x4", "[linear_algebra]")
{
  Mat4 m{ 1.f };
  CHECK(m.determinant() == 1.f);
  CHECK(m.determinant_3x3() == 1.f);

  m = Mat4{ 2.f };
  CHECK(m.determinant() == 16.f);
  CHECK(m.determinant_3x3() == 8.f);

  m = Mat4(
    Vec4(1, 2, 1, 0),
    Vec4(-2, 0, 5, 0),
    Vec4(3, 3, 4, 0),
    Vec4(0, 0, 0, 3)
  );
  CHECK(m.determinant() == 75.f);
  CHECK(m.determinant_3x3() == 25.f);

  m = Mat4(
    Vec4(8, -4, 3, 9),
    Vec4(7, 16, -21, 2),
    Vec4(-13, -25, -25, 9),
    Vec4(23, 16, -3, 11)
  );

  CHECK(m.determinant() == -44735.f);
  CHECK(m.determinant_3x3() == -9093.f);

  auto m3 = LMat3(8, -4, 3, 7, 16, -21, -13, -25, -25);

  CHECK(m.determinant_3x3() == m3.determinant());
}

// Global floating-point tolerance
constexpr float EPSILON = 1e-1f; /* !!!!!!!!!!!!!!!!!!!!!!! quite low */

using Quaternion = Vec4;
using Vector4    = Vec4;
using Vector3    = LVec3;
using Matrix3x3  = LMat3;
using Matrix4x4  = Mat4;

// Macro to compare two matrices element-by-element
#define REQUIRE_MATRIX_NEAR(mat1, mat2, epsilon) \
    auto diff0 = abs(mat1.cols[0] - mat2.cols[0]); \
    REQUIRE(diff0.x < epsilon); \
    REQUIRE(diff0.y < epsilon); \
    REQUIRE(diff0.z < epsilon); \
    REQUIRE(diff0.w < epsilon); \
    auto diff1 = abs(mat1.cols[1] - mat2.cols[1]); \
    REQUIRE(diff1.x < epsilon); \
    REQUIRE(diff1.y < epsilon); \
    REQUIRE(diff1.z < epsilon); \
    REQUIRE(diff1.w < epsilon); \
    auto diff2 = abs(mat1.cols[2] - mat2.cols[2]); \
    REQUIRE(diff2.x < epsilon); \
    REQUIRE(diff2.y < epsilon); \
    REQUIRE(diff2.z < epsilon); \
    REQUIRE(diff2.w < epsilon); \
    auto diff3 = abs(mat1.cols[3] - mat2.cols[3]); \
    REQUIRE(diff3.x < epsilon); \
    REQUIRE(diff3.y < epsilon); \
    REQUIRE(diff3.z < epsilon); \
    REQUIRE(diff3.w < epsilon); \
            //REQUIRE(all<4>(abs(mat1.cols[r] - mat2.cols[r]) < Vec4(epsilon,epsilon,epsilon,epsilon))); \


///////////////////////////////////////////////////////////////////
// Additional helper for comparing quaternions with tolerance.
// We consider two quaternions equivalent if they differ by sign
// (q and -q represent the same rotation) or if their components
// are within a tolerance.
///////////////////////////////////////////////////////////////////
static void REQUIRE_QUATERNION_CLOSE(
  const Quaternion& q1,
  const Quaternion& q2,
  float             epsilon
)
{
  // Check both direct and negated
  float direct = std::fabs(q1.x - q2.x)
               + std::fabs(q1.y - q2.y)
               + std::fabs(q1.z - q2.z)
               + std::fabs(q1.w - q2.w);
  float negated = std::fabs(q1.x + q2.x)
                + std::fabs(q1.y + q2.y)
                + std::fabs(q1.z + q2.z)
                + std::fabs(q1.w + q2.w);

  REQUIRE((direct < 4.0f * epsilon || negated < 4.0f * epsilon));
}

struct Matrix3x3TestCase
{
  Matrix3x3 matrix;
  float     expectedDeterminant;
};

struct Matrix4x4TestCase
{
  Matrix4x4 matrix;
  float     expectedDeterminant;
};

///////////////////////////////////////////////////////////////////
// Matrix3x3 Tests
///////////////////////////////////////////////////////////////////

TEST_CASE("Matrix3x3 Determinant Tests", "[Matrix3x3]")
{
  // Cases for determinant correctness
  std::vector<Matrix3x3TestCase> testCases = {
    // Slightly off singular
    {   Matrix3x3(1.0f,    2.0f,3.0f,    4.0f,5.0f,    6.0f,7.000001f,8.0f,9.0f),
     -3.0e-6f                 },
    // Random-ish values
    {     Matrix3x3(
     3.12f,  -1.45f,
     2.76f,   5.67f,
     9.81f,  -3.45f,
     -7.11f,
     0.93f, 4.62f
 ),     360.892611f },
    // Clearly singular
    {  Matrix3x3(10.0f,   20.0f, 30.0f,   40.0f, 50.0f,   60.0f,     70.0f,   80.0f, 90.0f),
     0.0f                     },
    // Slightly perturbed singular
    {   Matrix3x3(
   1.0001f, 2.0001f,
     3.0001f, 4.0001f,
     5.0001f, 6.0001f,
     7.0001f, 8.0001f,
     9.0001f
     ),        -3.0e-4f       },
    // Large magnitude differences
    { Matrix3x3(1e-10f,    1.0f, 1e10f,   1e-5f,  2.0f,    3e5f,     4e-2f,    5.0f,  6e2f),
     -799488000.0061482f      }
  };

  for (const auto& testCase: testCases)
  {
    float det = testCase.matrix.determinant();
    REQUIRE(std::fabs(det - testCase.expectedDeterminant) < EPSILON);
  }
}

//#define REQUIRE_MATRIX_NEAR(actual, expected, eps) \
//    for (int r = 0; r < actual.Rows(); ++r) { \
//        for (int c = 0; c < actual.Cols(); ++c) { \
//            CAPTURE(r, c, actual(r, c), expected(r, c)); \
//            REQUIRE(std::fabs(actual(r, c) - expected(r, c)) < eps); \
//        } \
//    }

TEST_CASE("Matrix3x3 Additional Multiplication Tests", "[Matrix3x3]")
{
  SECTION("Identity x Identity")
  {
    Matrix3x3 I      = Matrix3x3(1.f);
    Matrix4x4 result = I * I;
    REQUIRE_MATRIX_NEAR(result, Mat4(1.f), EPSILON);
  }

  SECTION("Identity x Random")
  {
    Matrix3x3 R(1.5f, 2.5f, -3.0f, -4.1f, 5.3f, 6.2f, 7.7f, -8.8f, 9.9f);

    Matrix3x3 I      = Matrix3x3(1.f);
    auto      result = I * R;
    REQUIRE_MATRIX_NEAR(result, Mat4(R), EPSILON);
  }

  SECTION("General Large-Scale Multiplication")
  {
    //Matrix3x3 A(
    //    1e10f, 1.0f,   2e-5f,
    //    3.0f,  4e-6f,  5.0f,
    //    6.0f,  7.0f,   8e10f
    //);
    //Matrix3x3 B(
    //    9.1f,     -1e10f,  2.0f,
    //    3e-6f,     4.7f,   5e10f,
    //   -6.3f,      7.0f,   1e-9f
    //);

    //auto result = A * B;

    //Matrix4x4 expected(
    //);

    //REQUIRE_MATRIX_NEAR(result, Mat4(expected), 1.0e-2); // A slightly looser epsilon might be needed
  }

  //SECTION("Near-Singular Matrices")
  //{
  //    Matrix3x3 A(
  //        1.0f,  2.0f,    3.0f,
  //        2.0f,  4.0f,    6.000001f,
  //        3.0f,  6.0f,    9.000002f
  //    );
  //    Matrix3x3 B(
  //        2.1f,    -1.7f,  9.0f,
  //        4.2f,     8.3f, -2.5f,
  //        0.0001f,  6.2f,  1.2f
  //    );

  //    auto result = A * B;

  //    Matrix3x3 expected(
  //        2.1, -3.4,  2.7e+01,
  //        8.4,  3.32e+01, -1.5e+01,
  //        3e-04,  3.72e+01,  1.08000024e+01
  //    );

  //    REQUIRE_MATRIX_NEAR(result, Mat4(expected), 1e-3f);
  //}
}

TEST_CASE("Matrix4x4 Additional Multiplication Tests", "[Matrix4x4]")
{
  constexpr auto mulHelper = [](const Mat4& a, const Mat4& b, const Mat4& axb)
  {
    using DStdVec4     = std::array<double, 4>;
    using StdMatrix4x4 = std::array<DStdVec4, 4>;

    const stdVec4 ac0 = a.cols[0].components();
    const stdVec4 ac1 = a.cols[1].components();
    const stdVec4 ac2 = a.cols[2].components();
    const stdVec4 ac3 = a.cols[3].components();

    const stdVec4 bc0 = b.cols[0].components();
    const stdVec4 bc1 = b.cols[1].components();
    const stdVec4 bc2 = b.cols[2].components();
    const stdVec4 bc3 = b.cols[3].components();

    const DStdVec4 dac0 = { ac0[0], ac0[1], ac0[2], ac0[3] };
    const DStdVec4 dac1 = { ac1[0], ac1[1], ac1[2], ac1[3] };
    const DStdVec4 dac2 = { ac2[0], ac2[1], ac2[2], ac2[3] };
    const DStdVec4 dac3 = { ac3[0], ac3[1], ac3[2], ac3[3] };

    const DStdVec4 dbc0 = { bc0[0], bc0[1], bc0[2], bc0[3] };
    const DStdVec4 dbc1 = { bc1[0], bc1[1], bc1[2], bc1[3] };
    const DStdVec4 dbc2 = { bc2[0], bc2[1], bc2[2], bc2[3] };
    const DStdVec4 dbc3 = { bc3[0], bc3[1], bc3[2], bc3[3] };

    StdMatrix4x4 A{ dac0, dac1, dac2, dac3 };
    StdMatrix4x4 B{ dbc0, dbc1, dbc2, dbc3 };

    for (int col = 0; col < 4; ++col)
    {
      for (int row = 0; row < 4; ++row)
      {
        double sum = 0.0;
        for (int k = 0; k < 4; ++k)
        {
          sum += A[k][row] * B[col][k];
        }
        CHECK(axb[col].components()[row] - sum < EPSILON);
      }
    }
  };

  SECTION("Identity x Identity")
  {
    Matrix4x4 I      = Matrix4x4(1.f);
    Matrix4x4 result = I * I;
    mulHelper(I, I, result);
  }

  SECTION("Identity x Random")
  {
    Matrix4x4 R(
      2.2f,
      -3.3f,
      4.4f,
      0.5f,
      -6.5f,
      1e-4f,
      7.0f,
      -8.9f,
      9.1f,
      10.2f,
      11.3f,
      12.4f,
      0.0f,
      -0.1f,
      1e-10f,
      1.0f
    );

    Matrix4x4 I      = Matrix4x4(1.f);
    Matrix4x4 result = I * R;

    mulHelper(I, R, result);
    mulHelper(R, I, result);

    result = R * I;

    mulHelper(I, R, result);
    mulHelper(R, I, result);
  }

  //SECTION("General Large-Scale Multiplication")
  //{
  //    Matrix4x4 A(
  //        1e10f, 1.0f,    2e-5f,  -1.0f,
  //        3.0f,  4e-6f,  5.0f,    6e10f,
  //        7.0f,  8.0f,    9e10f, 10.0f,
  //        0.0f,  1.0f,    1e-9f, 1e10f
  //    );
  //    Matrix4x4 B(
  //        2.5f,    -1e10f, 2.0f,     3.0f,
  //        3e-6f,   4.7f,   5e10f,    6.1f,
  //       -6.3f,    7.0f,   1e-9f,    8.0f,
  //        9.0f,    1.0f,   2.0f,     3e10f
  //    );

  //    Matrix4x4 result = A * B;

  //    mulHelper(A, B, result);
  //}
  SECTION("General Large-Scale Multiplication")
  {
    Matrix4x4 A(
      1e8f,
      1.0f,
      2e-5f,
      -1.0f,
      3.0f,
      4e-6f,
      5.0f,
      6e8f,
      7.0f,
      8.0f,
      9e8f,
      10.0f,
      0.0f,
      1.0f,
      1e-8f,
      1e8f
    );
    Matrix4x4 B(
      2.5f,
      -1e8f,
      2.0f,
      3.0f,
      3e-6f,
      4.7f,
      5e8f,
      6.1f,
      -6.3f,
      7.0f,
      1e-8f,
      8.0f,
      9.0f,
      1.0f,
      2.0f,
      3e8f
    );

    Matrix4x4 result = A * B;

    mulHelper(A, B, result);
  }

  SECTION("Near-Singular Matrices")
  {
    Matrix4x4 A(
      1.0f,
      2.0f,
      3.0f,
      4.0f,
      2.0f,
      4.0f,
      6.0f,
      8.000001f,
      3.0f,
      6.0f,
      9.0001f,
      12.0f,
      4.0f,
      8.0f,
      12.0f,
      16.0f
    );
    Matrix4x4 B(
      2.0f,
      5.0f,
      -1.0f,
      7.0f,
      0.5f,
      1e-4f,
      2.6f,
      3.1f,
      9.9f,
      -8.1f,
      1e-5f,
      2.2f,
      6.6f,
      7.7f,
      8.8f,
      9.9f
    );

    Matrix4x4 result = A * B;

    mulHelper(A, B, result);
  }
}

//TEST_CASE("Matrix3x3 Inverse Tests", "[Matrix3x3]") {
//    // Various invertible matrices
//    std::vector<Matrix3x3> testCases = {
//        // Edge case with small diagonal entries
//        Matrix3x3(0.0001f, 1.0f,   2.0f,
//                  3.0f,    0.0001f,4.0f,
//                  5.0f,    6.0f,   0.0001f),
//        // Mixed signs
//        Matrix3x3(1.5f,    -2.0f,   3.0f,
//                  -4.0f,   5.0f,   -6.0f,
//                  7.0f,    -8.0f,   9.0f),
//        // Huge differences in scale
//        Matrix3x3(1e-10f,  1.0f,   1e10f,
//                  1e-5f,   2.0f,   3e5f,
//                  4e-2f,   5.0f,   6e2f)
//    };
//
//    for (const auto& mat : testCases) {
//        Matrix3x3 inv = mat.inverse();
//        Mat4 product = mat * Mat4(inv);
//        REQUIRE_MATRIX_NEAR(product, Mat4(1.0f), EPSILON);
//    }
//}

TEST_CASE("Matrix3x3 Operator Tests", "[Matrix3x3]")
{
  //SECTION("Matrix + Matrix") {
  //    Matrix3x3 A(1, 2, 3,
  //                4, 5, 6,
  //                7, 8, 9);
  //    Matrix3x3 B(9, 8, 7,
  //                6, 5, 4,
  //                3, 2, 1);
  //    auto C = A + B;
  //    Matrix3x3 expected(10, 10, 10,
  //                       10, 10, 10,
  //                       10, 10, 10);
  //    REQUIRE_MATRIX_NEAR(C, expected, EPSILON);
  //}

  //SECTION("Matrix - Matrix") {
  //    Matrix3x3 A(5, 5, 5,
  //                5, 5, 5,
  //                5, 5, 5);
  //    Matrix3x3 B(1, 2, 3,
  //                4, 5, 6,
  //                7, 8, 9);
  //    auto C = A - B;
  //    Matrix3x3 expected(4, 3, 2,
  //                       1, 0, -1,
  //                       -2, -3, -4);
  //    REQUIRE_MATRIX_NEAR(C, expected, EPSILON);
  //}

  //SECTION("Matrix * Matrix") {
  //    Matrix3x3 A(1, 2, 3,
  //                4, 5, 6,
  //                7, 8, 9);
  //    Matrix3x3 B(9, 8, 7,
  //                6, 5, 4,
  //                3, 2, 1);
  //    auto C = A * B;
  //    Matrix3x3 expected(30, 24, 18,
  //                       84, 69, 54,
  //                       138,114,90);
  //    REQUIRE_MATRIX_NEAR(C, expected, EPSILON);
  //}

  //SECTION("Matrix * Scalar") {
  //    Matrix3x3 A(1, 2, 3,
  //                4, 5, 6,
  //                7, 8, 9);
  //    float s = 2.0f;
  //    Matrix3x3 B = LMat3(A * s);
  //    Matrix3x3 expected(2, 4, 6,
  //                       8, 10,12,
  //                       14,16,18);
  //    REQUIRE_MATRIX_NEAR(B, expected, EPSILON);
  //}

  //SECTION("Matrix / Scalar") {
  //    Matrix3x3 A(2, 4, 6,
  //                8, 10,12,
  //                14,16,18);
  //    float s = 2.0f;
  //    Matrix3x3 B = LMat3(A / s);
  //    Matrix3x3 expected(1, 2, 3,
  //                       4, 5, 6,
  //                       7, 8, 9);
  //    REQUIRE_MATRIX_NEAR(B, expected, EPSILON);
  //}

  //SECTION("Vector Transform") {
  //    Matrix4x4 I = Matrix4x4(1.f);
  //    Vector3 v(1, 2, 3);
  //    auto result = I * Vector4(v);
  //    REQUIRE(result.x == Approx(1.0f).margin(EPSILON));
  //    REQUIRE(result.y == Approx(2.0f).margin(EPSILON));
  //    REQUIRE(result.z == Approx(3.0f).margin(EPSILON));
  //}
}

/*
TEST_CASE("Matrix3x3 Accessor Tests", "[Matrix3x3]") {
    Matrix3x3 mat(1,2,3,
                  4,5,6,
                  7,8,9);

    SECTION("GetRow") {
        Vector3 r0 = mat.GetRow(0);
        Vector3 r1 = mat.GetRow(1);
        Vector3 r2 = mat.GetRow(2);
        REQUIRE(r0.x == Approx(1.0f));
        REQUIRE(r0.y == Approx(2.0f));
        REQUIRE(r0.z == Approx(3.0f));
        REQUIRE(r1.x == Approx(4.0f));
        REQUIRE(r2.z == Approx(9.0f));
    }

    SECTION("GetColumn") {
        Vector3 c0 = mat.GetColumn(0);
        Vector3 c2 = mat.GetColumn(2);
        REQUIRE(c0.x == Approx(1.0f));
        REQUIRE(c0.y == Approx(4.0f));
        REQUIRE(c0.z == Approx(7.0f));
        REQUIRE(c2.x == Approx(3.0f));
        REQUIRE(c2.y == Approx(6.0f));
    }
}
*/

///////////////////////////////////////////////////////////////////
// Matrix4x4 Tests
///////////////////////////////////////////////////////////////////
//TEST_CASE("Matrix4x4 Basic Tests", "[Matrix4x4]") {
//    SECTION("Identity") {
//        Matrix4x4 identity = Matrix4x4::Identity();
//        for (int i = 0; i < 4; ++i) {
//            for (int j = 0; j < 4; ++j) {
//                float expected = (i == j) ? 1.0f : 0.0f;
//                REQUIRE(identity(i, j) == Approx(expected).margin(EPSILON));
//            }
//        }
//    }
//
//    SECTION("Zero") {
//        Matrix4x4 zero = Matrix4x4::Zero();
//        for (int i = 0; i < 4; ++i) {
//            for (int j = 0; j < 4; ++j) {
//                REQUIRE(zero(i, j) == Approx(0.0f).margin(EPSILON));
//            }
//        }
//    }
//}

TEST_CASE("Matrix4x4 Determinant Tests", "[Matrix4x4]")
{
  // Various cases for 4x4 determinant
  std::vector<Matrix4x4TestCase> testCases = {
    // Slightly off singular
    {  Matrix4x4(
  1.0f,   2.0f,
     3.0f,   4.0f,
     5.0f,  6.0f,
     7.0f,8.0f,
     9.0f,10.0f,
     11.0f,12.0f,
     13.000001f,  14.0f,
     15.0f, 16.0f
 ),           -3.0e-6f },
    // Random-ish
    { Matrix4x4(
 2.34f, -1.23f,
     4.56f, -3.21f,
     5.78f, 9.12f,
     -6.54f,
     8.43f, -7.89f,
     0.32f, 1.67f,
     -4.58f,
     3.14f, -8.97f,
     2.76f, 9.81f
 ), 3694.227357599999f },
    // Clearly singular
    {  Matrix4x4(
  1.0f,   0.0f,
     0.0f,   0.0f,
     0.0f,  1.0f,
     0.0f,   0.0f,
     0.0f,  0.0f,
     1.0f,  0.0f,
     0.0f,   0.0f,
     0.0f, 0.0f
 ),               0.0f },
    //// Large magnitude differences
    //{Matrix4x4(1e-10f,1.0f,   1e10f, 1.0f,
    //           1e-5f,  2.0f,   3e5f,  2.0f,
    //           4e-2f,  5.0f,   6e2f,  3.0f,
    //           7e-3f,  8.0f,   9e4f,  4.0f), 4.56e20f},
    // Another tricky case
    { Matrix4x4(
 1e10f,   2.0f,
     3.0f,   4.0f,
     5.0f,  6.0f,
     7.0f, 1e-10f,
     9.0f, 10.0f,
     1e5f, 12.0f,
     13.0f,  14.0f,
     15.0f, 16.0f
 ),       9.598976e16f }
  };

  for (const auto& testCase: testCases)
  {
    float det = testCase.matrix.determinant();
    REQUIRE(std::fabs(det - testCase.expectedDeterminant) < EPSILON);
  }
}

TEST_CASE("Matrix4x4 Inverse Tests", "[Matrix4x4]")
{
  SECTION("Invertible large magnitude cases")
  {
    // Invertible test matrices
    std::vector<Matrix4x4> testCases
      = { Matrix4x4(
            0.0001f,
            1.0f,
            2.0f,
            3.0f,
            4.0f,
            0.0001f,
            5.0f,
            6.0f,
            7.0f,
            8.0f,
            0.0001f,
            9.0f,
            10.0f,
            11.0f,
            12.0f,
            0.0001f
          ),
          //Matrix4x4(1e-10f,1.0f,   1e10f,  1.0f,
          //          1e-5f,  2.0f,   3e5f,   2.0f,
          //          4e-2f,  5.0f,   6e2f,   3.0f,
          //          7e-3f,  8.0f,   9e4f,   4.0f),
          // Another near-singular but still invertible case
          Matrix4x4(
            1e10f,
            2.0f,
            3.0f,
            4.0f,
            5.0f,
            6.0f,
            7.0f,
            1e-10f,
            9.0f,
            10.0f,
            1e5f,
            12.0f,
            13.0f,
            14.0f,
            15.0f,
            16.000001f
          ) };

    for (const auto& mat: testCases)
    {
      Matrix4x4 invMat  = mat.inverse();
      Matrix4x4 product = mat * invMat;
      REQUIRE_MATRIX_NEAR(
        product,
        Matrix4x4(1.f),
        0.1f /*!!!!!!!!!!!!!!!!!!!!!!!!!!*/
      );
    }
  }

  SECTION("Singular matrix")
  {
    // Expect failure when inverting a truly singular matrix
    Matrix4x4 singular(
      1.0f,
      2.0f,
      3.0f,
      4.0f,
      5.0f,
      6.0f,
      7.0f,
      8.0f,
      9.0f,
      10.0f,
      11.0f,
      12.0f,
      13.0f,
      14.0f,
      15.0f,
      16.0f
    );
    CHECK(!singular.inverse_safe());
  }
}

TEST_CASE("Matrix4x4 Operator Tests", "[Matrix4x4]")
{
  //SECTION("Matrix + Matrix") {
  //    Matrix4x4 A(
  //        1,2,3,4,
  //        5,6,7,8,
  //        9,10,11,12,
  //        13,14,15,16
  //    );
  //    Matrix4x4 B(
  //        16,15,14,13,
  //        12,11,10,9,
  //        8,7,6,5,
  //        4,3,2,1
  //    );
  //    Matrix4x4 C = A + B;
  //    Matrix4x4 expected(
  //        17,17,17,17,
  //        17,17,17,17,
  //        17,17,17,17,
  //        17,17,17,17
  //    );
  //    REQUIRE_MATRIX_NEAR(C, expected, EPSILON);
  //}

  //SECTION("Matrix - Matrix") {
  //    Matrix4x4 A(
  //        5,5,5,5,
  //        5,5,5,5,
  //        5,5,5,5,
  //        5,5,5,5
  //    );
  //    Matrix4x4 B(
  //        1,2,3,4,
  //        5,6,7,8,
  //        9,10,11,12,
  //        13,14,15,16
  //    );
  //    Matrix4x4 C = A - B;
  //    Matrix4x4 expected(
  //        4,3,2,1,
  //        0,-1,-2,-3,
  //        -4,-5,-6,-7,
  //        -8,-9,-10,-11
  //    );
  //    REQUIRE_MATRIX_NEAR(C, expected, EPSILON);
  //}

  //SECTION("Matrix * Matrix") {
  //    Matrix4x4 A(
  //        1,2,3,4,
  //        5,6,7,8,
  //        9,10,11,12,
  //        13,14,15,16
  //    );
  //    Matrix4x4 B(
  //        16,15,14,13,
  //        12,11,10,9,
  //        8,7,6,5,
  //        4,3,2,1
  //    );
  //    Matrix4x4 C = A * B;
  //    Matrix4x4 expected(
  //        80,70,60,50,
  //        240,214,188,162,
  //        400,358,316,274,
  //        560,502,444,386
  //    );
  //    REQUIRE_MATRIX_NEAR(C, expected, EPSILON);
  //}

  //SECTION("Matrix * Scalar") {
  //    Matrix4x4 A(
  //        1,2,3,4,
  //        5,6,7,8,
  //        9,10,11,12,
  //        13,14,15,16
  //    );
  //    float s = 2.0f;
  //    Matrix4x4 result = A * s;
  //    Matrix4x4 expected(
  //        2,4,6,8,
  //        10,12,14,16,
  //        18,20,22,24,
  //        26,28,30,32
  //    );
  //    REQUIRE_MATRIX_NEAR(result, expected, EPSILON);
  //}

  //SECTION("Matrix / Scalar") {
  //    Matrix4x4 A(
  //        2,4,6,8,
  //        10,12,14,16,
  //        18,20,22,24,
  //        26,28,30,32
  //    );
  //    float s = 2.0f;
  //    Matrix4x4 result = A / s;
  //    Matrix4x4 expected(
  //        1,2,3,4,
  //        5,6,7,8,
  //        9,10,11,12,
  //        13,14,15,16
  //    );
  //    REQUIRE_MATRIX_NEAR(result, expected, EPSILON);
  //}

  //SECTION("Vector4 Transform") {
  //    Matrix4x4 I = Matrix4x4(1.f);
  //    Vector4 v(1,2,3,1);
  //    Vector4 result = I * v;
  //    REQUIRE(result.x == Approx(1.0f));
  //    REQUIRE(result.y == Approx(2.0f));
  //    REQUIRE(result.z == Approx(3.0f));
  //    REQUIRE(result.w == Approx(1.0f));
  //}
}

TEST_CASE("Matrix4x4 Accessor Tests", "[Matrix4x4]")
{
  Matrix4x4 mat(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
  SECTION("GetRow")
  {
    Vector4 r0 = mat.cols[0];
    Vector4 r1 = mat.cols[1];
    Vector4 r3 = mat.cols[3];
    REQUIRE(r0.x == Approx(1.0f));
    REQUIRE(r0.y == Approx(2.0f));
    REQUIRE(r0.z == Approx(3.0f));
    REQUIRE(r0.w == Approx(4.0f));
    REQUIRE(r1.w == Approx(8.0f));
    REQUIRE(r3.x == Approx(13.0f));
  }
  SECTION("GetColumn")
  {
    auto    matT = mat.transpose();
    Vector4 c0   = matT.cols[0];
    Vector4 c2   = matT.cols[2];
    REQUIRE(c0.x == Approx(1.0f));
    REQUIRE(c0.y == Approx(5.0f));
    REQUIRE(c0.z == Approx(9.0f));
    REQUIRE(c0.w == Approx(13.0f));
    REQUIRE(c2.x == Approx(3.0f));
    REQUIRE(c2.y == Approx(7.0f));
  }
}

//TEST_CASE("Matrix4x4 Additional Constructors & Methods", "[Matrix4x4]") {
//    SECTION("Translation") {
//        Vector3 t(1.0f, 2.0f, 3.0f);
//        Matrix4x4 M = Matrix4x4::translation(t);
//        Vector4 origin(0,0,0,1);
//        Vector4 transformed = M * origin;
//        REQUIRE(transformed.x == Approx(1.0f));
//        REQUIRE(transformed.y == Approx(2.0f));
//        REQUIRE(transformed.z == Approx(3.0f));
//        REQUIRE(transformed.w == Approx(1.0f));
//    }
//
//    SECTION("Scaling") {
//        Matrix4x4 S = Matrix4x4::Scaling(2.0f, 3.0f, 4.0f);
//        Vector4 v(1,1,1,1);
//        Vector4 out = S * v;
//        REQUIRE(out.x == Approx(2.0f));
//        REQUIRE(out.y == Approx(3.0f));
//        REQUIRE(out.z == Approx(4.0f));
//        REQUIRE(out.w == Approx(1.0f));
//    }
//
//    SECTION("RotationX") {
//        Matrix4x4 Rx = Matrix4x4::RotationX(3.14159f / 2.0f);
//        Vector4 v(0,1,0,1);
//        Vector4 out = Rx * v;
//        REQUIRE(out.x == Approx(0.0f));
//        REQUIRE(out.y == Approx(0.0f));
//        REQUIRE(out.z == Approx(1.0f));
//    }
//
//    SECTION("RotationY") {
//        Matrix4x4 Ry = Matrix4x4::RotationY(3.14159f / 2.0f);
//        Vector4 v(0,0,1,1);
//        Vector4 out = Ry * v;
//        REQUIRE(out.x == Approx(1.0f));
//        REQUIRE(out.y == Approx(0.0f));
//        REQUIRE(out.z == Approx(0.0f));
//    }
//
//    SECTION("RotationZ") {
//        Matrix4x4 Rz = Matrix4x4::RotationZ(3.14159f / 2.0f);
//        Vector4 v(1,0,0,1);
//        Vector4 out = Rz * v;
//        REQUIRE(out.x == Approx(0.0f));
//        REQUIRE(out.y == Approx(1.0f));
//        REQUIRE(out.z == Approx(0.0f));
//    }
//
//    SECTION("Perspective") {
//        Matrix4x4 persp = Matrix4x4::Perspective(45.0f, 1.0f, 0.1f, 100.0f);
//        Vector4 testVec(0,0,-1,1);
//        (void)(persp * testVec);
//        REQUIRE(true);
//    }
//
//    SECTION("Orthographic") {
//        Matrix4x4 ortho = Matrix4x4::Orthographic(-1,1,-1,1,0.1f,100.0f);
//        Vector4 testVec(2,2,-2,1);
//        Vector4 out = ortho * testVec;
//        REQUIRE(true);
//    }
//
//    SECTION("LookAt") {
//        Matrix4x4 look = Matrix4x4::LookAt(
//            Vector3(0,0,0),   // eye
//            Vector3(0,0,-1),  // target
//            Vector3(0,1,0)    // up
//        );
//        Vector4 forward(0,0,-1,1);
//        Vector4 out = look * forward;
//        REQUIRE(std::fabs(out.x) < EPSILON);
//        REQUIRE(std::fabs(out.y) < EPSILON);
//    }
//
//    SECTION("FromColumns") {
//        Vector4 c1(1,2,3,4);
//        Vector4 c2(5,6,7,8);
//        Vector4 c3(9,10,11,12);
//        Vector4 c4(13,14,15,16);
//        Matrix4x4 M = Matrix4x4::FromColumns(c1, c2, c3, c4);
//        REQUIRE(M(0,0) == Approx(1.0f));
//        REQUIRE(M(0,1) == Approx(5.0f));
//        REQUIRE(M(1,3) == Approx(14.0f));
//        REQUIRE(M(3,3) == Approx(16.0f));
//    }
//
//    SECTION("FromAffine") {
//        Matrix3x3 R(1,2,3,4,5,6,7,8,9);
//        Vector3 trans(10,11,12);
//        Matrix4x4 aff = Matrix4x4::FromAffine(R, trans);
//        REQUIRE(aff(0,3) == Approx(10.0f));
//        REQUIRE(aff(1,3) == Approx(11.0f));
//        REQUIRE(aff(2,3) == Approx(12.0f));
//        REQUIRE(aff(0,0) == Approx(1.0f));
//        REQUIRE(aff(2,2) == Approx(9.0f));
//    }
//
//    SECTION("FromProjective") {
//        Matrix3x3 R(1,2,3,4,5,6,7,8,9);
//        Vector3 trans(10,11,12);
//        float w = 0.5f;
//        Matrix4x4 proj = Matrix4x4::FromProjective(R, trans, w);
//        REQUIRE(proj(0,0) == Approx(1.0f));
//        REQUIRE(proj(2,2) == Approx(9.0f));
//        REQUIRE(proj(3,3) == Approx(0.5f));
//    }
//}

TEST_CASE("Matrix4x4 Extra Features", "[Matrix4x4]")
{
  //SECTION("Transpose") {
  //    Matrix4x4 original(
  //        1.0f, 2.0f,  3.0f,  4.0f,
  //        5.0f, 6.0f,  7.0f,  8.0f,
  //        9.0f, 10.0f, 11.0f, 12.0f,
  //        13.0f,14.0f, 15.0f,16.0f
  //    );
  //    Matrix4x4 trans = original.transpose();
  //    // TODO
  //}

  SECTION("TransposedInverse")
  {
    Matrix4x4 M(
      2.0f,
      0.0f,
      0.0f,
      0.0f,
      0.0f,
      2.0f,
      1.0f,
      0.0f,
      0.0f,
      1.0f,
      2.0f,
      0.0f,
      1.0f,
      0.0f,
      0.0f,
      2.0f
    );
    Matrix4x4 inv  = M.inverse();
    Matrix4x4 tinv = M.inverse_transposed();
    Matrix4x4 invT = inv.transpose();
    REQUIRE_MATRIX_NEAR(tinv, invT, EPSILON);
  }

  SECTION("Adjoint / TransposedAdjoint")
  {
    Matrix4x4 mat(
      2.0f,
      1.0f,
      0.0f,
      0.0f,
      1.0f,
      2.0f,
      0.0f,
      0.0f,
      0.0f,
      0.0f,
      2.0f,
      1.0f,
      0.0f,
      0.0f,
      1.0f,
      2.0f
    );
    Matrix4x4 adj  = mat.adj();
    Matrix4x4 tadj = mat.adj_transposed();
    for (int c = 0; c < 4; ++c)
    {
      REQUIRE(adj[c].x == Approx(tadj[c].x).margin(EPSILON));
      REQUIRE(adj[c].y == Approx(tadj[c].y).margin(EPSILON));
      REQUIRE(adj[c].z == Approx(tadj[c].z).margin(EPSILON));
      REQUIRE(adj[c].w == Approx(tadj[c].w).margin(EPSILON));
    }
    float     det       = mat.determinant();
    Matrix4x4 inv       = mat.inverse();
    Matrix4x4 scaledInv = inv * det;
    REQUIRE_MATRIX_NEAR(adj, scaledInv, EPSILON);
  }

  SECTION("Extract3x3")
  {
    Matrix4x4 mat(
      1.1f,
      2.2f,
      3.3f,
      4.4f,
      5.5f,
      6.6f,
      7.7f,
      8.8f,
      9.9f,
      10.1f,
      11.1f,
      12.1f,
      13.1f,
      14.1f,
      15.1f,
      16.1f
    );
    Matrix3x3 extracted = LMat3(mat);
    Matrix3x3 expected(1.1f, 2.2f, 3.3f, 5.5f, 6.6f, 7.7f, 9.9f, 10.1f, 11.1f);
    REQUIRE_MATRIX_NEAR(extracted, expected, EPSILON);
  }
}

///////////////////////////////////////////////////////////////////
// Quaternion - Extended Coverage
///////////////////////////////////////////////////////////////////
//TEST_CASE("Matrix4x4 Rotation/Euler/Quaternion Tests", "[Matrix4x4][Quaternion]") {
//    SECTION("Euler angles - rotate around Z by 90 deg") {
//        float angleRad = 3.14159265359f / 2.0f;
//        Vector3 euler(0.0f, 0.0f, angleRad);
//        Matrix4x4 eulerMat = Matrix4x4::RotationFromEuler(euler);
//        Vector4 xAxis(1.0f, 0.0f, 0.0f, 1.0f);
//        Vector4 transformed = eulerMat * xAxis;
//        REQUIRE(transformed.x == Approx(0.0f).margin(EPSILON));
//        REQUIRE(transformed.y == Approx(1.0f).margin(EPSILON));
//    }
//
//    SECTION("Euler angles - combined rotations") {
//        // Rotate 30 deg about X, then 45 deg about Y, then 60 deg about Z
//        float xRad = 3.14159265359f * 30.0f / 180.0f;
//        float yRad = 3.14159265359f * 45.0f / 180.0f;
//        float zRad = 3.14159265359f * 60.0f / 180.0f;
//        Vector3 euler(xRad, yRad, zRad);
//        Matrix4x4 eulerMat = Matrix4x4::RotationFromEuler(euler);
//        // We'll do a small sanity check on a known vector
//        Vector4 test(1,0,0,1);
//        Vector4 out = eulerMat * test;
//        // No exact simple floats here, but we can ensure it didn't break
//        REQUIRE(std::fabs(out.x) < 2.0f);
//        REQUIRE(std::fabs(out.y) < 2.0f);
//        REQUIRE(std::fabs(out.z) < 2.0f);
//    }
//
//    SECTION("Rotation from Quaternion - Identity") {
//        Quaternion qIdentity(0.0f, 0.0f, 0.0f, 1.0f);
//        Matrix4x4 quatMat = Matrix4x4::RotationFromQuaternion(qIdentity);
//        REQUIRE_MATRIX_NEAR(quatMat, Matrix4x4::Identity(), EPSILON);
//    }
//
//    SECTION("Rotation from Quaternion - 180deg about X") {
//        // 180 deg about X => (x=1, y=0, z=0, w=0) in many conventions
//        Quaternion qHalfX(1.0f, 0.0f, 0.0f, 0.0f);
//        Matrix4x4 rotMat = Matrix4x4::RotationFromQuaternion(qHalfX);
//        // Check transform of (0,1,0,1) => (0, -1, 0, 1)
//        Vector4 v(0,1,0,1);
//        Vector4 out = rotMat * v;
//        REQUIRE(out.x == Approx(0.0f).margin(EPSILON));
//        REQUIRE(out.y == Approx(-1.0f).margin(EPSILON));
//        REQUIRE(out.z == Approx(0.0f).margin(EPSILON));
//    }
//
//    SECTION("Rotation from Quaternion - random axis") {
//        // For example: 120 deg about (1,1,1) normalized
//        float angle = 3.14159f * 120.0f / 180.0f;
//        float halfAngle = angle * 0.5f;
//        Vector3 axis(1.0f, 1.0f, 1.0f);
//        float len = std::sqrt(axis.x*axis.x + axis.y*axis.y + axis.z*axis.z);
//        axis.x /= len; axis.y /= len; axis.z /= len;
//        float s = std::sin(halfAngle);
//        Quaternion q(axis.x * s, axis.y * s, axis.z * s, std::cos(halfAngle));
//        // Build matrix
//        Matrix4x4 rotMat = Matrix4x4::RotationFromQuaternion(q);
//        // Norm check: test a vector to see if length is preserved
//        Vector4 v(1,0,0,1);
//        Vector4 out = rotMat * v;
//        float inLen = std::sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
//        float outLen = std::sqrt(out.x*out.x + out.y*out.y + out.z*out.z);
//        REQUIRE(std::fabs(inLen - outLen) < 1e-4f);
//    }
//
//    SECTION("Transform Quaternion - multiple rotations") {
//        // Start with a quaternion for 90 deg about Z
//        Quaternion qZ(0.0f, 0.0f, std::sin(3.14159f/4), std::cos(3.14159f/4));
//        // Rotate by 90 deg about X via a matrix
//        Matrix4x4 matX = Matrix4x4::RotationX(3.14159f / 2.0f);
//        Quaternion qTrans = matX.TransformQuaternion(qZ);
//        float norm = std::sqrt(qTrans.x*qTrans.x + qTrans.y*qTrans.y + qTrans.z*qTrans.z + qTrans.w*qTrans.w);
//        REQUIRE(std::fabs(norm - 1.0f) < 1e-3f);
//    }
//
//    SECTION("TransformByQuaternion - combine with existing matrix") {
//        Matrix4x4 M = Matrix4x4::Identity();
//        // 180 deg about Y => (x=0,y=1,z=0,w=0) in many conventions
//        Quaternion qHalfY(0.0f, 1.0f, 0.0f, 0.0f);
//        Matrix4x4 combined = M.TransformByQuaternion(qHalfY);
//        Vector4 test(1,0,0,1);
//        Vector4 out = combined * test;
//        REQUIRE(out.x == Approx(-1.0f).margin(EPSILON));
//    }
//
//    SECTION("Compare direct multiplication with TransformQuaternion") {
//        Quaternion qZ(0.0f, 0.0f, std::sin(3.14159f/8), std::cos(3.14159f/8)); // 45 deg about Z
//        Matrix4x4 rotZ = Matrix4x4::RotationZ(3.14159f / 4.0f);
//        Quaternion qOut = rotZ.TransformQuaternion(qZ);
//        // This is effectively 45 deg + 45 deg => 90 deg about Z
//        Quaternion qExpected(0.0f, 0.0f, std::sin(3.14159f/2 / 2.0f), std::cos(3.14159f/2 / 2.0f));
//        REQUIRE_QUATERNION_CLOSE(qOut, qExpected, EPSILON);
//    }
//}
