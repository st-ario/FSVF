#pragma once

#include "GenericMethodsImpl.hpp"
#include "LMat3.hpp"

namespace FSVF
{
FSVF_FORCEINLINE LMat3 FSVF_VECCALL LMat3::adj_transposed() const
{
  auto m0 = LVec3{ cols[0] };
  auto m1 = LVec3{ cols[1] };
  auto m2 = LVec3{ cols[2] };

  auto s0 = shuffle<Shuf3::yzx>(m1);
  auto s1 = shuffle<Shuf3::zxy>(m2);
  auto s2 = shuffle<Shuf3::zxy>(m1);
  auto s3 = shuffle<Shuf3::yzx>(m2);

  const auto c0 = s0 * s1 - s2 * s3;

  s0 = shuffle<Shuf3::zxy>(m0);
  s1 = shuffle<Shuf3::yzx>(m2);
  s2 = shuffle<Shuf3::yzx>(m0);
  s3 = shuffle<Shuf3::zxy>(m2);

  const auto c1 = s0 * s1 - s2 * s3;

  s0 = shuffle<Shuf3::yzx>(m0);
  s1 = shuffle<Shuf3::zxy>(m1);
  s2 = shuffle<Shuf3::zxy>(m0);
  s3 = shuffle<Shuf3::yzx>(m1);

  const auto c2 = s0 * s1 - s2 * s3;

  return LMat3{ c0, c1, c2 };
}

inline LMat3 FSVF_VECCALL LMat3::adj() const
{
  return unsafe_construct<true>(adj_transposed().transpose());
}

inline LMat3 FSVF_VECCALL LMat3::inverse() const
{
  const float inv_det = 1.f / determinant();
  return unsafe_construct<true>(adj().scale_by(inv_det));
}

inline LMat3 FSVF_VECCALL LMat3::inverse_transposed() const
{
  const float inv_det = 1.f / determinant();
  return unsafe_construct<true>(adj_transposed().scale_by(inv_det));
}

inline std::optional<LMat3> FSVF_VECCALL LMat3::inverse_safe() const
{
  const float det = determinant();

  if (det == 0.f) return {};

  const float inv_det = 1.f / det;

  auto res = adj();

  res.cols[0] *= inv_det;
  res.cols[1] *= inv_det;
  res.cols[2] *= inv_det;

  return res;
}

inline std::optional<LMat3> FSVF_VECCALL LMat3::inverse_transposed_safe() const
{
  const float det = determinant();

  if (det == 0.f) return {};

  const float inv_det = 1.f / det;

  auto res = adj_transposed();

  res.cols[0] *= inv_det;
  res.cols[1] *= inv_det;
  res.cols[2] *= inv_det;

  return res;
}

}    // namespace FSVF
