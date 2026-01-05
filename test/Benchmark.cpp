#define ANKERL_NANOBENCH_IMPLEMENT
#include <nanobench.h>

#include <FSVF.h>

#include <Eigen/Dense>
#include <Eigen/LU>
#include <Eigen/QR>
#include <Eigen/SVD>
#include <cstdint>
#include <iostream>
#include <limits>
#include <random>

#include <sched.h>
#include <unistd.h>

using namespace FSVF;

namespace {
  constexpr size_t valsN = 256u;

  std::random_device rd;
  std::mt19937 gen(rd());

  std::vector<float> gen_params(uint32_t n)
  {
    std::uniform_int_distribution<> ranb(0,1);
    std::uniform_real_distribution<float> ranf(1.0f,
      std::numeric_limits<float>::max() / 64.0f);

    std::vector<float> params{};

    for (uint32_t i = 0; i < n; ++i)
    {
      params.push_back(static_cast<float>(-1 * ranb(gen)) * ranf(gen));
    }

    return params;
  }

  const std::vector<float> Fpool = []()
  {
    return gen_params(valsN);
  }();
} // namespace

auto my_determinant(const Mat4& m) {
  return m.determinant();
}

auto my_inverse(const Mat4& m) {
  return m.inverse();
}

auto eig_determinant(const Eigen::Matrix4f& m) {
  return m.determinant();
}

auto eig_inverse(const Eigen::Matrix4f& m) {
  Eigen::Matrix4f eres;
  eres = m.inverse().eval();
  return eres;
}


int main() {

  cpu_set_t set;
  CPU_ZERO(&set);
  CPU_SET(8, &set);
  //CPU_SET(9, &set);
  //CPU_SET(10, &set);
  //CPU_SET(11, &set);

  pid_t pid = getpid();

  if (sched_setaffinity(pid, sizeof(cpu_set_t), &set) != 0) {
    std::cerr << "sched_setaffinity failed: " << std::strerror(errno) << "\n";
    return 1;
  }

  std::cout << "Process pinned to CPU " << 8 << "\n";


  struct sched_param sp = { .sched_priority = 99 };
  sched_setscheduler(pid, SCHED_FIFO, &sp);

    ankerl::nanobench::Bench bench;

  bench
    .title("FSVF Determinant")
    .relative(true)
    .epochs(10'000u)
    .clockResolutionMultiple(5'000ull)
    .warmup(2'000'000ull)
  ;
  {
    // the largest uint8_t is 255, and being unsigned it's guaranteed to work mod 256
    // so we can just increment `i` without care, given that valsN is 256
    uint8_t i = 0;

    Eigen::Matrix4f em;
    float edet;

    bench.run("Eigen Determinant", [&]() {
      em = Eigen::Matrix4f(&Fpool[i]);
      ankerl::nanobench::doNotOptimizeAway(edet = em.determinant());
      i += 16;
    });

    Mat4 m;
    float det;

    bench.run("FSVF Determinant", [&]() {
      m = Mat4(&Fpool[i]);
      ankerl::nanobench::doNotOptimizeAway(det = m.determinant());
      i += 16;
    });

  }

  bench
    .title("Inverse")
    .relative(true)
    .minEpochIterations(1'000'000ull)
    .epochs(2'000u)
    .clockResolutionMultiple(5'000ull)
    .warmup(2'000'000ull)
  ;
  {
    // the largest uint8_t is 255, and being unsigned it's guaranteed to work mod 256
    // so we can just increment `i` without care, given that valsN is 256
    uint8_t i = 0;

    Eigen::Matrix4f em;
    Eigen::Matrix4f eres;
    bench.run("Eigen Inverse", [&]() {
      em = Eigen::Matrix4f(&Fpool[i]);
      ankerl::nanobench::doNotOptimizeAway(eres = em.inverse().eval());
      i += 16;
    });

    Mat4 m;
    Mat4 res;

    bench.run("FSVF Inverse", [&]() {
      m = Mat4(&Fpool[i]);
      ankerl::nanobench::doNotOptimizeAway(res = m.inverse());
      i += 16;
    });
  }

  std::ofstream fout("mat_times_vec.json");
  bench.render(ankerl::nanobench::templates::json(), fout);

}
