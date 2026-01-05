#include "TestUtils.h"

#include <algorithm>
#include <cmath>

using namespace FSVF;
using Catch::Approx;

TEST_CASE("FSVF::sum", "[GenericMethods]")
{
  Vec4  v4{ 1.0f, 2.0f, 3.0f, 4.0f };
  LVec3 v3{ 5.0f, 6.0f, 7.0f };
  CHECK(sum(v4) == Approx(10.0f));
  CHECK(sum(v3) == Approx(18.0f));

  for (int i = 0; i < 100; ++i) {
    v4 = rnd4();
    v3 = rnd3();

    const auto c4 = v4.components();
    const auto c3 = v3.components();

    CHECK(sum(v4) == Approx((c4[0] + c4[1]) + (c4[2] + c4[3])));
    CHECK(sum(v3) == Approx(c3[0] + c3[1] + c3[2]));
  }
}

TEST_CASE("FSVF::prod", "[GenericMethods]")
{
  Vec4  v4{ 2.0f, 3.0f, 4.0f, 5.0f };
  LVec3 v3{ 2.0f, 3.0f, 4.0f };
  CHECK(prod(v4) == Approx(120.0f));
  CHECK(prod(v3) == Approx(24.0f));

  for (int i = 0; i < 100; ++i) {
    v4 = rnd4();
    v3 = rnd3();

    const auto c4 = v4.components();
    const auto c3 = v3.components();

    CHECK(prod(v4) == Approx(c4[0] * c4[1] * c4[2] * c4[3]));
    CHECK(prod(v3) == Approx(c3[0] * c3[1] * c3[2]));
  }
}

TEST_CASE("FSVF::normalize", "[GenericMethods]")
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

  for (int i = 0; i < 100; ++i) {
    v4 = rnd4();
    v3 = rnd3();

    n4 = normalize(v4);
    l4 = len(n4);
    CHECK(l4 == Approx(1.0f));

    n3 = normalize(v3);
    l3 = len(n3);
    CHECK(l3 == Approx(1.0f));
  }
}

TEST_CASE("FSVF::len", "[GenericMethods]")
{
  Vec4  v4{ 1.0f, 2.0f, 2.0f, 1.0f };
  LVec3 v3{ 2.0f, 3.0f, 6.0f };
  CHECK(len(v4) == Approx(std::sqrt(10.0f)));
  CHECK(len(v3) == Approx(7.0f));


  for (int i = 0; i < 100; ++i) {
    v4 = rnd4();
    v3 = rnd3();

    const auto c4 = v4.components();
    const auto c3 = v3.components();

    CHECK(len(v4) == Approx(std::sqrt(c4[0] * c4[0] + c4[1] * c4[1] + c4[2] * c4[2] + c4[3] * c4[3])));
    CHECK(len(v3) == Approx(std::sqrt(c3[0] * c3[0] + c3[1] * c3[1] + c3[2] * c3[2])));
  }
}

TEST_CASE("FSVF::len_squared", "[GenericMethods]")
{
  Vec4  v4{ 1.0f, 2.0f, 2.0f, 1.0f };
  LVec3 v3{ 2.0f, 3.0f, 6.0f };
  CHECK(len_squared(v4) == Approx(10.0f));
  CHECK(len_squared(v3) == Approx(49.0f));

  for (int i = 0; i < 100; ++i) {
    v4 = rnd4();
    v3 = rnd3();

    const auto c4 = v4.components();
    const auto c3 = v3.components();

    CHECK(len_squared(v4) == Approx(c4[0] * c4[0] + c4[1] * c4[1] + c4[2] * c4[2] + c4[3] * c4[3]));
    CHECK(len_squared(v3) == Approx(c3[0] * c3[0] + c3[1] * c3[1] + c3[2] * c3[2]));
  }
}

TEST_CASE("Sort, max and min - trivial", "[GenericMethods]")
{
  for (int i = 0; i < 100; ++i) {
    const auto f = random_float();

    Vec4 v4{f};
    LVec3 v3{f};

    CHECK(sort(v4) == v4);
    CHECK(sort(v3) == v3);
    CHECK(reverse_sort(v4) == v4);
    CHECK(reverse_sort(v3) == v3);

    CHECK(max_component_value(v4) == Vec4{f});
    CHECK(max_component_value_f32(v4) == f);
    CHECK(max_component_value(v3) == LVec3{f});
    CHECK(max_component_value_f32(v3) == f);

    CHECK(min_component_value(v4) == Vec4{f});
    CHECK(min_component_value_f32(v4) == f);
    CHECK(min_component_value(v3) == LVec3{f});
    CHECK(min_component_value_f32(v3) == f);

    CHECK((max_component_index(v4) == 0 ||
          max_component_index(v4) == 1 ||
          max_component_index(v4) == 2 ||
          max_component_index(v4) == 3));

    CHECK((max_component_index(v3) == 0 ||
          max_component_index(v3) == 1 ||
          max_component_index(v3) == 2));

    CHECK((min_component_index(v4) == 0 ||
          min_component_index(v4) == 1 ||
          min_component_index(v4) == 2 ||
          min_component_index(v4) == 3));

    CHECK((min_component_index(v3) == 0 ||
          min_component_index(v3) == 1 ||
          min_component_index(v3) == 2));
  }
}

TEST_CASE("Sort, max and min - two-value vectors", "[GenericMethods]")
{
  for (int i = 0; i < 100; ++i) {
    std::vector<float> f{};

    f.push_back(random_float());

    Vec4 v4{f[0]};
    LVec3 v3{f[0]};

    auto r = random_float();
    while (r == f[0]) r = random_float();

    f.push_back(r);
    std::ranges::sort(f);

    //  one option
    for (int j = 0; j < 4; ++j) {
      stdVec4 a4{f[0], f[0], f[0], f[0]};

      a4[j] = f[1];

      v4 = Vec4(a4.data());
      v3 = LVec3(a4.data());

      {
        auto ra4 = a4;
        std::ranges::sort(a4);
        std::ranges::sort(ra4, std::greater<float>{});

        CHECK(sort(v4) == Vec4{a4.data()});
        CHECK(reverse_sort(v4) == Vec4{ra4.data()});
      }

      CHECK(max_component_index(v4) == j);
      CHECK((min_component_index(v4) != j &&
            min_component_index(v4) >= 0 &&
            min_component_index(v4) <= 3));

      CHECK(max_component_value(v4) == Vec4{f[1]});
      CHECK(max_component_value_f32(v4) == f[1]);
      CHECK(min_component_value(v4) == Vec4{f[0]});
      CHECK(min_component_value_f32(v4) == f[0]);

      if (j != 3) {
        {
          auto a3 = v3.components();
          auto ra3 = a3;

          std::ranges::sort(a3);
          std::ranges::sort(ra3, std::greater<float>{});

          CHECK(sort(v3) == LVec3{a3.data()});
          CHECK(reverse_sort(v3) == LVec3{ra3.data()});
        }

        CHECK(max_component_index(v3) == j);
        CHECK((min_component_index(v3) != j &&
              min_component_index(v3) >= 0 &&
              min_component_index(v3) <= 2));

        CHECK(max_component_value(v3) == LVec3{f[1]});
        CHECK(max_component_value_f32(v3) == f[1]);
        CHECK(min_component_value(v3) == LVec3{f[0]});
        CHECK(min_component_value_f32(v3) == f[0]);
      }
    }

    //  two options
    for (int j = 0; j < 4; ++j) {
      for (int k = 0; k < 4; ++k) {
        if (k == j) continue;

        stdVec4 a4{f[0], f[0], f[0], f[0]};

        a4[j] = f[1];
        a4[k] = f[1];

        v4 = Vec4(a4.data());
        v3 = LVec3(a4.data());

        std::ranges::sort(a4);

        auto ra4 = a4;
        std::ranges::sort(ra4, std::greater<float>{});

        CHECK(sort(v4) == Vec4{a4.data()});
        CHECK(reverse_sort(v4) == Vec4{ra4.data()});

        CHECK(max_component_value(v4) == Vec4{f[1]});
        CHECK(max_component_value_f32(v4) == f[1]);
        CHECK(min_component_value(v4) == Vec4{f[0]});
        CHECK(min_component_value_f32(v4) == f[0]);
        // TODO min
        CHECK((max_component_index(v4) == j ||
              max_component_index(v4) == k));

        if (j != 3 && k != 3) {
          auto a3 = v3.components();
          auto ra3 = a3;

          std::ranges::sort(a3);
          std::ranges::sort(ra3, std::greater<float>{});

          CHECK(sort(v3) == LVec3{a3.data()});
          CHECK(reverse_sort(v3) == LVec3{ra3.data()});

          CHECK(max_component_value(v3) == LVec3{f[1]});
          CHECK(max_component_value_f32(v3) == f[1]);
          CHECK(min_component_value(v3) == LVec3{f[0]});
          CHECK(min_component_value_f32(v3) == f[0]);

          CHECK((max_component_index(v3) == j ||
                max_component_index(v3) == k));
          CHECK((min_component_index(v3) != j &&
                min_component_index(v3) != k &&
                min_component_index(v3) >= 0 &&
                min_component_index(v3) <= 2));
        }
      }
    }

    //  three options
    for (int j = 0; j < 4; ++j) {
      stdVec4 a4{f[1], f[1], f[1], f[1]};

      a4[j] = f[0];

      v4 = Vec4(a4.data());
      v3 = LVec3(a4.data());

      std::ranges::sort(a4);

      auto ra4 = a4;
      std::ranges::sort(ra4, std::greater<float>{});

      CHECK(sort(v4) == Vec4{a4.data()});
      CHECK(reverse_sort(v4) == Vec4{ra4.data()});

      CHECK(max_component_value(v4) == Vec4{f[1]});
      CHECK(max_component_value_f32(v4) == f[1]);
      CHECK(min_component_value(v4) == Vec4{f[0]});
      CHECK(min_component_value_f32(v4) == f[0]);

      CHECK((min_component_index(v4) == j));
      CHECK((max_component_index(v4) != j &&
            max_component_index(v4) >= 0 &&
            max_component_index(v4) <= 3));

      if (j != 3) {
        auto a3 = v3.components();
        auto ra3 = a3;

        std::ranges::sort(a3);
        std::ranges::sort(ra3, std::greater<float>{});

        CHECK(sort(v3) == LVec3{a3.data()});
        CHECK(reverse_sort(v3) == LVec3{ra3.data()});

        CHECK(max_component_value(v3) == LVec3{f[1]});
        CHECK(max_component_value_f32(v3) == f[1]);
        CHECK(min_component_value(v3) == LVec3{f[0]});
        CHECK(min_component_value_f32(v3) == f[0]);

        CHECK((min_component_index(v3) == j));
        CHECK((max_component_index(v3) != j &&
              max_component_index(v3) >= 0 &&
              max_component_index(v3) <= 2));
      }
    }
  }
}

TEST_CASE("Sort, max and min - vectors with three distinct values", "[GenericMethods]")
{
  for (int i = 0; i < 100; ++i) {
    std::vector<float> f{};

    f.push_back(random_float());

    Vec4 v4{f[0]};
    LVec3 v3{f[0]};

    auto r = random_float();
    while (r == f[0]) r = random_float();
    f.push_back(r);

    while (r == f[0] || r == f[1]) r = random_float();
    f.push_back(r);
    std::ranges::sort(f);

    //  one option
    {
      // LVec3
      {
        auto permutations = create_permutations<3>();
        for (auto perm : permutations) {
          auto f_copy = f;
          for (int i = 0; i < 3; ++i) {
            f_copy[i] = f[perm[i]];
          }

          LVec3 v3(f_copy.data());

          auto a3 = v3.components();
          auto ra3 = a3;

          std::ranges::sort(a3);
          std::ranges::sort(ra3, std::greater<float>{});

          CHECK(sort(v3) == LVec3{a3.data()});
          CHECK(reverse_sort(v3) == LVec3{ra3.data()});

          // find where the highest index has been moved by the permutation
          size_t expected = 0;
          while (perm[expected] != 2)
            ++expected;

          CHECK(max_component_value(v3) == LVec3{f[2]});
          CHECK(max_component_value_f32(v3) == f[2]);
          CHECK(min_component_value(v3) == LVec3{f[0]});
          CHECK(min_component_value_f32(v3) == f[0]);

          CHECK(max_component_index(v3) == expected);
          CHECK((min_component_index(v3) != expected &&
                min_component_index(v3) >= 0 &&
                min_component_index(v3) <= 2));
        }
      }

      // Vec4
      {
        auto perm0 = create_permutations<4>();

        std::vector<std::array<size_t, 4>> perm_idx{};

        // repeat middle value
        std::array<size_t, 4> indices{0,1,1,2};

        // create permutations patterns with repeated values
        for (auto perm : perm0) {
          auto p = indices;
          for (int i = 0; i < 4; ++i) {
            p[i] = indices[perm[i]];
          }
          perm_idx.push_back(p);
        }

        // check actual permutations
        for (auto perm : perm_idx) {
          auto f_copy = f;
          for (int i = 0; i < 4; ++i) {
            f_copy[i] = f[perm[i]];
          }

          Vec4 v4(f_copy.data());

          auto a4 = v4.components();
          auto ra4 = a4;

          std::ranges::sort(a4);
          std::ranges::sort(ra4, std::greater<float>{});

          CHECK(sort(v4) == Vec4{a4.data()});
          CHECK(reverse_sort(v4) == Vec4{ra4.data()});

          // find where the highest index has been moved by the permutation
          size_t expected = 0;
          while (perm[expected] != 2)
            ++expected;

          CHECK(max_component_value(v4) == Vec4{f[2]});
          CHECK(max_component_value_f32(v4) == f[2]);
          CHECK(min_component_value(v4) == Vec4{f[0]});
          CHECK(min_component_value_f32(v4) == f[0]);

          CHECK(max_component_index(v4) == expected);
          CHECK((min_component_index(v4) != expected &&
                min_component_index(v4) >= 0 &&
                min_component_index(v4) <= 3));
        }
      }
    }

    //  two options

    // Vec4
    {
      auto perm0 = create_permutations<4>();

      std::vector<std::array<size_t, 4>> perm_idx{};

      // repeat highest value
      std::array<size_t, 4> indices{0,1,2,2};

      // create permutations patterns with repeated values
      for (auto perm : perm0) {
        auto p = indices;
        for (int i = 0; i < 4; ++i) {
          p[i] = indices[perm[i]];
        }
        perm_idx.push_back(p);
      }

      // check actual permutations
      for (auto perm : perm_idx) {
        auto f_copy = f;
        for (int i = 0; i < 4; ++i) {
          f_copy[i] = f[perm[i]];
        }

        Vec4 v4(f_copy.data());

        auto a4 = v4.components();
        auto ra4 = a4;

        std::ranges::sort(a4);
        std::ranges::sort(ra4, std::greater<float>{});

        CHECK(sort(v4) == Vec4{a4.data()});
        CHECK(reverse_sort(v4) == Vec4{ra4.data()});

        // find where the highest index has been moved by the permutation
        size_t expected0 = 0;
        while (perm[expected0] != 2)
          ++expected0;

        size_t expected1 = expected0 + 1;
        while (perm[expected1] != 2)
          ++expected1;

        CHECK(max_component_value(v4) == Vec4{f[2]});
        CHECK(max_component_value_f32(v4) == f[2]);
        CHECK((max_component_index(v4) == expected0 || max_component_index(v4) == expected1));
        CHECK((min_component_index(v4) != expected0 &&
              min_component_index(v4) != expected1 &&
              min_component_index(v4) >= 0 &&
              min_component_index(v4) <= 3));
      }
    }

    // LVec3
    {
      auto perm0 = create_permutations<3>();

      std::vector<std::array<size_t, 3>> perm_idx{};

      // repeat highest value
      std::array<size_t, 3> indices{0,1,1};

      // create permutations patterns with repeated values
      for (auto perm : perm0) {
        auto p = indices;
        for (int i = 0; i < 3; ++i) {
          p[i] = indices[perm[i]];
        }
        perm_idx.push_back(p);
      }

      // check actual permutations
      for (auto perm : perm_idx) {
        auto f_copy = f;
        for (int i = 0; i < 3; ++i) {
          f_copy[i] = f[perm[i]];
        }

        LVec3 v3(f_copy.data());

        auto a3 = v3.components();
        auto ra3 = a3;

        std::ranges::sort(a3);
        std::ranges::sort(ra3, std::greater<float>{});

        CHECK(sort(v3) == LVec3{a3.data()});
        CHECK(reverse_sort(v3) == LVec3{ra3.data()});

        // find where the highest index has been moved by the permutation
        size_t expected0 = 0;
        while (perm[expected0] != 1)
          ++expected0;

        size_t expected1 = expected0 + 1;
        while (perm[expected1] != 1)
          ++expected1;

        CHECK(max_component_value(v3) == LVec3{f[1]});
        CHECK(max_component_value_f32(v3) == f[1]);
        CHECK((max_component_index(v3) == expected0 || max_component_index(v3) == expected1));
        CHECK((min_component_index(v3) != expected0 &&
              min_component_index(v3) != expected1 &&
              min_component_index(v3) >= 0 &&
              min_component_index(v3) <= 2));
      }
    }
  }
}

TEST_CASE("Sort, max and min - vectors with 4 distinct values", "[GenericMethods]")
{
  for (int i = 0; i < 100; ++i) {
    std::vector<float> f{};
    f.push_back(random_float());

    Vec4 v4{f[0]};
    LVec3 v3{f[0]};

    auto r = random_float();
    while (r == f[0]) r = random_float();
    f.push_back(r);

    while (r == f[0] || r == f[1]) r = random_float();
    f.push_back(r);

    while (r == f[0] || r == f[1] || r == f[2]) r = random_float();
    f.push_back(r);
    std::ranges::sort(f);

    auto permutations = create_permutations<4>();

    for (auto perm : permutations) {
      auto f_copy = f;
      for (int i = 0; i < 4; ++i) {
        f_copy[i] = f[perm[i]];
      }

      Vec4 v4(f_copy.data());

      auto a4 = v4.components();
      auto ra4 = a4;

      std::ranges::sort(a4);
      std::ranges::sort(ra4, std::greater<float>{});

      CHECK(sort(v4) == Vec4{a4.data()});
      CHECK(reverse_sort(v4) == Vec4{ra4.data()});

      // find where the highest index has been moved by the permutation
      size_t expected = 0;
      while (perm[expected] != 3)
        ++expected;

      CHECK(max_component_value(v4) == Vec4{f[3]});
      CHECK(max_component_value_f32(v4) == f[3]);
      CHECK(max_component_index(v4) == expected);
    }
  }
}

TEST_CASE("floor, ceil, abs, sqrt, and rsqrt", "[GenericMethods]")
{
  for (int i = 0; i < 100; ++i) {
    Vec4  v4{ rnd4() };
    LVec3  v3{ rnd3() };

    auto a4 = v4.components();
    auto a3 = v3.components();

    CHECK_VEC4_COMPONENTS(floor(v4), std::floor(a4[0]), std::floor(a4[1]), std::floor(a4[2]), std::floor(a4[3]));
    CHECK_LVEC3_COMPONENTS(floor(v3), std::floor(a3[0]), std::floor(a3[1]), std::floor(a3[2]));

    CHECK_VEC4_COMPONENTS(ceil(v4), std::ceil(a4[0]), std::ceil(a4[1]), std::ceil(a4[2]), std::ceil(a4[3]));
    CHECK_LVEC3_COMPONENTS(ceil(v3), std::ceil(a3[0]), std::ceil(a3[1]), std::ceil(a3[2]));

    CHECK_VEC4_COMPONENTS(abs(v4), std::abs(a4[0]), std::abs(a4[1]), std::abs(a4[2]), std::abs(a4[3]));
    CHECK_LVEC3_COMPONENTS(abs(v3), std::abs(a3[0]), std::abs(a3[1]), std::abs(a3[2]));
  }

  for (int i = 0; i < 100; ++i) {
    Vec4  v4{ random_float(0.1f), random_float(0.1f), random_float(0.1f), random_float(0.1f) };
    LVec3  v3{ random_float(0.1f), random_float(0.1f), random_float(0.1f) };

    auto a4 = v4.components();
    auto a3 = v3.components();

    CHECK_VEC4_COMPONENTS(sqrt(v4), std::sqrt(a4[0]), std::sqrt(a4[1]), std::sqrt(a4[2]), std::sqrt(a4[3]));
    CHECK_LVEC3_COMPONENTS(sqrt(v3), std::sqrt(a3[0]), std::sqrt(a3[1]), std::sqrt(a3[2]));

    CHECK_VEC4_COMPONENTS(
      rsqrt(v4),
      Approx(1.f / std::sqrt(a4[0])).epsilon(0.001),
      Approx(1.f / std::sqrt(a4[1])).epsilon(0.001),
      Approx(1.f / std::sqrt(a4[2])).epsilon(0.001),
      Approx(1.f / std::sqrt(a4[3])).epsilon(0.001));
    CHECK_LVEC3_COMPONENTS(
      rsqrt(v3),
      Approx(1.f / std::sqrt(a3[0])).epsilon(0.001),
      Approx(1.f / std::sqrt(a3[1])).epsilon(0.001),
      Approx(1.f / std::sqrt(a3[2])).epsilon(0.001));
  }
}

TEST_CASE("Merge max and min", "[GenericMethods]")
{
  for (int i = 0; i < 100; ++i)
  {
    Vec4  v40{ rnd4() };
    Vec4  v41{ rnd4() };
    LVec3  v30{ rnd3() };
    LVec3  v31{ rnd3() };

    auto a40 = v40.components();
    auto a30 = v30.components();
    auto a41 = v41.components();
    auto a31 = v31.components();

    auto max4 = merge_max(v40, v41);
    auto max3 = merge_max(v30, v31);
    auto min4 = merge_min(v40, v41);
    auto min3 = merge_min(v30, v31);

    CHECK_VEC4_COMPONENTS(max4, std::max(a40[0],a41[0]), std::max(a40[1],a41[1]), std::max(a40[2],a41[2]), std::max(a40[3],a41[3]));
    CHECK_VEC4_COMPONENTS(min4, std::min(a40[0],a41[0]), std::min(a40[1],a41[1]), std::min(a40[2],a41[2]), std::min(a40[3],a41[3]));

    CHECK_LVEC3_COMPONENTS(max3, std::max(a30[0],a31[0]), std::max(a30[1],a31[1]), std::max(a30[2],a31[2]));
    CHECK_LVEC3_COMPONENTS(min3, std::min(a30[0],a31[0]), std::min(a30[1],a31[1]), std::min(a30[2],a31[2]));
  }
}

TEST_CASE("Dot product", "[GenericMethods]")
{
  for (int i = 0; i < 100; ++i)
  {
    Vec4  v40{ rnd4(-100.f, 100.f) };
    Vec4  v41{ rnd4(-100.f, 100.f) };
    LVec3  v30{ rnd3(-100.f, 100.f) };
    LVec3  v31{ rnd3(-100.f, 100.f) };

    auto a40 = v40.components();
    auto a30 = v30.components();
    auto a41 = v41.components();
    auto a31 = v31.components();

    CHECK(dot(v40, v41) == Approx(a40[0] * a41[0] + a40[1] * a41[1] + a40[2] * a41[2] + a40[3] * a41[3]).epsilon(5e-4));
    CHECK(dot(v30, v31) == Approx(a30[0] * a31[0] + a30[1] * a31[1] + a30[2] * a31[2]).epsilon(1e-4));
  }
}

TEST_CASE("Cross product", "[GenericMethods]")
{
  for (int i = 0; i < 100; ++i)
  {
    LVec3  v30{ rnd3() };
    LVec3  v31{ rnd3() };

    auto a30 = v30.components();
    auto a31 = v31.components();

    auto x = a30[1] * a31[2] - a30[2] * a31[1];
    auto y = a30[2] * a31[0] - a30[0] * a31[2];
    auto z = a30[0] * a31[1] - a30[1] * a31[0];

    CHECK_LVEC3_COMPONENTS(cross(v30, v31), x, y, z);
  }
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

TEST_CASE("Blend with mask", "[GenericMethods]")
{
  for (int i = 0; i < 100; ++i)
  {
    auto x = random_float();
    auto y = random_float();
    auto z = random_float();
    auto w = random_float();

    auto a = random_float();
    auto b = random_float();
    auto c = random_float();
    auto d = random_float();

    Vec4 v1{x, y, z, w};
    Vec4 v2{a, b, c, d};
    LVec3 v3{x, y, z};
    LVec3 v4{a, b, c};

    static_assert(std::is_same_v<decltype(blend_with_mask<0b0011>(v1, v2)), Vec4>);
    static_assert(std::is_same_v<decltype(blend_with_mask<0b0011>(v1, v3)), Vec4>);
    static_assert(std::is_same_v<decltype(blend_with_mask<0b0011>(v3, v1)), Vec4>);
    static_assert(std::is_same_v<decltype(blend_with_mask<0b0011>(v3, v4)), LVec3>);

    // 4D
    auto blended = blend_with_mask<0b0000>(v1, v2);
    CHECK_VEC4_COMPONENTS(blended, x, y, z, w);
    blended = blend_with_mask<0b0001>(v1, v2);
    CHECK_VEC4_COMPONENTS(blended, x, y, z, d);
    blended = blend_with_mask<0b0010>(v1, v2);
    CHECK_VEC4_COMPONENTS(blended, x, y, c, w);
    blended = blend_with_mask<0b0011>(v1, v2);
    CHECK_VEC4_COMPONENTS(blended, x, y, c, d);
    blended = blend_with_mask<0b0100>(v1, v2);
    CHECK_VEC4_COMPONENTS(blended, x, b, z, w);
    blended = blend_with_mask<0b0101>(v1, v2);
    CHECK_VEC4_COMPONENTS(blended, x, b, z, d);
    blended = blend_with_mask<0b0110>(v1, v2);
    CHECK_VEC4_COMPONENTS(blended, x, b, c, w);
    blended = blend_with_mask<0b0111>(v1, v2);
    CHECK_VEC4_COMPONENTS(blended, x, b, c, d);
    blended = blend_with_mask<0b1000>(v1, v2);
    CHECK_VEC4_COMPONENTS(blended, a, y, z, w);
    blended = blend_with_mask<0b1001>(v1, v2);
    CHECK_VEC4_COMPONENTS(blended, a, y, z, d);
    blended = blend_with_mask<0b1010>(v1, v2);
    CHECK_VEC4_COMPONENTS(blended, a, y, c, w);
    blended = blend_with_mask<0b1011>(v1, v2);
    CHECK_VEC4_COMPONENTS(blended, a, y, c, d);
    blended = blend_with_mask<0b1100>(v1, v2);
    CHECK_VEC4_COMPONENTS(blended, a, b, z, w);
    blended = blend_with_mask<0b1101>(v1, v2);
    CHECK_VEC4_COMPONENTS(blended, a, b, z, d);
    blended = blend_with_mask<0b1110>(v1, v2);
    CHECK_VEC4_COMPONENTS(blended, a, b, c, w);
    blended = blend_with_mask<0b1111>(v1, v2);
    CHECK_VEC4_COMPONENTS(blended, a, b, c, d);

    // 4D+3D
    blended = blend_with_mask<0b0000>(v1, v4);
    CHECK_VEC4_COMPONENTS(blended, x, y, z, w);
    blended = blend_with_mask<0b0001>(v1, v4);
    CHECK_VEC4_COMPONENTS(blended, x, y, z, 0.f);
    blended = blend_with_mask<0b0010>(v1, v4);
    CHECK_VEC4_COMPONENTS(blended, x, y, c, w);
    blended = blend_with_mask<0b0011>(v1, v4);
    CHECK_VEC4_COMPONENTS(blended, x, y, c, 0.f);
    blended = blend_with_mask<0b0100>(v1, v4);
    CHECK_VEC4_COMPONENTS(blended, x, b, z, w);
    blended = blend_with_mask<0b0101>(v1, v4);
    CHECK_VEC4_COMPONENTS(blended, x, b, z, 0.f);
    blended = blend_with_mask<0b0110>(v1, v4);
    CHECK_VEC4_COMPONENTS(blended, x, b, c, w);
    blended = blend_with_mask<0b0111>(v1, v4);
    CHECK_VEC4_COMPONENTS(blended, x, b, c, 0.f);
    blended = blend_with_mask<0b1000>(v1, v4);
    CHECK_VEC4_COMPONENTS(blended, a, y, z, w);
    blended = blend_with_mask<0b1001>(v1, v4);
    CHECK_VEC4_COMPONENTS(blended, a, y, z, 0.f);
    blended = blend_with_mask<0b1010>(v1, v4);
    CHECK_VEC4_COMPONENTS(blended, a, y, c, w);
    blended = blend_with_mask<0b1011>(v1, v4);
    CHECK_VEC4_COMPONENTS(blended, a, y, c, 0.f);
    blended = blend_with_mask<0b1100>(v1, v4);
    CHECK_VEC4_COMPONENTS(blended, a, b, z, w);
    blended = blend_with_mask<0b1101>(v1, v4);
    CHECK_VEC4_COMPONENTS(blended, a, b, z, 0.f);
    blended = blend_with_mask<0b1110>(v1, v4);
    CHECK_VEC4_COMPONENTS(blended, a, b, c, w);
    blended = blend_with_mask<0b1111>(v1, v4);
    CHECK_VEC4_COMPONENTS(blended, a, b, c, 0.f);

    //3D
    blended = blend_with_mask<0b0000>(v3, v4);
    CHECK_LVEC3_COMPONENTS(blended, x, y, z);
    blended = blend_with_mask<0b0001>(v3, v4);
    CHECK_LVEC3_COMPONENTS(blended, x, y, z);
    blended = blend_with_mask<0b0010>(v3, v4);
    CHECK_LVEC3_COMPONENTS(blended, x, y, c);
    blended = blend_with_mask<0b0011>(v3, v4);
    CHECK_LVEC3_COMPONENTS(blended, x, y, c);
    blended = blend_with_mask<0b0100>(v3, v4);
    CHECK_LVEC3_COMPONENTS(blended, x, b, z);
    blended = blend_with_mask<0b0101>(v3, v4);
    CHECK_LVEC3_COMPONENTS(blended, x, b, z);
    blended = blend_with_mask<0b0110>(v3, v4);
    CHECK_LVEC3_COMPONENTS(blended, x, b, c);
    blended = blend_with_mask<0b0111>(v3, v4);
    CHECK_LVEC3_COMPONENTS(blended, x, b, c);
    blended = blend_with_mask<0b1000>(v3, v4);
    CHECK_LVEC3_COMPONENTS(blended, a, y, z);
    blended = blend_with_mask<0b1001>(v3, v4);
    CHECK_LVEC3_COMPONENTS(blended, a, y, z);
    blended = blend_with_mask<0b1010>(v3, v4);
    CHECK_LVEC3_COMPONENTS(blended, a, y, c);
    blended = blend_with_mask<0b1011>(v3, v4);
    CHECK_LVEC3_COMPONENTS(blended, a, y, c);
    blended = blend_with_mask<0b1100>(v3, v4);
    CHECK_LVEC3_COMPONENTS(blended, a, b, z);
    blended = blend_with_mask<0b1101>(v3, v4);
    CHECK_LVEC3_COMPONENTS(blended, a, b, z);
    blended = blend_with_mask<0b1110>(v3, v4);
    CHECK_LVEC3_COMPONENTS(blended, a, b, c);
    blended = blend_with_mask<0b1111>(v3, v4);
    CHECK_LVEC3_COMPONENTS(blended, a, b, c);
  }
}

TEST_CASE("FSVF::clamp", "[GenericMethods]")
{
  Vec4 v4{-2.0f, 0.5f, 3.0f, 5.0f};
  Vec4 min4{0.0f, 1.0f, 1.0f, 1.0f};
  Vec4 max4{2.0f, 2.0f, 2.0f, 2.0f};
  auto clamped4 = clamp(v4, min4, max4);
  CHECK_VEC4_COMPONENTS(clamped4, 0.0f, 1.0f, 2.0f, 2.0f);

  LVec3 v3{-1.0f, 2.5f, 0.5f};
  LVec3 min3{0.0f, 0.0f, 0.0f};
  LVec3 max3{2.0f, 2.0f, 2.0f};
  auto clamped3 = clamp(v3, min3, max3);
  CHECK_LVEC3_COMPONENTS(clamped3, 0.0f, 2.0f, 0.5f);

  // Homogenous limits
  for (int i = 0; i < 100; ++i) {
    v4 = rnd4();
    v3 = rnd3();

    float minVal = random_float(-100.0f, 0.0f);
    float maxVal = random_float(0.0f, 100.0f);

    min4 = Vec4{minVal};
    max4 = Vec4{maxVal};
    min3 = LVec3{minVal};
    max3 = LVec3{maxVal};

    clamped4 = clamp(v4, min4, max4);
    clamped3 = clamp(v3, min3, max3);

    auto c4 = clamped4.components();
    auto c3 = clamped3.components();

    for (int j = 0; j < 4; ++j) {
      CHECK(c4[j] >= minVal);
      CHECK(c4[j] <= maxVal);
    }

    for (int j = 0; j < 3; ++j) {
      CHECK(c3[j] >= minVal);
      CHECK(c3[j] <= maxVal);
    }
  }

  // Varying min/max per component
  for (int i = 0; i < 100; ++i) {
    v4 = rnd4();
    auto w1 = rnd4();
    auto w2 = rnd4();

    min4 = merge_min(w1, w2);
    max4 = merge_max(w1, w2);

    clamped4 = clamp(v4, min4, max4);
    auto c4 = clamped4.components();
    auto min_c = min4.components();
    auto max_c = max4.components();

    for (int j = 0; j < 4; ++j) {
      CHECK(c4[j] >= min_c[j]);
      CHECK(c4[j] <= max_c[j]);
    }
  }
}

TEST_CASE("FSVF::all, any, none", "[GenericMethods]")
{
  constexpr float ones = bits_to_float(0xFFFF'FFFFu);

  // Test all() with 4D
  {
    Vec4 v0 = Vec4 { 0.f, 0.f, 0.f, 0.f };
    Vec4 v1 = Vec4 { 0.f, 0.f, 0.f, ones };
    Vec4 v2 = Vec4 { 0.f, 0.f, ones, 0.f };
    Vec4 v3 = Vec4 { 0.f, 0.f, ones, ones };
    Vec4 v4 = Vec4 { 0.f, ones, 0.f, 0.f };
    Vec4 v5 = Vec4 { 0.f, ones, 0.f, ones };
    Vec4 v6 = Vec4 { 0.f, ones, ones, 0.f };
    Vec4 v7 = Vec4 { 0.f, ones, ones, ones };
    Vec4 v8 = Vec4 { ones, 0.f, 0.f, 0.f };
    Vec4 v9 = Vec4 { ones, 0.f, 0.f, ones };
    Vec4 vA = Vec4 { ones, 0.f, ones, 0.f };
    Vec4 vB = Vec4 { ones, 0.f, ones, ones };
    Vec4 vC = Vec4 { ones, ones, 0.f, 0.f };
    Vec4 vD = Vec4 { ones, ones, 0.f, ones };
    Vec4 vE = Vec4 { ones, ones, ones, 0.f };
    Vec4 vF = Vec4 { ones, ones, ones, ones };

    CHECK(all<4>(v0) == false);
    CHECK(all<4>(v1) == false);
    CHECK(all<4>(v2) == false);
    CHECK(all<4>(v3) == false);
    CHECK(all<4>(v4) == false);
    CHECK(all<4>(v5) == false);
    CHECK(all<4>(v6) == false);
    CHECK(all<4>(v7) == false);
    CHECK(all<4>(v8) == false);
    CHECK(all<4>(v9) == false);
    CHECK(all<4>(vA) == false);
    CHECK(all<4>(vB) == false);
    CHECK(all<4>(vC) == false);
    CHECK(all<4>(vD) == false);
    CHECK(all<4>(vE) == false);
    CHECK(all<4>(vF) == true);

    CHECK(any<4>(v0) == false);
    CHECK(any<4>(v1) == true);
    CHECK(any<4>(v2) == true);
    CHECK(any<4>(v3) == true);
    CHECK(any<4>(v4) == true);
    CHECK(any<4>(v5) == true);
    CHECK(any<4>(v6) == true);
    CHECK(any<4>(v7) == true);
    CHECK(any<4>(v8) == true);
    CHECK(any<4>(v9) == true);
    CHECK(any<4>(vA) == true);
    CHECK(any<4>(vB) == true);
    CHECK(any<4>(vC) == true);
    CHECK(any<4>(vD) == true);
    CHECK(any<4>(vE) == true);
    CHECK(any<4>(vF) == true);

    CHECK(none<4>(v0) == true);
    CHECK(none<4>(v1) == false);
    CHECK(none<4>(v2) == false);
    CHECK(none<4>(v3) == false);
    CHECK(none<4>(v4) == false);
    CHECK(none<4>(v5) == false);
    CHECK(none<4>(v6) == false);
    CHECK(none<4>(v7) == false);
    CHECK(none<4>(v8) == false);
    CHECK(none<4>(v9) == false);
    CHECK(none<4>(vA) == false);
    CHECK(none<4>(vB) == false);
    CHECK(none<4>(vC) == false);
    CHECK(none<4>(vD) == false);
    CHECK(none<4>(vE) == false);
    CHECK(none<4>(vF) == false);

    CHECK(all<3>(v0) == false);
    CHECK(all<3>(v1) == false);
    CHECK(all<3>(v2) == false);
    CHECK(all<3>(v3) == false);
    CHECK(all<3>(v4) == false);
    CHECK(all<3>(v5) == false);
    CHECK(all<3>(v6) == false);
    CHECK(all<3>(v7) == false);
    CHECK(all<3>(v8) == false);
    CHECK(all<3>(v9) == false);
    CHECK(all<3>(vA) == false);
    CHECK(all<3>(vB) == false);
    CHECK(all<3>(vC) == false);
    CHECK(all<3>(vD) == false);
    CHECK(all<3>(vE) == true);
    CHECK(all<3>(vF) == true);

    CHECK(any<3>(v0) == false);
    CHECK(any<3>(v1) == false);
    CHECK(any<3>(v2) == true);
    CHECK(any<3>(v3) == true);
    CHECK(any<3>(v4) == true);
    CHECK(any<3>(v5) == true);
    CHECK(any<3>(v6) == true);
    CHECK(any<3>(v7) == true);
    CHECK(any<3>(v8) == true);
    CHECK(any<3>(v9) == true);
    CHECK(any<3>(vA) == true);
    CHECK(any<3>(vB) == true);
    CHECK(any<3>(vC) == true);
    CHECK(any<3>(vD) == true);
    CHECK(any<3>(vE) == true);
    CHECK(any<3>(vF) == true);

    CHECK(none<3>(v0) == true);
    CHECK(none<3>(v1) == true);
    CHECK(none<3>(v2) == false);
    CHECK(none<3>(v3) == false);
    CHECK(none<3>(v4) == false);
    CHECK(none<3>(v5) == false);
    CHECK(none<3>(v6) == false);
    CHECK(none<3>(v7) == false);
    CHECK(none<3>(v8) == false);
    CHECK(none<3>(v9) == false);
    CHECK(none<3>(vA) == false);
    CHECK(none<3>(vB) == false);
    CHECK(none<3>(vC) == false);
    CHECK(none<3>(vD) == false);
    CHECK(none<3>(vE) == false);
    CHECK(none<3>(vF) == false);
  }
}

TEST_CASE("FSVF::and_not", "[GenericMethods]")
{
  // Test basic functionality
  for (int i = 0; i < 100; ++i) {
    uint32_t a = random_bits();
    uint32_t b = random_bits();
    uint32_t c = random_bits();
    uint32_t d = random_bits();

    uint32_t x = random_bits();
    uint32_t y = random_bits();
    uint32_t z = random_bits();
    uint32_t w = random_bits();

    Vec4 v1{bits_to_float(a), bits_to_float(b), bits_to_float(c), bits_to_float(d)};
    Vec4 v2{bits_to_float(x), bits_to_float(y), bits_to_float(z), bits_to_float(w)};

    auto result = and_not(v1, v2);
    auto res_c = result.components();

    CHECK_BITWISE_EQ(res_c[0], bits_to_float(a & ~x));
    CHECK_BITWISE_EQ(res_c[1], bits_to_float(b & ~y));
    CHECK_BITWISE_EQ(res_c[2], bits_to_float(c & ~z));
    CHECK_BITWISE_EQ(res_c[3], bits_to_float(d & ~w));
  }

  // Test with known patterns
  {
    Vec4 all_ones{bits_to_float(0xFFFFFFFF)};
    Vec4 all_zeros{bits_to_float(0u)};
    Vec4 pattern{bits_to_float(0xF0F0F0F0)};

    auto res1 = and_not(all_ones, all_zeros);
    CHECK(bitwise_equal(res1, all_ones));

    auto res2 = and_not(all_zeros, all_ones);
    CHECK(bitwise_equal(res2, all_zeros));

    auto res3 = and_not(pattern, all_zeros);
    CHECK(bitwise_equal(res3, pattern));

    auto res4 = and_not(pattern, all_ones);
    CHECK(bitwise_equal(res4, all_zeros));
  }
}

TEST_CASE("FSVF::bitwise_equal", "[GenericMethods]")
{
  for (int i = 0; i < 100; ++i) {
    Vec4 v = rnd4();
    LVec3 v3 = rnd3();

    CHECK(bitwise_equal(v, v) == true);
    CHECK(bitwise_equal(v3, v3) == true);
  }

  for (int i = 0; i < 100; ++i) {
    Vec4 v1 = rnd4();
    Vec4 v2 = rnd4();

    while (bitwise_equal(v1, v2)) {
      v2 = rnd4();
    }

    CHECK(bitwise_equal(v1, v2) == false);
  }

  {
#ifndef FSVF_FINITE_MATH
    Vec4 nan1{create_nan(0x12345)};
    Vec4 nan2{create_nan(0x54321)};

    CHECK(bitwise_equal(nan1, nan1) == true);
    CHECK(bitwise_equal(nan1, nan2) == false);
#endif

    Vec4 z1 = Vec4{0.f};
    Vec4 z2 = Vec4{-0.f};
    Vec4 z3 = Vec4{-0.f, 0.f, 0.f, -0.f};

    CHECK(bitwise_equal(z1, z1));
    CHECK(!bitwise_equal(z1, z2));
    CHECK(!bitwise_equal(z1, z3));
    CHECK(!bitwise_equal(z2, z1));
    CHECK(bitwise_equal(z2, z2));
    CHECK(!bitwise_equal(z2, z3));
    CHECK(!bitwise_equal(z3, z1));
    CHECK(!bitwise_equal(z3, z2));
    CHECK(bitwise_equal(z3, z3));
  }
}

TEST_CASE("Equality", "[GenericMethods]")
{
  {
    Vec4 v1{1.0f, 2.0f, 3.0f, 4.0f};
    Vec4 v2{1.0f, 2.0f, 5.0f, 4.0f};
    Vec4 v3{2.0f, 4.0f, 8.0f, 1.0f};

    auto eq11 = BitMasks::cmp_equal_f32(v1, v1);
    auto eq12 = BitMasks::cmp_equal_f32(v1, v2);
    auto eq13 = BitMasks::cmp_equal_f32(v1, v3);
    auto beq11 = BitMasks::cmp_equal_bitwise(v1, v1);
    auto beq12 = BitMasks::cmp_equal_bitwise(v1, v2);
    auto beq13 = BitMasks::cmp_equal_bitwise(v1, v3);

    CHECK(all<4>(eq11) == true);
    CHECK(all<4>(eq12) == false);
    CHECK(all<4>(eq13) == false);
    CHECK(all<4>(beq11) == true);
    CHECK(all<4>(beq12) == false);
    CHECK(all<4>(beq13) == false);

    CHECK(any<4>(eq11) == true);
    CHECK(any<4>(eq12) == true);
    CHECK(any<4>(eq13) == false);
    CHECK(any<4>(beq11) == true);
    CHECK(any<4>(beq12) == true);
    CHECK(any<4>(beq13) == false);

    auto neq11 = BitMasks::cmp_not_equal_f32(v1, v1);
    auto neq12 = BitMasks::cmp_not_equal_f32(v1, v2);
    auto neq13 = BitMasks::cmp_not_equal_f32(v1, v3);
    auto bneq11 = BitMasks::cmp_not_equal_bitwise(v1, v1);
    auto bneq12 = BitMasks::cmp_not_equal_bitwise(v1, v2);
    auto bneq13 = BitMasks::cmp_not_equal_bitwise(v1, v3);

    CHECK(all<4>(neq11) == false);
    CHECK(all<4>(neq12) == false);
    CHECK(all<4>(neq13) == true);

    CHECK(any<4>(neq11) == false);
    CHECK(any<4>(neq12) == true);
    CHECK(any<4>(neq13) == true);
  }

  // Test with random values
  for (int i = 0; i < 100; ++i) {

    // allow few bits of difference between min and max,
    // to increase the chances of getting equal values
    constexpr auto min = bits_to_float(0x0F02'B60u);
    constexpr auto max = bits_to_float(0x0F02'B70u);

    Vec4 v1 = rnd4(min, max);
    Vec4 v2 = rnd3(min, max);

    auto eq_mask = BitMasks::cmp_equal_f32(v1, v2);
    auto neq_mask = BitMasks::cmp_not_equal_f32(v1, v2);
    auto beq_mask = BitMasks::cmp_equal_f32(v1, v2);
    auto bneq_mask = BitMasks::cmp_not_equal_f32(v1, v2);

    auto v1_c = v1.components();
    auto v2_c = v2.components();
    auto eq_c = eq_mask.components();
    auto neq_c = neq_mask.components();
    auto beq_c = beq_mask.components();
    auto bneq_c = bneq_mask.components();

    for (int j = 0; j < 4; ++j) {
      if (v1_c[j] == v2_c[j]) {
        CHECK_BITWISE_EQ(eq_c[j], bits_to_float(0xFFFFFFFF));
        CHECK_BITWISE_EQ(neq_c[j], bits_to_float(0u));
        CHECK_BITWISE_EQ(beq_c[j], bits_to_float(0xFFFFFFFF));
        CHECK_BITWISE_EQ(bneq_c[j], bits_to_float(0u));
      } else {
        CHECK_BITWISE_EQ(eq_c[j], bits_to_float(0u));
        CHECK_BITWISE_EQ(neq_c[j], bits_to_float(0xFFFFFFFF));
        CHECK_BITWISE_EQ(beq_c[j], bits_to_float(0u));
        CHECK_BITWISE_EQ(bneq_c[j], bits_to_float(0xFFFFFFFF));
      }
    }
  }

#define CHECK_BITWISE_OPPOSITE(v1, v2) \
    { \
      using namespace BitMasks; \
      CHECK(all<4>(cmp_equal_bitwise(cmp_equal_f32(v1,v2), ~cmp_not_equal_f32(v1, v2)))); \
      CHECK(all<4>(cmp_equal_bitwise(cmp_equal_bitwise(v1,v2), ~cmp_not_equal_bitwise(v1, v2)))); \
    }

  {
    using namespace BitMasks;

    Vec4 z1 = Vec4{0.f};
    Vec4 z2 = Vec4{-0.f};
    Vec4 z3 = Vec4{-0.f, 0.f, 0.f, -0.f};

    Vec4 v1 = Vec4{0.f};
    Vec4 v2 = Vec4{-0.f};
    Vec4 v3 = Vec4{-0.f, 0.f, 0.f, -0.f};

    // TODO add checks for negation once we added tests for bit flip

    CHECK(all<4>(cmp_equal_f32(z1, z1)));
    CHECK(all<4>(cmp_equal_f32(z1, z2)));
    CHECK(all<4>(cmp_equal_f32(z1, z3)));
    CHECK(all<4>(cmp_equal_f32(z2, z1)));
    CHECK(all<4>(cmp_equal_f32(z2, z2)));
    CHECK(all<4>(cmp_equal_f32(z2, z3)));
    CHECK(all<4>(cmp_equal_f32(z3, z1)));
    CHECK(all<4>(cmp_equal_f32(z3, z2)));
    CHECK(all<4>(cmp_equal_f32(z3, z3)));

    CHECK_BITWISE_OPPOSITE(z1, z1);
    CHECK_BITWISE_OPPOSITE(z1, z2);
    CHECK_BITWISE_OPPOSITE(z1, z3);
    CHECK_BITWISE_OPPOSITE(z2, z1);
    CHECK_BITWISE_OPPOSITE(z2, z2);
    CHECK_BITWISE_OPPOSITE(z2, z3);
    CHECK_BITWISE_OPPOSITE(z3, z1);
    CHECK_BITWISE_OPPOSITE(z3, z2);
    CHECK_BITWISE_OPPOSITE(z3, z3);

    CHECK(all<4>(cmp_equal_bitwise(z1, z1)));
    CHECK(all<4>(cmp_equal_bitwise(z2, z2)));
    CHECK(all<4>(cmp_equal_bitwise(z3, z3)));

    CHECK(none<4>(cmp_equal_bitwise(z1, z2)));
    CHECK(none<4>(cmp_equal_bitwise(z2, z1)));

    CHECK(!all<4>(cmp_equal_bitwise(z1, z3)));
    CHECK(!all<4>(cmp_equal_bitwise(z2, z3)));
    CHECK(!all<4>(cmp_equal_bitwise(z3, z1)));
    CHECK(!all<4>(cmp_equal_bitwise(z3, z2)));

    CHECK(any<4>(cmp_equal_bitwise(z1, z3)));
    CHECK(any<4>(cmp_equal_bitwise(z2, z3)));
    CHECK(any<4>(cmp_equal_bitwise(z3, z1)));
    CHECK(any<4>(cmp_equal_bitwise(z3, z2)));

#ifndef FSVF_FINITE_MATH
    constexpr auto inf = std::numeric_limits<float>::infinity();
    constexpr uint32_t t = 0xFFFF'FFFFu;
    constexpr uint32_t f = 0u;

    Vec4 nan{create_nan()};
    Vec4 vinf{inf, -inf, -inf, inf};
    Vec4 normal{1.0f, 3.14f, 4.44f, 0.6f};
    Vec4 mixed{1.0f, create_nan(), 4.44f, create_nan()};

    CHECK_VEC4_BITWISE_EQ(cmp_equal_f32(nan, nan), f, f, f, f);
    CHECK_VEC4_BITWISE_EQ(cmp_equal_f32(vinf, vinf), t, t, t, t);
    CHECK_VEC4_BITWISE_EQ(cmp_equal_f32(nan, normal), f, f, f, f);
    CHECK_VEC4_BITWISE_EQ(cmp_equal_f32(nan, mixed), f, f, f, f);
    CHECK_VEC4_BITWISE_EQ(cmp_equal_f32(mixed, normal), t, f, t, f);

    CHECK_VEC4_BITWISE_EQ(cmp_equal_bitwise(nan, nan), t, t, t, t);
    CHECK_VEC4_BITWISE_EQ(cmp_equal_bitwise(vinf, vinf), t, t, t, t);
    CHECK_VEC4_BITWISE_EQ(cmp_equal_bitwise(nan, normal), f, f, f, f);
    CHECK_VEC4_BITWISE_EQ(cmp_equal_bitwise(nan, mixed), f, t, f, t);
    CHECK_VEC4_BITWISE_EQ(cmp_equal_bitwise(mixed, normal), t, f, t, f);

    CHECK_VEC4_BITWISE_EQ(cmp_not_equal_f32(nan, nan), t, t, t, t);
    CHECK_VEC4_BITWISE_EQ(cmp_not_equal_f32(vinf, vinf), f, f, f, f);
    CHECK_VEC4_BITWISE_EQ(cmp_not_equal_f32(nan, normal), t, t, t, t);
    CHECK_VEC4_BITWISE_EQ(cmp_not_equal_f32(nan, mixed), t, t, t, t);
    CHECK_VEC4_BITWISE_EQ(cmp_not_equal_f32(mixed, normal), f, t, f, t);

    CHECK_VEC4_BITWISE_EQ(cmp_not_equal_bitwise(nan, nan), f, f, f, f);
    CHECK_VEC4_BITWISE_EQ(cmp_not_equal_bitwise(vinf, vinf), f, f, f, f);
    CHECK_VEC4_BITWISE_EQ(cmp_not_equal_bitwise(nan, normal), t, t, t, t);
    CHECK_VEC4_BITWISE_EQ(cmp_not_equal_bitwise(nan, mixed), t, f, t, f);
    CHECK_VEC4_BITWISE_EQ(cmp_not_equal_bitwise(mixed, normal), f, t, f, t);
#endif
  }
}

TEST_CASE("FSVF::hi_bits", "[GenericMethods]")
{
  {
    Vec4 all_positive{1.0f, 2.0f, 3.0f, 4.0f};
    Vec4 all_negative{-1.0f, -2.0f, -3.0f, -4.0f};
    Vec4 mixed{-1.0f, -2.0f, 3.0f, 4.0f};

    CHECK(BitMasks::hi_bits(all_positive) == 0b0000);
    CHECK(BitMasks::hi_bits(all_negative) == 0b1111);
    CHECK(BitMasks::hi_bits(mixed) == 0b1100);
  }

  for (int i = 0; i < 100; ++i) {
    Vec4 v = rnd4();
    auto components = v.components();
    uint32_t res = BitMasks::hi_bits(v);
    uint32_t check = 0;

    for (int j = 0; j < 4; ++j) {
      if (std::signbit(components[j])) {
        check |= (1 << (3-j));
      }
    }

    res |= 0x8000'0000;
    check |= 0x8000'0000;

    CHECK(res == check);
  }
}
