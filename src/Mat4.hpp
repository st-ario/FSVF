#pragma once

#include "Vec4.hpp"

#include <optional>

namespace FSVF
{

// aligned to cache line, column-major representation
class alignas(64) Mat4
{
public:
  // constructs a Mat4 with uninitialized values
  Mat4() noexcept
    : cols{ _mm_undefined_ps(),
            _mm_undefined_ps(),
            _mm_undefined_ps(),
            _mm_undefined_ps() }
  {}

  Mat4(const Mat4& m) noexcept            = default;
  Mat4& operator=(const Mat4& m) noexcept = default;

  // constructs a matrix with the value x on the main diagonal
  explicit Mat4(float x);

  // loads 16 floats from a float buffer into a matrix object (column-major representation)
  // this constructor deals properly with alignment and it's
  // the only way a matrix should be created to access buffers
  explicit Mat4(const float* p);

  // constructs a matrix having the given arguments as columns
  Mat4(Vec4 v0, Vec4 v1, Vec4 v2, Vec4 v3)
    : cols{ v0, v1, v2, v3 }
  {}

  //explicit
  Mat4(LMat3 m);

  // constructs a matrix with the specified components (column-major representation);
  // prefer using another constructor if the data is already available in a contiguous buffer
  // clang-format off
  Mat4(float x0, float x1, float x2, float x3,
       float x4, float x5, float x6, float x7,
       float x8, float x9, float xA, float xB,
       float xC, float xD, float xE, float xF);
  // clang-format on

  // constructs a matrix having the given arguments as main diagonal entries
  Mat4(float d0, float d1, float d2, float d3);

  bool operator==(const Mat4& m) const = default;
  bool operator!=(const Mat4& m) const = default;

  // multiply with a 4D vector
  template<typename T>
    requires FastVector<T> && (!std::derived_from<T, LVec3>)
  T FSVF_VECCALL operator*(T v) const;

  // (deleted) multiply with a 3D vector
  // deleted as potentially ambiguous, use `transform_3x3()`, `transform_affine()` or
  // `transform_projective()` instead
  template<std::derived_from<LVec3> T>
  T FSVF_VECCALL operator*(T m) const = delete;

  // multiply the (top-left) 3x3 submatrix of `*this` with `v`
  template<std::derived_from<LVec3> T>
  T FSVF_VECCALL transform_3x3(T v) const;

  // transform `v` by `*this`, when `*this` is an affine transformation
  // the result is undefined if the last _row_ of `*this` is != (0 0 0 1)
  //
  // when the preconditions are met, this function is logically equivalent to
  // `(*this) * Vec4{v, 1.f}`
  template<std::derived_from<LVec3> T>
  T FSVF_VECCALL transform_affine(T m) const;


  // transform `v` by `*this`, when `*this` is a projective transformation
  // unlike transform_affine(), this method has no preconditions
  //
  // this function is logically equivalent to
  // ```(pseudo-code)
  // res = (*this) * {v.x, v.y, v.z, 1.f}
  // scale = res.wwww
  // return Vec3(res / res.wwww)
  // ```
  template<std::derived_from<LVec3> T>
  T FSVF_VECCALL transform_projective(T m) const;

  // TODO transposed variants for transform_*()?

  // access by column
  Vec4 FSVF_VECCALL operator[](int i) const { return cols[i]; }

  Vec4& FSVF_VECCALL operator[](int i) { return cols[i]; }

  Mat4 FSVF_VECCALL transpose() const;
  Mat4 FSVF_VECCALL adj_transposed() const;
  Mat4 FSVF_VECCALL inverse_transposed() const;

  // multiply all entries by x (equivalent to multiplying by diagonal matrix with entries x)
  Mat4 FSVF_VECCALL scale_by(float x) const;
  Mat4 FSVF_VECCALL operator*(float x) const { return scale_by(x); }

  // component-wise multiply each column by v (equivalent to multiplying by diagonal matrix with entries v)
  Mat4 FSVF_VECCALL scale_by(Vec4 v) const;

  // divide all entries by x (equivalent to multiplying by diagonal matrix with entries 1/x)
  Mat4 FSVF_VECCALL scale_by_inv(float x) const;

  // component-wise divide each column by v (equivalent to multiplying by diagonal matrix with entries 1/v)
  Mat4 FSVF_VECCALL scale_by_inv(Vec4 v) const;

  // TODO rename
  // TODO needed?
  Mat4 FSVF_VECCALL mul_transp(Mat4 m) const;

  // returns the adjunct matrix of `*this`
  // NOTE that `adj_transposed()` is faster than `adj()`;
  // avoid doing things like `adj().transpose()`
  Mat4 FSVF_VECCALL adj() const { return adj_transposed().transpose(); }

  // returns the inverse matrix of `*this`
  // NOTE that `inverse_transposed()` is faster than `inverse()`;
  // avoid doing things like `inverse().transpose()`
  Mat4 FSVF_VECCALL inverse() const;

  // returns the inverse matrix of `*this`, if the determinant is !=0, or a `nullopt` otherwise
  // NOTE that `inverse_transposed_safe()` is faster than `inverse_transposed()`;
  std::optional<Mat4> FSVF_VECCALL inverse_safe() const;

  // if the determinant is 0, returns the inverse transposed of `*this`, otherwise a `nullopt`
  std::optional<Mat4> FSVF_VECCALL inverse_transposed_safe() const;

  float FSVF_VECCALL determinant() const;

  // returns the determinant of the (top-left) 3x3 submatrix of `*this`, ignoring whatever is
  // contained in the last row and column
  float FSVF_VECCALL determinant_3x3() const;

public:
  std::array<Vec4, 4> cols;
};

Mat4 FSVF_VECCALL operator/(Mat4 m, float x);
Mat4 FSVF_VECCALL operator/(float x, Mat4 m);

}    // namespace FSVF
