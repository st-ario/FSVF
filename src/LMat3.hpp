#pragma once

#include "LVec3.hpp"
#include "Mat4.hpp"

namespace FSVF
{

class alignas(64) LMat3 : public Mat4
{
  friend class Mat4;    // TODO remove/fix properly
  // TODO documentation

public:
  explicit LMat3(Mat4 m)
    : Mat4{
      LVec3{ m[0] },
      LVec3{ m[1] },
      LVec3{ m[2] },
      Vec4{ 0.f, 0.f, 0.f, 1.f }, // TODO careful here
  }
  {}

  // constructs a matrix by loading its components from a contiguous buffer
  explicit LMat3(const float* m);

  // constructs a matrix with the value x on the main diagonal
  explicit LMat3(float x)
    : Mat4{ x, x, x, 1.f }
  {}

  // constructs a matrix with the specified components (column-major representation);
  // prefer using another constructor if the data is already available in a contiguous buffer
  // clang-format off
  LMat3(float x0, float x1, float x2,
        float x3, float x4, float x5,
        float x6, float x7, float x8);
  // clang-format on

  // constructs a matrix having the given arguments as main diagonal entries
  LMat3(float d0, float d1, float d2)
    : Mat4(d0, d1, d2, 1.f)
  {}

  // constructs a matrix having the given arguments as columns
  LMat3(LVec3 v0, LVec3 v1, LVec3 v2)
    : Mat4{
      v0,
      v1,
      v2,
      Vec4{ 0.f, 0.f, 0.f, 1.f }
  }
  {}

  bool operator==(const LMat3& m) const = default;
  bool operator!=(const LMat3& m) const = default;

  // unsafe constructor, doesn't check/enforce the class' invariants
  // the user needs to pass a `true` template argument for the call to compile
  // this is just a small measure taken to make users think twice about correctness
  template<bool EnableUnsafeConstruction = false, typename T = Internals::Empty>
  static LMat3 FSVF_VECCALL unsafe_construct(Mat4 m);

  using Mat4::transpose;
  using Mat4::operator*;

  LMat3 FSVF_VECCALL adj_transposed() const;
  LMat3 FSVF_VECCALL inverse_transposed() const;

  // if the determinant is 0, returns the inverse transposed of `*this`, otherwise a `nullopt`
  std::optional<LMat3> FSVF_VECCALL inverse_transposed_safe() const;

  // returns the adjunct matrix of `*this`
  // NOTE that `adj_transposed()` is faster than `adj()`;
  // avoid doing things like `adj().transpose()`
  LMat3 FSVF_VECCALL adj() const;

  // returns the inverse matrix of `*this`
  // NOTE that `inverse_transposed()` is faster than `inverse()`;
  // avoid doing things like `inverse().transpose()`
  LMat3 FSVF_VECCALL inverse() const;

  // returns the inverse matrix of `*this`, if the determinant is !=0, or a `nullopt` otherwise
  // NOTE that `inverse_transposed_safe()` is faster than `inverse_transposed()`;
  std::optional<LMat3> FSVF_VECCALL inverse_safe() const;

  FSVF_FORCEINLINE
  float FSVF_VECCALL determinant() const { return Mat4::determinant_3x3(); }

private:
  // unsafe constructor, doesn't check/enforce the class' invariants
  LMat3(__m128 c0, __m128 c1, __m128 c2, __m128 c3)
    : Mat4(Vec4{ c0 }, Vec4{ c1 }, Vec4{ c2 }, Vec4{ c3 })
  {}
};

// clang-format off
FSVF_FORCEINLINE LMat3::LMat3(
  float x0, float x1, float x2,
  float x3, float x4, float x5,
  float x6, float x7, float x8
)
// clang-format on
{
  cols[0] = Vec4(x0, x1, x2, 0);
  cols[1] = Vec4(x3, x4, x5, 0);
  cols[2] = Vec4(x6, x7, x8, 0);
  cols[3] = Vec4(0, 0, 0, 1.f);
}

FSVF_FORCEINLINE LMat3::LMat3(const float* m)
{
  cols[0] = Vec4(m[0]);
  cols[1] = Vec4(m[3]);
  cols[2] = Vec4(m[6]);
  cols[3] = Vec4(0, 0, 0, 1.f);
}

template<bool EnableUnsafeConstruction, typename T>
FSVF_FORCEINLINE LMat3 FSVF_VECCALL LMat3::unsafe_construct(Mat4 m)
{
  return LMat3{ m_sse(m[0]), m_sse(m[1]), m_sse(m[2]), m_sse(m[3]) };
}

}    // namespace FSVF
