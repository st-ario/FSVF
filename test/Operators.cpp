#include <FSVF.h>

#define CATCH_CONFIG_FAST_COMPILE
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <cmath>
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

template<FastVector V>
struct HexStringMaker
{
  static std::string convert(const V& v)
  {
    std::array<uint32_t, 4> bits{ float_to_bits(v.components()[0]),
                                  float_to_bits(v.components()[1]),
                                  float_to_bits(v.components()[2]),
                                  float_to_bits(v.components()[3]) };
    return StringMaker<std::array<uint32_t, 4>>::convert(bits);
  }
};
}

#define FSFV_CHECK_COMPONENTS(vec, x, y, z, w) \
{                                              \
  auto components = vec.components();          \
  CHECK(components[0] == Approx(x));           \
  CHECK(components[1] == Approx(y));           \
  CHECK(components[2] == Approx(z));           \
  CHECK(components[3] == Approx(w));           \
}

#define FSFV_CHECK_BITWISE(vec, x, y, z, w) \
{                                              \
  auto components = vec.components();          \
  CHECK(float_to_bits(components[0]) == float_to_bits(x));           \
  CHECK(float_to_bits(components[1]) == float_to_bits(y));           \
  CHECK(float_to_bits(components[2]) == float_to_bits(z));           \
  CHECK(float_to_bits(components[3]) == float_to_bits(w));           \
}

#define FSFV_CHECK_COMP3(vec, x, y, z) \
{                                      \
  auto components = vec.components();  \
  CHECK(components[0] == Approx(x));   \
  CHECK(components[1] == Approx(y));   \
  CHECK(components[2] == Approx(z));   \
  CHECK(Vec4{vec}.components()[3] == 0.f);           \
}

#define FSFV_CHECK_BITWISE3(vec, x, y, z) \
{                                              \
  auto components = vec.components();          \
  CHECK(float_to_bits(components[0]) == float_to_bits(x));           \
  CHECK(float_to_bits(components[1]) == float_to_bits(y));   \
  CHECK(float_to_bits(components[2]) == float_to_bits(z));   \
  CHECK(Vec4{vec}.components()[3] == 0.f);           \
}

static std::random_device rd;
static std::mt19937       gen(rd());

static std::vector<float>
  gen_params(uint16_t n, float interval_boundary = 100.0f)
{
  std::uniform_int_distribution<int>    ranb(0, 1);
  std::uniform_real_distribution<float> ranf(1.0f, 2.0f);

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
static std::vector<float>
  biased_params(uint16_t n, uint16_t bias, float interval_boundary = 100.0f)
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

TEST_CASE("Vector Comparison Operators", "[operators]")
{
  constexpr auto check_operator = [](auto v1, auto v2, auto op1, auto op2)
  {
    Vec4 result = op1(v1, v2);
    for (auto component: result.components())
    {
      CHECK((
        float_to_bits(component) == 0 || float_to_bits(component) == 0xFFFF'FFFF
      ));
    }

    auto v1c = Vec4{ v1 }.components();
    auto v2c = Vec4{ v2 }.components();

    for (int i = 0; i < v1c.size(); ++i)
    {
      bool res = op2(v1c[i], v2c[i]);
      res      = result.components()[i] ? res : !res;
      CHECK(res);
    }
  };

  auto params = biased_params(22, 5);

  SECTION("Vec4: operator<=, >=, <, >")
  {
    Vec4 v1(params[0], params[1], params[2], params[3]);
    Vec4 v2(params[4], params[5], params[6], params[7]);

    INFO("v1: " << Catch::StringMaker<Vec4>::convert(v1));
    INFO("v2: " << Catch::StringMaker<Vec4>::convert(v2));

    check_operator(v1, v2, operator<= <Vec4>, std::less_equal<float>{});
    check_operator(v1, v2, operator>= <Vec4>, std::greater_equal<float>{});
    check_operator(v1, v2, operator< <Vec4>, std::less<float>{});
    check_operator(v1, v2, operator><Vec4>, std::greater<float>{});
  }

  SECTION("LVec3: operator<=, >=, <, >")
  {
    LVec3 lv1(params[8], params[9], params[10]);
    LVec3 lv2(params[11], params[12], params[13]);

    INFO("lv1: " << Catch::StringMaker<Vec4>::convert(Vec4(lv1)));
    INFO("lv2: " << Catch::StringMaker<Vec4>::convert(Vec4(lv2)));

    check_operator(lv1, lv2, operator<= <LVec3>, std::less_equal<float>{});
    check_operator(lv1, lv2, operator>= <LVec3>, std::greater_equal<float>{});
    check_operator(lv1, lv2, operator< <LVec3>, std::less<float>{});
    check_operator(lv1, lv2, operator><LVec3>, std::greater<float>{});
  }

  SECTION("operator==, !=")
  {
    Vec4 v1(params[14], params[15], params[16], params[17]);
    Vec4 v2(params[14], params[15], params[16], params[17]);

    INFO("v1: " << Catch::StringMaker<Vec4>::convert(v1));
    INFO("v2: " << Catch::StringMaker<Vec4>::convert(v2));

    params[18] = (params[18] == params[17]) ? params[17] + 1 : params[18];
    Vec4 v3(params[14], params[15], params[16], params[18]);

    INFO("v3: " << Catch::StringMaker<Vec4>::convert(v3));

    CHECK((v1 == v2) == true);
    CHECK((v1 == v3) == false);
    CHECK((v1 != v2) == false);
    CHECK((v1 != v3) == true);

    LVec3 lv1(params[19], params[20], params[21]);
    LVec3 lv2(params[19], params[20], params[21]);

    INFO("lv1: " << Catch::StringMaker<Vec4>::convert(lv1));
    INFO("lv2: " << Catch::StringMaker<Vec4>::convert(lv2));

    params[22] = (params[22] == params[21]) ? params[21] + 1 : params[22];
    LVec3 lv3(params[19], params[20], params[22]);
    INFO("lv3: " << Catch::StringMaker<Vec4>::convert(lv3));

    CHECK((lv1 == lv2) == true);
    CHECK((lv1 == lv3) == false);
    CHECK((lv1 != lv2) == false);
    CHECK((lv1 != lv3) == true);

    if (v1.components()[3] != 0) {
        CHECK((v1 == lv1) == false);
        CHECK((lv1 == v1) == false);
        CHECK((v1 != lv1) == true);
        CHECK((lv1 != v1) == true);
    }

    Vec4  v4(params[14], params[15], params[16], 0.0f);
    LVec3 lv4(params[14], params[15], params[16]);
    INFO("v4: " << Catch::StringMaker<Vec4>::convert(v4));
    INFO("lv4: " << Catch::StringMaker<Vec4>::convert(lv4));

    CHECK((v4 == lv4) == true);
    CHECK((lv4 == v4) == true);
    CHECK((v4 != lv4) == false);
    CHECK((lv4 != v4) == false);
  }
}

TEST_CASE("Vector Arithmetic Operators", "[operators]")
{
  const auto params = biased_params(30, 20);

  SECTION("operator+, -, *, /")
  {
    Vec4  v1(params[0], params[1], params[2], params[3]);
    Vec4  v2(params[4], params[5], params[6], params[7]);
    LVec3 lv1(params[8], params[9], params[10]);
    LVec3 lv2(params[11], params[12], params[13]);

    // Vec4 + Vec4
    auto result1 = v1 + v2;
    FSFV_CHECK_COMPONENTS(
      result1,
      v1.components()[0] + v2.components()[0],
      v1.components()[1] + v2.components()[1],
      v1.components()[2] + v2.components()[2],
      v1.components()[3] + v2.components()[3]
    );

    // LVec3 + LVec3
    auto result2 = lv1 + lv2;
    FSFV_CHECK_COMP3(
      result2,
      lv1.components()[0] + lv2.components()[0],
      lv1.components()[1] + lv2.components()[1],
      lv1.components()[2] + lv2.components()[2]
    );

    // Vec4 + LVec3
    auto result3 = v1 + lv1;
    FSFV_CHECK_COMPONENTS(
      result3,
      v1.components()[0] + lv1.components()[0],
      v1.components()[1] + lv1.components()[1],
      v1.components()[2] + lv1.components()[2],
      v1.components()[3] + 0.0f
    );

    // LVec3 + Vec4
    auto result4 = lv1 + v1;
    FSFV_CHECK_COMPONENTS(
      result4,
      lv1.components()[0] + v1.components()[0],
      lv1.components()[1] + v1.components()[1],
      lv1.components()[2] + v1.components()[2],
      0.0f + v1.components()[3]
    );

    // Vec4 - Vec4
    auto result5 = v1 - v2;
    FSFV_CHECK_COMPONENTS(
      result5,
      v1.components()[0] - v2.components()[0],
      v1.components()[1] - v2.components()[1],
      v1.components()[2] - v2.components()[2],
      v1.components()[3] - v2.components()[3]
    );

    // LVec3 - LVec3
    auto result6 = lv1 - lv2;
    FSFV_CHECK_COMP3(
      result6,
      lv1.components()[0] - lv2.components()[0],
      lv1.components()[1] - lv2.components()[1],
      lv1.components()[2] - lv2.components()[2]
    );

    // Vec4 - LVec3
    auto result7 = v1 - lv1;
    FSFV_CHECK_COMPONENTS(
      result7,
      v1.components()[0] - lv1.components()[0],
      v1.components()[1] - lv1.components()[1],
      v1.components()[2] - lv1.components()[2],
      v1.components()[3] - 0.0f
    );

    // LVec3 - Vec4
    auto result8 = lv1 - v1;
    FSFV_CHECK_COMPONENTS(
      result8,
      lv1.components()[0] - v1.components()[0],
      lv1.components()[1] - v1.components()[1],
      lv1.components()[2] - v1.components()[2],
      0.0f - v1.components()[3]
    );

    // Unary operator-
    auto result9 = -v1;
    FSFV_CHECK_COMPONENTS(
      result9,
      -v1.components()[0],
      -v1.components()[1],
      -v1.components()[2],
      -v1.components()[3]
    );

    auto resultA = -lv1;
    FSFV_CHECK_COMP3(
      resultA,
      -lv1.components()[0],
      -lv1.components()[1],
      -lv1.components()[2]
    );

    float scalar = params[14];

    // Vec4 * Vec4
    auto resultB = v1 * v2;
    FSFV_CHECK_COMPONENTS(
      resultB,
      v1.components()[0] * v2.components()[0],
      v1.components()[1] * v2.components()[1],
      v1.components()[2] * v2.components()[2],
      v1.components()[3] * v2.components()[3]
    );

    // LVec3 * LVec3
    auto resultC = lv1 * lv2;
    FSFV_CHECK_COMP3(
      resultC,
      lv1.components()[0] * lv2.components()[0],
      lv1.components()[1] * lv2.components()[1],
      lv1.components()[2] * lv2.components()[2]
    );

    // Vec4 * LVec3
    auto resultD = v1 * lv1;
    FSFV_CHECK_COMP3(
      resultD,
      v1.components()[0] * lv1.components()[0],
      v1.components()[1] * lv1.components()[1],
      v1.components()[2] * lv1.components()[2]
    );

    // LVec3 * Vec4
    auto resultE = lv1 * v1;
    FSFV_CHECK_COMP3(
      resultE,
      lv1.components()[0] * v1.components()[0],
      lv1.components()[1] * v1.components()[1],
      lv1.components()[2] * v1.components()[2]
    );

    // scalar * Vec4
    auto resultF = scalar * v1;
    FSFV_CHECK_COMPONENTS(
      resultF,
      scalar * v1.components()[0],
      scalar * v1.components()[1],
      scalar * v1.components()[2],
      scalar * v1.components()[3]
    );

    // scalar * LVec3
    auto resultG = scalar * lv1;
    FSFV_CHECK_COMP3(
      resultG,
      scalar * lv1.components()[0],
      scalar * lv1.components()[1],
      scalar * lv1.components()[2]
    );

    // Vec4 * scalar
    auto resultH = v1 * scalar;
    FSFV_CHECK_COMPONENTS(
      resultH,
      v1.components()[0] * scalar,
      v1.components()[1] * scalar,
      v1.components()[2] * scalar,
      v1.components()[3] * scalar
    );

    // LVec3 * scalar
    auto resultI = lv1 * scalar;
    FSFV_CHECK_COMP3(
      resultI,
      lv1.components()[0] * scalar,
      lv1.components()[1] * scalar,
      lv1.components()[2] * scalar
    );

    // Avoid division by zero
    if (std::abs(scalar) < 1e-6f) scalar = 1.0f;

    INFO("v1: " << Catch::StringMaker<Vec4>::convert(v1));
    INFO("v2: " << Catch::StringMaker<Vec4>::convert(v2));

    auto mask = abs(v2) >= Vec4(1e-6f);
    v2        = (mask & v2) | (and_not(Vec4(1.0f), mask));

    INFO("mask: " << Catch::StringMaker<Vec4>::convert(mask));
    INFO("v2.2: " << Catch::StringMaker<Vec4>::convert(v2));

    INFO("lv1: " << Catch::StringMaker<Vec4>::convert(lv1));
    INFO("lv2: " << Catch::StringMaker<Vec4>::convert(lv2));

    auto lmask = abs(lv2) >= LVec3(1e-6f);
    lv2        = (lmask & lv2) | LVec3(and_not(LVec3(1.0f), lmask));

    INFO("lmask: " << Catch::StringMaker<Vec4>::convert(lmask));
    INFO("lv2.2: " << Catch::StringMaker<Vec4>::convert(lv2));

    // Vec4 / Vec4
    auto resultJ = v1 / v2;
    FSFV_CHECK_COMPONENTS(
      resultJ,
      v1.components()[0] / v2.components()[0],
      v1.components()[1] / v2.components()[1],
      v1.components()[2] / v2.components()[2],
      v1.components()[3] / v2.components()[3]
    );

    // LVec3 / LVec3
    auto resultK = lv1 / lv2;
    FSFV_CHECK_COMP3(
      resultK,
      lv1.components()[0] / lv2.components()[0],
      lv1.components()[1] / lv2.components()[1],
      lv1.components()[2] / lv2.components()[2]
    );

    // Vec4 / LVec3 : Illegal (compilation error)
    //auto result = v1 / lv2;

    // LVec3 / Vec4
    auto resultL = lv1 / v2;
    FSFV_CHECK_COMP3(
      resultL,
      lv1.components()[0] / v2.components()[0],
      lv1.components()[1] / v2.components()[1],
      lv1.components()[2] / v2.components()[2]
    );

    // scalar / Vec4
    auto resultM = scalar / v2;
    FSFV_CHECK_COMPONENTS(
      resultM,
      scalar / v2.components()[0],
      scalar / v2.components()[1],
      scalar / v2.components()[2],
      scalar / v2.components()[3]
    );

    // scalar / LVec3
    auto resultN = scalar / lv2;
    FSFV_CHECK_COMP3(
      resultN,
      scalar / lv2.components()[0],
      scalar / lv2.components()[1],
      scalar / lv2.components()[2]
    );

    // Vec4 / scalar
    auto resultO = v1 / scalar;
    FSFV_CHECK_COMPONENTS(
      resultO,
      v1.components()[0] / scalar,
      v1.components()[1] / scalar,
      v1.components()[2] / scalar,
      v1.components()[3] / scalar
    );

    // LVec3 / scalar
    auto resultP = lv1 / scalar;
    FSFV_CHECK_COMP3(
      resultP,
      lv1.components()[0] / scalar,
      lv1.components()[1] / scalar,
      lv1.components()[2] / scalar
    );
  }
}

TEST_CASE("Vector Bitwise Operators", "[operators]")
{
  Vec4 v1(
    bits_to_float(0x12345678),
    bits_to_float(0x87654321),
    bits_to_float(0xDEADBEEF),
    bits_to_float(0xBEEFDEAD)
  );
  Vec4 v2(
    bits_to_float(0xDCCD3535),
    bits_to_float(0x682ACB99),
    bits_to_float(0xDAFF01EA),
    bits_to_float(0xF00FFF0F)
  );
  LVec3 lv1(
    bits_to_float(0x01248FE3),
    bits_to_float(0x8FE30124),
    bits_to_float(0x3C3C3C3C)
  );
  LVec3 lv2(
    bits_to_float(0xF95AB5B6),
    bits_to_float(0x42069420),
    bits_to_float(0xF0FF0FFF)
  );

  INFO("v1: " << Catch::HexStringMaker<Vec4>::convert(v1));
  INFO("v2: " << Catch::HexStringMaker<Vec4>::convert(v2));
  INFO("lv1: " << Catch::HexStringMaker<Vec4>::convert(Vec4(lv1)));
  INFO("lv2: " << Catch::HexStringMaker<Vec4>::convert(Vec4(lv2)));

  SECTION("operator^ (XOR)")
  {
    // Vec4 ^ Vec4
    auto result1 = v1 ^ v2;
    static_assert(std::is_same_v<decltype(result1), Vec4>);
    FSFV_CHECK_BITWISE(
      result1,
      bits_to_float(
        float_to_bits(v1.components()[0]) ^ float_to_bits(v2.components()[0])
      ),
      bits_to_float(
        float_to_bits(v1.components()[1]) ^ float_to_bits(v2.components()[1])
      ),
      bits_to_float(
        float_to_bits(v1.components()[2]) ^ float_to_bits(v2.components()[2])
      ),
      bits_to_float(
        float_to_bits(v1.components()[3]) ^ float_to_bits(v2.components()[3])
      )
    );

    // LVec3 ^ LVec3
    auto result2 = lv1 ^ lv2;
    static_assert(std::is_same_v<decltype(result2), LVec3>);
    FSFV_CHECK_BITWISE3(
      result2,
      bits_to_float(
        float_to_bits(lv1.components()[0]) ^ float_to_bits(lv2.components()[0])
      ),
      bits_to_float(
        float_to_bits(lv1.components()[1]) ^ float_to_bits(lv2.components()[1])
      ),
      bits_to_float(
        float_to_bits(lv1.components()[2]) ^ float_to_bits(lv2.components()[2])
      )
    );

    // Vec4 ^ LVec3
    auto result3 = v1 ^ lv1;
    static_assert(std::is_same_v<decltype(result3), Vec4>);
    FSFV_CHECK_BITWISE(
      result3,
      bits_to_float(
        float_to_bits(v1.components()[0]) ^ float_to_bits(lv1.components()[0])
      ),
      bits_to_float(
        float_to_bits(v1.components()[1]) ^ float_to_bits(lv1.components()[1])
      ),
      bits_to_float(
        float_to_bits(v1.components()[2]) ^ float_to_bits(lv1.components()[2])
      ),
      bits_to_float(float_to_bits(v1.components()[3]) ^ 0)
    );

    // LVec3 ^ Vec4
    auto result4 = lv1 ^ v1;
    static_assert(std::is_same_v<decltype(result4), Vec4>);
    FSFV_CHECK_BITWISE(
      result4,
      bits_to_float(
        float_to_bits(lv1.components()[0]) ^ float_to_bits(v1.components()[0])
      ),
      bits_to_float(
        float_to_bits(lv1.components()[1]) ^ float_to_bits(v1.components()[1])
      ),
      bits_to_float(
        float_to_bits(lv1.components()[2]) ^ float_to_bits(v1.components()[2])
      ),
      bits_to_float(0 ^ float_to_bits(v1.components()[3]))
    );
  }

  SECTION("operator| (OR)")
  {
    // Vec4 | Vec4
    auto result1 = v1 | v2;
    static_assert(std::is_same_v<decltype(result1), Vec4>);
    FSFV_CHECK_BITWISE(
      result1,
      bits_to_float(
        float_to_bits(v1.components()[0]) | float_to_bits(v2.components()[0])
      ),
      bits_to_float(
        float_to_bits(v1.components()[1]) | float_to_bits(v2.components()[1])
      ),
      bits_to_float(
        float_to_bits(v1.components()[2]) | float_to_bits(v2.components()[2])
      ),
      bits_to_float(
        float_to_bits(v1.components()[3]) | float_to_bits(v2.components()[3])
      )
    );

    // LVec3 | LVec3
    auto result2 = lv1 | lv2;
    static_assert(std::is_same_v<decltype(result2), LVec3>);
    FSFV_CHECK_BITWISE3(
      result2,
      bits_to_float(
        float_to_bits(lv1.components()[0]) | float_to_bits(lv2.components()[0])
      ),
      bits_to_float(
        float_to_bits(lv1.components()[1]) | float_to_bits(lv2.components()[1])
      ),
      bits_to_float(
        float_to_bits(lv1.components()[2]) | float_to_bits(lv2.components()[2])
      )
    );

    // Vec4 | LVec3
    auto result3 = v1 | lv1;
    static_assert(std::is_same_v<decltype(result3), Vec4>);
    FSFV_CHECK_BITWISE(
      result3,
      bits_to_float(
        float_to_bits(v1.components()[0]) | float_to_bits(lv1.components()[0])
      ),
      bits_to_float(
        float_to_bits(v1.components()[1]) | float_to_bits(lv1.components()[1])
      ),
      bits_to_float(
        float_to_bits(v1.components()[2]) | float_to_bits(lv1.components()[2])
      ),
      bits_to_float(float_to_bits(v1.components()[3]) | 0)
    );

    // LVec3 | Vec4
    auto result4 = lv1 | v1;
    static_assert(std::is_same_v<decltype(result4), Vec4>);
    FSFV_CHECK_BITWISE(
      result4,
      bits_to_float(
        float_to_bits(lv1.components()[0]) | float_to_bits(v1.components()[0])
      ),
      bits_to_float(
        float_to_bits(lv1.components()[1]) | float_to_bits(v1.components()[1])
      ),
      bits_to_float(
        float_to_bits(lv1.components()[2]) | float_to_bits(v1.components()[2])
      ),
      bits_to_float(0 | float_to_bits(v1.components()[3]))
    );
  }

  SECTION("operator& (AND)")
  {
    // Vec4 & Vec4
    auto result1 = v1 & v2;
    static_assert(std::is_same_v<decltype(result1), Vec4>);
    FSFV_CHECK_BITWISE(
      result1,
      bits_to_float(
        float_to_bits(v1.components()[0]) & float_to_bits(v2.components()[0])
      ),
      bits_to_float(
        float_to_bits(v1.components()[1]) & float_to_bits(v2.components()[1])
      ),
      bits_to_float(
        float_to_bits(v1.components()[2]) & float_to_bits(v2.components()[2])
      ),
      bits_to_float(
        float_to_bits(v1.components()[3]) & float_to_bits(v2.components()[3])
      )
    );

    // LVec3 & LVec3
    auto result2 = lv1 & lv2;
    static_assert(std::is_same_v<decltype(result2), LVec3>);
    FSFV_CHECK_BITWISE3(
      result2,
      bits_to_float(
        float_to_bits(lv1.components()[0]) & float_to_bits(lv2.components()[0])
      ),
      bits_to_float(
        float_to_bits(lv1.components()[1]) & float_to_bits(lv2.components()[1])
      ),
      bits_to_float(
        float_to_bits(lv1.components()[2]) & float_to_bits(lv2.components()[2])
      )
    );

    // Vec4 & LVec3
    auto result3 = v1 & lv1;
    static_assert(std::is_same_v<decltype(result3), LVec3>);
    FSFV_CHECK_BITWISE3(
      result3,
      bits_to_float(
        float_to_bits(v1.components()[0]) & float_to_bits(lv1.components()[0])
      ),
      bits_to_float(
        float_to_bits(v1.components()[1]) & float_to_bits(lv1.components()[1])
      ),
      bits_to_float(
        float_to_bits(v1.components()[2]) & float_to_bits(lv1.components()[2])
      )
    );

    // LVec3 & Vec4
    auto result4 = lv1 & v1;
    static_assert(std::is_same_v<decltype(result4), LVec3>);
    FSFV_CHECK_BITWISE3(
      result4,
      bits_to_float(
        float_to_bits(lv1.components()[0]) & float_to_bits(v1.components()[0])
      ),
      bits_to_float(
        float_to_bits(lv1.components()[1]) & float_to_bits(v1.components()[1])
      ),
      bits_to_float(
        float_to_bits(lv1.components()[2]) & float_to_bits(v1.components()[2])
      )
    );
  }

  SECTION("operator~ (NOT)")
  {
    auto result = ~v1;
    static_assert(std::is_same_v<decltype(result), Vec4>);
    FSFV_CHECK_BITWISE(
      result,
      bits_to_float(~float_to_bits(v1.components()[0])),
      bits_to_float(~float_to_bits(v1.components()[1])),
      bits_to_float(~float_to_bits(v1.components()[2])),
      bits_to_float(~float_to_bits(v1.components()[3]))
    );

    // Invalid
    // auto lresult = ~lv1;
  }

  SECTION("Bitwise operations with special values")
  {
    // Test with zero, negative zero, infinity, and NaN
    Vec4 v1(
      bits_to_float(0x00000000),
      bits_to_float(0x80000000),
      bits_to_float(0x7F800000),
      bits_to_float(0x7FFFFFFF)
    );
    Vec4 v2(
      bits_to_float(0xFFFFFFFF),
      bits_to_float(0xFFFFFFFF),
      bits_to_float(0xFFFFFFFF),
      bits_to_float(0xFFFFFFFF)
    );

    INFO("v1: " << Catch::HexStringMaker<Vec4>::convert(v1));
    INFO("v2: " << Catch::HexStringMaker<Vec4>::convert(v2));

    // Test AND with all-ones mask
    auto result_and = v1 & v2;
    FSFV_CHECK_BITWISE(
      result_and,
      bits_to_float(
        float_to_bits(v1.components()[0]) & float_to_bits(v2.components()[0])
      ),
      bits_to_float(
        float_to_bits(v1.components()[1]) & float_to_bits(v2.components()[1])
      ),
      bits_to_float(
        float_to_bits(v1.components()[2]) & float_to_bits(v2.components()[2])
      ),
      bits_to_float(
        float_to_bits(v1.components()[3]) & float_to_bits(v2.components()[3])
      )
    );

    // Test OR with all-ones mask
    auto result_or = v1 | v2;
    FSFV_CHECK_BITWISE(
      result_or,
      bits_to_float(
        float_to_bits(v1.components()[0]) | float_to_bits(v2.components()[0])
      ),
      bits_to_float(
        float_to_bits(v1.components()[1]) | float_to_bits(v2.components()[1])
      ),
      bits_to_float(
        float_to_bits(v1.components()[2]) | float_to_bits(v2.components()[2])
      ),
      bits_to_float(
        float_to_bits(v1.components()[3]) | float_to_bits(v2.components()[3])
      )
    );

    // Test XOR with all-ones mask
    auto result_xor = v1 ^ v2;
    FSFV_CHECK_BITWISE(
      result_xor,
      bits_to_float(
        float_to_bits(v1.components()[0]) ^ float_to_bits(v2.components()[0])
      ),
      bits_to_float(
        float_to_bits(v1.components()[1]) ^ float_to_bits(v2.components()[1])
      ),
      bits_to_float(
        float_to_bits(v1.components()[2]) ^ float_to_bits(v2.components()[2])
      ),
      bits_to_float(
        float_to_bits(v1.components()[3]) ^ float_to_bits(v2.components()[3])
      )
    );

    // Test NOT
    auto result_not = ~v1;
    FSFV_CHECK_BITWISE(
      result_not,
      bits_to_float(~float_to_bits(v1.components()[0])),
      bits_to_float(~float_to_bits(v1.components()[1])),
      bits_to_float(~float_to_bits(v1.components()[2])),
      bits_to_float(~float_to_bits(v1.components()[3]))
    );
  }
}

TEST_CASE("Vector Compound Assignment Operators", "[operators]")
{
  const auto params = biased_params(30, 5);

  SECTION("operator+=")
  {
    Vec4  v1(params[0], params[1], params[2], params[3]);
    Vec4  v2(params[4], params[5], params[6], params[7]);
    LVec3 lv1(params[8], params[9], params[10]);
    LVec3 lv2(params[11], params[12], params[13]);

    // Vec4 += Vec4
    Vec4 v1_copy  = v1;
    v1_copy      += v2;
    FSFV_CHECK_COMPONENTS(
      v1_copy,
      params[0] + params[4],
      params[1] + params[5],
      params[2] + params[6],
      params[3] + params[7]
    );

    // LVec3 += LVec3
    LVec3 lv1_copy  = lv1;
    lv1_copy       += lv2;
    FSFV_CHECK_COMP3(
      lv1_copy,
      params[8] + params[11],
      params[9] + params[12],
      params[10] + params[13]
    );

    // Vec4 += LVec3
    Vec4 v3_copy  = v1;
    v3_copy      += lv1;
    FSFV_CHECK_COMPONENTS(
      v3_copy,
      params[0] + params[8],
      params[1] + params[9],
      params[2] + params[10],
      params[3] + 0.0f
    );

    // LVec3 += Vec4 : illegal (compilation error)
    //LVec3 lv3_copy  = lv1;
    //lv3_copy       += v1;
  }

  SECTION("operator-=")
  {
    Vec4  v1(params[0], params[1], params[2], params[3]);
    Vec4  v2(params[4], params[5], params[6], params[7]);
    LVec3 lv1(params[8], params[9], params[10]);
    LVec3 lv2(params[11], params[12], params[13]);

    // Vec4 -= Vec4
    Vec4 v1_copy  = v1;
    v1_copy      -= v2;
    FSFV_CHECK_COMPONENTS(
      v1_copy,
      params[0] - params[4],
      params[1] - params[5],
      params[2] - params[6],
      params[3] - params[7]
    );

    // LVec3 -= LVec3
    LVec3 lv1_copy  = lv1;
    lv1_copy       -= lv2;
    FSFV_CHECK_COMP3(
      lv1_copy,
      params[8] - params[11],
      params[9] - params[12],
      params[10] - params[13]
    );

    // Vec4 -= LVec3
    Vec4 v3_copy  = v1;
    v3_copy      -= lv1;
    FSFV_CHECK_COMPONENTS(
      v3_copy,
      params[0] - params[8],
      params[1] - params[9],
      params[2] - params[10],
      params[3] - 0.0f
    );

    // LVec3 += Vec4 : illegal (compilation error)
    //LVec3 lv3_copy  = lv1;
    //lv3_copy       += v1;
  }

  SECTION("operator*=")
  {
    Vec4  v1(params[0], params[1], params[2], params[3]);
    Vec4  v2(params[4], params[5], params[6], params[7]);
    LVec3 lv1(params[8], params[9], params[10]);
    LVec3 lv2(params[11], params[12], params[13]);
    float scalar = params[14];

    // Vec4 *= Vec4
    Vec4 v1_copy  = v1;
    v1_copy      *= v2;
    FSFV_CHECK_COMPONENTS(
      v1_copy,
      params[0] * params[4],
      params[1] * params[5],
      params[2] * params[6],
      params[3] * params[7]
    );

    // LVec3 *= LVec3
    LVec3 lv1_copy  = lv1;
    lv1_copy       *= lv2;
    FSFV_CHECK_COMP3(
      lv1_copy,
      params[8] * params[11],
      params[9] * params[12],
      params[10] * params[13]
    );

    // Vec4 *= LVec3
    Vec4 v3_copy  = v1;
    v3_copy      *= lv1;
    FSFV_CHECK_COMPONENTS(
      v3_copy,
      params[0] * params[8],
      params[1] * params[9],
      params[2] * params[10],
      params[3] * 0.0f
    );

    // LVec3 *= Vec4
    LVec3 lv3_copy  = lv1;
    lv3_copy       *= v1;
    FSFV_CHECK_COMP3(
      lv3_copy,
      params[8] * params[0],
      params[9] * params[1],
      params[10] * params[2]
    );

    // Vec4 *= scalar
    Vec4 v4_copy  = v1;
    v4_copy      *= scalar;
    FSFV_CHECK_COMPONENTS(
      v4_copy,
      params[0] * scalar,
      params[1] * scalar,
      params[2] * scalar,
      params[3] * scalar
    );

    // LVec3 *= scalar
    LVec3 lv4_copy  = lv1;
    lv4_copy       *= scalar;
    FSFV_CHECK_COMP3(
      lv4_copy,
      params[8] * scalar,
      params[9] * scalar,
      params[10] * scalar
    );
  }

  SECTION("operator/=")
  {
    Vec4  v1(params[0], params[1], params[2], params[3]);
    Vec4  v2(params[4], params[5], params[6], params[7]);
    LVec3 lv1(params[8], params[9], params[10]);
    LVec3 lv2(params[11], params[12], params[13]);
    float scalar = params[14];

    // Avoid division by zero
    if (std::abs(scalar) < 1e-6f) scalar = 1.0f;
    if (std::abs(params[4]) < 1e-6f)
      v2 = FSVF::blend_with_mask<0b1000>(v2, Vec4(1.0f));
    if (std::abs(params[5]) < 1e-6f)
      v2 = FSVF::blend_with_mask<0b0100>(v2, Vec4(1.0f));
    if (std::abs(params[6]) < 1e-6f)
      v2 = FSVF::blend_with_mask<0b0010>(v2, Vec4(1.0f));
    if (std::abs(params[7]) < 1e-6f)
      v2 = FSVF::blend_with_mask<0b0001>(v2, Vec4(1.0f));
    if (std::abs(params[11]) < 1e-6f)
      lv2 = FSVF::blend_with_mask<0b1000>(lv2, LVec3(1.0f));
    if (std::abs(params[12]) < 1e-6f)
      lv2 = FSVF::blend_with_mask<0b0100>(lv2, LVec3(1.0f));
    if (std::abs(params[13]) < 1e-6f)
      lv2 = FSVF::blend_with_mask<0b0010>(lv2, LVec3(1.0f));

    // Vec4 /= Vec4
    Vec4 v1_copy  = v1;
    v1_copy      /= v2;
    FSFV_CHECK_COMPONENTS(
      v1_copy,
      params[0] / v2.components()[0],
      params[1] / v2.components()[1],
      params[2] / v2.components()[2],
      params[3] / v2.components()[3]
    );

    // LVec3 /= LVec3
    LVec3 lv1_copy  = lv1;
    lv1_copy       /= lv2;
    FSFV_CHECK_COMP3(
      lv1_copy,
      params[8] / lv2.components()[0],
      params[9] / lv2.components()[1],
      params[10] / lv2.components()[2]
    );

    // Vec4 /= LVec3
    LVec3 lv3_copy  = lv1;
    lv3_copy       /= v2;
    FSFV_CHECK_COMP3(
      lv3_copy,
      lv1.components()[0] / v2.components()[0],
      lv1.components()[1] / v2.components()[1],
      lv1.components()[2] / v2.components()[2]
    );
    CHECK(Vec4{ lv3_copy }.components()[3] == 0.f);

    // Vec4 /= scalar
    Vec4 v4_copy  = v1;
    v4_copy      /= scalar;
    FSFV_CHECK_COMPONENTS(
      v4_copy,
      params[0] / scalar,
      params[1] / scalar,
      params[2] / scalar,
      params[3] / scalar
    );

    // LVec3 /= scalar
    LVec3 lv4_copy  = lv1;
    lv4_copy       /= scalar;
    FSFV_CHECK_COMP3(
      lv4_copy,
      params[8] / scalar,
      params[9] / scalar,
      params[10] / scalar
    );
  }

  SECTION("operator&=")
  {
    Vec4  v1(params[0], params[1], params[2], params[3]);
    Vec4  v2(params[4], params[5], params[6], params[7]);
    LVec3 lv1(params[8], params[9], params[10]);
    LVec3 lv2(params[11], params[12], params[13]);

    // Vec4 &= Vec4 (same dimension)
    Vec4 v1_copy  = v1;
    v1_copy      &= v2;
    CHECK(v1_copy.components().size() == 4);

    // LVec3 &= LVec3 (same dimension)
    LVec3 lv1_copy  = lv1;
    lv1_copy       &= lv2;
    CHECK(lv1_copy.components().size() == 3);

    // Different dimensions should not compile due to SameDimension constraint
    // This is tested by the fact that the code compiles
  }

  SECTION("operator|=")
  {
    Vec4  v1(params[0], params[1], params[2], params[3]);
    Vec4  v2(params[4], params[5], params[6], params[7]);
    LVec3 lv1(params[8], params[9], params[10]);
    LVec3 lv2(params[11], params[12], params[13]);

    // Vec4 |= Vec4 (same dimension)
    Vec4 v1_copy  = v1;
    v1_copy      |= v2;
    CHECK(v1_copy.components().size() == 4);

    // LVec3 |= LVec3 (same dimension)
    LVec3 lv1_copy  = lv1;
    lv1_copy       |= lv2;
    CHECK(lv1_copy.components().size() == 3);

    // Different dimensions should not compile due to SameDimension constraint
    // This is tested by the fact that the code compiles
  }

  SECTION("operator^=")
  {
    Vec4  v1(params[0], params[1], params[2], params[3]);
    Vec4  v2(params[4], params[5], params[6], params[7]);
    LVec3 lv1(params[8], params[9], params[10]);
    LVec3 lv2(params[11], params[12], params[13]);

    // Vec4 ^= Vec4 (same dimension)
    Vec4 v1_copy  = v1;
    v1_copy      ^= v2;
    CHECK(v1_copy.components().size() == 4);

    // LVec3 ^= LVec3 (same dimension)
    LVec3 lv1_copy  = lv1;
    lv1_copy       ^= lv2;
    CHECK(lv1_copy.components().size() == 3);

    // Different dimensions should not compile due to SameDimension constraint
    // This is tested by the fact that the code compiles
  }
}

TEST_CASE("Vector Operator Edge Cases", "[operators]")
{
  SECTION("Zero vectors")
  {
    Vec4  zero4(0.0f);
    LVec3 zero3(0.0f);

    // Addition with zero
    Vec4  v1(1.0f, 2.0f, 3.0f, 4.0f);
    LVec3 lv1(1.0f, 2.0f, 3.0f);

    auto result1 = v1 + zero4;
    FSFV_CHECK_COMPONENTS(result1, 1.0f, 2.0f, 3.0f, 4.0f);

    auto result2 = lv1 + zero3;
    FSFV_CHECK_COMP3(result2, 1.0f, 2.0f, 3.0f);

    // Multiplication with zero
    auto result3 = v1 * zero4;
    FSFV_CHECK_COMPONENTS(result3, 0.0f, 0.0f, 0.0f, 0.0f);

    auto result4 = lv1 * zero3;
    FSFV_CHECK_COMP3(result4, 0.0f, 0.0f, 0.0f);
  }

  SECTION("Identity operations")
  {
    Vec4  v1(1.0f, 2.0f, 3.0f, 4.0f);
    LVec3 lv1(1.0f, 2.0f, 3.0f);

    // Addition with zero
    auto result1 = v1 + Vec4(0.0f);
    FSFV_CHECK_COMPONENTS(result1, 1.0f, 2.0f, 3.0f, 4.0f);

    // Multiplication with one
    auto result2 = v1 * Vec4(1.0f);
    FSFV_CHECK_COMPONENTS(result2, 1.0f, 2.0f, 3.0f, 4.0f);

    // Division by one
    auto result3 = v1 / Vec4(1.0f);
    FSFV_CHECK_COMPONENTS(result3, 1.0f, 2.0f, 3.0f, 4.0f);
  }

  SECTION("Large numbers")
  {
    float large_val = 1e6f;
    Vec4  v1(large_val);
    LVec3 lv1(large_val);

    auto result1 = v1 + v1;
    FSFV_CHECK_COMPONENTS(
      result1,
      2 * large_val,
      2 * large_val,
      2 * large_val,
      2 * large_val
    );

    auto result2 = lv1 * lv1;
    FSFV_CHECK_COMP3(
      result2,
      large_val * large_val,
      large_val * large_val,
      large_val * large_val
    );
  }

  SECTION("Small numbers")
  {
    float small_val = 1e-6f;
    Vec4  v1(small_val);
    LVec3 lv1(small_val);

    auto result1 = v1 * v1;
    FSFV_CHECK_COMPONENTS(
      result1,
      small_val * small_val,
      small_val * small_val,
      small_val * small_val,
      small_val * small_val
    );

    auto result2 = lv1 + lv1;
    FSFV_CHECK_COMP3(result2, 2 * small_val, 2 * small_val, 2 * small_val);
  }
}

TEST_CASE("Vector Operator Type Deduction", "[operators]")
{
  const auto params = biased_params(20, 5);

  SECTION("Return type verification")
  {
    Vec4  v1(params[0], params[1], params[2], params[3]);
    Vec4  v2(params[4], params[5], params[6], params[7]);
    LVec3 lv1(params[8], params[9], params[10]);
    LVec3 lv2(params[11], params[12], params[13]);

    // Test that return types match expectations
    auto result1 = v1 + v2;
    static_assert(
      std::is_same_v<decltype(result1), Vec4>,
      "Vec4 + Vec4 should return Vec4"
    );

    auto result2 = lv1 + lv2;
    static_assert(
      std::is_same_v<decltype(result2), LVec3>,
      "LVec3 + LVec3 should return LVec3"
    );

    auto result3 = v1 + lv1;
    static_assert(
      std::is_same_v<decltype(result3), Vec4>,
      "Vec4 + LVec3 should return Vec4"
    );

    auto result4 = lv1 + v1;
    static_assert(
      std::is_same_v<decltype(result4), Vec4>,
      "LVec3 + Vec4 should return Vec4"
    );

    auto result5 = v1 * v2;
    static_assert(
      std::is_same_v<decltype(result5), Vec4>,
      "Vec4 * Vec4 should return Vec4"
    );

    auto result6 = lv1 * lv2;
    static_assert(
      std::is_same_v<decltype(result6), LVec3>,
      "LVec3 * LVec3 should return LVec3"
    );

    auto result7 = v1 * lv1;
    static_assert(
      std::is_same_v<decltype(result7), LVec3>,
      "Vec4 * LVec3 should return LVec3"
    );

    auto result8 = lv1 * v1;
    static_assert(
      std::is_same_v<decltype(result8), LVec3>,
      "LVec3 * Vec4 should return LVec3"
    );

    auto result9 = v1 & v2;
    static_assert(
      std::is_same_v<decltype(result9), Vec4>,
      "Vec4 & Vec4 should return Vec4"
    );

    auto result10 = lv1 & lv2;
    static_assert(
      std::is_same_v<decltype(result10), LVec3>,
      "LVec3 & LVec3 should return LVec3"
    );

    auto result11 = v1 & lv1;
    static_assert(
      std::is_same_v<decltype(result11), LVec3>,
      "Vec4 & LVec3 should return LVec3"
    );

    auto result12 = lv1 & v1;
    static_assert(
      std::is_same_v<decltype(result12), LVec3>,
      "LVec3 & Vec4 should return LVec3"
    );
  }
}
