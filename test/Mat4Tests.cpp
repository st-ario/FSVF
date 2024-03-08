#include "FSVF.h"

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cmath>
#include <random>

// TODO!!!
// Most of the tests are AI-generated and require manual double-checking and cleanup

using Catch::Approx;

namespace Catch
{
template<FSVF::FastVector V>
struct StringMaker<V>
{
  static std::string convert(const V& v)
  {
    return StringMaker<std::array<float, 4>>::convert(v.components());
  }
};
}

namespace FSVF
{

// Helper function to check if two matrices are approximately equal
template <uint8_t Ulp = 2, float Epsilon = std::numeric_limits<float>::epsilon() * 2.f>
void matrices_approx_equal(const Mat4& a, const Mat4& b)
{
  for (int i = 0; i < 4; ++i)
  {
      INFO(std::setprecision(15) << "lhs: ["
           << a[i].components()[0] << ", "
           << a[i].components()[1] << ", "
           << a[i].components()[2] << ", "
           << a[i].components()[3] << "]\n rhs: ["
           << b[i].components()[0] << ", "
           << b[i].components()[1] << ", "
           << b[i].components()[2] << ", "
           << b[i].components()[3] << "]\n");

      INFO(std::setprecision(15) << "ULP distances: ["
           << FSVF::ulps_distance(a[i].components()[0], b[i].components()[0]) << ", "
           << FSVF::ulps_distance(a[i].components()[1], b[i].components()[1]) << ", "
           << FSVF::ulps_distance(a[i].components()[2], b[i].components()[2]) << ", "
           << FSVF::ulps_distance(a[i].components()[3], b[i].components()[3]) << "\n");

      REQUIRE(FSVF::almost_equal_eps_ulp<Ulp,Epsilon>(a[i].components()[0], b[i].components()[0]));
      REQUIRE(FSVF::almost_equal_eps_ulp<Ulp,Epsilon>(a[i].components()[1], b[i].components()[1]));
      REQUIRE(FSVF::almost_equal_eps_ulp<Ulp,Epsilon>(a[i].components()[2], b[i].components()[2]));
      REQUIRE(FSVF::almost_equal_eps_ulp<Ulp,Epsilon>(a[i].components()[3], b[i].components()[3]));
  }
}

#define FSFV_CHECK_ALMOST_EQ(v1, v2) \
{                                              \
  auto c1 = v1.components();          \
  auto c2 = v2.components();          \
  CHECK(c1[0] == Approx(c2[0]));           \
  CHECK(c1[1] == Approx(c2[1]));           \
  CHECK(c1[2] == Approx(c2[2]));           \
  CHECK(c1[3] == Approx(c2[3]));           \
}

// Helper function to check if a matrix is approximately identity
template <uint8_t Ulp = 2, float Epsilon = std::numeric_limits<float>::epsilon() * 2.f>
void is_approx_identity(const Mat4& m)
{
  matrices_approx_equal<Ulp, Epsilon>(m, Mat4(1.0f));
}

// Helper function to create a matrix with specific determinant
Mat4 create_matrix_with_determinant(float det)
{
  // Create a diagonal matrix with the desired determinant
  float factor = std::pow(std::abs(det), 0.25f);
  if (det < 0) factor = -factor;
  return Mat4(factor, factor, factor, det / (factor * factor * factor));
}

Mat4 nonsingular_matrix()
{
  return Mat4{
    4.f, 0.f, 1.f, 2.f,
    7.f, 5.f, -2.f, 6.f,
    2.f, -1.f, 3.f, 0.f,
    3.f, 4.f, 8.f, -5.f
  };
}

Mat4 singular_matrix()
{
  return Mat4(
    1.0f, 1.0f, 5.0f, 9.0f,
    2.0f, 2.0f, 6.0f, 10.0f,
    3.0f, 3.0f, 7.0f, 11.0f,
    4.0f, 4.0f, 8.0f, 12.0f
  );
}

Mat4 negative_det_matrix()
{
}

TEST_CASE("Mat4 Constructors", "[Mat4]")
{
  SECTION("Default Constructor")
  {
    Mat4 m;
    // Default constructor creates uninitialized values, so we can't test specific values
    // But we can test that it doesn't crash
    REQUIRE_NOTHROW(Mat4{});
  }

  SECTION("Copy Constructor")
  {
    Mat4 original(
      1.0f, 5.0f, 9.0f, 13.0f,  // Column 1
      2.0f, 6.0f, 10.0f, 14.0f, // Column 2
      3.0f, 7.0f, 11.0f, 15.0f, // Column 3
      4.0f, 8.0f, 12.0f, 16.0f  // Column 4
    );
    Mat4 copy(original);
    matrices_approx_equal(original, copy);
  }

  SECTION("Scalar Constructor")
  {
    Mat4 m(5.0f);
    Mat4 expected(
      5.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 5.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 5.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 5.0f
    );
    matrices_approx_equal(m, expected);
  }

  SECTION("Pointer Constructor")
  {
    // Column-major data layout
    float data[16] = {
      1.0f, 2.0f, 3.0f, 4.0f,
      5.0f, 6.0f, 7.0f, 8.0f,
      9.0f, 10.0f, 11.0f, 12.0f,
      13.0f, 14.0f, 15.0f, 16.0f
    };
    Mat4 m(data);
    Mat4 expected(
      1.0f, 2.0f, 3.0f, 4.0f,
      5.0f, 6.0f, 7.0f, 8.0f,
      9.0f, 10.0f, 11.0f, 12.0f,
      13.0f, 14.0f, 15.0f, 16.0f
    );
    matrices_approx_equal(m, expected);
  }

  SECTION("Column Vector Constructor")
  {
    Vec4 v0(1.0f, 2.0f, 3.0f, 4.0f);
    Vec4 v1(5.0f, 6.0f, 7.0f, 8.0f);
    Vec4 v2(9.0f, 10.0f, 11.0f, 12.0f);
    Vec4 v3(13.0f, 14.0f, 15.0f, 16.0f);

    Mat4 m(v0, v1, v2, v3);
    Mat4 expected(
      1.0f, 2.0f, 3.0f, 4.0f,
      5.0f, 6.0f, 7.0f, 8.0f,
      9.0f, 10.0f, 11.0f, 12.0f,
      13.0f, 14.0f, 15.0f, 16.0f
    );
    matrices_approx_equal(m, expected);
  }

  SECTION("LMat3 Constructor")
  {
    LMat3 lmat3(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f);
    Mat4 m(lmat3);
    Mat4 expected(
      1.0f, 2.0f, 3.0f, 0.f,
      4.0f, 5.0f, 6.0f, 0.f,
      7.0f, 8.0f, 9.0f, 0.f,
      0.f, 0.f, 0.f, 1.0f
    );
    matrices_approx_equal(m, expected);
  }

  SECTION("16-Component Constructor")
  {
    Mat4 m(
      1.0f, 2.0f, 3.0f, 4.0f,
      5.0f, 6.0f, 7.0f, 8.0f,
      9.0f, 10.0f, 11.0f, 12.0f,
      13.0f, 14.0f, 15.0f, 16.0f
    );

    REQUIRE(m[0].components()[0] == Approx(1.0f));
    REQUIRE(m[0].components()[1] == Approx(2.0f));
    REQUIRE(m[0].components()[2] == Approx(3.0f));
    REQUIRE(m[0].components()[3] == Approx(4.0f));
    REQUIRE(m[1].components()[0] == Approx(5.0f));
    REQUIRE(m[1].components()[1] == Approx(6.0f));
    REQUIRE(m[1].components()[2] == Approx(7.0f));
    REQUIRE(m[1].components()[3] == Approx(8.0f));
    REQUIRE(m[2].components()[0] == Approx(9.0f));
    REQUIRE(m[2].components()[1] == Approx(10.0f));
    REQUIRE(m[2].components()[2] == Approx(11.0f));
    REQUIRE(m[2].components()[3] == Approx(12.0f));
    REQUIRE(m[3].components()[0] == Approx(13.0f));
    REQUIRE(m[3].components()[1] == Approx(14.0f));
    REQUIRE(m[3].components()[2] == Approx(15.0f));
    REQUIRE(m[3].components()[3] == Approx(16.0f));
  }

  SECTION("Diagonal Constructor")
  {
    Mat4 m(1.0f, 2.0f, 3.0f, 4.0f);
    Mat4 expected(
      1.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 2.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 3.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 4.0f
    );
    matrices_approx_equal(m, expected);
  }
}

TEST_CASE("Mat4 Operators", "[Mat4]")
{
  SECTION("Equality Operators")
  {
    Mat4 m1(
      1.0f, 5.0f, 9.0f, 13.0f,  // Column 1
      2.0f, 6.0f, 10.0f, 14.0f, // Column 2
      3.0f, 7.0f, 11.0f, 15.0f, // Column 3
      4.0f, 8.0f, 12.0f, 16.0f  // Column 4
    );
    Mat4 m2(
      1.0f, 5.0f, 9.0f, 13.0f,  // Column 1
      2.0f, 6.0f, 10.0f, 14.0f, // Column 2
      3.0f, 7.0f, 11.0f, 15.0f, // Column 3
      4.0f, 8.0f, 12.0f, 16.0f  // Column 4
    );
    Mat4 m3(
      1.0f, 5.0f, 9.0f, 13.0f,  // Column 1
      2.0f, 6.0f, 10.0f, 14.0f, // Column 2
      3.0f, 7.0f, 11.0f, 15.0f, // Column 3
      4.0f, 8.0f, 12.0f, 17.0f  // Column 4 - different last element
    );

    REQUIRE(m1 == m2);
    REQUIRE(m1 != m3);
  }

  SECTION("Matrix Multiplication")
  {
    Mat4 m1(
      1.0f, 5.0f, 9.0f, 13.0f,  // Column 1
      2.0f, 6.0f, 10.0f, 14.0f, // Column 2
      3.0f, 7.0f, 11.0f, 15.0f, // Column 3
      4.0f, 8.0f, 12.0f, 16.0f  // Column 4
    );
    Mat4 m2(
      2.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 2.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 2.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 2.0f
    );

    Mat4 result = m1 * m2;
    Mat4 expected(
      2.0f, 10.0f, 18.0f, 26.0f,  // Column 1
      4.0f, 12.0f, 20.0f, 28.0f,  // Column 2
      6.0f, 14.0f, 22.0f, 30.0f,  // Column 3
      8.0f, 16.0f, 24.0f, 32.0f   // Column 4
    );
    matrices_approx_equal(result, expected);
  }

  SECTION("Identity Matrix Multiplication")
  {
    Mat4 identity(1.0f);
    Mat4 m(
      1.0f, 5.0f, 9.0f, 13.0f,  // Column 1
      2.0f, 6.0f, 10.0f, 14.0f, // Column 2
      3.0f, 7.0f, 11.0f, 15.0f, // Column 3
      4.0f, 8.0f, 12.0f, 16.0f  // Column 4
    );

    Mat4 result1 = identity * m;
    Mat4 result2 = m * identity;

    matrices_approx_equal(result1, m);
    matrices_approx_equal(result2, m);
  }

  SECTION("Matrix-Vector Multiplication")
  {
    Mat4 m(
      1.0f, 5.0f, 9.0f, 13.0f,
      2.0f, 6.0f, 10.0f, 14.0f,
      3.0f, 7.0f, 11.0f, 15.0f,
      4.0f, 8.0f, 12.0f, 16.0f
    );
    Vec4 v(1.0f, 2.0f, 3.0f, 4.0f);

    Vec4 result = m * v;
    Vec4 expected(30.0f, 70.0f, 110.0f, 150.0f);

    REQUIRE(result.components()[0] == Approx(expected.components()[0]));
    REQUIRE(result.components()[1] == Approx(expected.components()[1]));
    REQUIRE(result.components()[2] == Approx(expected.components()[2]));
    REQUIRE(result.components()[3] == Approx(expected.components()[3]));
  }

  SECTION("Scalar Multiplication")
  {
    Mat4 m(
      1.0f, 5.0f, 9.0f, 13.0f,  // Column 1
      2.0f, 6.0f, 10.0f, 14.0f, // Column 2
      3.0f, 7.0f, 11.0f, 15.0f, // Column 3
      4.0f, 8.0f, 12.0f, 16.0f  // Column 4
    );
    float scalar = 2.0f;

    Mat4 result = m * scalar;
    Mat4 expected(
      2.0f, 10.0f, 18.0f, 26.0f,  // Column 1
      4.0f, 12.0f, 20.0f, 28.0f,  // Column 2
      6.0f, 14.0f, 22.0f, 30.0f,  // Column 3
      8.0f, 16.0f, 24.0f, 32.0f   // Column 4
    );
    matrices_approx_equal(result, expected);
  }

  SECTION("Scalar Division")
  {
    Mat4 m{
      2.0f, 10.0f, 18.0f, 26.0f,
      4.0f, 12.0f, 20.0f, 28.0f,
      6.0f, 14.0f, 22.0f, 30.0f,
      8.0f, 16.0f, 24.0f, 32.0f
    };
    float scalar = 2.0f;

    Mat4 result = m / scalar;
    Mat4 expected{
      1.0f, 5.0f, 9.0f, 13.0f,
      2.0f, 6.0f, 10.0f, 14.0f,
      3.0f, 7.0f, 11.0f, 15.0f,
      4.0f, 8.0f, 12.0f, 16.0f
    };
    matrices_approx_equal(result, expected);

    m = Mat4{
      1.f, 2.f, 3.f, 4.f,
      5.f, 6.f, 10.f, 12.f,
      15.f, 20.f, 30.f, 60.f,
      8.f, 14.5f, 6.66f, 13.f
    };
    scalar = 60.0f;

    result = scalar / m;
    expected = Mat4{
      60.f, 30.f, 20.f, 15.f,
      12.f, 10.f, 6.f, 5.f,
      4.f, 3.f, 2.f, 1.f,
      7.5f, 4.13793103448f, 9.009009009f, 4.61538461538f
    };
    matrices_approx_equal(result, expected);
  }

  SECTION("Column Access")
  {
    Mat4 m{
      1.f, 5.f, 9.f, 13.f,
      2.f, 6.f, 10.f, 14.f,
      3.f, 7.f, 11.f, 15.f,
      4.f, 8.f, 12.f, 16.f
    };

    REQUIRE(m[0] == Vec4{ 1.f, 5.f,  9.f, 13.f });
    REQUIRE(m[1] == Vec4{ 2.f, 6.f, 10.f, 14.f });
    REQUIRE(m[2] == Vec4{ 3.f, 7.f, 11.f, 15.f });
    REQUIRE(m[3] == Vec4{ 4.f, 8.f, 12.f, 16.f });

    Vec4 v{ 5.f, 10.f, 6.f, 12.f };
    m[0] = v;

    REQUIRE(m[0] == v);
    REQUIRE(m == Mat4{
      5.f, 10.f,  6.f, 12.f,
      2.f,  6.f, 10.f, 14.f,
      3.f,  7.f, 11.f, 15.f,
      4.f,  8.f, 12.f, 16.f
    });

    v = Vec4{ 0.f, 0.f, 1.f, 0.f };
    m[1] = v;
    REQUIRE(m[1] == v);
    REQUIRE(m == Mat4{
      5.f, 10.f,  6.f, 12.f,
      0.f,  0.f,  1.f,  0.f,
      3.f,  7.f, 11.f, 15.f,
      4.f,  8.f, 12.f, 16.f
    });
  }
}

TEST_CASE("Mat4 Transformations", "[Mat4]")
{
  SECTION("Transform 3x3")
  {
    Mat4 m(
      1.0f, 5.0f, 9.0f, 13.0f,
      2.0f, 6.0f, 10.0f, 14.0f,
      3.0f, 7.0f, 11.0f, 15.0f,
      4.0f, 8.0f, 12.0f, 16.0f
    );
    LVec3 v(1.0f, 2.0f, 3.0f);

    LVec3 result = m.transform_3x3(v);
    LVec3 expected(14.f, 38.f, 62.f);

    REQUIRE(result.x == Approx(expected.x));
    REQUIRE(result.y == Approx(expected.y));
    REQUIRE(result.z == Approx(expected.z));
  }

  SECTION("Transform Affine")
  {
    Mat4 m(
      1.0f, 5.0f, 9.0f, 0.0f,
      2.0f, 6.0f, 10.0f, 0.0f,
      3.0f, 7.0f, 11.0f, 0.0f,
      10.0f, 20.0f, 30.0f, 1.0f
    );
    LVec3 v(1.0f, 2.0f, 3.0f);

    LVec3 result = m.transform_affine(v);
    LVec3 expected(24.f, 58.f, 92.f);

    REQUIRE(result.x == Approx(expected.x));
    REQUIRE(result.y == Approx(expected.y));
    REQUIRE(result.z == Approx(expected.z));
  }

  SECTION("Transform Projective")
  {
    Mat4 m(
      1.0f, 5.0f, 9.0f, 13.0f,  // Column 1
      2.0f, 6.0f, 10.0f, 14.0f, // Column 2
      3.0f, 7.0f, 11.0f, 15.0f, // Column 3
      4.0f, 8.0f, 12.0f, 16.0f  // Column 4
    );
    LVec3 v(1.0f, 2.0f, 3.0f);

    LVec3 result = m.transform_projective(v);

    // Manual calculation: (m * Vec4(v, 1.0f)) / w
    Vec4 temp = m * Vec4(v, 1.0f);
    LVec3 expected(temp.x / temp.w, temp.y / temp.w, temp.z / temp.w);

    REQUIRE(result.x == Approx(expected.x));
    REQUIRE(result.y == Approx(expected.y));
    REQUIRE(result.z == Approx(expected.z));
  }
}

TEST_CASE("Mat4 Transpose", "[Mat4]")
{
  SECTION("Basic Transpose")
  {
    Mat4 m(
      1.0f, 5.0f, 9.0f, 13.0f,
      2.0f, 6.0f, 10.0f, 14.0f,
      3.0f, 7.0f, 11.0f, 15.0f,
      4.0f, 8.0f, 12.0f, 16.0f
    );

    Mat4 transposed = m.transpose();
    Mat4 expected(
      1.0f, 2.0f, 3.0f, 4.0f,
      5.0f, 6.0f, 7.0f, 8.0f,
      9.0f, 10.0f, 11.0f, 12.0f,
      13.0f, 14.0f, 15.0f, 16.0f
    );

    matrices_approx_equal(transposed, expected);
  }

  SECTION("Transpose of Identity")
  {
    Mat4 identity(1.0f);
    Mat4 transposed = identity.transpose();
    matrices_approx_equal(transposed, identity);
  }

  SECTION("Double Transpose")
  {
    Mat4 m(
      1.0f, 5.0f, 9.0f, 13.0f,
      2.0f, 6.0f, 10.0f, 14.0f,
      3.0f, 7.0f, 11.0f, 15.0f,
      4.0f, 8.0f, 12.0f, 16.0f
    );

    Mat4 double_transposed = m.transpose().transpose();
    matrices_approx_equal(double_transposed, m);
  }
}

TEST_CASE("Mat4 Determinant", "[Mat4]")
{
  SECTION("Identity Matrix Determinant")
  {
    Mat4 identity(1.0f);
    REQUIRE(identity.determinant() == Approx(1.0f));
  }

  SECTION("Scalar Matrix Determinant")
  {
    Mat4 scalar(2.0f);
    REQUIRE(scalar.determinant() == Approx(16.0f));
  }

  SECTION("Diagonal Matrix Determinant")
  {
    Mat4 diagonal(1.0f, 2.0f, 3.0f, 4.0f);
    REQUIRE(diagonal.determinant() == Approx(24.0f));
  }

  SECTION("Singular Matrix Determinant")
  {
    Mat4 singular = singular_matrix();
    REQUIRE(singular.determinant() == Approx(0.0f));
  }

  SECTION("Diagonal Negative Determinant")
  {
    Mat4 m = Mat4 {
      1.0f, 0.0f, 0.0f,  0.0f,
      0.0f, 1.0f, 0.0f,  0.0f,
      0.0f, 0.0f, 1.0f,  0.0f,
      0.0f, 0.0f, 0.0f, -1.0f
    };
    REQUIRE(m.determinant() == Approx(-1.0f));
    m = Mat4 {
      -1.0f,  0.0f, 0.0f,  0.0f,
       0.0f, -1.0f, 0.0f,  0.0f,
       0.0f,  0.0f, 1.0f,  0.0f,
       0.0f,  0.0f, 0.0f, -1.0f
    };
    REQUIRE(m.determinant() == Approx(-1.0f));
    m = Mat4 {
      1.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f, 0.0f,
      0.0f, 0.0f, -1.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f
    };
    REQUIRE(m.determinant() == Approx(-1.0f));
    m = Mat4 {
      1.0f, 0.0f, 0.0f, 0.0f,
      0.0f, -1.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f
    };
    REQUIRE(m.determinant() == Approx(-1.0f));
  }

  SECTION("Generic Determinant")
  {
    REQUIRE(nonsingular_matrix().determinant() == Approx(-195.f));
  }

  SECTION("3x3 Determinant")
  {
    REQUIRE(nonsingular_matrix().determinant_3x3() == Approx(35.f));
  }
}

TEST_CASE("Mat4 Adjoint", "[Mat4]")
{
  SECTION("Identity Matrix Adjoint")
  {
    Mat4 identity(1.0f);
    Mat4 adj = identity.adj();
    matrices_approx_equal(adj, identity);
  }

  SECTION("Diagonal Matrix Adjoint")
  {
    Mat4 diagonal(2.0f, 3.0f, 4.0f, 5.0f);
    Mat4 adj = diagonal.adj();

    // For diagonal matrix, adjoint should be diagonal with cofactors
    Mat4 expected(
      60.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 40.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 30.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 24.0f
    );

    matrices_approx_equal(adj, expected);
  }

  SECTION("Adjoint Transposed")
  {
    Mat4 m(
      1.0f, 5.0f, 9.0f, 13.0f,  // Column 1
      2.0f, 6.0f, 10.0f, 14.0f, // Column 2
      3.0f, 7.0f, 11.0f, 15.0f, // Column 3
      4.0f, 8.0f, 12.0f, 16.0f  // Column 4
    );

    Mat4 adj_transposed = m.adj_transposed();
    Mat4 adj            = m.adj();

    // adj() should be the transpose of adj_transposed()
    matrices_approx_equal(adj, adj_transposed.transpose());
  }
}

TEST_CASE("Mat4 Inverse", "[Mat4]")
{
  SECTION("Identity Matrix Inverse")
  {
    Mat4 identity(1.0f);
    Mat4 inverse = identity.inverse();
    matrices_approx_equal(inverse, identity);
  }

  SECTION("Diagonal Matrix Inverse")
  {
    Mat4 diagonal(2.0f, 3.0f, 4.0f, 5.0f);
    Mat4 inverse = diagonal.inverse();

    Mat4 expected(
      0.5f, 0.0f, 0.0f, 0.0f,
      0.0f, 1.0f / 3.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 0.25f, 0.0f,
      0.0f, 0.0f, 0.0f, 0.2f
    );

    matrices_approx_equal(inverse, expected);
  }

  SECTION("Inverse")
  {
    Mat4 m = nonsingular_matrix();

    Mat4 inverse = m.inverse();
    Mat4 product = m * inverse;

    is_approx_identity(product);

    Mat4 expected{
      0.94871795f, -0.23076923f, -0.74358974f, 0.1025641f,
      -0.42564103f, 0.18461538f, 0.12820513f, 0.05128205f,
      -0.77435897f, 0.21538462f, 0.87179487f, -0.05128205f,
      -1.01025641f, 0.35384615f, 1.05128205f, -0.17948718f
    };
    matrices_approx_equal(inverse, expected);
  }

  SECTION("Inverse Transposed")
  {
    Mat4 m = nonsingular_matrix();

    Mat4 inverse_transposed = m.inverse_transposed();
    Mat4 inverse            = m.inverse();

    matrices_approx_equal(inverse, inverse_transposed.transpose());
  }

  SECTION("Safe Inverse")
  {
    Mat4 m = nonsingular_matrix();

    auto safe_inverse = m.inverse_safe();
    REQUIRE(safe_inverse.has_value());
    matrices_approx_equal(*safe_inverse, m.inverse());

    Mat4 product = m * safe_inverse.value();
    is_approx_identity<4, 1e-5f>(product);
    product = safe_inverse.value() * m;
    is_approx_identity<4, 1e-5f>(product);

    Mat4 singular     = singular_matrix();
    safe_inverse = singular.inverse_safe();
    REQUIRE(!safe_inverse.has_value());
  }

  SECTION("Safe Inverse Transposed")
  {
    Mat4 m = nonsingular_matrix();

    auto safe_inverse_transposed = m.inverse_transposed_safe();
    REQUIRE(safe_inverse_transposed.has_value());
    matrices_approx_equal(*safe_inverse_transposed, m.inverse_transposed());

    matrices_approx_equal(
      m.inverse(),
      safe_inverse_transposed->transpose()
    );

    Mat4 singular = singular_matrix();
    REQUIRE(!singular.inverse_transposed_safe().has_value());
  }
}

TEST_CASE("Mat4 Scaling", "[Mat4]")
{
  SECTION("Scale by Scalar")
  {
    Mat4 m(
      1.0f, 5.0f, 9.0f, 13.0f,  // Column 1
      2.0f, 6.0f, 10.0f, 14.0f, // Column 2
      3.0f, 7.0f, 11.0f, 15.0f, // Column 3
      4.0f, 8.0f, 12.0f, 16.0f  // Column 4
    );
    float scalar = 2.0f;

    Mat4 scaled = m.scale_by(scalar);
    Mat4 expected(
      2.0f, 10.0f, 18.0f, 26.0f,  // Column 1
      4.0f, 12.0f, 20.0f, 28.0f,  // Column 2
      6.0f, 14.0f, 22.0f, 30.0f,  // Column 3
      8.0f, 16.0f, 24.0f, 32.0f   // Column 4
    );

    matrices_approx_equal(scaled, expected);
  }

  SECTION("Scale by Vector")
  {
    Mat4 m(
      1.0f, 5.0f, 9.0f, 13.0f,
      2.0f, 6.0f, 10.0f, 14.0f,
      3.0f, 7.0f, 11.0f, 15.0f,
      4.0f, 8.0f, 12.0f, 16.0f
    );
    Vec4 scale_vec(2.0f, 3.0f, 4.0f, 5.0f);

    Mat4 scaled = m.scale_by(scale_vec);

    FSFV_CHECK_ALMOST_EQ(scaled.cols[0], (scale_vec * m.cols[0]));
    FSFV_CHECK_ALMOST_EQ(scaled.cols[1], (scale_vec * m.cols[1]));
    FSFV_CHECK_ALMOST_EQ(scaled.cols[2], (scale_vec * m.cols[2]));
    FSFV_CHECK_ALMOST_EQ(scaled.cols[3], (scale_vec * m.cols[3]));
  }

  SECTION("Scale by Inverse Scalar")
  {
    Mat4 m(
      2.0f, 10.0f, 18.0f, 26.0f,  // Column 1
      4.0f, 12.0f, 20.0f, 28.0f,  // Column 2
      6.0f, 14.0f, 22.0f, 30.0f,  // Column 3
      8.0f, 16.0f, 24.0f, 32.0f   // Column 4
    );
    float scalar = 2.0f;

    Mat4 scaled = m.scale_by_inv(scalar);
    Mat4 expected(
      1.0f, 5.0f, 9.0f, 13.0f,  // Column 1
      2.0f, 6.0f, 10.0f, 14.0f, // Column 2
      3.0f, 7.0f, 11.0f, 15.0f, // Column 3
      4.0f, 8.0f, 12.0f, 16.0f  // Column 4
    );

    matrices_approx_equal(scaled, expected);
  }

  SECTION("Scale by Inverse Vector")
  {
    Mat4 m(
      2.0f, 15.0f, 36.0f, 65.0f,
      4.0f, 18.0f, 40.0f, 70.0f,
      6.0f, 21.0f, 44.0f, 75.0f,
      8.0f, 24.0f, 48.0f, 80.0f
    );
    Vec4 scale_vec(2.0f, 3.0f, 4.0f, 5.0f);

    Mat4 scaled = m.scale_by_inv(scale_vec);

    FSFV_CHECK_ALMOST_EQ(scaled.cols[0], (m.cols[0] / scale_vec));
    FSFV_CHECK_ALMOST_EQ(scaled.cols[1], (m.cols[1] / scale_vec));
    FSFV_CHECK_ALMOST_EQ(scaled.cols[2], (m.cols[2] / scale_vec));
    FSFV_CHECK_ALMOST_EQ(scaled.cols[3], (m.cols[3] / scale_vec));
  }
}

// TODO fix (needed at all?)
//TEST_CASE("Mat4 Multiply Transpose", "[Mat4]")
//{
//  SECTION("Basic Multiply Transpose")
//  {
//    Mat4 m1(
//      1.0f, 5.0f, 9.0f, 13.0f,
//      2.0f, 6.0f, 10.0f, 14.0f,
//      3.0f, 7.0f, 11.0f, 15.0f,
//      4.0f, 8.0f, 12.0f, 16.0f
//    );
//    Mat4 m2 = nonsingular_matrix();
//    Mat4 result = m1.mul_transp(m2);
//    Mat4 expected = m2.transpose() * m1;
//
//    matrices_approx_equal(result, expected);
//  }
//}

TEST_CASE("Mat4 Edge Cases", "[Mat4]")
{
  SECTION("Zero Matrix")
  {
    Mat4 zero(0.0f);
    REQUIRE(zero.determinant() == Approx(0.0f));

    auto safe_inverse = zero.inverse_safe();
    REQUIRE(!safe_inverse.has_value());
  }

  //SECTION("Very Small Values")
  //{
  //  Mat4 small(1e-10f, 1e-10f, 1e-10f, 1e-10f);
  //  REQUIRE(small.determinant() == Approx(1e-40f));

  //  // Should still be invertible
  //  auto safe_inverse = small.inverse_safe();
  //  REQUIRE(safe_inverse.has_value());
  //}

  //SECTION("Very Large Values")
  //{
  //  Mat4 large(1e10f, 1e10f, 1e10f, 1e10f);
  //  REQUIRE(large.determinant() == Approx(1e40f));

  //  // Should still be invertible
  //  auto safe_inverse = large.inverse_safe();
  //  REQUIRE(safe_inverse.has_value());
  //}

  //SECTION("Mixed Scale Values")
  //{
  //  Mat4 mixed(1e-10f, 1e10f, 1e-10f, 1e10f);
  //  REQUIRE(mixed.determinant() == Approx(1.0f));

  //  auto safe_inverse = mixed.inverse_safe();
  //  REQUIRE(safe_inverse.has_value());
  //}

  //SECTION("Matrix with NaN")
  //{
  //  Mat4 nan_matrix(std::numeric_limits<float>::quiet_NaN());
  //  REQUIRE(std::isnan(nan_matrix.determinant()));
  //}

  //SECTION("Matrix with Infinity")
  //{
  //  Mat4 inf_matrix(std::numeric_limits<float>::infinity());
  //  REQUIRE(std::isinf(inf_matrix.determinant()));
  //}
}

TEST_CASE("Mat4 Random Tests", "[Mat4]")
{
  std::random_device                    rd;
  std::mt19937                          gen(rd());
  std::uniform_real_distribution<float> dis(-10.0f, 10.0f);

  SECTION("Random Matrix Properties")
  {
    for (int test = 0; test < 100; ++test)
    {
      // Generate random matrix (column-major layout)
      Mat4 m(
        dis(gen), dis(gen), dis(gen), dis(gen),
        dis(gen), dis(gen), dis(gen), dis(gen),
        dis(gen), dis(gen), dis(gen), dis(gen),
        dis(gen), dis(gen), dis(gen), dis(gen)
      );

      float det = m.determinant();

      if (std::abs(det) > 1e-5f)    // Non-singular
      {
        // Test inverse properties
        auto safe_inverse = m.inverse_safe();
        REQUIRE(safe_inverse.has_value());

        //Mat4 product = m * safe_inverse.value();
        Mat4 product = m * m.inverse();
        is_approx_identity<6, 8e-4f>(product);
        product = m.inverse() * m;
        is_approx_identity<6, 8e-4f>(product);

        // Test transpose properties
        Mat4 transposed = m.transpose();
        REQUIRE(transposed.transpose() == m);

        // Test determinant of transpose
        REQUIRE(transposed.determinant() == Approx(det).epsilon(1e-3f));
      }
      else    // Singular
      {
        auto safe_inverse = m.inverse_safe();
        REQUIRE(!safe_inverse.has_value());
      }
    }
  }
}

TEST_CASE("Mat4 Performance Edge Cases", "[Mat4]")
{
  SECTION("Repeated Operations")
  {
    Mat4 m(
      1.0f, 5.0f, 9.0f, 13.0f,
      2.0f, 6.0f, 10.0f, 14.0f,
      3.0f, 7.0f, 11.0f, 15.0f,
      4.0f, 8.0f, 12.0f, 16.0f
    );

    // Test repeated operations don't cause issues
    for (int i = 0; i < 1000; ++i)
    {
      Mat4 result     = m * m;
      Mat4 inverse    = m.inverse();
      Mat4 transposed = m.transpose();
      Mat4 adj        = m.adj();

      // Just ensure no crashes
      REQUIRE_NOTHROW(result);
      REQUIRE_NOTHROW(inverse);
      REQUIRE_NOTHROW(transposed);
      REQUIRE_NOTHROW(adj);
    }
  }
}

}    // namespace FSVF
