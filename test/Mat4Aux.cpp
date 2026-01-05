// TODO clean up and set up properly
#if 0
#include <FSVF.h>

#include <Eigen/Dense>
#include <Eigen/LU>
#include <Eigen/QR>
#include <Eigen/SVD>
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <limits>
#include <random>
#include <stdexcept>

static inline std::random_device rd;
static inline std::mt19937       gen(rd());

auto fmadet(FSVF::Mat4 m) {
  return m.determinant();
}

auto fmainverse(FSVF::Mat4 m) {
  return m.inverse();
}

auto eigendet(Eigen::Matrix4f m) {
  return m.determinant();
}

auto eigeninverse(Eigen::Matrix4f m) {
  return m.inverse().eval();
}
namespace {


template <uint8_t N>
using FMatrix = Eigen::Matrix<float, N, N>;

template <uint8_t N>
using FVector = Eigen::Vector<float, N>;

template <uint8_t N>
using DMatrix = Eigen::Matrix<double, N, N>;

template <uint8_t N>
using DVector = Eigen::Vector<double, N>;

// accurate log2
template <typename Float>
inline Float log2f(double x) {
  return static_cast<Float>(std::log(x) / std::log(2.0));
}

template <uint8_t N>
inline auto cond2(const DMatrix<N>& A) -> double {
  Eigen::JacobiSVD<DMatrix<N>> svd(A);
  const auto s = svd.singularValues();
  const auto smin = s.template tail<1>()[0];
  const auto smax = s.template head<1>()[0];
  if (smin <= 0.0) return std::numeric_limits<double>::infinity();
  return smax / smin;
}

// Returns a random Orthogonal matrix (uniformly distributed over O(N) with the Haar measure)
template <uint8_t N>
inline auto random_orthogonal() -> DMatrix<N> {
  // QR of a Gaussian matrix
  std::normal_distribution<float> normal(0.0f, 1.0f);

  DMatrix<N> res;

  for (int i = 0; i < N; ++i)
      for (int j = 0; j < N; ++j)
          res(i, j) = normal(gen);

  Eigen::HouseholderQR<DMatrix<N>> qr(res);
  return qr.householderQ();
}

// Clip |a_ij| into [2^L, 2^U], preserving sign.
template <uint8_t N>
inline void clip_to_range(FMatrix<N>& A, int L, int U) {
    const float minMag = std::pow(2.0, L);
    const float maxMag = std::pow(2.0, U);

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            float v = A(i, j);

            // leave zeroes unchanged
            if (v == 0.f)
              continue;

            float res  = std::abs(v);

            if (res < minMag) res = minMag;
            if (res > maxMag) res = maxMag;

            const float sign = (v >= 0.0f) ? 1.0f : -1.0f;
            A(i, j) = sign * res;
        }
    }
}

template <typename Mat>
constexpr auto log2_minmax(const Mat& A) {
  constexpr auto N = Mat::RowsAtCompileTime;
  double minAbs = std::numeric_limits<double>::infinity();
  double maxAbs = 0.0;

  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      auto v = std::abs<double>(A(i, j));
      if (v < std::numeric_limits<float>::epsilon()) continue;
      if (v < minAbs) minAbs = v;
      if (v > maxAbs) maxAbs = v;
    }
  }
  double log2Min;
  double log2Max;

  log2Min = log2f<double>(minAbs);
  log2Max = log2f<double>(maxAbs);

  return std::make_pair(log2Min, log2Max);
};


// - L, U: base-2 exponent bounds; we aim for |a_ij| in [2^(L-1), 2^(U+1)]
// - k_min, k_max: desired condition number band
// NOTE Will diverge for extreme/unreasonable values
template <uint8_t N>
inline std::optional<FMatrix<N>> random_finetuned(int8_t L, int8_t U, float k_min, float k_max)
{
  if (L < U) std::swap(L, U);
  if (L < -126) L = -126;

  if (k_min <= 1.f) k_min = 1.f;
  if (k_max <= 1.f) k_max = 1.f;
  if (k_max < k_min) std::swap(k_min, k_max);

  constexpr uint32_t max_core_spread = 2;

  // Build core matrix A0 with condition number close to `k0`
  // using A0 = U * diag(sigma) * V^T, with sigma in geometric progression.
  constexpr auto build_core_matrix = [](double k0) -> DMatrix<N> {
      if (k0 < 1.) k0 = 1.;

      DMatrix<N> U = random_orthogonal<N>();
      DMatrix<N> V = random_orthogonal<N>();

      DVector<N> sigma{};
      if constexpr  (N == 1) {
          sigma(0) = 1.0;
      } else {
          for (int k = 0; k < N; ++k) {
              // t in [0,1], sigma(0)=1, sigma(n-1)=1/kappa0
              auto t = static_cast<double>(k) / (N - 1);
              sigma(k) = std::pow(k0, -t);
          }
      }

      auto A = U * sigma.asDiagonal() * V.transpose();

      return A;
  };

  // Try to keep the core matrix within a small log2 span
  constexpr auto build_bounded_core_matrix = [build_core_matrix](double k0) {
    auto res = build_core_matrix(k0);
    auto [lo, hi] = log2_minmax(res);
    auto diff = std::abs(hi - lo);

    //while (diff != max_core_spread)
    while ((diff > max_core_spread + 1) || (diff < max_core_spread -1))
    {
      res = build_core_matrix(k0);
      const auto [l, h] = log2_minmax(res);
      diff = std::abs(l - h);
    }

    // rescale to be in range [2^0, 2^diff]
    res *= std::pow(2.0, -lo);

    return res;
  };

  const uint32_t target_spread = std::abs(U - L); // desired log2 spread in magnitudes
  const uint32_t log2_spread_target = std::max(max_core_spread, target_spread - max_core_spread);
  //const uint32_t log2_spread_target = std::abs(U - L); // desired log2 spread in magnitudes

  // We'll scale columns and rows independently, so each will contribute to the total resulting
  // order of magnitude
  const double spread_row = log2_spread_target / 2.0;
  const double spread_col = log2_spread_target - spread_row;

  // scale uniformly up to allowed spread
  std::uniform_real_distribution<double> row_factor(1., std::pow(2., spread_row));
  std::uniform_real_distribution<double> col_factor(1., std::pow(2., spread_col));

  constexpr int maxTries = 10'000'000;

  for (int attempt = 0; attempt < maxTries; ++attempt) {
    // 1. Build new core matrix A0 with condition number close to k_min
    auto A0 = build_bounded_core_matrix(k_min);

    auto B = A0;
    for (int i = 0; i < N; ++i) {
      B.row(i) = B.row(i).eval() * row_factor(gen);
      B.col(i) = B.col(i).eval() * col_factor(gen);
    }

    // 3. Compute current log2 range of B and shift globally into [2^L,2^U] window.
    const auto [log2MinB, log2MaxB] = log2_minmax(B);

    if (!std::isfinite(log2MinB) || !std::isfinite(log2MaxB)) {
        // Something degenerate; resample.
        continue;
    }

    const double centerB      = (log2MinB + log2MaxB) / 2.;
    const double centerTarget = (L + U) / 2.;
    const double log2_shift   = centerTarget - centerB;
    const double alpha        = std::pow(2., log2_shift);
    //const int centerB      = ((int)log2MinB + (int)log2MaxB) / 2;
    //const int centerTarget = (L + U) / 2;
    //const int log2_shift   = centerTarget - centerB;
    //const double alpha     = std::pow(2., log2_shift);

    auto A = alpha * B;
    //auto A = B;

    //clip_to_range<N>(A, L-1, U+1);

    [[maybe_unused]] const auto [log2MinA, log2MaxA] = log2_minmax(A);

    //if ( ((log2MaxA > U) && ((int)log2MaxA - U > 10))
    //  || ((log2MinA < L) && (L - (int)log2MinA > 10)))
    //{
    //  continue;
    //}

    // Check that we didn't alter the condition number too much with the random noise
    const auto kA = cond2<N>(A);
    if (!std::isfinite(kA) || kA < k_min || kA > k_max)
    {
        continue;
    }

    return A.template cast<float>();
  }

  return {};
}

} // namespace

int main(int argc, char** argv) noexcept
{
    if (argc < 5) {
        std::cerr << "Usage: " << argv[0]
                  << " k low_exp high_exp\n";
        return 1;
    }

    int low = std::strtol(argv[1], nullptr, 10);
    int high = std::strtol(argv[2], nullptr, 10);
    float min_k = std::strtof(argv[3], nullptr);
    float max_k = std::strtof(argv[4], nullptr);

    constexpr std::uint8_t N = 4;

    for (int i = 0; i < 100'000; ++i) {
      //auto A = randomMatrixWithRange<N>(low_exp, high_exp, k).value();
      //const float d = high_exp - low_exp;
      //const float dd = d - (d / 3.4f);
      //auto A = wellConditionedWithRange<N>(low_exp, high_exp, std::pow(2.f, dd), k);
    //auto A = wellConditionedAroundExp<N>(k, low_exp).value();
    [[maybe_unused]] auto A = random_finetuned<N>(low, high, min_k, max_k).value();

      // --- stderr: print the matrix ---
      std::cerr << "Matrix " << i << ":\n" << A << "\n\n";

      // Compute min and max absolute entries
      float minAbs = A.cwiseAbs().minCoeff();
      float maxAbs = A.cwiseAbs().maxCoeff();

      //const auto [log2Min, log2Max] = log2_minmax<N>(A);

      // Print: condition_number:minAbs:maxAbs
      std::cout << cond2<N>(A.template cast<double>()) << ":" << minAbs << ":" << maxAbs << '\n';
      //std::cout << cond2<N>(A) << ":" << log2Min << ":" << log2Max << '\n';
    }

    return 0;
}
#endif
