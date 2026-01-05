#include "TestUtils.h"

using namespace FSVF;

static constexpr float smallest_positive_denormal = bits_to_float(0x1u);
static constexpr float highest_denormal = bits_to_float(0x1F'FFFFu);

static constexpr float normal_full_mantissa = bits_to_float(0x3E7F'FFFFu);
static constexpr float normal_mantissa_zero = bits_to_float(0x3E7F'FFFFu);
static constexpr float normal_mantissa_one = bits_to_float(0x3E7F'FFFFu);

static_assert(is_finite(normal_full_mantissa));
static_assert(is_finite(normal_mantissa_zero));
static_assert(is_finite(normal_mantissa_one));
static_assert(!is_inf_or_nan(normal_full_mantissa));
static_assert(!is_inf_or_nan(normal_mantissa_zero));
static_assert(!is_inf_or_nan(normal_mantissa_one));
static_assert(!is_denormal(normal_full_mantissa));
static_assert(!is_denormal(normal_mantissa_zero));
static_assert(!is_denormal(normal_mantissa_one));

// Test across many orders of magnitude
static const std::vector<float> float_test_values = {
  1e-5f, 1e-4f, 1e-3f,
  0.0f, 0.1f, 0.5f, 0.9f,
  normal_full_mantissa, normal_mantissa_zero, normal_mantissa_one,
  1.0f, 2.0f, 5.0f, 10.0f,
  123.0f, 4567.0f, 78'877.0f, 420'666.0f, 2'112'211.0f,
  3.4567e10f, 6.8765e20f, 9.1029e30f, 2.2837e38f, bits_to_float(0x3FFF'FFF0u)
  -1e-5f, -1e-4f, -1e-3f,
  -0.0f, -0.1f, -0.5f, -0.9f,
  -normal_full_mantissa, -normal_mantissa_zero, -normal_mantissa_one,
  -1.0f, -2.0f, -5.0f, -10.0f,
  -123.0f, -4567.0f, -78'877.0f, -420'666.0f, -2'112'211.0f, -bits_to_float(0x3FFF'FFF0u)
  -3.4567e10f, -6.8765e20f, -9.1029e30f, -2.2837e38f,
};

// Denormals can be weird, just test for all of them (~ 17M values)
static const std::vector<float> float_denormal_test_values = [] -> std::vector<float> {
  std::vector<float> res{};
  res.reserve(17'000'000);
  for (uint32_t i = 1; i <= 0x7F'FFFF; ++i) {
    res.push_back(bits_to_float(i));
    res.push_back(bits_to_float(i | f32_sign_mask));
  }
  return res;
}();

TEST_CASE("FloatUtils - Bit manipulation functions", "[float_utils]")
{

  SECTION("Float - bit conversion + is_inf/nan/finite/denormal")
  {
    CHECK(f32_sign_mask == float_to_bits(-0.0f));

    // Ordinary values
    for (auto x : float_test_values) {
      CHECK(is_finite(x));
      CHECK(!is_denormal(x));
      CHECK(!is_inf(x));
      CHECK(!is_nan(x));
      CHECK(!is_inf_or_nan(x));

      const auto result_bits  = float_to_bits(x);
      const float test_val = bits_to_float(result_bits);

      CHECK(test_val == x);
    }
    {
      const uint32_t test_bits   = 0x4049'0FDB;
      const float    f           = bits_to_float(test_bits);
      const uint32_t result_bits = float_to_bits(f);
      CHECK(result_bits == test_bits);
    }

    // Denormals
    int i = -1;
    for (auto x : float_denormal_test_values) {
      ++i;
      INFO("i = " << i);
      INFO("x = " << float_to_bits(x));
      INFO("x = 0x" << std::hex << float_to_bits(x));
#ifdef FSVF_FLOAT_DENORMAL_MATH
      CHECK(is_finite(x));
      CHECK(is_denormal(x));
      CHECK(!is_inf(x));
      CHECK(!is_nan(x));
      CHECK(!is_inf_or_nan(x));

      auto result_bits  = float_to_bits(x);
      float test_val = bits_to_float(result_bits);

      CHECK(test_val == x);
#endif
    }
#ifdef FSVF_FLOAT_DENORMAL_MATH
    {
      const uint32_t test_bits   = 0x12'0A1F;
      volatile float x = bits_to_float(test_bits);
      CHECK(is_finite(x));
      CHECK(is_denormal(x));
      CHECK(!is_inf(x));
      CHECK(!is_nan(x));
      CHECK(!is_inf_or_nan(x));

      CHECK(is_finite(smallest_positive_denormal));
      CHECK(is_denormal(smallest_positive_denormal));
      CHECK(!is_inf(smallest_positive_denormal));
      CHECK(!is_nan(smallest_positive_denormal));
      CHECK(!is_inf_or_nan(smallest_positive_denormal));

      CHECK(is_finite(-smallest_positive_denormal));
      CHECK(is_denormal(-smallest_positive_denormal));
      CHECK(!is_inf(-smallest_positive_denormal));
      CHECK(!is_nan(-smallest_positive_denormal));
      CHECK(!is_inf_or_nan(-smallest_positive_denormal));
    }
#endif

#ifndef FSVF_FLOAT_FINITE_MATH
    const float inf = std::numeric_limits<float>::infinity();
    const float nan1 = create_nan(0);
    const float nan2 = create_nan();

    CHECK(is_inf(inf));
    CHECK(is_inf(-inf));

    CHECK(is_inf_or_nan(inf));
    CHECK(is_inf_or_nan(-inf));

    CHECK(!is_finite(inf));
    CHECK(!is_finite(-inf));

    CHECK(!is_denormal(inf));
    CHECK(!is_denormal(-inf));

    CHECK(!is_nan(inf));
    CHECK(!is_nan(-inf));

    CHECK(float_to_bits(inf) == 0x7F800000);
    CHECK(float_to_bits(-inf) == 0xFF800000);

    const uint32_t nan_bits = 0x7FC0'0000;

    CHECK((nan_bits & f32_exponent_mask) == f32_exponent_mask);
    CHECK((nan_bits & f32_mantissa_mask) != 0);

    const float nan3 = bits_to_float(nan_bits);

    CHECK(!is_finite(nan1));
    CHECK(!is_denormal(nan1));
    CHECK(!is_inf(nan1));
    CHECK(is_nan(nan1));
    CHECK(is_inf_or_nan(nan1));

    CHECK(!is_finite(nan2));
    CHECK(!is_denormal(nan2));
    CHECK(!is_inf(nan2));
    CHECK(is_nan(nan2));
    CHECK(is_inf_or_nan(nan2));

    CHECK(!is_finite(nan3));
    CHECK(!is_denormal(nan3));
    CHECK(!is_inf(nan3));
    CHECK(is_nan(nan3));
    CHECK(is_inf_or_nan(nan3));
#endif
  }

  SECTION("Float - floating point structure")
  {
    const float inf = std::numeric_limits<float>::infinity();
    const float nan1 = create_nan(0);
    const float nan2 = create_nan();

    CHECK(mantissa(0.0f) == 0x000000);
    CHECK(mantissa(1.0f) == 0x000000);
    CHECK(mantissa(2.0f) == 0x000000);
    CHECK(mantissa(1.5f) == 0x400000);

    CHECK(exponent(0.0f) == 0x00000000);
    CHECK(exponent(1.0f) == 0x3F800000);
    CHECK(exponent(2.0f) == 0x40000000);
    CHECK(exponent(0.5f) == 0x3F000000);

    CHECK(signed_exponent(0.0f) == 0x00000000);
    CHECK(signed_exponent(1.0f) == 0x3F800000);
    CHECK(signed_exponent(2.0f) == 0x40000000);
    CHECK(signed_exponent(0.5f) == 0x3F000000);

    CHECK(sign_bit(0.0f) == 0);
    CHECK(sign_bit(1.0f) == 0);
    CHECK(sign_bit(2.0f) == 0);
    CHECK(sign_bit(0.5f) == 0);

    CHECK(mantissa(-0.0f) == 0x000000);
    CHECK(mantissa(-1.0f) == 0x000000);
    CHECK(mantissa(-2.0f) == 0x000000);
    CHECK(mantissa(-1.5f) == 0x400000);

    CHECK(exponent(-0.0f) == 0x00000000);
    CHECK(exponent(-1.0f) == 0x3F800000);
    CHECK(exponent(-2.0f) == 0x40000000);
    CHECK(exponent(-0.5f) == 0x3F000000);

    CHECK(signed_exponent(-0.0f) == 0x80000000);
    CHECK(signed_exponent(-1.0f) == 0xBF800000);
    CHECK(signed_exponent(-2.0f) == 0xC0000000);
    CHECK(signed_exponent(-0.5f) == 0xBF000000);

    CHECK(sign_bit(-0.0f) == f32_sign_mask);
    CHECK(sign_bit(-1.0f) == f32_sign_mask);
    CHECK(sign_bit(-2.0f) == f32_sign_mask);
    CHECK(sign_bit(-0.5f) == f32_sign_mask);

#ifdef FSVF_FLOAT_DENORMAL_MATH
    CHECK(mantissa(smallest_positive_denormal) == 0x000001);
    CHECK(mantissa(-smallest_positive_denormal) == 0x000001);

    CHECK(exponent(smallest_positive_denormal) == 0);
    CHECK(exponent(-smallest_positive_denormal) == 0);

    CHECK(signed_exponent(smallest_positive_denormal) == 0);
    CHECK(signed_exponent(-smallest_positive_denormal) == f32_sign_mask);

    CHECK(sign_bit(smallest_positive_denormal) == 0);
    CHECK(sign_bit(-smallest_positive_denormal) == f32_sign_mask);
#endif

#ifndef FSVF_FLOAT_FINITE_MATH
    CHECK(mantissa(inf) == 0);
    CHECK(mantissa(-inf) == 0);
    CHECK(mantissa(nan1) != 0);
    CHECK(mantissa(nan2) != 0);

    CHECK(exponent(inf) == f32_exponent_mask);
    CHECK(exponent(-inf) == f32_exponent_mask);
    CHECK(exponent(nan1) == f32_exponent_mask);
    CHECK(exponent(nan2) == f32_exponent_mask);

    CHECK(signed_exponent(inf) == f32_exponent_mask);
    CHECK(signed_exponent(-inf) == f32_signed_exponent_mask);
    CHECK(signed_exponent(nan1) == f32_exponent_mask);
    CHECK(signed_exponent(nan2) == f32_exponent_mask);

    CHECK(sign_bit(inf) == 0);
    CHECK(sign_bit(-inf) == f32_sign_mask);
#endif
  }
}

TEST_CASE("FloatUtils - Next float up/down", "[float_utils]")
{
  const float inf = std::numeric_limits<float>::infinity();
  const float nan1 = create_nan(0);
  const float nan2 = create_nan();

  for (auto x : float_test_values) {
    if (x == 0.0f) continue;

    CHECK(next_float_up(x) > next_float_down(x));

    const auto bits = float_to_bits(x);

    if (x > 0.f) CHECK(next_float_up(x) == bits_to_float(bits + 1));
    else CHECK(next_float_up(x) == bits_to_float(bits - 1));

    if (x > 0.f) CHECK(next_float_down(x) == bits_to_float(bits - 1));
    else CHECK(next_float_down(x) == bits_to_float(bits + 1));
  }

#ifndef FSVF_FLOAT_DENORMAL_MATH
  CHECK(next_float_up(0.0f) == smallest_normal_float);
  CHECK(next_float_up(-0.0f) == smallest_normal_float);
  CHECK(next_float_down(0.0f) == -smallest_normal_float);
  CHECK(next_float_down(-0.0f) == -smallest_normal_float);
#else
  CHECK(next_float_up(0.0f) == smallest_positive_denormal);
  CHECK(next_float_up(-0.0f) == smallest_positive_denormal);
  CHECK(next_float_down(0.0f) == -smallest_positive_denormal);
  CHECK(next_float_down(-0.0f) == -smallest_positive_denormal);

  for (auto x : float_denormal_test_values) {
    CHECK(next_float_up(x) > next_float_down(x));

    const auto bits = float_to_bits(x);

    if (x > 0.f) CHECK(next_float_up(x) == bits_to_float(bits + 1));
    else CHECK(next_float_up(x) == bits_to_float(bits - 1));

    if (x > 0.f) CHECK(next_float_down(x) == bits_to_float(bits - 1));
    else CHECK(next_float_down(x) == bits_to_float(bits + 1));
  }
#endif
}


TEST_CASE("FloatUtils - ULP functions", "[float_utils]")
{
  SECTION("ULP Values")
  {
    CHECK(ulps_distance(0.f, -0.f) == 0);

    for (float x : float_test_values) {
      INFO("x = 0x" << std::hex << float_to_bits(x));

      CHECK(ulps_distance(x,x) == 0);

#ifndef FSVF_FLOAT_DENORMAL_MATH
      if (x == 0.f) continue;

      CHECK(ulps_distance(x,-x) == 0xFFFF'FFFF);
#endif

      auto next = x;
      auto prev = x;

      for (uint32_t i = 1; i < 10; ++i) {
        next = next_float_up(next);
        prev = next_float_down(prev);

        INFO("next = 0x" << std::hex << float_to_bits(next));
        INFO("prev = 0x" << std::hex << float_to_bits(prev));

        CHECK(ulps_distance(x,next) == i);
        CHECK(ulps_distance(x,prev) == i);
        CHECK(ulps_distance(next,x) == i);
        CHECK(ulps_distance(prev,x) == i);
      }
    }

#ifdef FSVF_FLOAT_DENORMAL_MATH
    for (float x : float_denormal_test_values) {
      INFO("x = 0x" << std::hex << float_to_bits(x));

      auto next = x;
      auto prev = x;

      for (uint32_t i = 1; i < 10; ++i) {
        next = next_float_up(next);
        prev = next_float_down(prev);

        INFO("next = 0x" << std::hex << float_to_bits(next));
        INFO("prev = 0x" << std::hex << float_to_bits(prev));

        CHECK(ulps_distance(x,next) == i);
        CHECK(ulps_distance(x,prev) == i);
        CHECK(ulps_distance(next,x) == i);
        CHECK(ulps_distance(prev,x) == i);
      }
    }
#endif
  }
}

TEST_CASE("FloatUtils - Comparison functions", "[float_utils]")
{
  SECTION("almost_equal_ulp function")
  {
    CHECK(almost_equal_ulp<0>(-0.0f, 0.0f));
    CHECK(almost_equal_ulp<1>(-0.0f, 0.0f));
    CHECK(almost_equal_ulp<2>(-0.0f, 0.0f));

    for (float x : float_test_values)
    {
#ifndef FSVF_FLOAT_DENORMAL_MATH
      if (x == 0.f) continue;
#endif
      INFO("Testing ULP threshold: 0, value:" << x);
      CHECK(almost_equal_ulp<0>(x, x));
      if (x != 0.f)
      {
        CHECK_FALSE(almost_equal_ulp<0>(x, x + (x/4.f)));
        CHECK_FALSE(almost_equal_ulp<0>(x, x - (x/4.f)));
      }
      CHECK_FALSE(almost_equal_ulp<0>(x, next_float_up(x)));
      CHECK_FALSE(almost_equal_ulp<0>(x, next_float_down(x)));

      INFO("Testing ULP threshold: 1, value:" << x);
      CHECK(almost_equal_ulp<1>(x, x));
      CHECK(almost_equal_ulp<1>(x, next_float_up(x)));
      CHECK(almost_equal_ulp<1>(x, next_float_down(x)));
      CHECK_FALSE(almost_equal_ulp<1>(x, next_float_up(next_float_up(x))));
      CHECK_FALSE(almost_equal_ulp<1>(x, next_float_down(next_float_down(x))));

      INFO("Testing ULP threshold: 2, value:" << x);
      CHECK(almost_equal_ulp<2>(x, x));
      CHECK(almost_equal_ulp<2>(x, next_float_up(x)));
      CHECK(almost_equal_ulp<2>(x, next_float_down(x)));
      CHECK(almost_equal_ulp<2>(x, next_float_up(next_float_up(x))));
      CHECK(almost_equal_ulp<2>(x, next_float_down(next_float_down(x))));
      CHECK_FALSE(almost_equal_ulp<2>(x, next_float_up(next_float_up(next_float_up(x)))));
      CHECK_FALSE(almost_equal_ulp<2>(x, next_float_down(next_float_down(next_float_down(x)))));
    }
#ifdef FSVF_FLOAT_DENORMAL_MATH
    for (float x : float_denormal_test_values)
    {
      CHECK(almost_equal_ulp<0xFF>(x, x + (x/0x80000)));
      CHECK(almost_equal_ulp<0xFF>(x, x - (x/0x80000)));

      INFO("Testing ULP threshold: 0, value:" << x);
      CHECK(almost_equal_ulp<0>(x, x));
      CHECK_FALSE(almost_equal_ulp<0>(x, next_float_up(x)));
      CHECK_FALSE(almost_equal_ulp<0>(x, next_float_down(x)));

      INFO("Testing ULP threshold: 1, value:" << x);
      CHECK(almost_equal_ulp<1>(x, x));
      CHECK(almost_equal_ulp<1>(x, next_float_up(x)));
      CHECK(almost_equal_ulp<1>(x, next_float_down(x)));
      CHECK_FALSE(almost_equal_ulp<1>(x, next_float_up(next_float_up(x))));
      CHECK_FALSE(almost_equal_ulp<1>(x, next_float_down(next_float_down(x))));

      INFO("Testing ULP threshold: 2, value:" << x);
      CHECK(almost_equal_ulp<2>(x, x));
      CHECK(almost_equal_ulp<2>(x, next_float_up(x)));
      CHECK(almost_equal_ulp<2>(x, next_float_down(x)));
      CHECK(almost_equal_ulp<2>(x, next_float_up(next_float_up(x))));
      CHECK(almost_equal_ulp<2>(x, next_float_down(next_float_down(x))));
      CHECK_FALSE(almost_equal_ulp<2>(x, next_float_up(next_float_up(next_float_up(x)))));
      CHECK_FALSE(almost_equal_ulp<2>(x, next_float_down(next_float_down(next_float_down(x)))));
    }
#endif
  }

  SECTION("almost_equal_eps - default epsilon")
  {
    CHECK(almost_equal_eps(-0.0f, 0.0f));
    CHECK(almost_equal_eps(0.0f, -0.0f));

    float eps = std::numeric_limits<float>::epsilon() / 2.f;

    CHECK(almost_equal_eps(1.0f, 1.0f + eps));
    CHECK(almost_equal_eps(1.0f + eps, 1.0f));

    CHECK(almost_equal_eps(1.0f, 1.0f - eps));
    CHECK(almost_equal_eps(1.0f - eps, 1.0f));

    CHECK(almost_equal_eps(1.0f, 1.0f + eps));
    CHECK(almost_equal_eps(1.0f + eps, 1.0f));
    CHECK(almost_equal_eps(1.0f, 1.0f - eps));
    CHECK(almost_equal_eps(1.0f - eps, 1.0f));

    CHECK_FALSE(almost_equal_eps(1.0f, 1.0f + 2.f * eps));
    CHECK_FALSE(almost_equal_eps(1.0f + 2.f * eps, 1.0f));
    CHECK_FALSE(almost_equal_eps(1.0f, 1.0f - 2.f * eps));
    CHECK_FALSE(almost_equal_eps(1.0f - 2.f * eps, 1.0f));

    CHECK(almost_equal_eps(0.0f, eps * 0.5f));
    CHECK(almost_equal_eps(eps * 0.5f, 0.f));

    CHECK(almost_equal_eps(0.0f, -eps * 0.25f));
    CHECK(almost_equal_eps(-eps * 0.25f, 0.0f));

    CHECK(almost_equal_eps(0.0f, eps));
    CHECK(almost_equal_eps(eps, 0.0f));

    CHECK(almost_equal_eps(0.0f, -eps));
    CHECK(almost_equal_eps(-eps, 0.0f));

    CHECK_FALSE(almost_equal_eps(0.0f, eps * 1.1f));
    CHECK_FALSE(almost_equal_eps(eps * 1.1f, 0.0f));

    CHECK_FALSE(almost_equal_eps(0.0f, -eps * 1.1f));
    CHECK_FALSE(almost_equal_eps(-eps * 1.1f, 0.0f));

    CHECK_FALSE(almost_equal_eps(-eps * 0.828f, eps * 0.788f));
    CHECK_FALSE(almost_equal_eps(eps * 0.788f, -eps * 0.828f));

    CHECK_FALSE(almost_equal_eps(-eps, eps));
    CHECK_FALSE(almost_equal_eps(eps, -eps));

    CHECK_FALSE(almost_equal_eps(1.0f, 2.0f));
    CHECK_FALSE(almost_equal_eps(2.0f, 1.0f));
    CHECK_FALSE(almost_equal_eps(0.0f, 1.0f));
    CHECK_FALSE(almost_equal_eps(1.0f, 0.0f));

    for (float x : float_test_values)
    {
      CHECK(almost_equal_eps(x, x));

      float y = x + eps * (std::abs(x) * 0.5f);
      CHECK(almost_equal_eps(x, y));

      y = x - eps * std::abs(x) * 0.5f;
      CHECK(almost_equal_eps(x, y));
    }
#ifdef FSVF_FLOAT_DENORMAL_MATH
    for (float x : float_denormal_test_values)
    {
      CHECK(almost_equal_eps(x, x));

      float y = x + eps * (std::abs(x) * 0.5f);
      CHECK(almost_equal_eps(x, y));

      y = x - eps * std::abs(x) * 0.5f;
      CHECK(almost_equal_eps(x, y));
    }
#endif

#ifndef FSVF_FLOAT_FINITE_MATH
    CHECK(almost_equal_eps(
      std::numeric_limits<float>::infinity(),
      std::numeric_limits<float>::infinity()
    ));
    CHECK(almost_equal_eps(
      -std::numeric_limits<float>::infinity(),
      -std::numeric_limits<float>::infinity()
    ));
    CHECK_FALSE(almost_equal_eps(
      std::numeric_limits<float>::infinity(),
      -std::numeric_limits<float>::infinity()
    ));
    CHECK_FALSE(almost_equal_eps(
      std::numeric_limits<float>::infinity(),
      1.0f
    ));
    CHECK_FALSE(almost_equal_eps(
      1.0f,
      std::numeric_limits<float>::infinity()
    ));

    // NaN cases - should never be equal to anything
    CHECK_FALSE(almost_equal_eps(create_nan(), create_nan()));
    CHECK_FALSE(almost_equal_eps(create_nan(0), create_nan(0)));
    CHECK_FALSE(almost_equal_eps(std::numeric_limits<float>::infinity(), create_nan(0)));
    CHECK_FALSE(almost_equal_eps(create_nan(), std::numeric_limits<float>::infinity()));
    CHECK_FALSE(almost_equal_eps(0.f, create_nan(0)));
    CHECK_FALSE(almost_equal_eps(create_nan(), 1.f));
#endif
  }

  SECTION("almost_equal")
  {
    CHECK(almost_equal(-0.0f, 0.0f));
    CHECK(almost_equal(0.0f, -0.0f));

    // very large finite number
    constexpr float huge = bits_to_float(0x3FFF'FFFEu);

    CHECK(almost_equal(huge, next_float_up(huge)));
    CHECK_FALSE(almost_equal_eps(huge, next_float_up(huge)));
    CHECK(almost_equal_ulp<1>(huge, next_float_up(huge)));

    // double the standard epsilon we use in the library
    constexpr float eps = std::numeric_limits<float>::epsilon();

    CHECK(almost_equal(1.0f, 1.0f + eps));
    CHECK(almost_equal(1.0f + eps, 1.0f));

    CHECK(almost_equal(1.0f, 1.0f - eps));
    CHECK(almost_equal(1.0f - eps, 1.0f));

    CHECK(almost_equal(1.0f, 1.0f + eps));
    CHECK(almost_equal(1.0f + eps, 1.0f));
    CHECK(almost_equal(1.0f, 1.0f - eps));
    CHECK(almost_equal(1.0f - eps, 1.0f));

    CHECK(almost_equal(1.0f, 1.0f + 2.f * eps));
    CHECK(almost_equal(1.0f + 2.f * eps, 1.0f));
    CHECK(almost_equal(1.0f, 1.0f - 2.f * eps));
    CHECK(almost_equal(1.0f - 2.f * eps, 1.0f));

    CHECK(almost_equal(0.0f, eps * 0.5f));
    CHECK(almost_equal(eps * 0.5f, 0.f));

    CHECK(almost_equal(0.0f, -eps * 0.25f));
    CHECK(almost_equal(-eps * 0.25f, 0.0f));

    CHECK(almost_equal(0.0f, eps));
    CHECK(almost_equal(eps, 0.0f));

    CHECK(almost_equal(0.0f, -eps));
    CHECK(almost_equal(-eps, 0.0f));

    CHECK(almost_equal(1.0f, 1.0f + eps * 2.0f));
    CHECK(almost_equal(1.0f + eps * 2.0f, 1.0f));

    CHECK_FALSE(almost_equal(-eps * 0.828f, eps * 0.788f));
    CHECK_FALSE(almost_equal(eps * 0.788f, -eps * 0.828f));

    CHECK_FALSE(almost_equal(-eps, eps));
    CHECK_FALSE(almost_equal(eps, -eps));

    CHECK_FALSE(almost_equal(1.0f, 2.0f));
    CHECK_FALSE(almost_equal(2.0f, 1.0f));
    CHECK_FALSE(almost_equal(0.0f, 1.0f));
    CHECK_FALSE(almost_equal(1.0f, 0.0f));

    for (float x : float_test_values)
    {
      CHECK(almost_equal(x, x));

      float y = x + eps * (std::abs(x) * 0.5f);
      CHECK(almost_equal(x, y));

      y = x - eps * std::abs(x) * 0.5f;
      CHECK(almost_equal(x, y));

      float next = next_float_up(x);
      float prev = next_float_down(x);

      CHECK(almost_equal(x, next));
      CHECK(almost_equal(x, prev));
    }
#ifdef FSVF_FLOAT_DENORMAL_MATH
    for (float x : float_denormal_test_values)
    {
      CHECK(almost_equal(x, x));

      float y = x + eps * (std::abs(x) * 0.5f);
      CHECK(almost_equal(x, y));

      y = x - eps * std::abs(x) * 0.5f;
      CHECK(almost_equal(x, y));

      float next = next_float_up(x);
      float prev = next_float_down(x);

      CHECK(almost_equal(x, next));
      CHECK(almost_equal(x, prev));
    }
#endif

#ifndef FSVF_FLOAT_FINITE_MATH
    CHECK(almost_equal(
      std::numeric_limits<float>::infinity(),
      std::numeric_limits<float>::infinity()
    ));
    CHECK(almost_equal(
      -std::numeric_limits<float>::infinity(),
      -std::numeric_limits<float>::infinity()
    ));
    CHECK_FALSE(almost_equal(
      std::numeric_limits<float>::infinity(),
      -std::numeric_limits<float>::infinity()
    ));
    CHECK_FALSE(almost_equal(
      std::numeric_limits<float>::infinity(),
      1.0f
    ));
    CHECK_FALSE(almost_equal(
      1.0f,
      std::numeric_limits<float>::infinity()
    ));

    // NaN cases - should never be equal to anything
    CHECK_FALSE(almost_equal(create_nan(), create_nan()));
    CHECK_FALSE(almost_equal(create_nan(0), create_nan(0)));
    CHECK_FALSE(almost_equal(std::numeric_limits<float>::infinity(), create_nan(0)));
    CHECK_FALSE(almost_equal(create_nan(), std::numeric_limits<float>::infinity()));
    CHECK_FALSE(almost_equal(0.f, create_nan(0)));
    CHECK_FALSE(almost_equal(create_nan(), 1.f));
#endif
  }
}
