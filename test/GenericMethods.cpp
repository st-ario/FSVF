#include <array>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cmath>
#include <FSVF.h>
#include <GenericMethods.hpp>
#include <immintrin.h>
#include <vector>

using namespace FSVF;
using Catch::Approx;

// TODO!!!
// Most of the tests are AI-generated and require manual double-checking and cleanup

// Helper macros for component checks
#define CHECK_VEC4_COMPONENTS(vec, x, y, z, w) \
    { \
        auto c = (vec).components(); \
        CHECK(c[0] == Approx(x)); \
        CHECK(c[1] == Approx(y)); \
        CHECK(c[2] == Approx(z)); \
        CHECK(c[3] == Approx(w)); \
    }
#define CHECK_LVEC3_COMPONENTS(vec, x, y, z) \
    { \
        auto c = (vec).components(); \
        CHECK(c[0] == Approx(x)); \
        CHECK(c[1] == Approx(y)); \
        CHECK(c[2] == Approx(z)); \
    }

#define CHECK_BITWISE_EQ(x, y) \
    { \
    CHECK(std::bit_cast<uint32_t>(x) == std::bit_cast<uint32_t>(y)); \
    }

#define CHECK_BITWISE_NEQ(x, y) \
    { \
    CHECK(std::bit_cast<uint32_t>(x) != std::bit_cast<uint32_t>(y)); \
    }

TEST_CASE(
  "FSVF::sum computes the sum of all vector components",
  "[GenericMethods]"
)
{
  Vec4  v4{ 1.0f, 2.0f, 3.0f, 4.0f };
  LVec3 v3{ 5.0f, 6.0f, 7.0f };
  CHECK(sum(v4) == Approx(10.0f));
  CHECK(sum(v3) == Approx(18.0f));
}

TEST_CASE(
  "FSVF::prod computes the product of all vector components",
  "[GenericMethods]"
)
{
  Vec4  v4{ 2.0f, 3.0f, 4.0f, 5.0f };
  LVec3 v3{ 2.0f, 3.0f, 4.0f };
  CHECK(prod(v4) == Approx(120.0f));
  CHECK(prod(v3) == Approx(24.0f));
}

TEST_CASE("FSVF::normalize returns a unit vector", "[GenericMethods]")
{
  Vec4  v4{ 3.0f, 0.0f, 4.0f, 0.0f };
  auto  n4 = normalize(v4);
  float l4 = len(n4);
  CHECK(l4 == Approx(1.0f));
  CHECK_VEC4_COMPONENTS(n4, 0.6f, 0.0f, 0.8f, 0.0f);

  LVec3 v3{ 0.0f, 12.0f, 5.0f };
  auto  n3 = normalize(v3);
  float l3 = len(n3);
  CHECK(l3 == Approx(1.0f));
  CHECK_LVEC3_COMPONENTS(n3, 0.0f, 0.923077f, 0.384615f);
}

TEST_CASE("FSVF::len computes the Euclidean length", "[GenericMethods]")
{
  Vec4  v4{ 1.0f, 2.0f, 2.0f, 1.0f };
  LVec3 v3{ 2.0f, 3.0f, 6.0f };
  CHECK(len(v4) == Approx(std::sqrt(10.0f)));
  CHECK(len(v3) == Approx(7.0f));
}

TEST_CASE("FSVF::len_squared computes the squared length", "[GenericMethods]")
{
  Vec4  v4{ 1.0f, 2.0f, 2.0f, 1.0f };
  LVec3 v3{ 2.0f, 3.0f, 6.0f };
  CHECK(len_squared(v4) == Approx(10.0f));
  CHECK(len_squared(v3) == Approx(49.0f));
}

TEST_CASE(
  "FSVF::max_component_index and min_component_index",
  "[GenericMethods]"
)
{
  Vec4  v4{ 1.0f, 5.0f, -3.0f, 2.0f };
  LVec3 v3{ 7.0f, 2.0f, 9.0f };
  CHECK(max_component_index(v4) == 1);    // 5.0 at index 1
  CHECK(min_component_index(v4) == 2);    // -3.0 at index 2
  CHECK(max_component_index(v3) == 2);    // 9.0 at index 2
  CHECK(min_component_index(v3) == 1);    // 2.0 at index 1
}

TEST_CASE(
  "FSVF::max_component_value and min_component_value",
  "[GenericMethods]"
)
{
  Vec4  v4{ 1.0f, 5.0f, -3.0f, 2.0f };
  LVec3 v3{ 7.0f, 2.0f, 9.0f };
  auto  max_v4 = max_component_value(v4);
  auto  min_v4 = min_component_value(v4);
  auto  max_v3 = max_component_value(v3);
  auto  min_v3 = min_component_value(v3);
  CHECK_VEC4_COMPONENTS(max_v4, 5.0f, 5.0f, 5.0f, 5.0f);
  CHECK_VEC4_COMPONENTS(min_v4, -3.0f, -3.0f, -3.0f, -3.0f);
  CHECK_VEC4_COMPONENTS(max_v3, 9.0f, 9.0f, 9.0f, 9.0f);
  CHECK_VEC4_COMPONENTS(min_v3, 2.0f, 2.0f, 2.0f, 2.0f);
}

TEST_CASE(
  "FSVF::max_component_value_f32 and min_component_value_f32",
  "[GenericMethods]"
)
{
  Vec4  v4{ 1.0f, 5.0f, -3.0f, 2.0f };
  LVec3 v3{ 7.0f, 2.0f, 9.0f };
  CHECK(max_component_value_f32(v4) == Approx(5.0f));
  CHECK(min_component_value_f32(v4) == Approx(-3.0f));
  CHECK(max_component_value_f32(v3) == Approx(9.0f));
  CHECK(min_component_value_f32(v3) == Approx(2.0f));
}

TEST_CASE("FSVF::sort and reverse_sort sort the components", "[GenericMethods]")
{
  Vec4  v4{ 3.0f, 1.0f, 4.0f, 2.0f };
  LVec3 v3{ 7.0f, 2.0f, 5.0f };
  auto  sorted_v4    = sort(v4);
  auto  revsorted_v4 = reverse_sort(v4);
  auto  sorted_v3    = sort(v3);
  auto  revsorted_v3 = reverse_sort(v3);
  CHECK_VEC4_COMPONENTS(sorted_v4, 1.0f, 2.0f, 3.0f, 4.0f);
  CHECK_VEC4_COMPONENTS(revsorted_v4, 4.0f, 3.0f, 2.0f, 1.0f);
  CHECK_LVEC3_COMPONENTS(sorted_v3, 2.0f, 5.0f, 7.0f);
  CHECK_LVEC3_COMPONENTS(revsorted_v3, 7.0f, 5.0f, 2.0f);
}

TEST_CASE(
  "FSVF::floor, ceil, abs, sqrt, and rsqrt are component-wise",
  "[GenericMethods]"
)
{
  Vec4  v4{ -1.5f, 2.3f, 4.9f, -3.1f };
  LVec3 v3{ -2.7f, 3.2f, -4.8f };
  auto  v4_floor = floor(v4);
  auto  v3_floor = floor(v3);
  auto  v4_ceil  = ceil(v4);
  auto  v3_ceil  = ceil(v3);
  auto  v4_abs   = abs(v4);
  auto  v3_abs   = abs(v3);
  auto  v4_sqrt  = sqrt(Vec4{ 1.0f, 4.0f, 9.0f, 16.0f });
  auto  v3_sqrt  = sqrt(LVec3{ 1.0f, 4.0f, 9.0f });
  auto  v4_rsqrt = rsqrt(Vec4{ 4.0f, 9.0f, 16.0f, 25.0f });
  auto  v3_rsqrt = rsqrt(LVec3{ 4.0f, 9.0f, 16.0f });
  CHECK_VEC4_COMPONENTS(v4_floor, -2.0f, 2.0f, 4.0f, -4.0f);
  CHECK_LVEC3_COMPONENTS(v3_floor, -3.0f, 3.0f, -5.0f);
  CHECK_VEC4_COMPONENTS(v4_ceil, -1.0f, 3.0f, 5.0f, -3.0f);
  CHECK_LVEC3_COMPONENTS(v3_ceil, -2.0f, 4.0f, -4.0f);
  CHECK_VEC4_COMPONENTS(v4_abs, 1.5f, 2.3f, 4.9f, 3.1f);
  CHECK_LVEC3_COMPONENTS(v3_abs, 2.7f, 3.2f, 4.8f);
  CHECK_VEC4_COMPONENTS(v4_sqrt, 1.0f, 2.0f, 3.0f, 4.0f);
  CHECK_LVEC3_COMPONENTS(v3_sqrt, 1.0f, 2.0f, 3.0f);
  CHECK_VEC4_COMPONENTS(
    v4_rsqrt,
    Approx(0.5f).epsilon(0.01),
    Approx(1.0f / 3.0f).epsilon(0.01),
    Approx(0.25f).epsilon(0.01),
    Approx(0.2f).epsilon(0.01)
  );
  CHECK_LVEC3_COMPONENTS(
    v3_rsqrt,
    Approx(0.5f).epsilon(0.01),
    Approx(1.0f / 3.0f).epsilon(0.01),
    Approx(0.25f).epsilon(0.01)
  );
}

TEST_CASE(
  "FSVF::merge_max and merge_min are component-wise max/min",
  "[GenericMethods]"
)
{
  Vec4 v1{ 1.0f, 5.0f, 3.0f, 7.0f };
  Vec4 v2{ 2.0f, 4.0f, 6.0f, 1.0f };
  auto max_v = merge_max(v1, v2);
  auto min_v = merge_min(v1, v2);
  CHECK_VEC4_COMPONENTS(max_v, 2.0f, 5.0f, 6.0f, 7.0f);
  CHECK_VEC4_COMPONENTS(min_v, 1.0f, 4.0f, 3.0f, 1.0f);
  LVec3 v3{ 1.0f, 8.0f, 2.0f };
  LVec3 v4{ 3.0f, 2.0f, 5.0f };
  auto  max_v3 = merge_max(v3, v4);
  auto  min_v3 = merge_min(v3, v4);
  CHECK_LVEC3_COMPONENTS(max_v3, 3.0f, 8.0f, 5.0f);
  CHECK_LVEC3_COMPONENTS(min_v3, 1.0f, 2.0f, 2.0f);
}

TEST_CASE("FSVF::dot computes the dot product", "[GenericMethods]")
{
  Vec4 v1{ 1.0f, 2.0f, 3.0f, 4.0f };
  Vec4 v2{ 5.0f, 6.0f, 7.0f, 8.0f };
  CHECK(dot(v1, v2) == Approx(70.0f));
  LVec3 v3{ 1.0f, 2.0f, 3.0f };
  LVec3 v4{ 4.0f, 5.0f, 6.0f };
  CHECK(dot(v3, v4) == Approx(32.0f));
}

TEST_CASE(
  "FSVF::cross computes the cross product for LVec3",
  "[GenericMethods]"
)
{
  LVec3 v1{ 1.0f, 2.0f, 3.0f };
  LVec3 v2{ 4.0f, 5.0f, 6.0f };
  auto  cr = cross(v1, v2);
  CHECK_LVEC3_COMPONENTS(cr, -3.0f, 6.0f, -3.0f);
}

// TODO
//TEST_CASE("FSVF::parallel_sum and parallel_prod", "[GenericMethods]") {
//    Vec4 v1{1.0f, 2.0f, 3.0f, 4.0f};
//    Vec4 v2{5.0f, 6.0f, 7.0f, 8.0f};
//    Vec4 v3{9.0f, 10.0f, 11.0f, 12.0f};
//    Vec4 v4{13.0f, 14.0f, 15.0f, 16.0f};
//    auto psum = parallel_sum(v1, v2, v3, v4);
//    auto pprod = parallel_prod(v1, v2, v3, v4);
//    CHECK_VEC4_COMPONENTS(psum, 10.0f, 26.0f, 42.0f, 58.0f);
//    CHECK_VEC4_COMPONENTS(pprod, 24.0f, 1680.0f, 3564.0f, 12288.0f);
//}

TEST_CASE(
  "FSVF::blend_with_mask blends components according to mask",
  "[GenericMethods]"
)
{
  Vec4 a{ 1.0f, 2.0f, 3.0f, 4.0f };
  Vec4 b{ 5.0f, 6.0f, 7.0f, 8.0f };
  // Mask 0b1010: take b.x, a.y, b.z, a.w
  auto blended = blend_with_mask<0b1010>(a, b);
  CHECK_VEC4_COMPONENTS(blended, 5.0f, 2.0f, 7.0f, 4.0f);
}

TEST_CASE(
  "FSVF::clamp clamps each component between min and max",
  "[GenericMethods]"
)
{
  Vec4 v{ 1.0f, 5.0f, -3.0f, 10.0f };
  Vec4 minv{ 0.0f, 2.0f, 0.0f, 8.0f };
  Vec4 maxv{ 2.0f, 4.0f, 1.0f, 9.0f };
  auto clamped = clamp(v, minv, maxv);
  CHECK_VEC4_COMPONENTS(clamped, 1.0f, 4.0f, 0.0f, 9.0f);
}

TEST_CASE("FSVF::all, any, none for bitmask vectors", "[GenericMethods]")
{
  float ones = bits_to_float(0xFFFF'FFFF);
  float zero = 0.0f;

  // Test all<4> - all components should be ones
  Vec4 all_ones{ ones, ones, ones, ones };
  Vec4 some_ones{ ones, zero, ones, zero };
  Vec4 all_zero{ zero, zero, zero, zero };
  Vec4 mixed{ ones, zero, ones, ones };

  CHECK(all<4>(all_ones));
  CHECK_FALSE(all<4>(some_ones));
  CHECK_FALSE(all<4>(all_zero));
  CHECK_FALSE(all<4>(mixed));

  // Test all<3> - first 3 components should be ones (4th is ignored)
  Vec4 three_ones{ ones, ones, ones, zero };
  Vec4 two_ones{ ones, ones, zero, ones };
  CHECK(all<3>(three_ones));
  CHECK_FALSE(all<3>(two_ones));
  CHECK_FALSE(all<3>(all_zero));

  // Test any<4> - at least one component should be ones
  CHECK(any<4>(all_ones));
  CHECK(any<4>(some_ones));
  CHECK_FALSE(any<4>(all_zero));
  CHECK(any<4>(mixed));

  // Test any<3> - at least one of first 3 components should be ones
  CHECK(any<3>(three_ones));
  CHECK(any<3>(two_ones));
  CHECK_FALSE(any<3>(Vec4{ zero, zero, zero, ones }));

  // Test none<4> - no component should be ones
  CHECK(none<4>(all_zero));
  CHECK_FALSE(none<4>(all_ones));
  CHECK_FALSE(none<4>(some_ones));
  CHECK_FALSE(none<4>(mixed));

  // Test none<3> - none of first 3 components should be ones
  CHECK(none<3>(Vec4{ zero, zero, zero, ones }));
  CHECK_FALSE(none<3>(three_ones));
  CHECK_FALSE(none<3>(two_ones));
}

TEST_CASE("FSVF::and_not computes v AND (NOT w)", "[GenericMethods]")
{
  float ones = bits_to_float(0xFFFF'FFFF);
  float zero = 0.0f;

  // Test case 1: v has some ones, w has zeros -> result should have ones where v has ones and w has zeros
  Vec4 v1{ ones, zero, ones, zero };
  Vec4 w1{ zero, zero, ones, ones };
  auto res1 = and_not(v1, w1);
  CHECK_BITWISE_EQ(res1.components()[0], ones);    // 1 & ~0 = 1
  CHECK_BITWISE_EQ(res1.components()[1], zero);    // 0 & ~0 = 0
  CHECK_BITWISE_EQ(res1.components()[2], zero);    // 1 & ~1 = 0
  CHECK_BITWISE_EQ(res1.components()[3], zero);    // 0 & ~1 = 0

  // Test case 2: v and w are identical -> result should be all zeros
  Vec4 v2{ ones, zero, ones, zero };
  Vec4 w2{ ones, zero, ones, zero };
  auto res2 = and_not(v2, w2);
  CHECK_BITWISE_EQ(res2.components()[0], zero);
  CHECK_BITWISE_EQ(res2.components()[1], zero);
  CHECK_BITWISE_EQ(res2.components()[2], zero);
  CHECK_BITWISE_EQ(res2.components()[3], zero);

  // Test case 3: w is all ones -> result should be all zeros
  Vec4 v3{ ones, zero, ones, zero };
  Vec4 w3{ ones, ones, ones, ones };
  auto res3 = and_not(v3, w3);
  CHECK_BITWISE_EQ(res3.components()[0], zero);
  CHECK_BITWISE_EQ(res3.components()[1], zero);
  CHECK_BITWISE_EQ(res3.components()[2], zero);
  CHECK_BITWISE_EQ(res3.components()[3], zero);

  // Test case 4: w is all zeros -> result should be same as v
  Vec4 v4{ ones, zero, ones, zero };
  Vec4 w4{ zero, zero, zero, zero };
  auto res4 = and_not(v4, w4);
  CHECK_BITWISE_EQ(res4.components()[0], ones);
  CHECK_BITWISE_EQ(res4.components()[1], zero);
  CHECK_BITWISE_EQ(res4.components()[2], ones);
  CHECK_BITWISE_EQ(res4.components()[3], zero);
}

TEST_CASE(
  "FSVF::bitwise_equal returns true for identical vectors",
  "[GenericMethods]"
)
{
  Vec4 v1{ 1.0f, 2.0f, 3.0f, 4.0f };
  Vec4 v2{ 1.0f, 2.0f, 3.0f, 4.0f };
  Vec4 v3{ 1.0f, 2.0f, 3.0f, 5.0f };
  Vec4 v4{ 1.0f, 2.0f, 5.0f, 4.0f };
  Vec4 v5{ 1.0f, 5.0f, 3.0f, 4.0f };
  Vec4 v6{ 5.0f, 2.0f, 3.0f, 4.0f };

  CHECK(bitwise_equal(v1, v2));
  CHECK_FALSE(bitwise_equal(v1, v3));    // different w component
  CHECK_FALSE(bitwise_equal(v1, v4));    // different z component
  CHECK_FALSE(bitwise_equal(v1, v5));    // different y component
  CHECK_FALSE(bitwise_equal(v1, v6));    // different x component

  // Test with LVec3
  LVec3 lv1{ 1.0f, 2.0f, 3.0f };
  LVec3 lv2{ 1.0f, 2.0f, 3.0f };
  LVec3 lv3{ 1.0f, 2.0f, 4.0f };

  CHECK(bitwise_equal(lv1, lv2));
  CHECK_FALSE(bitwise_equal(lv1, lv3));
}

TEST_CASE(
  "FSVF::shuffle (Shuf4 and Shuf3) permutes components",
  "[GenericMethods]"
)
{
  Vec4 v{ 1.0f, 2.0f, 3.0f, 4.0f };
  auto s4 = shuffle<Shuf4::wzyx>(v);    // should reverse
  CHECK_VEC4_COMPONENTS(s4, 4.0f, 3.0f, 2.0f, 1.0f);
  LVec3 v3{ 5.0f, 6.0f, 7.0f };
  auto  s3 = shuffle<Shuf3::zyx>(v3);    // should reverse
  CHECK_LVEC3_COMPONENTS(s3, 7.0f, 6.0f, 5.0f);
}

TEST_CASE("FSVF::BitMasks functions", "[GenericMethods]")
{
  float ones = bits_to_float(0xFFFF'FFFF);
  float zero = 0.0f;

  Vec4 v1{ 1.0f, 2.0f, 3.0f, 4.0f };
  Vec4 v2{ 1.0f, 2.0f, 0.0f, 4.0f };

  // Test cmp_equal_f32
  auto eq = BitMasks::cmp_equal_f32(v1, v2);
  CHECK_BITWISE_EQ(eq.components()[0], ones);    // 1.0f == 1.0f
  CHECK_BITWISE_EQ(eq.components()[1], ones);    // 2.0f == 2.0f
  CHECK_BITWISE_EQ(eq.components()[2], zero);    // 3.0f != 0.0f
  CHECK_BITWISE_EQ(eq.components()[3], ones);    // 4.0f == 4.0f

  // Test cmp_not_equal_f32
  auto neq = BitMasks::cmp_not_equal_f32(v1, v2);
  CHECK_BITWISE_EQ(neq.components()[0], zero);    // 1.0f == 1.0f
  CHECK_BITWISE_EQ(neq.components()[1], zero);    // 2.0f == 2.0f
  CHECK_BITWISE_EQ(neq.components()[2], ones);    // 3.0f != 0.0f
  CHECK_BITWISE_EQ(neq.components()[3], zero);    // 4.0f == 4.0f

  // Test cmp_equal_bitwise
  auto beq = BitMasks::cmp_equal_bitwise(v1, v2);
  CHECK_BITWISE_EQ(beq.components()[0], ones);    // 1.0f == 1.0f
  CHECK_BITWISE_EQ(beq.components()[1], ones);    // 2.0f == 2.0f
  CHECK_BITWISE_EQ(beq.components()[2], zero);    // 3.0f != 0.0f
  CHECK_BITWISE_EQ(beq.components()[3], ones);    // 4.0f == 4.0f

  // Test all_ones_bitwise and all_zeroes_bitwise
  CHECK(BitMasks::all_ones_bitwise(Vec4{ ones, ones, ones, ones }));
  CHECK_FALSE(BitMasks::all_ones_bitwise(Vec4{ ones, zero, ones, ones }));
  CHECK(BitMasks::all_zeroes_bitwise(Vec4{ zero, zero, zero, zero }));
  CHECK_FALSE(BitMasks::all_zeroes_bitwise(Vec4{ zero, ones, zero, zero }));

  // Test hi_bits - extracts sign bits of 32-bit floats (4 bits total)
  // _mm_movemask_ps extracts the sign bit of each 32-bit float
  Vec4 pos_32bit_vec{ 1.0f, 2.0f, 3.0f, 4.0f };
  Vec4 neg_32bit_vec{ -1.0f, -2.0f, -3.0f, -4.0f };
  Vec4 mixed_32bit_vec1{ -1.0f, -2.0f, 3.0f, 4.0f };
  Vec4 mixed_32bit_vec2{ -1.0f, 2.0f, 3.0f, -4.0f };
  Vec4 mixed_32bit_vec3{ -1.0f, 2.0f, -3.0f, -4.0f };
  Vec4 mixed_32bit_vec4{ -1.0f, -2.0f, 3.0f, -4.0f };

  uint8_t pos_hi32     = BitMasks::hi_bits(pos_32bit_vec);
  uint8_t neg_hi32     = BitMasks::hi_bits(neg_32bit_vec);
  uint8_t mixed_hi32_1 = BitMasks::hi_bits(mixed_32bit_vec1);
  uint8_t mixed_hi32_2 = BitMasks::hi_bits(mixed_32bit_vec2);
  uint8_t mixed_hi32_3 = BitMasks::hi_bits(mixed_32bit_vec3);
  uint8_t mixed_hi32_4 = BitMasks::hi_bits(mixed_32bit_vec4);

  CHECK(pos_hi32 == 0);
  CHECK(neg_hi32 == 0b1111);
  CHECK(mixed_hi32_1 == 0b1100);
  CHECK(mixed_hi32_2 == 0b1001);
  CHECK(mixed_hi32_3 == 0b1011);
  CHECK(mixed_hi32_4 == 0b1101);
}
