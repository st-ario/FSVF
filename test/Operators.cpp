#include "TestUtils.h"

using namespace FSVF;
using Catch::Approx;

TEST_CASE("Boolean equality operators", "[Operators]")
{
  // allow few bits of difference between min and max,
  // to increase the chances of getting equal values
  constexpr auto min = bits_to_float(0x0F02'B60u);
  constexpr auto max = bits_to_float(0x0F02'B68u);

  for (int i = 0; i < 100; ++i)
  {
    Vec4 v1 = rnd4(min, max);
    Vec4 v2 = rnd4(min, max);
    LVec3 v3 = rnd3(min, max);
    LVec3 v4 = rnd3(min, max);

    CHECK(v1 == v1);
    CHECK(v3 == v3);
    CHECK(!(v1 != v1));
    CHECK(!(v3 != v3));

    CHECK((v1 == v2) == all<4>(BitMasks::cmp_equal_f32(v1, v2)));
    CHECK((v3 == v4) == all<3>(BitMasks::cmp_equal_f32(v3, v4)));

    CHECK((v1 == v2) == (v2 == v1));
    CHECK((v1 == v3) == (v3 == v1));
    CHECK((v3 == v4) == (v4 == v3));
    CHECK((v1 == v2) == !(v2 != v1));
    CHECK((v1 == v3) == !(v3 != v1));
    CHECK((v3 == v4) == !(v4 != v3));

    CHECK(blend_with_mask<0b0001>(v1, Vec4{0.f}) != v1);
    CHECK(blend_with_mask<0b0010>(v1, Vec4{0.f}) != v1);
    CHECK(blend_with_mask<0b0011>(v1, Vec4{0.f}) != v1);
    CHECK(blend_with_mask<0b0100>(v1, Vec4{0.f}) != v1);
    CHECK(blend_with_mask<0b0101>(v1, Vec4{0.f}) != v1);
    CHECK(blend_with_mask<0b0110>(v1, Vec4{0.f}) != v1);
    CHECK(blend_with_mask<0b0111>(v1, Vec4{0.f}) != v1);
    CHECK(blend_with_mask<0b1000>(v1, Vec4{0.f}) != v1);
    CHECK(blend_with_mask<0b1001>(v1, Vec4{0.f}) != v1);
    CHECK(blend_with_mask<0b1010>(v1, Vec4{0.f}) != v1);
    CHECK(blend_with_mask<0b1011>(v1, Vec4{0.f}) != v1);
    CHECK(blend_with_mask<0b1100>(v1, Vec4{0.f}) != v1);
    CHECK(blend_with_mask<0b1101>(v1, Vec4{0.f}) != v1);
    CHECK(blend_with_mask<0b1110>(v1, Vec4{0.f}) != v1);

    LVec3 alt1 = LVec3{v1};
    Vec4 alt2 = blend_with_mask<0b0001>(v1, Vec4{0.f});

    CHECK(alt2 == alt1);
    CHECK(alt1 == alt2);
  }
}

TEST_CASE("Ordering: >=, <=, >, <", "[Operators]")
{
  for (int i = 0; i < 100; ++i)
  {
    {
      Vec4 v1 = rnd4();
      Vec4 v2 = rnd4();

      CHECK(all<4>(v1 <= v1));
      CHECK(all<4>(v1 >= v1));
      CHECK(none<4>(v1 < v1));
      CHECK(none<4>(v1 > v1));

      auto leq = v1 <= v2;
      auto geq = v1 >= v2;
      auto less = v1 <= v2;
      auto greater = v1 >= v2;

      CHECK(bitwise_equal(leq & greater, Vec4{0.f}));
      CHECK(bitwise_equal(leq | greater, Vec4{bits_to_float(0xFFFF'FFFF)}));

      CHECK(bitwise_equal(geq & less, Vec4{0.f}));
      CHECK(bitwise_equal(geq | less, Vec4{bits_to_float(0xFFFF'FFFF)}));

      CHECK(bitwise_equal(and_not(geq, greater), BitMasks::cmp_equal_f32(v1, v2)));
      CHECK(bitwise_equal(and_not(leq, less), BitMasks::cmp_equal_f32(v1, v2)));

      auto v1_c = v1.components();
      auto v2_c = v2.components();

      auto cleq = leq.components();
      auto cgeq = geq.components();
      auto cless = less.components();
      auto cgreater = greater.components();

      for (int j = 0; j < 4; ++j)
      {
        if (v1_c[j] <= v2_c[j])
          CHECK_BITMASK_TRUE(cleq[j]);
        else
          CHECK_BITMASK_FALSE(cleq[j]);

        if (v1_c[j] >= v2_c[j])
          CHECK_BITMASK_TRUE(cgeq[j]);
        else
          CHECK_BITMASK_FALSE(cgeq[j]);

        if (v1_c[j] < v2_c[j])
          CHECK_BITMASK_TRUE(cless[j]);
        else
          CHECK_BITMASK_FALSE(cless[j]);

        if (v1_c[j] > v2_c[j])
          CHECK_BITMASK_TRUE(cgreater[j]);
        else
          CHECK_BITMASK_FALSE(cgreater[j]);
      }
    }

    {
      LVec3 v1 = rnd3();
      LVec3 v2 = rnd3();

      CHECK(all<3>(v1 <= v1));
      CHECK(all<3>(v1 >= v1));
      CHECK(none<3>(v1 < v1));
      CHECK(none<3>(v1 > v1));

      auto leq = v1 <= v2;
      auto geq = v1 >= v2;
      auto less = v1 < v2;
      auto greater = v1 > v2;

      CHECK(bitwise_equal(leq & greater, Vec4{0.f}));
      CHECK(bitwise_equal(leq | greater, Vec4{bits_to_float(0xFFFF'FFFF)}));

      CHECK(bitwise_equal(geq & less, Vec4{0.f}));
      CHECK(bitwise_equal(geq | less, Vec4{bits_to_float(0xFFFF'FFFF)}));

      CHECK(bitwise_equal(and_not(geq, greater), BitMasks::cmp_equal_f32(v1, v2)));
      CHECK(bitwise_equal(and_not(leq, less), BitMasks::cmp_equal_f32(v1, v2)));

      auto v1_c = v1.components();
      auto v2_c = v2.components();

      auto cleq = leq.components();
      auto cgeq = geq.components();
      auto cless = less.components();
      auto cgreater = greater.components();

      for (int j = 0; j < 3; ++j)
      {
        if (v1_c[j] <= v2_c[j])
          CHECK_BITMASK_TRUE(cleq[j]);
        else
          CHECK_BITMASK_FALSE(cleq[j]);

        if (v1_c[j] >= v2_c[j])
          CHECK_BITMASK_TRUE(cgeq[j]);
        else
          CHECK_BITMASK_FALSE(cgeq[j]);

        if (v1_c[j] < v2_c[j])
          CHECK_BITMASK_TRUE(cless[j]);
        else
          CHECK_BITMASK_FALSE(cless[j]);

        if (v1_c[j] > v2_c[j])
          CHECK_BITMASK_TRUE(cgreater[j]);
        else
          CHECK_BITMASK_FALSE(cgreater[j]);
      }
    }
  }
}

TEST_CASE("Scalar-vector multiplication", "[Operators]")
{
  for (int i = 0; i < 100; ++i)
  {
    float scalar = random_float(-10.0f, 10.0f);

    {
      Vec4 v4 = rnd4();

      CHECK(v4 * -1.f == -v4);
      CHECK(v4 * 0.f == Vec4{0.f});
      CHECK(v4 * 1.f == v4);

      auto res4 = scalar * v4;

      CHECK(res4 == v4 * scalar);

      auto cv4 = v4.components();
      auto cres4 = res4.components();

      for (int j = 0; j < 4; ++j)
      {
        CHECK(cres4[j] == Approx(scalar * cv4[j]));
      }
    }
    {
      LVec3 v3 = rnd3();

      CHECK(v3 * -1.f == -v3);
      CHECK(v3 * 0.f == LVec3{0.f});
      CHECK(v3 * 1.f == v3);

      auto res3 = scalar * v3;

      CHECK(res3 == v3 * scalar);

      auto cv3 = v3.components();
      auto cres3 = res3.components();

      for (int j = 0; j < 3; ++j)
      {
        CHECK(cres3[j] == Approx(scalar * cv3[j]));
      }
    }
  }
}

TEST_CASE("Scalar-vector division", "[Operators]")
{
  for (int i = 0; i < 100; ++i)
  {
    float scalar = random_float(0.1f, 1e6f);

    {
      Vec4 v4 = rnd4();

      CHECK_VEC4_ALMOST_EQ((v4 / -1.f), -v4);
      CHECK_VEC4_ALMOST_EQ((v4 / 1.f), v4);

      auto r1 = v4 / scalar;
      auto r2 = scalar / v4;
      auto one = (1.f / v4) * v4;

      auto cv4 = v4.components();
      auto cr1 = r1.components();
      auto cr2 = r2.components();
      auto cone = one.components();

      for (int j = 0; j < 4; ++j)
      {
        CHECK(cr1[j] == Approx(cv4[j] / scalar));
        CHECK(cr2[j] == Approx(scalar / cv4[j]));
        CHECK(cone[j] == Approx(1.f));
      }
    }
    {
      LVec3 v3 = rnd3();

      CHECK_LVEC3_ALMOST_EQ((v3 / -1.f), -v3);
      CHECK_LVEC3_ALMOST_EQ((v3 / 1.f), v3);

      auto r1 = v3 / scalar;
      auto r2 = scalar / v3;
      auto one = (1.f / v3) * v3;

      auto cv3 = v3.components();
      auto cr1 = r1.components();
      auto cr2 = r2.components();
      auto cone = one.components();

      for (int j = 0; j < 3; ++j)
      {
        CHECK(cr1[j] == Approx(cv3[j] / scalar));
        CHECK(cr2[j] == Approx(scalar / cv3[j]));
        CHECK(cone[j] == Approx(1.f));
      }
    }
  }
}

TEST_CASE("Subtraction and negation", "[Operators]")
{
  for (int i = 0; i < 100; ++i)
  {
    {
      float rnd = random_float();
      CHECK(-Vec4{rnd} == Vec4{-rnd});

      Vec4 v1 = rnd4();
      Vec4 v2 = rnd4();

      CHECK(v1 == -(-v1));
      CHECK(-v1 == (Vec4{0.f} - v1));

      CHECK(v1 - v2 == -(v2 - v1));
      CHECK(v1 - Vec4{0.f} == v1);
      CHECK(v1 - (-v1) == (v1 + v1));

      auto cv1 = v1.components();
      auto cv2 = v2.components();

      auto csub = (v1 - v2).components();

      for (int j = 0; j < 4; ++j)
      {
        CHECK(csub[j] == Approx(cv1[j] - cv2[j]));
      }
    }
    {
      float rnd = random_float();
      CHECK(-LVec3{rnd} == LVec3{-rnd});

      LVec3 v1 = rnd3();
      LVec3 v2 = rnd3();

      CHECK(v1 == -(-v1));
      CHECK(-v1 == (LVec3{0.f} - v1));

      CHECK(v1 - v2 == -(v2 - v1));
      CHECK(v1 - LVec3{0.f} == v1);
      CHECK(v1 - (-v1) == (v1 + v1));

      auto cv1 = v1.components();
      auto cv2 = v2.components();

      auto csub = (v1 - v2).components();

      for (int j = 0; j < 3; ++j)
      {
        CHECK(csub[j] == Approx(cv1[j] - cv2[j]));
      }
    }
    {
      Vec4 v1 = rnd4();
      LVec3 v2 = rnd3();

      CHECK(v1 - v2 == -(v2 - v1));

      auto cv1 = v1.components();
      auto cv2 = Vec4{v2}.components();

      auto csub = (v1 - v2).components();

      for (int j = 0; j < 4; ++j)
      {
        CHECK(csub[j] == Approx(cv1[j] - cv2[j]));
      }
    }
    {
      LVec3 v1 = rnd3();
      Vec4 v2 = rnd4();

      CHECK(v1 - v2 == -(v2 - v1));

      auto cv1 = Vec4{v1}.components();
      auto cv2 = v2.components();

      auto csub = (v1 - v2).components();

      for (int j = 0; j < 4; ++j)
      {
        CHECK(csub[j] == Approx(cv1[j] - cv2[j]));
      }
    }
  }
}

TEST_CASE("Vector-vector addition", "[Operators]")
{
  for (int i = 0; i < 100; ++i)
  {
    {
      Vec4 v1 = rnd4();
      Vec4 v2 = rnd4();

      CHECK(v1 + v2 == v2 + v1);
      CHECK(v1 + Vec4{0.f} == v1);
      CHECK(v1 + (-v1) == Vec4{0.f});

      auto cv1 = v1.components();
      auto cv2 = v2.components();

      auto cop = (v1 + v2).components();

      for (int j = 0; j < 4; ++j)
      {
        CHECK(cop[j] == Approx(cv1[j] + cv2[j]));
      }
    }
    {
      LVec3 v1 = rnd3();
      LVec3 v2 = rnd3();

      CHECK(v1 + v2 == v2 + v1);
      CHECK(v1 + LVec3{0.f} == v1);
      CHECK(v1 + (-v1) == LVec3{0.f});

      auto cv1 = v1.components();
      auto cv2 = v2.components();

      auto cop = (v1 + v2).components();

      for (int j = 0; j < 3; ++j)
      {
        CHECK(cop[j] == Approx(cv1[j] + cv2[j]));
      }
    }
    {
      Vec4 v1 = rnd4();
      LVec3 v2 = rnd3();

      CHECK(v1 + v2 == v2 + v1);

      auto cv1 = v1.components();
      auto cv2 = Vec4{v2}.components();

      auto cop = (v1 + v2).components();

      for (int j = 0; j < 4; ++j)
      {
        CHECK(cop[j] == Approx(cv1[j] + cv2[j]));
      }
    }
    {
      LVec3 v1 = rnd3();
      Vec4 v2 = rnd4();

      CHECK(v1 + v2 == v2 + v1);

      auto cv1 = Vec4{v1}.components();
      auto cv2 = v2.components();

      auto cop = (v1 + v2).components();

      for (int j = 0; j < 4; ++j)
      {
        CHECK(cop[j] == Approx(cv1[j] + cv2[j]));
      }
    }
  }
}

TEST_CASE("Vector-vector multiplication", "[Operators]")
{
  for (int i = 0; i < 100; ++i)
  {
    {
      Vec4 v1 = rnd4();
      Vec4 v2 = rnd4();

      CHECK(v1 * v2 == v2 * v1);
      CHECK(v1 * Vec4{0.f} == Vec4{0.f});
      CHECK(v1 * Vec4{-1.f} == -v1);

      auto cv1 = v1.components();
      auto cv2 = v2.components();

      auto cop = (v1 * v2).components();

      for (int j = 0; j < 4; ++j)
      {
        CHECK(cop[j] == Approx(cv1[j] * cv2[j]));
      }
    }
    {
      LVec3 v1 = rnd3();
      LVec3 v2 = rnd3();

      CHECK(v1 * v2 == v2 * v1);
      CHECK(v1 * LVec3{0.f} == LVec3{0.f});
      CHECK(v1 * LVec3{-1.f} == -v1);

      auto cv1 = v1.components();
      auto cv2 = v2.components();

      auto cop = (v1 * v2).components();

      for (int j = 0; j < 3; ++j)
      {
        CHECK(cop[j] == Approx(cv1[j] * cv2[j]));
      }
    }
    {
      Vec4 v1 = rnd4();
      LVec3 v2 = rnd3();

      CHECK(v1 * v2 == v2 * v1);

      auto cv1 = v1.components();
      auto cv2 = v2.components();

      auto cop = (v1 * v2).components();

      for (int j = 0; j < 3; ++j)
      {
        CHECK(cop[j] == Approx(cv1[j] * cv2[j]));
      }
    }
    {
      LVec3 v1 = rnd3();
      Vec4 v2 = rnd4();

      CHECK(v1 * v2 == v2 * v1);

      auto cv1 = v1.components();
      auto cv2 = v2.components();

      auto cop = (v1 * v2).components();

      for (int j = 0; j < 3; ++j)
      {
        CHECK(cop[j] == Approx(cv1[j] * cv2[j]));
      }
    }
  }
}

TEST_CASE("Vector-vector division", "[Operators]")
{
  for (int i = 0; i < 100; ++i)
  {
    {
      Vec4 v1 = rnd4(0.001f, 1000.f);
      Vec4 v2 = rnd4(0.001f, 1000.f);

      CHECK_VEC4_ALMOST_EQ((v1 / Vec4{-1.f}), -v1);

      auto cv1 = v1.components();
      auto cv2 = v2.components();

      auto cop1 = (v1 / v2).components();
      auto cop2 = (v2 / v1).components();

      for (int j = 0; j < 4; ++j)
      {
        CHECK(cop1[j] == Approx(cv1[j] / cv2[j]));
        CHECK(cop1[j] == Approx(1.f / cop2[j]));
      }
    }
    {
      LVec3 v1 = rnd3(0.001f, 1000.f);
      LVec3 v2 = rnd3(0.001f, 1000.f);

      CHECK_LVEC3_ALMOST_EQ((v1 / Vec4{-1.f}), -v1);

      auto cv1 = v1.components();
      auto cv2 = v2.components();

      auto cop1 = (v1 / v2).components();
      auto cop2 = (v2 / v1).components();

      for (int j = 0; j < 3; ++j)
      {
        CHECK(cop1[j] == Approx(cv1[j] / cv2[j]));
        CHECK(cop1[j] == Approx(1.f / cop2[j]));
      }
    }
    // Vec4 / LVec3: disabled statically

    // LVec3 / Vec4
    {
      LVec3 v1 = rnd3(-1000.f, -0.001f);
      Vec4 v2 = rnd4(0.001f, 1000.f);

      auto cv1 = Vec4{v1}.components();
      auto cv2 = v2.components();

      auto cop = (v1 / v2).components();

      for (int j = 0; j < 4; ++j)
      {
        CHECK(cop[j] == Approx(cv1[j] / cv2[j]));
      }
    }
  }
}

TEST_CASE("Bitwise AND operator", "[Operators]")
{
  SECTION("Vec4 & Vec4")
  {
    Vec4 v1{bits_to_float(0xFF00FF00u), bits_to_float(0x0F0F0F0Fu),
            bits_to_float(0xAAAAAAAAu), bits_to_float(0x55555555u)};
    Vec4 v2{bits_to_float(0x00FF00FFu), bits_to_float(0xF0F0F0F0u),
            bits_to_float(0x55555555u), bits_to_float(0xAAAAAAAAu)};

    CHECK(all<4>(BitMasks::cmp_equal_bitwise(v1 & v2, v2 & v1)));
    CHECK_VEC4_BITWISE_EQ(v1 & v2, 0, 0, 0, 0);
  }

  SECTION("Mixed dimensions")
  {
    Vec4 v1{bits_to_float(0xFFFFFFFFu), bits_to_float(0xFFFFFFFFu),
            bits_to_float(0xFFFFFFFFu), bits_to_float(0xFFFFFFFFu)};
    LVec3 v2{bits_to_float(0xF0F0F0F0u), bits_to_float(0x0F0F0F0Fu),
             bits_to_float(0xAAAAAAAAu)};


    CHECK(all<4>(BitMasks::cmp_equal_bitwise(Vec4{v1 & v2}, Vec4{v2 & v1})));
    CHECK_LVEC3_BITWISE_EQ(v1 & v2, 0xF0F0F0F0, 0x0F0F0F0F, 0xAAAAAAAA);
  }

  SECTION("Random values")
  {
    for (int i = 0; i < 100; ++i)
    {
      uint32_t a1 = random_bits();
      uint32_t a2 = random_bits();
      uint32_t a3 = random_bits();
      uint32_t a4 = random_bits();

      uint32_t b1 = random_bits();
      uint32_t b2 = random_bits();
      uint32_t b3 = random_bits();
      uint32_t b4 = random_bits();

      Vec4 v1{bits_to_float(a1), bits_to_float(a2), bits_to_float(a3), bits_to_float(a4)};
      Vec4 v2{bits_to_float(b1), bits_to_float(b2), bits_to_float(b3), bits_to_float(b4)};

      CHECK(all<4>(BitMasks::cmp_equal_bitwise(v1 & v2, v2 & v1)));
      CHECK_VEC4_BITWISE_EQ(v1 & v2, a1 & b1, a2 & b2, a3 & b3, a4 & b4);

      LVec3 v3{bits_to_float(a1), bits_to_float(a2), bits_to_float(a3)};
      LVec3 v4{bits_to_float(b1), bits_to_float(b2), bits_to_float(b3)};

      CHECK(all<4>(BitMasks::cmp_equal_bitwise(Vec4{v1 & v2}, Vec4{v2 & v1})));
      CHECK_LVEC3_BITWISE_EQ(v3 & v4, a1 & b1, a2 & b2, a3 & b3);
    }
  }
}

TEST_CASE("Bitwise OR operator", "[Operators]")
{
  SECTION("Vec4 | Vec4")
  {
    Vec4 v1{bits_to_float(0xFF00FF00u), bits_to_float(0x0F0F0F0Fu),
            bits_to_float(0xAAAAAAAAu), bits_to_float(0x55555555u)};
    Vec4 v2{bits_to_float(0x00FF00FFu), bits_to_float(0xF0F0F0F0u),
            bits_to_float(0x55555555u), bits_to_float(0xAAAAAAAAu)};

    CHECK(all<4>(BitMasks::cmp_equal_bitwise(v1 | v2, v2 | v1)));
    CHECK_VEC4_BITWISE_EQ(v1 | v2, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF);
  }

  SECTION("Mixed dimensions")
  {
    Vec4 v1{bits_to_float(0xF0F0F0F0u), bits_to_float(0x0F0F0F0Fu),
            bits_to_float(0x00000000u), bits_to_float(0xFFFFFFFFu)};
    LVec3 v2{bits_to_float(0x0F0F0F0Fu), bits_to_float(0xF0F0F0F0u),
             bits_to_float(0xAAAAAAAAu)};

    CHECK(all<4>(BitMasks::cmp_equal_bitwise(v1 | v2, v2 | v1)));
    CHECK_VEC4_BITWISE_EQ(v1 | v2, 0xFFFFFFFF, 0xFFFFFFFF, 0xAAAAAAAA, 0xFFFFFFFF);
  }

  SECTION("Random values")
  {
    for (int i = 0; i < 100; ++i)
    {
      uint32_t a1 = random_bits();
      uint32_t a2 = random_bits();
      uint32_t a3 = random_bits();
      uint32_t a4 = random_bits();

      uint32_t b1 = random_bits();
      uint32_t b2 = random_bits();
      uint32_t b3 = random_bits();
      uint32_t b4 = random_bits();

      Vec4 v1{bits_to_float(a1), bits_to_float(a2), bits_to_float(a3), bits_to_float(a4)};
      Vec4 v2{bits_to_float(b1), bits_to_float(b2), bits_to_float(b3), bits_to_float(b4)};

      CHECK(all<4>(BitMasks::cmp_equal_bitwise(v1 | v2, v2 | v1)));
      CHECK_VEC4_BITWISE_EQ(v1 | v2, a1 | b1, a2 | b2, a3 | b3, a4 | b4);

      LVec3 v3{bits_to_float(a1), bits_to_float(a2), bits_to_float(a3)};
      LVec3 v4{bits_to_float(b1), bits_to_float(b2), bits_to_float(b3)};

      CHECK(all<4>(BitMasks::cmp_equal_bitwise(Vec4{v3 & v4}, Vec4{v4 & v3})));
      CHECK_LVEC3_BITWISE_EQ(v3 | v4, a1 | b1, a2 | b2, a3 | b3);
    }
  }
}

TEST_CASE("Bitwise XOR operator", "[Operators]")
{
  SECTION("Vec4 ^ Vec4")
  {
    Vec4 v1{bits_to_float(0xFF00FF00u), bits_to_float(0xAAAAAAAAu), bits_to_float(0xCC87C87Cu), bits_to_float(0x55555555u)};
    Vec4 v2{bits_to_float(0x00FF00FFu), bits_to_float(0x55555555u), bits_to_float(0x33783783u), bits_to_float(0xAAAAAAAAu)};

    CHECK(all<4>(BitMasks::cmp_equal_bitwise(v1 ^ v2, v2 ^ v1)));
    CHECK_VEC4_BITWISE_EQ(v1 ^ v1, 0, 0, 0, 0);
    CHECK_VEC4_BITWISE_EQ(v1 ^ v2, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF);
  }

  SECTION("Random values")
  {
    Vec4 zeros{0.f};
    Vec4 ones{bits_to_float(0xFFFFFFFF)};

    for (int i = 0; i < 100; ++i)
    {
      uint32_t a1 = random_bits();
      uint32_t a2 = random_bits();
      uint32_t a3 = random_bits();
      uint32_t a4 = random_bits();

      uint32_t b1 = random_bits();
      uint32_t b2 = random_bits();
      uint32_t b3 = random_bits();
      uint32_t b4 = random_bits();

      Vec4 v1{bits_to_float(a1), bits_to_float(a2), bits_to_float(a3), bits_to_float(a4)};
      Vec4 v2{bits_to_float(b1), bits_to_float(b2), bits_to_float(b3), bits_to_float(b4)};

      CHECK(all<4>(BitMasks::cmp_equal_bitwise(v1 ^ v2, v2 ^ v1)));
      CHECK_VEC4_BITWISE_EQ(v1 ^ v2, a1 ^ b1, a2 ^ b2, a3 ^ b3, a4 ^ b4);

      CHECK(bitwise_equal(v1 ^ zeros, v1));
      CHECK(bitwise_equal(v1 ^ v1, zeros));
      CHECK(bitwise_equal(v1 ^ ones, ~v1));

      auto result = v1 ^ v2;
      auto res_c = result.components();

      CHECK(std::bit_cast<uint32_t>(res_c[0]) == (a1 ^ b1));
      CHECK(std::bit_cast<uint32_t>(res_c[1]) == (a2 ^ b2));
      CHECK(std::bit_cast<uint32_t>(res_c[2]) == (a3 ^ b3));
      CHECK(std::bit_cast<uint32_t>(res_c[3]) == (a4 ^ b4));

      LVec3 v3{bits_to_float(a1), bits_to_float(a2), bits_to_float(a3)};
      LVec3 v4{bits_to_float(b1), bits_to_float(b2), bits_to_float(b3)};

      CHECK(all<4>(BitMasks::cmp_equal_bitwise(Vec4{v3 ^ v4}, Vec4{v4 ^ v3})));
      CHECK_LVEC3_BITWISE_EQ(v3 ^ v4, a1 ^ b1, a2 ^ b2, a3 ^ b3);
    }
  }
}

TEST_CASE("Bitwise NOT operator", "[Operators]")
{
  SECTION("Bitwise NOT on Vec4")
  {
    Vec4 v{bits_to_float(0u), bits_to_float(0xFFFFFFFFu),
           bits_to_float(0xF0F0F0F0u), bits_to_float(0x0F0F0F0Fu)};

    CHECK_VEC4_BITWISE_EQ(~v, 0xFFFFFFFF, 0, 0x0F0F0F0F, 0xF0F0F0F0);

    CHECK(bitwise_equal(~~v, v));
  }

  SECTION("Random values")
  {
    for (int i = 0; i < 100; ++i)
    {
      uint32_t a1 = random_bits();
      uint32_t a2 = random_bits();
      uint32_t a3 = random_bits();
      uint32_t a4 = random_bits();

      Vec4 v{bits_to_float(a1), bits_to_float(a2), bits_to_float(a3), bits_to_float(a4)};

      CHECK(bitwise_equal(~~v, v));
      CHECK_VEC4_BITWISE_EQ(v & (~v), 0, 0, 0, 0);
      CHECK_VEC4_BITWISE_EQ(v | (~v), 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF);
      CHECK_VEC4_BITWISE_EQ(v ^ (~v), 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF);
      CHECK_VEC4_BITWISE_EQ(~v, ~a1, ~a2, ~a3, ~a4);
    }
  }
}

TEST_CASE("Compound assignment operators", "[Operators]")
{
  SECTION("Addition assignment")
  {
    for (int i = 0; i < 100; ++i)
    {
      Vec4 v1 = rnd4();
      Vec4 v2 = rnd4();

      Vec4 original1 = v1;
      v1 += v2;
      CHECK(v1 == original1 + v2);

      LVec3 v3{rnd4()};
      LVec3 v4{rnd4()};

      LVec3 original3 = v3;
      v3 += v4;
      CHECK(v3 == original3 + v4);
    }
  }

  SECTION("Subtraction assignment")
  {
    for (int i = 0; i < 100; ++i)
    {
      Vec4 v1 = rnd4();
      Vec4 v2 = rnd4();

      Vec4 original1 = v1;
      v1 -= v2;
      CHECK(v1 == original1 - v2);

      LVec3 v3{rnd4()};
      LVec3 v4{rnd4()};

      LVec3 original3 = v3;
      v3 -= v4;
      CHECK(v3 == original3 - v4);
    }
  }

  SECTION("Multiplication assignment - scalar")
  {
    for (int i = 0; i < 100; ++i)
    {
      Vec4 v1 = rnd4();
      float s = random_float();

      Vec4 original1 = v1;
      v1 *= s;
      CHECK(v1 == original1 * s);

      LVec3 v3{rnd4()};

      LVec3 original3 = v3;
      v3 *= s;
      CHECK(v3 == original3 * s);
    }
  }

  SECTION("Division assignment - scalar")
  {
    for (int i = 0; i < 100; ++i)
    {
      {
        Vec4 v1 = rnd4();
        float s = random_float(0.f);

        Vec4 original1 = v1;
        v1 /= s;
        CHECK(v1 == original1 / s);

        LVec3 v3{rnd4()};

        LVec3 original3 = v3;
        v3 /= s;
        CHECK(v3 == original3 / s);
      }
    }
  }

  SECTION("Component-wise multiplication assignment")
  {
    for (int i = 0; i < 100; ++i)
    {
      Vec4 v1{rnd4()};
      Vec4 v2{rnd4()};

      Vec4 original1 = v1;
      v1 *= v2;
      CHECK(v1 == original1 * v2);

      LVec3 v3{rnd4()};
      LVec3 v4{rnd4()};

      LVec3 original3 = v3;
      v3 *= v4;
      CHECK(v3 == original3 * v4);
    }
  }

  SECTION("Component-wise division assignment")
  {
    for (int i = 0; i < 100; ++i)
    {
      Vec4 v1{ random_float(0.001f), random_float(0.001f), random_float(0.001f), random_float(0.001f)};
      Vec4 v2{ random_float(0.001f), random_float(0.001f), random_float(0.001f), random_float(0.001f)};

      Vec4 original1 = v1;
      v1 *= v2;
      CHECK(v1 == original1 * v2);

      LVec3 v3{ random_float(0.001f), random_float(0.001f), random_float(0.001f) };
      LVec3 v4{ random_float(0.001f), random_float(0.001f), random_float(0.001f) };

      LVec3 original3 = v3;
      v3 *= v4;
      CHECK(v3 == original3 * v4);
    }
  }

  SECTION("Bitwise AND assignment")
  {
    for (int i = 0; i < 100; ++i)
    {
      uint32_t a1 = random_bits();
      uint32_t a2 = random_bits();
      uint32_t a3 = random_bits();
      uint32_t a4 = random_bits();
      uint32_t b2 = random_bits();
      uint32_t b1 = random_bits();
      uint32_t b3 = random_bits();
      uint32_t b4 = random_bits();

      Vec4 vbit1{bits_to_float(a1), bits_to_float(a2), bits_to_float(a3), bits_to_float(a4)};
      Vec4 vbit2{bits_to_float(b1), bits_to_float(b2), bits_to_float(b3), bits_to_float(b4)};

      Vec4 original1 = vbit1;
      vbit1 &= vbit1;
      CHECK(bitwise_equal(vbit1, original1));

      vbit1 &= vbit2;
      CHECK(bitwise_equal(vbit1, original1 & vbit2));

      LVec3 vbit3{vbit1};
      LVec3 vbit4{vbit2};

      LVec3 original3 = vbit3;
      vbit3 &= vbit3;
      CHECK(bitwise_equal(vbit3, original3));

      vbit3 &= vbit4;
      CHECK(bitwise_equal(vbit3, original3 & vbit4));
    }
  }

  SECTION("Bitwise OR assignment")
  {
    for (int i = 0; i < 100; ++i)
    {
      uint32_t a1 = random_bits();
      uint32_t a2 = random_bits();
      uint32_t a3 = random_bits();
      uint32_t a4 = random_bits();
      uint32_t b2 = random_bits();
      uint32_t b1 = random_bits();
      uint32_t b3 = random_bits();
      uint32_t b4 = random_bits();

      Vec4 vbit1{bits_to_float(a1), bits_to_float(a2), bits_to_float(a3), bits_to_float(a4)};
      Vec4 vbit2{bits_to_float(b1), bits_to_float(b2), bits_to_float(b3), bits_to_float(b4)};

      Vec4 original1 = vbit1;
      vbit1 |= vbit1;
      CHECK(bitwise_equal(vbit1, original1));

      vbit1 |= vbit2;
      CHECK(bitwise_equal(vbit1, original1 | vbit2));

      LVec3 vbit3{vbit1};
      LVec3 vbit4{vbit2};

      LVec3 original3 = vbit3;
      vbit3 |= vbit3;
      CHECK(bitwise_equal(vbit3, original3));

      vbit3 |= vbit4;
      CHECK(bitwise_equal(vbit3, original3 | vbit4));
    }
  }

  SECTION("Bitwise XOR assignment")
  {
    for (int i = 0; i < 100; ++i)
    {
      uint32_t a1 = random_bits();
      uint32_t a2 = random_bits();
      uint32_t a3 = random_bits();
      uint32_t a4 = random_bits();
      uint32_t b2 = random_bits();
      uint32_t b1 = random_bits();
      uint32_t b3 = random_bits();
      uint32_t b4 = random_bits();

      Vec4 vbit1{bits_to_float(a1), bits_to_float(a2), bits_to_float(a3), bits_to_float(a4)};
      Vec4 vbit2{bits_to_float(b1), bits_to_float(b2), bits_to_float(b3), bits_to_float(b4)};

      Vec4 original1 = vbit1;
      vbit1 ^= vbit1;
      CHECK(bitwise_equal(vbit1, Vec4{0.f}));

      vbit1 = original1;
      vbit1 ^= vbit2;
      CHECK(bitwise_equal(vbit1, original1 ^ vbit2));

      LVec3 vbit3{vbit1};
      LVec3 vbit4{vbit2};

      LVec3 original3 = vbit3;
      vbit3 ^= vbit3;
      CHECK_LVEC3_BITWISE_EQ(vbit3, 0, 0, 0);
      CHECK(bitwise_equal(Vec4{vbit3}, Vec4{0.f}));

      vbit3 = original3;
      vbit3 ^= vbit4;
      CHECK(bitwise_equal(Vec4{vbit3}, Vec4{original3 ^ vbit4}));
    }
  }
}
