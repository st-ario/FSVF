#include "TestUtils.h"

#include <Eigen/LU>
#include <Eigen/Dense>
#include <Eigen/SVD>
#include <cstdint>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <optional>


// TODO!!!
// Most of the tests are AI-generated and require manual double-checking and cleanup

//using Catch::Approx;

namespace FSVF
{

// Helper function to check if two matrices are approximately equal
template <uint16_t Ulp = 0x1024, float Epsilon = std::numeric_limits<float>::epsilon() * 2.f>
void matrices_approx_equal(const Mat4& a, const Mat4& b)
{
  for (int i = 0; i < 4; ++i)
  {
      INFO(std::setprecision(15) << "lhs: ["
           << a[i].components()[0] << ", "
           << a[i].components()[1] << ", "
           << a[i].components()[2] << ", "
           << a[i].components()[3] << "]\nrhs: ["
           << b[i].components()[0] << ", "
           << b[i].components()[1] << ", "
           << b[i].components()[2] << ", "
           << b[i].components()[3] << "]\n");

      INFO(std::setprecision(15) << "ULP distances: ["
           << FSVF::ulps_distance(a[i].components()[0], b[i].components()[0]) << ", "
           << FSVF::ulps_distance(a[i].components()[1], b[i].components()[1]) << ", "
           << FSVF::ulps_distance(a[i].components()[2], b[i].components()[2]) << ", "
           << FSVF::ulps_distance(a[i].components()[3], b[i].components()[3]) << "]\n");

      INFO(std::setprecision(15) << "Diffs: ["
           << a[i].components()[0] - b[i].components()[0] << ", "
           << a[i].components()[1] - b[i].components()[1] << ", "
           << a[i].components()[2] - b[i].components()[2] << ", "
           << a[i].components()[3] - b[i].components()[3] << "]\n");

      CHECK(FSVF::almost_equal_eps_ulp<Ulp,Epsilon>(a[i].components()[0], b[i].components()[0]));
      CHECK(FSVF::almost_equal_eps_ulp<Ulp,Epsilon>(a[i].components()[1], b[i].components()[1]));
      CHECK(FSVF::almost_equal_eps_ulp<Ulp,Epsilon>(a[i].components()[2], b[i].components()[2]));
      CHECK(FSVF::almost_equal_eps_ulp<Ulp,Epsilon>(a[i].components()[3], b[i].components()[3]));
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
template <uint16_t Ulp = 0xFF, float Epsilon = std::numeric_limits<float>::epsilon() * 2.f>
void is_approx_identity(const Mat4& m)
{
  matrices_approx_equal<Ulp, Epsilon>(m, Mat4(1.0f));
}

float determinant_check3x3(const auto& c0, const auto& c1, const auto& c2) {
  return
      (c0[0] * (c1[1] * c2[2] - c2[1] * c1[2]))
    - (c1[0] * (c0[1] * c2[2] - c2[1] * c0[2]))
    + (c2[0] * (c0[1] * c1[2] - c1[1] * c0[2]));
}

using EigenM4 = Eigen::Matrix<float, 4, 4>;
using EigenM3 = Eigen::Matrix<float, 3, 3>;

template <typename T = EigenM4>
inline float cond2(const T& A) {
  Eigen::JacobiSVD<T> svd(A, 0);
  //Eigen::JacobiSVD<Eigen::Matrix<double, 4, 4>> svd(A.template cast<double>(), 0);
  const auto s = svd.singularValues();
  const float smin = s.template tail<1>()[0];
  const float smax = s.template head<1>()[0];
  if (smin == 0.0f) return std::numeric_limits<float>::infinity();
  return smax / smin;
}

// Returns a random Orthogonal matrix (uniformly distributed over O(N) with the Haar measure)
template <uint8_t N>
inline auto randomOrthogonal() {
  // QR of a Gaussian matrix
  std::normal_distribution<float> normal(0.0f, 1.0f);
  std::array<float, N*N> am;

  for (float& n : am)
    n = normal(gen);

  Eigen::Matrix<float, N, N> m{am.data()};

  Eigen::HouseholderQR<Eigen::Matrix<float, N, N>> qr(m);
  return qr.householderQ();
}

// overflow-protected 10^x in 32 bits
inline float pow10f(float x) {
  return std::pow(10.0, static_cast<double>(x));
}

// scrambled diagonal matrix with entries spanning [2^low_exp, 2^high_exp] almost uniformly
template <uint8_t N>
  requires(N > 1)
Eigen::Matrix<float, N, N> sparse_bounded(int low_exp, int high_exp)
{
  std::array<float, N> diag;

  constexpr auto offset = [] {
    constexpr auto step = 1.f / (N - 1);
    return (step / 1.99f);
  }();

  for (int i = 0; i < N; ++i) {
      float t = static_cast<float>(i) / static_cast<float>(N - 1);
      t += random_float(-offset, offset);

      const float exponent = low_exp + t * (high_exp - low_exp);
      const float val = std::pow(2.0f, exponent);

      // random signs
      const float sign = coin_flip() ? 1.0f : -1.0f;
      diag[i] = sign * val;
  }

  std::shuffle(diag.begin(), diag.end(), gen);

  return Eigen::DiagonalMatrix<float, N>(diag[0], diag[1], diag[2], diag[3]).toDenseMatrix();
}

template <uint8_t N>
auto randomly_permute(const auto& m)
{
  using Mat = Eigen::Matrix<float, N, N>;
  using Perm = Eigen::PermutationMatrix<N>;

  std::array<int, N> idx_row{};
  std::array<int, N> idx_col{};

  for (int i = 0; i < N; ++i) {
      idx_row[i] = i;
      idx_col[i] = i;
  }

  std::shuffle(idx_row.begin(), idx_row.end(), gen);
  std::shuffle(idx_col.begin(), idx_col.end(), gen);

  Perm P_row, P_col;
  for (int i = 0; i < N; ++i) {
      P_row.indices()(i) = idx_row[i];
      P_col.indices()(i) = idx_col[i];
  }

  return m.and_then([&](auto r) { return std::optional<Mat>(P_row * r * P_col.transpose()); });
}

template <uint8_t N>
std::optional<Eigen::Matrix<float, N, N>> add_low_conditioning_randomness(const auto& m, float sigma_min, float eps)
{
  using Mat = Eigen::Matrix<float, N, N>;

  // Dense random perturbation with controlled norm
  Mat perturbation = Mat::Random(); // entries in [-1,1]
  float rnd_norm = perturbation.norm();

  if (rnd_norm < 1e-7f)
    return {};

  // Scale to keep the norm below threshold
  if (eps > 0.0f)
    perturbation *= eps * sigma_min / rnd_norm;

  const Mat res0 = m + perturbation;

  // orthogonal similarity to scramble more
  const Mat Q = randomOrthogonal<N>();
  return Q.transpose() * res0 * Q;
}

// Dense random matrix with entries in [2^low_exp, 2^high_exp] (in module)
// Expected conditioning numbers (for small values of eps):
// 2^(d ± (d/3)) (with d = high_exp - low_exp)
template <uint8_t N>
auto randomMatrixWithRange(int low_exp, int high_exp, float eps = 0.1f) -> std::optional<Eigen::Matrix<float, N, N>>
{
  if (eps < 0.f)
    return {};

  if (low_exp > high_exp)
      std::swap(low_exp, high_exp);

  const auto sparse = sparse_bounded<N>(low_exp, high_exp);
  const float smallest_singular_val = std::pow(2.0f, static_cast<float>(low_exp));

  const auto res = add_low_conditioning_randomness<N>(sparse, smallest_singular_val, eps);
  return randomly_permute<N>(res);
}

// Dense random matrix with condition number k +- eps and entries in a neighborhood of 2^center_exp
template <uint8_t N>
auto wellConditionedAroundExp(float k, int center_exp, float eps = 0.1f) -> std::optional<Eigen::Matrix<float, N, N>>
{
  if (k < 1.f)
    return{};

  // log2(k) controls how wide the values can be while keeping cond ~ k
  const float log2k = std::log2(k);

  // Exponents range [center_exp - log2k/2, center_exp + log2k/2]
  const float left = center_exp - 0.5f * log2k;
  const float right = center_exp + 0.5f * log2k;

  const auto diagonal = binary_diagonal_between<N>(left, right);
  const float smallest_singular_val = std::pow(2.0f, left);

  const auto res = add_low_conditioning_randomness<N>(diagonal, smallest_singular_val, eps);
  return randomly_permute<N>(res);
}

int main(int argc, char** argv) noexcept
{
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0]
                  << " k low_exp high_exp\n";
        return 1;
    }

    int low_exp   = std::strtol(argv[1], nullptr, 10);
    int high_exp  = std::strtol(argv[2], nullptr, 10);
    float k       = std::strtof(argv[3], nullptr);

    constexpr std::uint8_t N = 4;

    for (int i = 0; i < 100000; ++i) {
        auto A = randomMatrixWithRange<N>(low_exp, high_exp, k).value();

        // --- stderr: print the matrix ---
        //std::cerr << "Matrix " << i << ":\n" << A << "\n\n";

        // Compute min and max absolute entries
        float minAbs = A.cwiseAbs().minCoeff();
        float maxAbs = A.cwiseAbs().maxCoeff();

        // Compute condition number (2-norm) via SVD
        Eigen::JacobiSVD<Eigen::Matrix4f> svd(A, 0);
        auto sv = svd.singularValues();
        float cond = sv(0) / sv(sv.size() - 1);

        // Print: condition_number:minAbs:maxAbs
        std::cout << cond << ":" << minAbs << ":" << maxAbs << '\n';
    }

    return 0;
}

// --- Make a non-singular, well-conditioned 4x4 whose entries span orders ----
// The entries will be log-uniform in [10^min_order, 10^max_order] *before*
// conditioning, and after conditioning they will still *strongly* span the range.
// Arguments:
//   min_order, max_order : inclusive integer bounds on entry orders of magnitude
//   cond_target          : desired spectral condition number (≥1, e.g. 5 or 10)
//   ensure_span          : if true, retry up to max_tries so min/max entries
//                          actually hit the requested orders (within a tolerance)
inline auto wellConditioned(
  int min_order,
  int max_order)
{
    if (min_order > max_order) std::swap(min_order, max_order);
    float cond_target = std::min(pow10f(max_order - min_order), 1e4f);

    std::uniform_real_distribution<float> U01(0.0f, 1.0f);
    std::bernoulli_distribution coin(0.5);

    auto make_once = [&](Eigen::Matrix4f& A) {
        // 1) Seed matrix with log-uniform entries spanning [10^min, 10^max]
        Eigen::Matrix4f E;
        for (int i = 0; i < 16; ++i) {
            const float u = min_order + (max_order - min_order) * U01(gen); // exponent
            const float mag = pow10f(u);
            const float sgn = coin(gen) ? 1.0f : -1.0f;
            E.data()[i] = sgn * mag;
        }

        // 2) Project to a well-conditioned matrix:
        //    keep U,V from E’s SVD; compress singulars into a narrow band.
        Eigen::JacobiSVD<Eigen::Matrix4f> svd(E, Eigen::ComputeThinU | Eigen::ComputeThinV);
        Eigen::Matrix4f U = svd.matrixU();
        Eigen::Matrix4f V = svd.matrixV();

        // Geometric mean of singular values (stable center)
        Eigen::Vector4f s = svd.singularValues();
        // Guard against degeneracy (shouldn’t happen with random E)
        for (int i = 0; i < 4; ++i) s[i] = std::max(s[i], 1e-30f);
        const float log_g = 0.25f * (std::log(s[0]) + std::log(s[1]) + std::log(s[2]) + std::log(s[3]));
        const float g = std::exp(log_g);

        // Target singulars: equally log-spaced around g with total ratio = cond_target.
        // Offsets ±3, ±1 scaled so that s_max / s_min = cond_target.
        const float L = std::log(cond_target);
        const float step = L / 6.0f; // gives offsets {+3,+1,-1,-3}*step
        const float r[4] = { std::exp(+3*step), std::exp(+1*step),
                             std::exp(-1*step), std::exp(-3*step) };
        Eigen::Vector4f s_tgt(g * r[0], g * r[1], g * r[2], g * r[3]);

        // Recompose
        A = U * s_tgt.asDiagonal() * V.transpose();

        // 3) Final scalar normalization so entries land in [10^min, 10^max] roughly.
        //    First set max|A_ij| to 10^max_order; then check span.
        const float maxabs = A.cwiseAbs().maxCoeff();
        if (maxabs > 0.0f) {
            const float scale_to_max = pow10f(static_cast<float>(max_order)) / maxabs;
            A *= scale_to_max;
        }
    };

    Eigen::Matrix4f A;
    make_once(A);

    return Mat4{A.data()};
}

inline auto pathologicalMatrix(
  float k_max,  // max condition number for result (ignored if target_rank is < 4)
  int target_rank = 4,
  int min_order = -1,
  int max_order = +1,
  bool random_scale = true,
  float target_k = 0.0f // precise condition number target for result (ignored if target_rank is < 4)
)
{
  if (!(k_max >= 1.0f) || !std::isfinite(k_max)) k_max = 1.0f;

  // Choose the actual condition number k we will realize.
  float k = (target_k > 0.0f ? std::max(1.0f, std::min(target_k, k_max))
                              : (1.0f + (k_max - 1.0f) * random_float(0.f, 1.f)));

  Eigen::Vector4f res = Eigen::Vector4f::Identity();
  //while (rcond2(res) != Approx(1.0/k).epsilon(0.1))
  {
    // Set singular values so s1 >= s2 >= s3 >= s4 and s1/s4 = k exactly.
    // We sample two exponents 0 <= t3 <= t2 <= 1 and set s = exp(log(1/k)*t).
    // This spreads intermediate singular values smoothly between s1 and s4.
    float t2 = random_float(0.f, 1.f);
    float t3 = random_float(0.f, 1.f);
    if (t3 > t2) std::swap(t2, t3);

    const float log_invk = std::log(1.0f / k);
    const float s1 = 1.0f;
    const float s2 = std::exp(log_invk * t2);
    const float s3 = std::exp(log_invk * t3);
    const float s4 = 1.0f / k;

    Eigen::Vector4f sv(s1, s2, s3, s4);

    // Optional global scaling so magnitudes vary while cond2 stays the same
    float scale = 1.0f;
    if (random_scale) {
        // Log-uniform scale in a modest range to avoid under/overflow in float
        const float lo = 0.25f, hi = 4.0f;
        scale = std::exp(std::log(lo) + random_float(0.f, 1.f) * std::log(hi / lo));
    }
    sv *= scale;

    if (target_rank < 4) {
        for (int i = 3; i >= target_rank; --i)
          sv[i] = 0.0f;
        // Make sure sv[0] is nonzero
        if (sv[0] == Approx(0.f)) sv[0] = 1.0f;
    }

    // Random orthogonal U, V
    Eigen::Matrix4f U = randomOrthogonal<4>();
    Eigen::Matrix4f V = randomOrthogonal<4>();

    // Assemble res = U * diag(s) * V^T
    Eigen::Matrix4f S = sv.asDiagonal();
    Eigen::Matrix4f res = U * S * V.transpose();

    // Choose the target *entry* scale as 10^u with u ∈ [min_order, max_order].
    const float u = (min_order == max_order)
                      ? static_cast<float>(min_order)
                      : (min_order + (max_order - min_order) * random_float(0.f, 1.f));
    const float target_scale = pow10f(u);

    // Compute current entry scale of A0 and rescale to hit the target.
    float current_scale = 1.0f;
    const float rms = std::sqrt(res.squaredNorm() / 16.0f);
    current_scale = (rms > 0.0f ? rms : 1.0f);

    float alpha = target_scale / current_scale;

    // Scale singular values (equivalent to scaling res), preserves cond.
    sv *= alpha;
    res = U * sv.asDiagonal() * V.transpose();

    // Optional: guard against Inf/NaN if extreme orders were requested.
    while (!res.allFinite()) {
      sv /= alpha;
      alpha /= 10.f;
      res  = U * sv.asDiagonal() * V.transpose();
    }
  }

  return Mat4{res.data()};
}

float determinant_check(Mat4 m)
{
  auto em = EigenM4(reinterpret_cast<float*>(&m));
  return em.determinant();

  //auto x = Eigen::FullPivLU<EigenM4>{em};
  //auto x = Eigen::PartialPivLU<EigenM4>{em};
  //return x.determinant();
}

Mat4 diagonal_with_determinant(float det)
{
  const float d = std::pow(std::abs(det), 0.25f);
  const float last = (det < 0) ? -d : d;

  return Mat4(d, d, d, last);
}

Mat4 rnd4x4()
{
  return Mat4{rnd4(), rnd4(), rnd4(), rnd4()};
}

auto culofiga(Mat4 m, Mat4 n) {
  //return m.determinant();
  return m * n; // 43 instr
}

//auto culoculo(EigenM4 m) {
auto culoculo(Mat4 m, Mat4 n) {
  return m.transpose() * n; // 51 instr
}

//auto culotette(Mat4 m, Mat4 n)
//{
//  const auto c0 = m.cols[0].components();
//  const auto c1 = m.cols[1].components();
//  const auto c2 = m.cols[2].components();
//  const auto c3 = m.cols[3].components();
//
//  return
//    - c0[3] * determinant_check(c1, c2, c3)
//    + c1[3] * determinant_check(c0, c2, c3)
//    - c2[3] * determinant_check(c0, c1, c3)
//    + c3[3] * determinant_check(c0, c1, c2);
//}

TEST_CASE("Mat4 Constructors", "[Mat4]")
{
  SECTION("Copy Constructor")
  {
    Mat4 m = rnd4x4();
    Mat4 copy(m);

    CHECK(m == copy);
  }

  SECTION("Scalar Constructor")
  {
    const auto r = random_float();

    const Mat4 m(r);
    const Mat4 expected(r, r, r, r);

    CHECK(m == expected);
  }

  SECTION("Pointer Constructor")
  {
    std::vector<float> vals{};
    for (int i = 0; i < 16; ++i)
    {
      vals.push_back(random_float());
    }

    const Mat4 m(vals.data());

    std::vector<float> expected{};
    for (int i = 0; i < 4; ++i)
    {
      const auto c = m.cols[i].components();
      for (int j = 0; j < 4; ++j)
      {
        expected.push_back(c[j]);
      }
    }

    CHECK(vals == expected);
  }

  SECTION("Column Vector Constructor")
  {
    const auto c0 = rnd4();
    const auto c1 = rnd4();
    const auto c2 = rnd4();
    const auto c3 = rnd4();

    const Mat4 m(c0, c1, c2, c3);

    const std::array<Vec4, 4> cols{c0, c1, c2, c3};

    std::vector<float> expected{};
    for (int i = 0; i < 4; ++i)
    {
      const auto c = cols[i].components();
      for (int j = 0; j < 4; ++j)
      {
        expected.push_back(c[j]);
      }
    }

    Mat4 e(expected.data());

    CHECK(m == e);
  }

  SECTION("16-Component Constructor")
  {
    std::vector<float> vals{};
    for (int i = 0; i < 16; ++i)
    {
      vals.push_back(random_float());
    }

    Mat4 m(
      vals[0], vals[1], vals[2], vals[3],
      vals[4], vals[5], vals[6], vals[7],
      vals[8], vals[9], vals[10], vals[11],
      vals[12], vals[13], vals[14], vals[15]
    );

    CHECK(m == Mat4(vals.data()));
  }

  SECTION("Diagonal Constructor")
  {
    const auto f0 = random_float();
    const auto f1 = random_float();
    const auto f2 = random_float();
    const auto f3 = random_float();

    const Mat4 m(f0, f1, f2, f3);

    const Vec4 c0(f0, 0, 0, 0);
    const Vec4 c1(0, f1, 0, 0);
    const Vec4 c2(0, 0, f2, 0);
    const Vec4 c3(0, 0, 0, f3);

    CHECK(m == Mat4(c0, c1, c2, c3));
  }
}

TEST_CASE("Mat4 Transpose", "[Mat4]")
{
  const auto c0 = rnd4();
  const auto c1 = rnd4();
  const auto c2 = rnd4();
  const auto c3 = rnd4();

  const auto r = random_float();

  const Mat4 m(c0, c1, c2, c3);

  CHECK(m.transpose().transpose() == m);
  CHECK(Mat4(r).transpose() == Mat4(r));

  const std::array<Vec4, 4> cols{c0, c1, c2, c3};

  std::vector<float> expected{};
  for (int i = 0; i < 4; ++i)
  {
    expected.push_back(cols[0].components()[i]);
    expected.push_back(cols[1].components()[i]);
    expected.push_back(cols[2].components()[i]);
    expected.push_back(cols[3].components()[i]);
  }

  CHECK(m.transpose() == Mat4{expected.data()});
}


TEST_CASE("Mat4 Operators", "[Mat4]")
{
  SECTION("Equality Operators")
  {
    const Mat4 m1 = rnd4x4();

    auto c = rnd4();
    while (len(c) < 1e-4) c = rnd4();

    const Mat4 m2 = m1 + Mat4(c, c, c, c);

    CHECK(m1 == m1);
    CHECK(m1 != m2);
  }

  SECTION("Matrix Multiplication")
  {
    const Mat4 m1 = rnd4x4();
    const Mat4 m2 = rnd4x4();

    const std::array am1 = m1.cols;
    const std::array am2 = m2.cols;

    const auto c00 =
        am1[0].components()[0] * am2[0].components()[0]
      + am1[1].components()[0] * am2[0].components()[1]
      + am1[2].components()[0] * am2[0].components()[2]
      + am1[3].components()[0] * am2[0].components()[3];
    const auto c01 =
        am1[0].components()[1] * am2[0].components()[0]
      + am1[1].components()[1] * am2[0].components()[1]
      + am1[2].components()[1] * am2[0].components()[2]
      + am1[3].components()[1] * am2[0].components()[3];
    const auto c02 =
        am1[0].components()[2] * am2[0].components()[0]
      + am1[1].components()[2] * am2[0].components()[1]
      + am1[2].components()[2] * am2[0].components()[2]
      + am1[3].components()[2] * am2[0].components()[3];
    const auto c03 =
        am1[0].components()[3] * am2[0].components()[0]
      + am1[1].components()[3] * am2[0].components()[1]
      + am1[2].components()[3] * am2[0].components()[2]
      + am1[3].components()[3] * am2[0].components()[3];
    const auto c10 =
        am1[0].components()[0] * am2[1].components()[0]
      + am1[1].components()[0] * am2[1].components()[1]
      + am1[2].components()[0] * am2[1].components()[2]
      + am1[3].components()[0] * am2[1].components()[3];
    const auto c11 =
        am1[0].components()[1] * am2[1].components()[0]
      + am1[1].components()[1] * am2[1].components()[1]
      + am1[2].components()[1] * am2[1].components()[2]
      + am1[3].components()[1] * am2[1].components()[3];
    const auto c12 =
        am1[0].components()[2] * am2[1].components()[0]
      + am1[1].components()[2] * am2[1].components()[1]
      + am1[2].components()[2] * am2[1].components()[2]
      + am1[3].components()[2] * am2[1].components()[3];
    const auto c13 =
        am1[0].components()[3] * am2[1].components()[0]
      + am1[1].components()[3] * am2[1].components()[1]
      + am1[2].components()[3] * am2[1].components()[2]
      + am1[3].components()[3] * am2[1].components()[3];
    const auto c20 =
        am1[0].components()[0] * am2[2].components()[0]
      + am1[1].components()[0] * am2[2].components()[1]
      + am1[2].components()[0] * am2[2].components()[2]
      + am1[3].components()[0] * am2[2].components()[3];
    const auto c21 =
        am1[0].components()[1] * am2[2].components()[0]
      + am1[1].components()[1] * am2[2].components()[1]
      + am1[2].components()[1] * am2[2].components()[2]
      + am1[3].components()[1] * am2[2].components()[3];
    const auto c22 =
        am1[0].components()[2] * am2[2].components()[0]
      + am1[1].components()[2] * am2[2].components()[1]
      + am1[2].components()[2] * am2[2].components()[2]
      + am1[3].components()[2] * am2[2].components()[3];
    const auto c23 =
        am1[0].components()[3] * am2[2].components()[0]
      + am1[1].components()[3] * am2[2].components()[1]
      + am1[2].components()[3] * am2[2].components()[2]
      + am1[3].components()[3] * am2[2].components()[3];
    const auto c30 =
        am1[0].components()[0] * am2[3].components()[0]
      + am1[1].components()[0] * am2[3].components()[1]
      + am1[2].components()[0] * am2[3].components()[2]
      + am1[3].components()[0] * am2[3].components()[3];
    const auto c31 =
        am1[0].components()[1] * am2[3].components()[0]
      + am1[1].components()[1] * am2[3].components()[1]
      + am1[2].components()[1] * am2[3].components()[2]
      + am1[3].components()[1] * am2[3].components()[3];
    const auto c32 =
        am1[0].components()[2] * am2[3].components()[0]
      + am1[1].components()[2] * am2[3].components()[1]
      + am1[2].components()[2] * am2[3].components()[2]
      + am1[3].components()[2] * am2[3].components()[3];
    const auto c33 =
        am1[0].components()[3] * am2[3].components()[0]
      + am1[1].components()[3] * am2[3].components()[1]
      + am1[2].components()[3] * am2[3].components()[2]
      + am1[3].components()[3] * am2[3].components()[3];

    const Mat4 expected{
      c00, c01, c02, c03,
      c10, c11, c12, c13,
      c20, c21, c22, c23,
      c30, c31, c32, c33
    };

    matrices_approx_equal(m1 * m2, expected);
  }

  SECTION("Identity Matrix Multiplication")
  {
    const Mat4 id(1.0f);
    const Mat4 m = rnd4x4();

    matrices_approx_equal(id * m, m);
    matrices_approx_equal(m * id, m);
  }

  SECTION("Matrix-Vector Multiplication")
  {
    const Mat4 m = rnd4x4();
    const Vec4 v = rnd4();

    const Mat4 mT = m.transpose();

    const auto res = m*v;

    const auto e0 = dot(mT[0], v);
    const auto e1 = dot(mT[1], v);
    const auto e2 = dot(mT[2], v);
    const auto e3 = dot(mT[3], v);

    CHECK_VEC4_COMPONENTS(res, e0, e1, e2, e3);
  }

  SECTION("Scalar Multiplication")
  {
    const Mat4 m = rnd4x4();
    const float x = random_float();

    matrices_approx_equal(m * x, x * m);
    matrices_approx_equal(m * x, Mat4{ x * m.cols[0], x * m.cols[1], x * m.cols[2], x * m.cols[3] });
  }

  SECTION("Scalar Division")
  {
    const Mat4 m = rnd4x4();
    const float x = random_float();

    matrices_approx_equal(m / x, m * (1.f / x));
    matrices_approx_equal(m / x, Mat4{ m.cols[0] / x, m.cols[1] / x, m.cols[2] / x, m.cols[3] / x });

    matrices_approx_equal(x / m, x * (1.f / m));
    matrices_approx_equal(x / m, Mat4{ x / m.cols[0], x / m.cols[1], x / m.cols[2], x / m.cols[3] });
  }
}

TEST_CASE("Mat4 Scaling", "[Mat4]")
{
  SECTION("Scale by Scalar")
  {
    auto m = rnd4x4();
    auto f = random_float();

    Mat4 scaled = m.scale_by(f);

    matrices_approx_equal(scaled, Mat4{m.cols[0] * f, m.cols[1] * f, m.cols[2] * f, m.cols[3] * f});
  }

  SECTION("Scale by Vector")
  {
    auto m = rnd4x4();
    auto v = rnd4();

    Mat4 scaled = m.scale_by(v);

    matrices_approx_equal(scaled, Mat4{m.cols[0] * v, m.cols[1] * v, m.cols[2] * v, m.cols[3] * v});
  }

  SECTION("Scale by Inverse Scalar")
  {
    auto m = rnd4x4();
    auto f = random_float();

    Mat4 scaled = m.scale_by_inv(f);

    matrices_approx_equal(scaled, Mat4{m.cols[0] / f, m.cols[1] / f, m.cols[2] / f, m.cols[3] / f});
  }

  SECTION("Scale by Inverse Vector")
  {
    auto m = rnd4x4();
    auto v = rnd4();

    Mat4 scaled = m.scale_by_inv(v);

    matrices_approx_equal(scaled, Mat4{m.cols[0] / v, m.cols[1] / v, m.cols[2] / v, m.cols[3] / v});
  }
}

TEST_CASE("Mat4 Determinant", "[Mat4]")
{
  SECTION("Base cases")
  {
    CHECK(Mat4(1.f).determinant() == Approx(1.f));

    CHECK(Mat4{ 1.f,  1.f,  1.f,  1.f}.determinant() == Approx(1.f));
    CHECK(Mat4{ 1.f,  1.f,  1.f, -1.f}.determinant() == Approx(-1.f));
    CHECK(Mat4{ 1.f,  1.f, -1.f,  1.f}.determinant() == Approx(-1.f));
    CHECK(Mat4{ 1.f,  1.f, -1.f, -1.f}.determinant() == Approx(1.f));
    CHECK(Mat4{ 1.f, -1.f,  1.f,  1.f}.determinant() == Approx(-1.f));
    CHECK(Mat4{ 1.f, -1.f,  1.f, -1.f}.determinant() == Approx(1.f));
    CHECK(Mat4{ 1.f, -1.f, -1.f,  1.f}.determinant() == Approx(1.f));
    CHECK(Mat4{ 1.f, -1.f, -1.f, -1.f}.determinant() == Approx(-1.f));
    CHECK(Mat4{-1.f,  1.f,  1.f,  1.f}.determinant() == Approx(-1.f));
    CHECK(Mat4{-1.f,  1.f,  1.f, -1.f}.determinant() == Approx(1.f));
    CHECK(Mat4{-1.f,  1.f, -1.f,  1.f}.determinant() == Approx(1.f));
    CHECK(Mat4{-1.f,  1.f, -1.f, -1.f}.determinant() == Approx(-1.f));
    CHECK(Mat4{-1.f, -1.f,  1.f,  1.f}.determinant() == Approx(1.f));
    CHECK(Mat4{-1.f, -1.f,  1.f, -1.f}.determinant() == Approx(-1.f));
    CHECK(Mat4{-1.f, -1.f, -1.f,  1.f}.determinant() == Approx(-1.f));
    CHECK(Mat4{-1.f, -1.f, -1.f, -1.f}.determinant() == Approx(1.f));

    CHECK(Mat4{0.f}.determinant() == Approx(0.f));
    CHECK(Mat4{0.f, 0.f, 0.f, 1.f}.determinant() == Approx(0.f));
    CHECK(Mat4{0.f, 0.f, 1.f, 0.f}.determinant() == Approx(0.f));
    CHECK(Mat4{0.f, 0.f, 1.f, 1.f}.determinant() == Approx(0.f));
    CHECK(Mat4{0.f, 1.f, 0.f, 0.f}.determinant() == Approx(0.f));
    CHECK(Mat4{0.f, 1.f, 0.f, 1.f}.determinant() == Approx(0.f));
    CHECK(Mat4{0.f, 1.f, 1.f, 0.f}.determinant() == Approx(0.f));
    CHECK(Mat4{0.f, 1.f, 1.f, 1.f}.determinant() == Approx(0.f));
    CHECK(Mat4{1.f, 0.f, 0.f, 0.f}.determinant() == Approx(0.f));
    CHECK(Mat4{1.f, 0.f, 0.f, 1.f}.determinant() == Approx(0.f));
    CHECK(Mat4{1.f, 0.f, 1.f, 0.f}.determinant() == Approx(0.f));
    CHECK(Mat4{1.f, 0.f, 1.f, 1.f}.determinant() == Approx(0.f));
    CHECK(Mat4{1.f, 1.f, 0.f, 0.f}.determinant() == Approx(0.f));
    CHECK(Mat4{1.f, 1.f, 0.f, 1.f}.determinant() == Approx(0.f));
    CHECK(Mat4{1.f, 1.f, 1.f, 0.f}.determinant() == Approx(0.f));

    for (int i = 0; i < 100; ++i)
    {
      const float x = random_float();
      CHECK(Mat4(x).determinant() == Approx(std::pow(x, 4)));

      const Vec4 v = rnd4();
      const auto av = v.components();
      CHECK(Mat4{av[0], av[1], av[2], av[3]}.determinant() == Approx(prod(v)));
    }
  }

  SECTION("Non-singular, [1e-6, 1e6], 2 orders of magnitude difference, bounded condition number")
  {
    for (int i = 0; i < 100; ++i)
    {
      for  (int min = -6; min <= 6; ++min)
      {
        for  (int d = 0; d < 3; ++d)
        {
          auto nm = wellConditioned(min, min+d);
          auto em = EigenM4(reinterpret_cast<float*>(&nm));
          float c = cond2(em);

          while (c > pow10f(d))
          {
            nm = wellConditioned(min, min+d);
            em = EigenM4(reinterpret_cast<float*>(&nm));
            c = cond2(em);
          }

          const auto det = nm.determinant();
          const auto detck = determinant_check(nm);

          INFO("det: " << det);
          INFO("detck: " << detck);
          INFO("diff:" << std::abs((double)det - (double)detck));
          INFO("ULPs:" << ulps_distance(det, detck));
          INFO("min: " << min);
          INFO("max: " << min+d);
          INFO("cond2: " << c);
          INFO("sm: " << Catch::StringMaker<Mat4>::convert(nm));

          if (is_finite(det))
          {
            CHECK(almost_equal_eps_ulp<0x1FF, 1e-6f>(det, detck));
            CHECK(det != Approx(0.0));
          }
        }
      }
    }
  }

  SECTION("Non-singular, [1e-8, 1e-4], 3 orders of magnitude difference, condition number < ~1000")
  {
    for (int i = 0; i < 100; ++i)
    {
      for  (int min = -8; min <= -4; ++min)
      {
        auto nm = wellConditioned(min, min+3);
        auto em = EigenM4(reinterpret_cast<float*>(&nm));
        float c = cond2(em);

        while (c > 999.969f)
        {
          nm = wellConditioned(min, min+3);
          em = EigenM4(reinterpret_cast<float*>(&nm));
          c = cond2(em);
        }

        const auto det = nm.determinant();
        const auto detck = determinant_check(nm);

        INFO("det: " << det);
        INFO("detck: " << detck);
        INFO("diff:" << std::abs((double)det - (double)detck));
        INFO("ULPs:" << ulps_distance(det, detck));
        INFO("min: " << min);
        INFO("max: " << min+3);
        INFO("cond2: " << c);
        INFO("sm: " << Catch::StringMaker<Mat4>::convert(nm));

        if (is_finite(det))
        {
          CHECK(almost_equal_eps_ulp<0x3FFFu, 1e-4f>(det, detck));
          CHECK(det != Approx(0.0));
        }
      }
    }
  }

  SECTION("(Near-)singular")
  {
    for (int i = 0; i < 10; ++i)
    {
      for  (int min = -6; min <= 9; ++min)
      {
        for  (int rk = 0; rk < 4; ++rk)
        {
          auto sm = pathologicalMatrix(0.f, rk, -10, 10);
          auto em = EigenM4(reinterpret_cast<float*>(&sm));
          float c = cond2(em);

          INFO("cond2: " << c);
          INFO("sm: " << Catch::StringMaker<Mat4>::convert(sm));

          CHECK(sm.determinant() == Approx(determinant_check(sm)));
          CHECK(sm.determinant() == Approx(0.0));
        }
      }
    }
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
    const auto v = rnd4();
    const auto av = v.components();
    const auto avi = (1.f / v).components();

    Mat4 diagonal(av[0], av[1], av[2], av[3]);
    Mat4 inverse = diagonal.inverse();

    Mat4 expected(avi[0], avi[1], avi[2], avi[3]);

    matrices_approx_equal(inverse, expected);
  }

  SECTION("Inverse")
  {
    Mat4 m = wellConditioned(-3, 3);

    Mat4 inverse = m.inverse();
    Mat4 product = m * inverse;

    is_approx_identity<0x1FF, 5e-3f>(product);

    EigenM4 em = EigenM4(reinterpret_cast<float*>(&m));

    matrices_approx_equal<0x1FF, 5e-3f>(inverse, Mat4{em.inverse().eval().data()});
  }

  SECTION("Inverse Transposed")
  {
    Mat4 m = wellConditioned(-3, 3);

    Mat4 inverse = m.inverse_transposed();
    matrices_approx_equal(inverse, m.inverse().transpose());

    Mat4 product = m.transpose() * inverse;
    is_approx_identity<0x1FF, 5e-3f>(product);

    EigenM4 em = EigenM4(reinterpret_cast<float*>(&m));
    auto expected = em.inverse().eval();
    expected.transposeInPlace();

    matrices_approx_equal<0x1FF, 5e-3f>(inverse, Mat4{expected.data()});
  }
}

///////////////////////////////////// Done until here

//TEST_CASE("Mat4 3x3 Determinant", "[Mat4]")
//{
//  SECTION("Base cases")
//  {
//    CHECK(Mat4(1.f, 1.f, 1.f, 0.f).determinant_3x3() == Approx(1.f));
//
//    CHECK(Mat4{ 1.f,  1.f,  1.f,  1.f}.determinant_3x3() == Approx(1.f));
//    CHECK(Mat4{ 1.f,  1.f,  1.f, -1.f}.determinant_3x3() == Approx(1.f));
//    CHECK(Mat4{ 1.f,  1.f, -1.f,  1.f}.determinant_3x3() == Approx(-1.f));
//    CHECK(Mat4{ 1.f,  1.f, -1.f, -1.f}.determinant_3x3() == Approx(-1.f));
//    CHECK(Mat4{ 1.f, -1.f,  1.f,  1.f}.determinant_3x3() == Approx(-1.f));
//    CHECK(Mat4{ 1.f, -1.f,  1.f, -1.f}.determinant_3x3() == Approx(-1.f));
//    CHECK(Mat4{ 1.f, -1.f, -1.f,  1.f}.determinant_3x3() == Approx(1.f));
//    CHECK(Mat4{ 1.f, -1.f, -1.f, -1.f}.determinant_3x3() == Approx(1.f));
//    CHECK(Mat4{-1.f,  1.f,  1.f,  1.f}.determinant_3x3() == Approx(-1.f));
//    CHECK(Mat4{-1.f,  1.f,  1.f, -1.f}.determinant_3x3() == Approx(-1.f));
//    CHECK(Mat4{-1.f,  1.f, -1.f,  1.f}.determinant_3x3() == Approx(1.f));
//    CHECK(Mat4{-1.f,  1.f, -1.f, -1.f}.determinant_3x3() == Approx(1.f));
//    CHECK(Mat4{-1.f, -1.f,  1.f,  1.f}.determinant_3x3() == Approx(1.f));
//    CHECK(Mat4{-1.f, -1.f,  1.f, -1.f}.determinant_3x3() == Approx(1.f));
//    CHECK(Mat4{-1.f, -1.f, -1.f,  1.f}.determinant_3x3() == Approx(-1.f));
//    CHECK(Mat4{-1.f, -1.f, -1.f, -1.f}.determinant_3x3() == Approx(-1.f));
//
//    CHECK(Mat4{0.f}.determinant_3x3() == Approx(0.f));
//    CHECK(Mat4{0.f, 0.f, 0.f, 1.f}.determinant_3x3() == Approx(0.f));
//    CHECK(Mat4{0.f, 0.f, 1.f, 0.f}.determinant_3x3() == Approx(0.f));
//    CHECK(Mat4{0.f, 0.f, 1.f, 1.f}.determinant_3x3() == Approx(0.f));
//    CHECK(Mat4{0.f, 1.f, 0.f, 0.f}.determinant_3x3() == Approx(0.f));
//    CHECK(Mat4{0.f, 1.f, 0.f, 1.f}.determinant_3x3() == Approx(0.f));
//    CHECK(Mat4{0.f, 1.f, 1.f, 0.f}.determinant_3x3() == Approx(0.f));
//    CHECK(Mat4{0.f, 1.f, 1.f, 1.f}.determinant_3x3() == Approx(0.f));
//    CHECK(Mat4{1.f, 0.f, 0.f, 0.f}.determinant_3x3() == Approx(0.f));
//    CHECK(Mat4{1.f, 0.f, 0.f, 1.f}.determinant_3x3() == Approx(0.f));
//    CHECK(Mat4{1.f, 0.f, 1.f, 0.f}.determinant_3x3() == Approx(0.f));
//    CHECK(Mat4{1.f, 0.f, 1.f, 1.f}.determinant_3x3() == Approx(0.f));
//    CHECK(Mat4{1.f, 1.f, 0.f, 0.f}.determinant_3x3() == Approx(0.f));
//    CHECK(Mat4{1.f, 1.f, 0.f, 1.f}.determinant_3x3() == Approx(0.f));
//
//    for (int i = 0; i < 100; ++i)
//    {
//      const float x = random_float();
//      CHECK(Mat4(x).determinant_3x3() == Approx(std::pow(x, 3)));
//
//      const Vec4 v = rnd4();
//      const auto av = v.components();
//      CHECK(Mat4{av[0], av[1], av[2], av[3]}.determinant_3x3() == Approx(prod(LVec3{v})));
//    }
//  }
//
//  //SECTION("Non-singular, [1e-6, 1e6], 2 orders of magnitude difference, bounded condition number")
//  //{
//  //  for (int i = 0; i < 100; ++i)
//  //  {
//  //    for  (int min = -6; min <= 6; ++min)
//  //    {
//  //      for  (int d = 0; d < 3; ++d)
//  //      {
//  //        auto nm = wellConditioned3x3(min, min+d);
//  //        const std::array<stdVec3, 3> cols3x3{
//  //          LVec3{nm.cols[0]}.components(),
//  //          LVec3{nm.cols[1]}.components(),
//  //          LVec3{nm.cols[2]}.components()
//  //        };
//
//  //        auto em = EigenM3((float*)cols3x3.data());
//  //        float c = cond2(em);
//
//  //        if (c > pow10f(d))
//  //        {
//  //          --d;
//  //          continue;
//  //        }
//
//  //        const auto det = nm.determinant_3x3();
//  //        const auto edet = em.determinant();
//  //        const auto detck = determinant_check3x3(cols3x3[0], cols3x3[1], cols3x3[2]);
//
//  //        INFO("det: " << det);
//  //        INFO("detck: " << detck);
//  //        INFO("diff:" << std::abs((double)det - (double)detck));
//  //        INFO("ULPs:" << ulps_distance(det, detck));
//  //        INFO("min: " << min);
//  //        INFO("max: " << min+d);
//  //        INFO("cond2: " << c);
//  //        INFO("sm: " << Catch::StringMaker<Mat4>::convert(nm));
//
//  //        if (is_finite(det))
//  //        {
//  //          CHECK(almost_equal_eps_ulp<0x1FF, 1e-6f>(det, detck));
//  //          CHECK(almost_equal_eps_ulp<0x1FF, 1e-6f>(det, edet));
//  //          CHECK(det != Approx(0.0));
//  //        }
//  //      }
//  //    }
//  //  }
//  //}
//
//  SECTION("Non-singular, [1e-8, 1e-4], 3 orders of magnitude difference, condition number < ~1000")
//  {
//    for (int i = 0; i < 100; ++i)
//    {
//      for  (int min = -8; min <= -4; ++min)
//      {
//        {
//          auto nm = wellConditioned(min, min+3);
//          const std::array<stdVec3, 3> cols3x3{
//            LVec3{nm.cols[0]}.components(),
//            LVec3{nm.cols[1]}.components(),
//            LVec3{nm.cols[2]}.components()
//          };
//
//          auto em = EigenM3((float*)cols3x3.data());
//          float c = cond2(em);
//          if (c > 1000.f)
//          {
//            --min;
//            continue;
//          }
//
//          const auto det = nm.determinant_3x3();
//          const auto edet = em.determinant();
//          const auto detck = determinant_check3x3(cols3x3[0], cols3x3[1], cols3x3[2]);
//
//          INFO("det: " << det);
//          INFO("detck: " << detck);
//          INFO("diff:" << std::abs((double)det - (double)detck));
//          INFO("ULPs:" << ulps_distance(det, detck));
//          INFO("min: " << min);
//          INFO("max: " << min+3);
//          INFO("cond2: " << c);
//          INFO("sm: " << Catch::StringMaker<Mat4>::convert(nm));
//
//          if (is_finite(det))
//          {
//            CHECK(almost_equal_eps_ulp<0x1FF, 1e-6f>(det, detck));
//            CHECK(almost_equal_eps_ulp<0x1FF, 1e-6f>(det, edet));
//            CHECK(det != Approx(0.0));
//          }
//        }
//      }
//    }
//  }
//
//  SECTION("(Near-)singular")
//  {
//    for (int i = 0; i < 10; ++i)
//    {
//      for  (int min = -6; min <= 9; ++min)
//      {
//        for  (int rk = 0; rk < 4; ++rk)
//        {
//          auto sm = pathologicalMatrix(0.f, rk, -10, 10);
//          const std::array<stdVec3, 3> cols3x3{
//            LVec3{sm.cols[0]}.components(),
//            LVec3{sm.cols[1]}.components(),
//            LVec3{sm.cols[2]}.components()
//          };
//
//          auto em = EigenM3((float*)cols3x3.data());
//          float c = cond2(em);
//
//          INFO("cond2: " << c);
//          INFO("sm: " << Catch::StringMaker<Mat4>::convert(sm));
//
//          const auto det = sm.determinant_3x3();
//          const auto edet = em.determinant();
//          const auto detck = determinant_check3x3(cols3x3[0], cols3x3[1], cols3x3[2]);
//
//          CHECK(almost_equal_eps_ulp<0x1FF, 1e-6f>(det, detck));
//          CHECK(almost_equal_eps_ulp<0x1FF, 1e-6f>(det, edet));
//          CHECK(det == Approx(0.0));
//        }
//      }
//    }
//  }
//}

TEST_CASE("Mat4 Transformations", "[Mat4]")
{
  SECTION("Transform 3x3")
  {
    for (int i = 0; i < 100; ++i)
    {
      const auto m = rnd4x4();
      const auto v = LVec3{rnd4()};

      const auto res = m.transform_3x3(v);

      const Mat4 mT = m.transpose();

      const auto e0 = dot(mT[0], v);
      const auto e1 = dot(mT[1], v);
      const auto e2 = dot(mT[2], v);

      CHECK_LVEC3_COMPONENTS(res, e0, e1, e2);
    }
  }

/*
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

    CHECK(result.x == Approx(expected.x));
    CHECK(result.y == Approx(expected.y));
    CHECK(result.z == Approx(expected.z));
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

    CHECK(result.x == Approx(expected.x));
    CHECK(result.y == Approx(expected.y));
    CHECK(result.z == Approx(expected.z));
  }
*/
}

}    // namespace FSVF
