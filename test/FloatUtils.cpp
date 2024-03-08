#include "FloatUtils.hpp"
#include <FSVF.h>

#define CATCH_CONFIG_FAST_COMPILE
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <limits>
#include <random>
#include <vector>

using namespace FSVF;
using Catch::Approx;

// TODO!!!
// Most of the tests are AI-generated and require manual double-checking and cleanup

// Test data generation utilities
static std::random_device rd;
static std::mt19937       gen(rd());

static float random_float(float min = -1000.0f, float max = 1000.0f)
{
  std::uniform_real_distribution<float> dist(min, max);
  return dist(gen);
}

static uint32_t random_bits()
{
  std::uniform_int_distribution<uint32_t> dist;
  return dist(gen);
}

// Helper function to create NaN values
static float create_nan(uint32_t payload = 0x123456)
{
  return FSVF::bits_to_float(FSVF::f32_exponent_mask | payload);
}

// Helper function to create infinity values
static float create_infinity(bool negative = false)
{
  uint32_t bits = FSVF::f32_exponent_mask;
  if (negative) bits |= FSVF::f32_sign_mask;
  return FSVF::bits_to_float(bits);
}

static constexpr float smallest_positive_denormal = FSVF::bits_to_float(0x1);
static constexpr float smallest_negative_denormal = -smallest_positive_denormal;

// Test across many orders of magnitude
static const std::vector<float> test_values = {
    // TODO depending on flag
    //1e-38f, 1e-30f, 1e-20f, 1e-10f,
    1e-5f, 1e-4f, 1e-3f,
    0.1f, 0.5f, 0.9f,
    1.0f, 2.0f, 5.0f, 10.0f,
    100.0f, 1000.0f, 1e4f, 1e5f, 1e6f,
    1e10f, 1e20f, 1e30f, 1e38f,
    // TODO depending on flag
    //-1e-38f, -1e-30f, -1e-20f, -1e-10f,
    -1e-5f, -1e-4f, -1e-3f,
    -0.1f, -0.5f, -0.9f,
    -1.0f, -2.0f, -5.0f, -10.0f,
    -100.0f, -1000.0f, -1e4f, -1e5f, -1e6f,
    -1e10f, -1e20f, -1e30f, -1e38f
};

TEST_CASE("FloatUtils - Bit manipulation functions", "[float_utils]")
{
  SECTION("bits_to_float and float_to_bits")
  {
    // Test round-trip conversion
    uint32_t test_bits   = 0x40490FDB;
    float    f           = FSVF::bits_to_float(test_bits);
    uint32_t result_bits = FSVF::float_to_bits(f);
    CHECK(result_bits == test_bits);

    f            = -42.42f;
    result_bits  = FSVF::float_to_bits(f);
    float test_f = FSVF::bits_to_float(result_bits);
    CHECK(f == test_f);

    CHECK(FSVF::float_to_bits(0.0f) == 0x00000000);
    CHECK(FSVF::float_to_bits(-0.0f) == 0x80000000);
    CHECK(FSVF::float_to_bits(1.0f) == 0x3F800000);
    CHECK(FSVF::float_to_bits(-1.0f) == 0xBF800000);

    // Test infinity
    CHECK(
      FSVF::float_to_bits(std::numeric_limits<float>::infinity()) == 0x7F800000
    );
    CHECK(
      FSVF::float_to_bits(-std::numeric_limits<float>::infinity()) == 0xFF800000
    );

    // Test NaN
    uint32_t nan_bits
      = FSVF::float_to_bits(std::numeric_limits<float>::quiet_NaN());
    CHECK((nan_bits & FSVF::f32_exponent_mask) == FSVF::f32_exponent_mask);
    CHECK((nan_bits & FSVF::f32_mantissa_mask) != 0);
  }

  SECTION("mantissa function")
  {
    // Normal numbers
    CHECK(FSVF::mantissa(1.0f) == 0x000000);
    CHECK(FSVF::mantissa(2.0f) == 0x000000);
    CHECK(FSVF::mantissa(1.5f) == 0x400000);

    CHECK(FSVF::mantissa(smallest_negative_denormal) == 0x000001);

    CHECK(FSVF::mantissa(std::numeric_limits<float>::infinity()) == 0);
    CHECK(FSVF::mantissa(std::numeric_limits<float>::quiet_NaN()) != 0);

    // TODO more tests
  }

  SECTION("exponent function")
  {
    // Normal numbers
    CHECK(FSVF::exponent(1.0f) == 0x3F800000);
    CHECK(FSVF::exponent(2.0f) == 0x40000000);
    CHECK(FSVF::exponent(0.5f) == 0x3F000000);

    CHECK(FSVF::exponent(smallest_negative_denormal) == 0);

    CHECK(
      FSVF::exponent(std::numeric_limits<float>::infinity())
      == FSVF::f32_exponent_mask
    );
    CHECK(
      FSVF::exponent(std::numeric_limits<float>::quiet_NaN())
      == FSVF::f32_exponent_mask
    );

    // TODO more tests
  }

  SECTION("signed_exponent function")
  {
    CHECK(FSVF::signed_exponent(1.0f) == 0x3F800000);
    CHECK(FSVF::signed_exponent(2.0f) == 0x40000000);

    CHECK(FSVF::signed_exponent(-1.0f) == 0xBF800000);
    CHECK(FSVF::signed_exponent(-2.0f) == 0xC0000000);

    CHECK(FSVF::signed_exponent(0.0f) == 0x00000000);
    CHECK(FSVF::signed_exponent(-0.0f) == 0x80000000);

    // TODO more tests
  }

  SECTION("sign_bit function")
  {
    CHECK(FSVF::sign_bit(1.0f) == 0);
    CHECK(FSVF::sign_bit(0.0f) == 0);

    CHECK(FSVF::sign_bit(-1.0f) == FSVF::f32_sign_mask);
    CHECK(FSVF::sign_bit(-0.0f) == FSVF::f32_sign_mask);

    CHECK(FSVF::sign_bit(std::numeric_limits<float>::infinity()) == 0);
    CHECK(FSVF::sign_bit(-std::numeric_limits<float>::infinity()) == FSVF::f32_sign_mask);
  }
}

TEST_CASE("FloatUtils - Float classification functions", "[float_utils]")
{
  SECTION("is_inf_or_nan function")
  {
    CHECK_FALSE(FSVF::is_inf_or_nan(0.0f));
    CHECK_FALSE(FSVF::is_inf_or_nan(1.0f));
    CHECK_FALSE(FSVF::is_inf_or_nan(-1.0f));
    CHECK_FALSE(FSVF::is_inf_or_nan(smallest_negative_denormal));

    CHECK(FSVF::is_inf_or_nan(std::numeric_limits<float>::infinity()));
    CHECK(FSVF::is_inf_or_nan(-std::numeric_limits<float>::infinity()));

    CHECK(FSVF::is_inf_or_nan(std::numeric_limits<float>::quiet_NaN()));
    CHECK(FSVF::is_inf_or_nan(std::numeric_limits<float>::signaling_NaN()));
  }

  SECTION("is_finite function")
  {
    CHECK(FSVF::is_finite(0.0f));
    CHECK(FSVF::is_finite(1.0f));
    CHECK(FSVF::is_finite(-1.0f));
    CHECK(FSVF::is_finite(smallest_negative_denormal));

    CHECK_FALSE(FSVF::is_finite(std::numeric_limits<float>::infinity()));
    CHECK_FALSE(FSVF::is_finite(-std::numeric_limits<float>::infinity()));

    CHECK_FALSE(FSVF::is_finite(std::numeric_limits<float>::quiet_NaN()));
    CHECK_FALSE(FSVF::is_finite(std::numeric_limits<float>::signaling_NaN()));
  }

  SECTION("is_inf function")
  {
    CHECK_FALSE(FSVF::is_inf(0.0f));
    CHECK_FALSE(FSVF::is_inf(1.0f));
    CHECK_FALSE(FSVF::is_inf(-1.0f));
    CHECK_FALSE(FSVF::is_inf(smallest_negative_denormal));

    CHECK(FSVF::is_inf(std::numeric_limits<float>::infinity()));
    CHECK(FSVF::is_inf(-std::numeric_limits<float>::infinity()));

    CHECK_FALSE(FSVF::is_inf(std::numeric_limits<float>::quiet_NaN()));
    CHECK_FALSE(FSVF::is_inf(std::numeric_limits<float>::signaling_NaN()));
  }

  SECTION("is_nan function")
  {
    CHECK_FALSE(FSVF::is_nan(0.0f));
    CHECK_FALSE(FSVF::is_nan(1.0f));
    CHECK_FALSE(FSVF::is_nan(-1.0f));
    CHECK_FALSE(FSVF::is_nan(smallest_negative_denormal));

    CHECK_FALSE(FSVF::is_nan(std::numeric_limits<float>::infinity()));
    CHECK_FALSE(FSVF::is_nan(-std::numeric_limits<float>::infinity()));

    CHECK(FSVF::is_nan(std::numeric_limits<float>::quiet_NaN()));
    CHECK(FSVF::is_nan(std::numeric_limits<float>::signaling_NaN()));
  }

  SECTION("is_denormal function")
  {
    CHECK_FALSE(FSVF::is_denormal(0.0f));
    CHECK_FALSE(FSVF::is_denormal(1.0f));
    CHECK_FALSE(FSVF::is_denormal(-1.0f));

    CHECK(FSVF::is_denormal(smallest_negative_denormal));
    CHECK(FSVF::is_denormal(smallest_positive_denormal));

    CHECK_FALSE(FSVF::is_denormal(std::numeric_limits<float>::infinity()));
    CHECK_FALSE(FSVF::is_denormal(std::numeric_limits<float>::quiet_NaN()));
  }
}

TEST_CASE("FloatUtils - Next float up/down", "[float_utils]")
{
    // TODO (test behavior at 0)
}

TEST_CASE("FloatUtils - Comparison functions", "[float_utils]")
{
  SECTION("almost_equal_ulp function")
  {
    CHECK(FSVF::almost_equal_ulp<0>(-0.0f, 0.0f));
    CHECK(FSVF::almost_equal_ulp<1>(-0.0f, 0.0f));
    CHECK(FSVF::almost_equal_ulp<2>(-0.0f, 0.0f));

    for (float x : test_values)
    {
        INFO("Testing ULP threshold: 0, value:" << x);
        CHECK(FSVF::almost_equal_ulp<0>(x, x));
        CHECK_FALSE(FSVF::almost_equal_ulp<0>(x, x + (x/4.f)));
        CHECK_FALSE(FSVF::almost_equal_ulp<0>(x, x - (x/4.f)));
        CHECK_FALSE(FSVF::almost_equal_ulp<0>(x, FSVF::next_float_up(x)));
        CHECK_FALSE(FSVF::almost_equal_ulp<0>(x, FSVF::next_float_down(x)));

        INFO("Testing ULP threshold: 1, value:" << x);
        CHECK(FSVF::almost_equal_ulp<1>(x, x));
        CHECK(FSVF::almost_equal_ulp<1>(x, FSVF::next_float_up(x)));
        CHECK(FSVF::almost_equal_ulp<1>(x, FSVF::next_float_down(x)));
        CHECK_FALSE(FSVF::almost_equal_ulp<1>(x, FSVF::next_float_up(FSVF::next_float_up(x))));
        CHECK_FALSE(FSVF::almost_equal_ulp<1>(x, FSVF::next_float_down(FSVF::next_float_down(x))));

        INFO("Testing ULP threshold: 2, value:" << x);
        CHECK(FSVF::almost_equal_ulp<2>(x, x));
        CHECK(FSVF::almost_equal_ulp<2>(x, FSVF::next_float_up(x)));
        CHECK(FSVF::almost_equal_ulp<2>(x, FSVF::next_float_down(x)));
        CHECK(FSVF::almost_equal_ulp<2>(x, FSVF::next_float_up(FSVF::next_float_up(x))));
        CHECK(FSVF::almost_equal_ulp<2>(x, FSVF::next_float_down(FSVF::next_float_down(x))));
        CHECK_FALSE(FSVF::almost_equal_ulp<2>(x, FSVF::next_float_up(FSVF::next_float_up(FSVF::next_float_up(x)))));
        CHECK_FALSE(FSVF::almost_equal_ulp<2>(x, FSVF::next_float_down(FSVF::next_float_down(FSVF::next_float_down(x)))));
    }

    // TODO special values
  }


  SECTION("almost_equal function - basic functionality")
  {
    CHECK(FSVF::almost_equal(-0.0f, 0.0f));
    CHECK(FSVF::almost_equal(0.0f, -0.0f));

    // Very close values (within epsilon)
    float eps = std::numeric_limits<float>::epsilon();
    CHECK(FSVF::almost_equal(1.0f, 1.0f + eps * 0.5f));
    CHECK(FSVF::almost_equal(1.0f, 1.0f - eps * 0.5f));
    CHECK(FSVF::almost_equal(0.0f, eps * 0.5f));
    CHECK(FSVF::almost_equal(0.0f, -eps * 0.5f));
    CHECK(FSVF::almost_equal(1.0f, 1.0f + eps * 2.0f));

    // Different values
    CHECK_FALSE(FSVF::almost_equal(1.0f, 2.0f));
    CHECK_FALSE(FSVF::almost_equal(0.0f, 1.0f));
  }

  SECTION("almost_equal function - diverse value ranges")
  {
    float eps = std::numeric_limits<float>::epsilon();

    for (float x : test_values)
    {
      INFO("Testing value: " << x);

      CHECK(FSVF::almost_equal(x, x));

      float y = x + eps * std::abs(x) * 0.5f;
      CHECK(FSVF::almost_equal(x, y));

      y = x - eps * std::abs(x) * 0.5f;
      CHECK(FSVF::almost_equal(x, y));

    float next = FSVF::next_float_up(x);
    float prev = FSVF::next_float_down(x);

    CHECK(FSVF::almost_equal(x, next));
    CHECK(FSVF::almost_equal(x, prev));
    }
  }

  SECTION("almost_equal function - special values")
  {
    // Infinity cases
    CHECK(FSVF::almost_equal(
      std::numeric_limits<float>::infinity(),
      std::numeric_limits<float>::infinity()
    ));
    CHECK(FSVF::almost_equal(
      -std::numeric_limits<float>::infinity(),
      -std::numeric_limits<float>::infinity()
    ));
    CHECK_FALSE(FSVF::almost_equal(
      std::numeric_limits<float>::infinity(),
      -std::numeric_limits<float>::infinity()
    ));
    CHECK_FALSE(FSVF::almost_equal(
      std::numeric_limits<float>::infinity(),
      1.0f
    ));
    CHECK_FALSE(FSVF::almost_equal(
      1.0f,
      std::numeric_limits<float>::infinity()
    ));

    // NaN cases - should never be equal to anything
    // TODO (depends on flags)
  }

  SECTION("almost_equal_eps function - systematic testing")
  {
    // Test with default epsilon
    constexpr float eps = std::numeric_limits<float>::epsilon() / 2.f;

    // Test values within epsilon
    CHECK(FSVF::almost_equal_eps(1.0f, 1.0f + eps * 0.5f));
    CHECK(FSVF::almost_equal_eps(1.0f, 1.0f - eps * 0.5f));
    CHECK(FSVF::almost_equal_eps(0.0f, eps * 0.5f));
    CHECK(FSVF::almost_equal_eps(0.0f, -eps * 0.5f));

    // Test values at epsilon boundary
    CHECK(FSVF::almost_equal_eps(1.0f, 1.0f + eps));
    CHECK(FSVF::almost_equal_eps(1.0f, 1.0f - eps));

    // Test values beyond epsilon
    CHECK_FALSE(FSVF::almost_equal_eps(1.0f, 1.0f + eps * 2.f));
    CHECK_FALSE(FSVF::almost_equal_eps(1.0f, 1.0f - eps * 2.f));

    // Test with different magnitudes
    float large = 1e6f;
    CHECK(FSVF::almost_equal_eps(large, large + eps * large * 0.5f));
    CHECK_FALSE(FSVF::almost_equal_eps(large, large + eps * large * 1.1f));
  }

  // TODO
  //SECTION("almost_equal_eps function - diverse value ranges")
  //{
  //  for (float x : test_values)
  //  {
  //    if (!FSVF::is_finite(x)) continue;
  //
  //    INFO("Testing value: " << x);
  //
  //    // Test with epsilon 1e-6f
  //    float y = x + 1e-6f * std::abs(x) * 0.5f;
  //    CHECK(FSVF::almost_equal_eps<1e-6f>(x, y));
  //
  //    y = x - 1e-6f * std::abs(x) * 0.5f;
  //    CHECK(FSVF::almost_equal_eps<1e-6f>(x, y));
  //
  //    // Test with epsilon 1e-5f
  //    y = x + 1e-5f * std::abs(x) * 0.5f;
  //    CHECK(FSVF::almost_equal_eps<1e-5f>(x, y));
  //
  //    y = x - 1e-5f * std::abs(x) * 0.5f;
  //    CHECK(FSVF::almost_equal_eps<1e-5f>(x, y));
  //
  //    // Test with epsilon 1e-4f
  //    y = x + 1e-4f * std::abs(x) * 0.5f;
  //    CHECK(FSVF::almost_equal_eps<1e-4f>(x, y));
  //
  //    y = x - 1e-4f * std::abs(x) * 0.5f;
  //    CHECK(FSVF::almost_equal_eps<1e-4f>(x, y));
  //
  //    // Test with epsilon 1e-3f
  //    y = x + 1e-3f * std::abs(x) * 0.5f;
  //    CHECK(FSVF::almost_equal_eps<1e-3f>(x, y));
  //
  //    y = x - 1e-3f * std::abs(x) * 0.5f;
  //    CHECK(FSVF::almost_equal_eps<1e-3f>(x, y));
  //
  //    // Test with epsilon 1e-2f
  //    y = x + 1e-2f * std::abs(x) * 0.5f;
  //    CHECK(FSVF::almost_equal_eps<1e-2f>(x, y));
  //
  //    y = x - 1e-2f * std::abs(x) * 0.5f;
  //    CHECK(FSVF::almost_equal_eps<1e-2f>(x, y));
  //
  //    // Test values at epsilon boundary
  //    y = x + 1e-6f * std::abs(x);
  //    CHECK(FSVF::almost_equal_eps<1e-6f>(x, y));
  //
  //    y = x + 1e-5f * std::abs(x);
  //    CHECK(FSVF::almost_equal_eps<1e-5f>(x, y));
  //
  //    y = x + 1e-4f * std::abs(x);
  //    CHECK(FSVF::almost_equal_eps<1e-4f>(x, y));
  //
  //    y = x + 1e-3f * std::abs(x);
  //    CHECK(FSVF::almost_equal_eps<1e-3f>(x, y));
  //
  //    y = x + 1e-2f * std::abs(x);
  //    CHECK(FSVF::almost_equal_eps<1e-2f>(x, y));
  //
  //    // Test values beyond epsilon
  //    y = x + 1e-6f * std::abs(x) * 2.f;
  //    CHECK_FALSE(FSVF::almost_equal_eps<1e-6f>(x, y));
  //
  //    y = x + 1e-5f * std::abs(x) * 2.f;
  //    CHECK_FALSE(FSVF::almost_equal_eps<1e-5f>(x, y));
  //
  //    y = x + 1e-4f * std::abs(x) * 2.f;
  //    CHECK_FALSE(FSVF::almost_equal_eps<1e-4f>(x, y));
  //
  //    y = x + 1e-3f * std::abs(x) * 2.f;
  //    CHECK_FALSE(FSVF::almost_equal_eps<1e-3f>(x, y));
  //
  //    y = x + 1e-2f * std::abs(x) * 2.f;
  //    CHECK_FALSE(FSVF::almost_equal_eps<1e-2f>(x, y));
  //  }
  //}

  SECTION("almost_equal_eps function - custom epsilon")
  {
    // Test values within custom epsilon
    CHECK(FSVF::almost_equal_eps<1e-6f>(1.0f, 1.0f + 1e-6f * 0.5f));
    CHECK(FSVF::almost_equal_eps<1e-6f>(1.0f, 1.0f - 1e-6f * 0.5f));

    // Test values at custom epsilon boundary
    CHECK(FSVF::almost_equal_eps<1e-6f>(1.1f, 1.1f + 1e-6f));
    CHECK(FSVF::almost_equal_eps<1e-6f>(1.1f, 1.1f - 1e-6f));
        // TODO need denormals not zero
    //CHECK_FALSE(FSVF::almost_equal_eps<1e-6f>(1.1f, 1.1f + FSVF::next_float_up(1e-6f)));
    //CHECK_FALSE(FSVF::almost_equal_eps<1e-6f>(1.1f, 1.1f - FSVF::next_float_up(1e-6f)));

    // Test values beyond custom epsilon
    CHECK_FALSE(FSVF::almost_equal_eps<1e-6f>(1.0f, 1.0f + 1e-6f * 1.1f));
    CHECK_FALSE(FSVF::almost_equal_eps<1e-6f>(1.0f, 1.0f - 1e-6f * 1.1f));

    // Test values within custom epsilon
    CHECK(FSVF::almost_equal_eps<1e-5f>(1.0f, 1.0f + 1e-5f * 0.5f));
    CHECK(FSVF::almost_equal_eps<1e-5f>(1.0f, 1.0f - 1e-5f * 0.5f));

    // Test values at custom epsilon boundary
    CHECK(FSVF::almost_equal_eps<1e-5f>(1.f, 1.f + 0.9e-6f));
    CHECK(FSVF::almost_equal_eps<1e-5f>(1.f, 1.f - 0.9e-6f));

    // Test values beyond custom epsilon
    CHECK_FALSE(FSVF::almost_equal_eps<1e-5f>(1.0f, 1.0f + 1e-5f * 1.1f));
    CHECK_FALSE(FSVF::almost_equal_eps<1e-5f>(1.0f, 1.0f - 1e-5f * 1.1f));

    // Test values within custom epsilon
    CHECK(FSVF::almost_equal_eps<1e-4f>(1.0f, 1.0f + 1e-4f * 0.5f));
    CHECK(FSVF::almost_equal_eps<1e-4f>(1.0f, 1.0f - 1e-4f * 0.5f));

    // Test values at custom epsilon boundary
    CHECK(FSVF::almost_equal_eps<1e-4f>(1.0f, 1.0f + 0.9e-5f));
    CHECK(FSVF::almost_equal_eps<1e-4f>(1.0f, 1.0f - 0.9e-5f));

    // Test values beyond custom epsilon
    CHECK_FALSE(FSVF::almost_equal_eps<1e-4f>(1.0f, 1.0f + 1e-4f * 1.1f));
    CHECK_FALSE(FSVF::almost_equal_eps<1e-4f>(1.0f, 1.0f - 1e-4f * 1.1f));

    // Test values within custom epsilon
    CHECK(FSVF::almost_equal_eps<1e-3f>(1.0f, 1.0f + 1e-3f * 0.5f));
    CHECK(FSVF::almost_equal_eps<1e-3f>(1.0f, 1.0f - 1e-3f * 0.5f));

    // Test values at custom epsilon boundary
    CHECK(FSVF::almost_equal_eps<1e-3f>(1.0f, 1.0f + 0.9e-4f));
    CHECK(FSVF::almost_equal_eps<1e-3f>(1.0f, 1.0f - 0.9e-4f));

    // Test values beyond custom epsilon
    CHECK_FALSE(FSVF::almost_equal_eps<1e-3f>(1.0f, 1.0f + 1e-3f * 1.1f));
    CHECK_FALSE(FSVF::almost_equal_eps<1e-3f>(1.0f, 1.0f - 1e-3f * 1.1f));

    INFO("Testing custom epsilon: 1e-2f");
    // Test values within custom epsilon
    CHECK(FSVF::almost_equal_eps<1e-2f>(1.0f, 1.0f + 1e-2f * 0.5f));
    CHECK(FSVF::almost_equal_eps<1e-2f>(1.0f, 1.0f - 1e-2f * 0.5f));

    // Test values at custom epsilon boundary
    CHECK(FSVF::almost_equal_eps<1e-2f>(1.0f, 1.0f + 1e-2f));
    CHECK(FSVF::almost_equal_eps<1e-2f>(1.0f, 1.0f - 1e-2f));

    // Test values beyond custom epsilon
    CHECK_FALSE(FSVF::almost_equal_eps<1e-2f>(1.0f, 1.0f + 1e-2f * 1.1f));
    CHECK_FALSE(FSVF::almost_equal_eps<1e-2f>(1.0f, 1.0f - 1e-2f * 1.1f));
  }

  SECTION("almost_equal_eps function - special values")
  {
    // Test with infinity
    CHECK(FSVF::almost_equal_eps(
      std::numeric_limits<float>::infinity(),
      std::numeric_limits<float>::infinity()
    ));
    CHECK_FALSE(FSVF::almost_equal_eps(
      std::numeric_limits<float>::infinity(),
      -std::numeric_limits<float>::infinity()
    ));
    CHECK_FALSE(FSVF::almost_equal_eps(
      std::numeric_limits<float>::infinity(),
      1.0f
    ));

    // Test with NaN
    CHECK_FALSE(FSVF::almost_equal_eps(
      std::numeric_limits<float>::quiet_NaN(),
      std::numeric_limits<float>::quiet_NaN()
    ));
    CHECK_FALSE(FSVF::almost_equal_eps(
      std::numeric_limits<float>::quiet_NaN(),
      1.0f
    ));
  }

  SECTION("almost_equal_eps_ulp function")
  {
        // TODO
        CHECK(true);
  }

  SECTION("Comparison functions - consistency checks")
  {
    // Test that almost_equal is equivalent to almost_equal_eps_ulp<2, epsilon>
    constexpr float eps = std::numeric_limits<float>::epsilon();

    std::vector<std::pair<float, float>> test_pairs = {
      {1.0f, 1.0f + eps * 0.5f},
      {1.0f, 1.0f - eps * 0.5f},
      {0.0f, eps * 0.5f},
      {1.0f, FSVF::next_float_up(1.0f)},
      {1.0f, FSVF::next_float_up(FSVF::next_float_up(1.0f))},
      {1.0f, 2.0f},  // Should fail
      {1.0f, 1.0f + eps * 2.0f},  // Should fail
    };

    for (const auto& pair : test_pairs)
    {
      float x = pair.first;
      float y = pair.second;

      bool result1 = FSVF::almost_equal(x, y);
      bool result2 = FSVF::almost_equal_eps_ulp<2, eps>(x, y);

      CHECK(result1 == result2);
    }
  }

  SECTION("Comparison functions - signed zero handling")
  {
    // Test that signed zeros are handled correctly
    CHECK(FSVF::almost_equal(0.0f, -0.0f));
    CHECK(FSVF::almost_equal_ulp<1>(0.0f, -0.0f));
    CHECK(FSVF::almost_equal_eps(0.0f, -0.0f));
    CHECK(FSVF::almost_equal_eps_ulp<1>(0.0f, -0.0f));

    // Test that zero is not equal to very small values beyond threshold
    float small_positive = std::numeric_limits<float>::epsilon() * 2.0f;
    CHECK_FALSE(FSVF::almost_equal_eps(0.0f, small_positive));
    CHECK_FALSE(FSVF::almost_equal_eps(0.0f, -small_positive));
  }

  SECTION("Comparison functions - denormal handling")
  {
    // Test with denormal values
    float denorm = smallest_positive_denormal;
    float denorm_next = FSVF::next_float_up(denorm);

    // Denormals should be handled correctly by ULP-based comparison
    CHECK(FSVF::almost_equal_ulp<1>(denorm, denorm_next));
    CHECK_FALSE(FSVF::almost_equal_ulp<0>(denorm, denorm_next));

    // Denormals should be handled correctly by epsilon-based comparison
    float eps = std::numeric_limits<float>::epsilon();
    if (std::abs(denorm - denorm_next) <= eps)
    {
      CHECK(FSVF::almost_equal_eps(denorm, denorm_next));
    }

    // Combined comparison should work
    CHECK(FSVF::almost_equal_eps_ulp<1>(denorm, denorm_next));
  }
}

TEST_CASE("FloatUtils - Float manipulation functions", "[float_utils]")
{
  SECTION("next_float_up function")
  {
    // Normal numbers
    float x    = 1.0f;
    float next = FSVF::next_float_up(x);
    CHECK(next > x);
    CHECK(FSVF::ulps_distance(x, next) == 1);
    x    = -1.0f;
    next = FSVF::next_float_up(x);
    CHECK(next > x);
    CHECK(FSVF::ulps_distance(x, next) == 1);

    // Zero
    float next_from_zero = FSVF::next_float_up(0.0f);
    CHECK(next_from_zero > 0.0f);
    //CHECK(FSVF::ulps_distance(0.0f, next_from_zero) == 1);

    // Negative zero
    //float next_from_neg_zero = FSVF::next_float_up(-0.0f);
    //CHECK(next_from_neg_zero > -0.0f);
    //CHECK(next_from_neg_zero > 0.0f);

    // Large numbers
    float large      = 1e6f;
    float next_large = FSVF::next_float_up(large);
    CHECK(next_large > large);
  }

  SECTION("next_float_down function")
  {
    // Normal numbers
    float x    = 1.0f;
    float prev = FSVF::next_float_down(x);
    CHECK(prev < x);
    CHECK(FSVF::ulps_distance(prev, x) == 1);
    x    = -1.0f;
    prev = FSVF::next_float_down(x);
    CHECK(prev < x);
    CHECK(FSVF::ulps_distance(x, prev) == 1);

    // Zero
    float prev_to_zero = FSVF::next_float_down(0.0f);
    CHECK(prev_to_zero < 0.0f);

    // Large numbers
    float large      = 1e6f;
    float prev_large = FSVF::next_float_down(large);
    CHECK(prev_large < large);
  }

  SECTION("ulp_value function")
  {
    // Test ULP values for different magnitudes
    float x   = 1.0f;
    float ulp = FSVF::ulp_value(x);
    CHECK(ulp > 0.0f);
    CHECK(ulp == FSVF::next_float_up(x) - x);

    // Test for zero
    float ulp_zero = FSVF::ulp_value(0.0f);
    CHECK(ulp_zero > 0.0f);

    // Test for large numbers
    float large     = 1e6f;
    float ulp_large = FSVF::ulp_value(large);
    CHECK(ulp_large > 0.0f);
    CHECK(ulp_large > ulp);    // ULP should be larger for larger numbers
  }

  SECTION("ulp_value_precise function")
  {
    // Test precise ULP calculation
    float  x           = 1.0f;
    double ulp_precise = FSVF::ulp_value_precise(x);
    CHECK(ulp_precise > 0.0);

    // Should be more precise than the float version
    float ulp_float = FSVF::ulp_value(x);
    CHECK(ulp_precise == Approx(static_cast<double>(ulp_float)).epsilon(1e-10));
  }

  SECTION("ulps_distance function")
  {
    // Test ULP distance calculation
    float x = 1.0f;
    float y = FSVF::next_float_up(x);
    CHECK(FSVF::ulps_distance(x, y) == 1);

    float z = FSVF::next_float_up(y);
    CHECK(FSVF::ulps_distance(x, z) == 2);

    // Test with negative numbers
    float neg_x = -1.0f;
    float neg_y = FSVF::next_float_down(neg_x);
    CHECK(FSVF::ulps_distance(neg_x, neg_y) == 1);

    // Test with zero
    //CHECK(FSVF::ulps_distance(0.0f, FSVF::next_float_up(0.0f)) == 1);
  }
}

TEST_CASE("FloatUtils - Utility functions", "[float_utils]")
{
  SECTION("clamp function")
  {
    // Test normal clamping
    CHECK(FSVF::clamp(0.5f, 0.0f, 1.0f) == 0.5f);
    CHECK(FSVF::clamp(-1.0f, 0.0f, 1.0f) == 0.0f);
    CHECK(FSVF::clamp(2.0f, 0.0f, 1.0f) == 1.0f);

    // Test edge cases
    CHECK(FSVF::clamp(0.0f, 0.0f, 1.0f) == 0.0f);
    CHECK(FSVF::clamp(1.0f, 0.0f, 1.0f) == 1.0f);

    // Test with negative ranges
    CHECK(FSVF::clamp(-2.0f, -1.0f, 1.0f) == -1.0f);
    CHECK(FSVF::clamp(2.0f, -1.0f, 1.0f) == 1.0f);
  }

  SECTION("saturate function")
  {
    // Test normal saturation
    CHECK(FSVF::saturate(0.5f) == 0.5f);
    CHECK(FSVF::saturate(0.0f) == 0.0f);
    CHECK(FSVF::saturate(1.0f) == 1.0f);

    // Test clamping to [0,1]
    CHECK(FSVF::saturate(-1.0f) == 0.0f);
    CHECK(FSVF::saturate(2.0f) == 1.0f);
  }
}

TEST_CASE("FloatUtils - SSE bit manipulation functions", "[float_utils]")
{
  SECTION("SSE bits_to_float and float_to_bits")
  {
    // Test SSE bit manipulation functions
    __m128i test_bits   = _mm_set1_epi32(0x40490FDB);    // π
    __m128  f           = FSVF::bits_to_float(test_bits);
    __m128i result_bits = FSVF::float_to_bits(f);

    // Extract and compare
    uint32_t extracted_bits = _mm_cvtsi128_si32(result_bits);
    CHECK(extracted_bits == 0x40490FDB);

    // Test with different values
    __m128i zero_bits   = _mm_set1_epi32(0x00000000);
    __m128  zero_float  = FSVF::bits_to_float(zero_bits);
    __m128i result_zero = FSVF::float_to_bits(zero_float);
    CHECK(_mm_cvtsi128_si32(result_zero) == 0x00000000);
  }
}

TEST_CASE("FloatUtils - Constants", "[float_utils]")
{
  SECTION("Float constants")
  {
    // Test that constants are correctly defined
    CHECK(FSVF::f32_sign_mask == 0x80000000u);
    CHECK(FSVF::f32_exponent_mask == 0x7F800000u);
    CHECK(FSVF::f32_mantissa_mask == 0x007FFFFFu);
    CHECK(
      FSVF::f32_signed_exponent_mask
      == (FSVF::f32_sign_mask | FSVF::f32_exponent_mask)
    );

    // Test special float constants
    CHECK(FSVF::negative_zero_float == -0.0f);
    CHECK(FSVF::negative_zero_float == 0.0f);
    CHECK(
      FSVF::float_to_bits(FSVF::negative_zero_float) == FSVF::f32_sign_mask
    );

    CHECK(FSVF::smallest_normal_float > 0.0f);
    CHECK(FSVF::smallest_normal_float == FSVF::bits_to_float(0x00800000u));

    CHECK(FSVF::largest_negative_normal_float < 0.0f);
    CHECK(
      FSVF::largest_negative_normal_float == FSVF::bits_to_float(0x80800000u)
    );
  }
}

TEST_CASE("FloatUtils - Type constraints", "[float_utils]")
{
  SECTION("Float concept constraint")
  {
    // These should compile
    float f = 1.0f;
    FSVF::float_to_bits(f);
    FSVF::mantissa(f);
    FSVF::exponent(f);
    FSVF::is_finite(f);

    // These should not compile (uncomment to test)
    // double d = 1.0;
    // FSVF::float_to_bits(d);  // Should fail
    // FSVF::mantissa(d);       // Should fail
  }

  SECTION("U32 concept constraint")
  {
    // These should compile
    uint32_t u = 0x40490FDB;
    FSVF::bits_to_float(u);

    // These should not compile (uncomment to test)
    // int i = 42;
    // FSVF::bits_to_float(i);  // Should fail
  }
}

TEST_CASE("FloatUtils - Edge cases and stress testing", "[float_utils]")
{
  SECTION("Comprehensive random value testing")
  {
    // Test with many random values across different ranges
    for (int i = 0; i < 1000; ++i)
    {
      float x = random_float();

      // Test bit manipulation round-trip
      uint32_t bits          = FSVF::float_to_bits(x);
      float    reconstructed = FSVF::bits_to_float(bits);
      CHECK(x == reconstructed);

      // Test classification functions consistency
      bool is_inf_or_nan_val = FSVF::is_inf_or_nan(x);
      bool is_finite_val     = FSVF::is_finite(x);
      bool is_inf_val        = FSVF::is_inf(x);
      bool is_nan_val        = FSVF::is_nan(x);
      bool is_denormal_val   = FSVF::is_denormal(x);

      // Consistency checks
      CHECK(is_inf_or_nan_val == !is_finite_val);
      CHECK(is_inf_or_nan_val == (is_inf_val || is_nan_val));
      CHECK(!(is_inf_val && is_nan_val));    // Can't be both inf and NaN

      // Test bit manipulation functions
      uint32_t mantissa_bits = FSVF::mantissa(x);
      uint32_t exp_bits      = FSVF::exponent(x);
      uint32_t sign_bits     = FSVF::sign_bit(x);

      // Verify bit manipulation consistency
      CHECK((mantissa_bits & FSVF::f32_mantissa_mask) == mantissa_bits);
      CHECK((exp_bits & FSVF::f32_exponent_mask) == exp_bits);
      CHECK((sign_bits & FSVF::f32_sign_mask) == sign_bits);

      // Test ULP functions for finite values
      if (FSVF::is_finite(x))
      {
        float next = FSVF::next_float_up(x);
        float prev = FSVF::next_float_down(x);
        float ulp  = FSVF::ulp_value(x);

        CHECK(next > x);
        CHECK(prev < x);
        CHECK(ulp > 0.0f);
        CHECK(ulp == (next - x));

        // Test ULP distance
        CHECK(FSVF::ulps_distance(x, next) == 1);
        CHECK(FSVF::ulps_distance(prev, x) == 1);
      }
    }
  }

  SECTION("Random value testing with different ranges")
  {
    // Test with random values in different magnitude ranges
    std::vector<std::pair<float, float>> ranges = {
      {-1e-10f, 1e-10f},      // Very small values
      {-1e-5f, 1e-5f},        // Small values
      {-1.0f, 1.0f},          // Unit range
      {-1e5f, 1e5f},          // Large values
      {-1e10f, 1e10f},        // Very large values
      {0.0f, 1e10f},          // Positive only
      {-1e10f, 0.0f},         // Negative only
    };

    for (const auto& range : ranges)
    {
      for (int i = 0; i < 100; ++i)
      {
        float x = random_float(range.first, range.second);

        // Test bit manipulation round-trip
        uint32_t bits          = FSVF::float_to_bits(x);
        float    reconstructed = FSVF::bits_to_float(bits);
        CHECK(x == reconstructed);

        // Test classification consistency
        bool is_inf_or_nan_val = FSVF::is_inf_or_nan(x);
        bool is_finite_val     = FSVF::is_finite(x);
        CHECK(is_inf_or_nan_val == !is_finite_val);

        // Test ULP functions for finite values
        if (FSVF::is_finite(x))
        {
          float ulp = FSVF::ulp_value(x);
          CHECK(ulp > 0.0f);

          // Test that ULP value is reasonable for the magnitude
          if (std::abs(x) > 1e-6f)
          {
            CHECK(ulp <= std::abs(x) * 1e-5f); // ULP should be much smaller than the value
          }
        }
      }
    }
  }

  SECTION("Random bit pattern testing")
  {
    // Test with random bit patterns to catch edge cases
    for (int i = 0; i < 500; ++i)
    {
      uint32_t random_bits_val = random_bits();
      float x = FSVF::bits_to_float(random_bits_val);

      // Test round-trip conversion
      uint32_t reconstructed_bits = FSVF::float_to_bits(x);
      CHECK(reconstructed_bits == random_bits_val);

      // Test all classification functions
      bool is_inf_or_nan_val = FSVF::is_inf_or_nan(x);
      bool is_finite_val     = FSVF::is_finite(x);
      bool is_inf_val        = FSVF::is_inf(x);
      bool is_nan_val        = FSVF::is_nan(x);
      bool is_denormal_val   = FSVF::is_denormal(x);

      // Consistency checks
      CHECK(is_inf_or_nan_val == !is_finite_val);
      CHECK(is_inf_or_nan_val == (is_inf_val || is_nan_val));
      CHECK(!(is_inf_val && is_nan_val));

      // Test bit manipulation functions
      uint32_t mantissa_bits = FSVF::mantissa(x);
      uint32_t exp_bits      = FSVF::exponent(x);
      uint32_t sign_bits     = FSVF::sign_bit(x);

      // Verify bit manipulation consistency
      CHECK((mantissa_bits & FSVF::f32_mantissa_mask) == mantissa_bits);
      CHECK((exp_bits & FSVF::f32_exponent_mask) == exp_bits);
      CHECK((sign_bits & FSVF::f32_sign_mask) == sign_bits);
    }
  }

  SECTION("Random comparison testing")
  {
    // Test comparison functions with random pairs
    for (int i = 0; i < 500; ++i)
    {
      float x = random_float();
      float y = random_float();

      // Test almost_equal with random values
      if (x == y)
      {
        CHECK(FSVF::almost_equal(x, y));
      }

      // Test ULP-based comparison
      if (FSVF::is_finite(x) && FSVF::is_finite(y))
      {
        uint32_t ulp_dist = FSVF::ulps_distance(x, y);

        // Test almost_equal_ulp with different thresholds
        if (ulp_dist <= 1)
        {
          CHECK(FSVF::almost_equal_ulp<1>(x, y));
        }
        if (ulp_dist <= 2)
        {
          CHECK(FSVF::almost_equal_ulp<2>(x, y));
        }
        if (ulp_dist <= 5)
        {
          CHECK(FSVF::almost_equal_ulp<5>(x, y));
        }

        // Test epsilon-based comparison
        float eps = std::numeric_limits<float>::epsilon();
        if (std::abs(x - y) <= eps)
        {
          CHECK(FSVF::almost_equal_eps(x, y));
        }
      }
    }
  }

  SECTION("Random utility function testing")
  {
    // Test clamp and saturate with random values
    for (int i = 0; i < 200; ++i)
    {
      float value = random_float(-10.0f, 10.0f);
      float min_val = random_float(-5.0f, 0.0f);
      float max_val = random_float(0.0f, 5.0f);

      // Ensure min <= max
      if (min_val > max_val) std::swap(min_val, max_val);

      float clamped = FSVF::clamp(value, min_val, max_val);
      CHECK(clamped >= min_val);
      CHECK(clamped <= max_val);

      // Test saturate
      float saturated = FSVF::saturate(value);
      CHECK(saturated >= 0.0f);
      CHECK(saturated <= 1.0f);
    }
  }

  SECTION("Special value testing")
  {
    // Test with all special float values
    std::vector<float> special_values
      = { 0.0f,
          -0.0f,
          std::numeric_limits<float>::infinity(),
          -std::numeric_limits<float>::infinity(),
          std::numeric_limits<float>::quiet_NaN(),
          std::numeric_limits<float>::signaling_NaN(),
          std::numeric_limits<float>::min(),
          std::numeric_limits<float>::max(),
          smallest_negative_denormal,
          smallest_positive_denormal };

    for (float x: special_values)
    {
      // Test bit manipulation round-trip (except NaN)
      if (!FSVF::is_nan(x))
      {
        uint32_t bits          = FSVF::float_to_bits(x);
        float    reconstructed = FSVF::bits_to_float(bits);
        CHECK(x == reconstructed);
      }

      // Test classification consistency
      bool is_inf_or_nan_val = FSVF::is_inf_or_nan(x);
      bool is_finite_val     = FSVF::is_finite(x);
      bool is_inf_val        = FSVF::is_inf(x);
      bool is_nan_val        = FSVF::is_nan(x);

      CHECK(is_inf_or_nan_val == !is_finite_val);
      CHECK(is_inf_or_nan_val == (is_inf_val || is_nan_val));
      CHECK(!(is_inf_val && is_nan_val));
    }
  }

  SECTION("ULP distance stress testing")
  {
    // Test ULP distance with various ranges
    std::vector<float> test_values = { /* 0.0f, */  1.0f,   10.0f,   100.0f,  1000.0f,
                                       /* -1.0f ,*/ -10.0f, -100.0f, -1000.0f };

    for (float x: test_values)
    {
      // Test consecutive ULP distances
      float next = FSVF::next_float_up(x);
      float prev = FSVF::next_float_down(x);

      CHECK(FSVF::ulps_distance(x, next) == 1);
      CHECK(FSVF::ulps_distance(prev, x) == 1);
      CHECK(FSVF::ulps_distance(prev, next) == 2);

      INFO("test_value: " << x);

      // Test ULP value consistency
      double ulp = FSVF::ulp_value_precise(x);
      CHECK(ulp == (next - x));
      CHECK(ulp > 0.0f);
    }

    // Test ULP distance with random values
    for (int i = 0; i < 200; ++i)
    {
      float x = random_float();
      if (FSVF::is_finite(x))
      {
        float next = FSVF::next_float_up(x);
        float prev = FSVF::next_float_down(x);

        CHECK(FSVF::ulps_distance(x, next) == 1);
        CHECK(FSVF::ulps_distance(prev, x) == 1);

        // Test that ULP distance is symmetric
        CHECK(FSVF::ulps_distance(x, next) == FSVF::ulps_distance(next, x));
        CHECK(FSVF::ulps_distance(x, prev) == FSVF::ulps_distance(prev, x));
      }
    }
  }
}
