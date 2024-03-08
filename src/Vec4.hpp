#pragma once

#include "HelperTypes.hpp"
#include "Platform.hpp"

#include <array>

namespace FSVF
{

class LVec3;

using stdVec4 = std::array<float, 4>;

/* ########################################### Vec4 ############################################# */

/* 4 packed 32-bit floats
 *
 * component-wise access to vector elements is intentionally made verbose, as reading or
 * writing into the components of a SIMD type can easily result in extra instructions/copies and
 * slower generated code (even with the typical type-punned union approach used commonly for
 * these kind of structures)
 *
 * when manipulating a FastVector object, users should try to work as much as possible with
 * functions and members operating on that type/concept, and structure their algorithms accordingly,
 * to help the compiler generate optimal code; component-wise access in hot paths should be resorted
 * to only when strictly necessary; when components need to be accessed to interact with other
 * vectors, consider using suitable shuffles if possible;
 *
 * when you really need to read the components of a FastVector v, use the method v.components()
 * to obtain a std::array from it;
 *
 * the only way to write into a specific component of a FastVector is to re-assing to it from a
 * newly constructed one
 *
 * if this seems wasteful, it's because it inevitably is; the type-punned approach will usually
 * generate the same binary instructions as with this library's approach, but the implications of
 * these sort of operations will be less evident in code */
class alignas(__m128) Vec4
{
public:
  // constructs a Vec4 with uninitialized values
  Vec4() noexcept
    : m_sse(_mm_undefined_ps())
  {}

  Vec4(const Vec4& m) noexcept = default;

  // constructs a Vec4 with the same value in all components
  explicit Vec4(float x)
    : m_sse{ _mm_set_ps1(x) }
  {}

  // constructs a Vec4 with the specified components;
  // prefer using another constructor if the data is already available in a contiguous buffer
  Vec4(float x, float y, float z, float w)
    : m_sse{ _mm_set_ps(w, z, y, x) }
  {}

  // loads 4 floats from a float buffer into a Vec4 object
  // this constructor deals properly with alignment and it's
  // the only way a Vec4 should be created to access buffers
  explicit Vec4(const float* p)
    : m_sse{ _mm_loadu_ps(p) }
  {}

  /* implicit */
  Vec4(LVec3 v);

  Vec4(LVec3 v, float w);

  // implicit conversions to/from intrinsic type
  FSVF_FORCEINLINE Vec4(__m128 sse)
    : m_sse{ sse }
  {}

  FSVF_FORCEINLINE operator __m128() const { return m_sse; }

  FSVF_FORCEINLINE
  static Vec4 zeroes() { return _mm_setzero_ps(); }

  // returns a Vec4 with all its bits set to 1
  static Vec4 all_bits_ones();

  Vec4& FSVF_VECCALL operator=(const Vec4& m) noexcept
  {
    m_sse = m.m_sse;
    return *this;
  }

  FSVF_FORCEINLINE
  stdVec4 components() const { return std::bit_cast<stdVec4, __m128>(m_sse); }

  //private:
  union {
    __m128 m_sse;

    struct
    {
      float x;
      float y;
      float z;
      float w;
    };
  };

  friend class LVec3;
  friend class Mat4;

  // public getter
  template<FastVector T>
  friend __m128 m_sse(T v);
};

}    // namespace FSVF
