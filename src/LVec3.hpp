#pragma once

#include "Shuffles.hpp"
#include "Vec4.hpp"

#include <array>
#include <cstring>

namespace FSVF
{
using stdVec3 = std::array<float, 3>;

/* ########################################## LVec3 ############################################# */

/* restricted component-wise access, see Vec4's documentation
 *
 * "Large" Vec3, 4 packed 32-bit floats, the last component is guaranteed to be == 0.0f, and access
 * to it is hidden from the user
 */
class alignas(__m128) LVec3
{
public:
  static LVec3 zeroes() { return unsafe_construct<true>(_mm_setzero_ps()); }

  LVec3()
    : m_sse{ _mm_setzero_ps() }
  {}

  LVec3(const LVec3& v) = default;

  // constructs a LVec3 with the same value in all components
  explicit LVec3(float x)
    : m_sse{ _mm_set_ps1(x) }
  {
    m_sse = precondition(m_sse);
  }

  // constructs a LVec3 with the specified components;
  // prefer using another constructor if the data is already available in a contiguous buffer
  LVec3(float x, float y, float z)
    : m_sse{ _mm_set_ps(0.0f, z, y, x) }
  {}

  // loads 3 floats from a float buffer into a LVec3 object
  // this constructor deals properly with alignment and it's
  // the only way a LVec3 should be created to access buffers
  explicit LVec3(const float* p)
    : m_sse{ _mm_loadu_ps(p) }
  {
    m_sse = precondition(m_sse);
  }

  // create a Vec3 from a Vec4 (zeroes out the last component, NOT a memcpy/no-op)
  explicit LVec3(Vec4 v)
    : m_sse{ v.m_sse }
  {
    m_sse = precondition(m_sse);
  }

  LVec3& FSVF_VECCALL operator=(const LVec3& m)
  {
    m_sse = m.m_sse;
    return *this;
  }

  FSVF_FORCEINLINE stdVec3 components() const
  {
    stdVec3 res;
    std::memcpy(&res, &m_sse, sizeof(res));
    return res;
  }

  // unsafe construction: doesn't check/enforce that the last element is 0
  // the user needs to pass a `true` template argument for the call to compile
  // this is just a small measure taken to make users think twice about correctness
  template<bool EnableUnsafeConstruction = false, typename T = Internals::Empty>
  static FSVF_FORCEINLINE LVec3 unsafe_construct(__m128 sse)
  {
    // the template parameter T is only there to enable using `always_false``
    if constexpr (!EnableUnsafeConstruction)
    {
      static_assert(
        Internals::always_false_v<T>,
        "ERROR: trying to use LVec3's unsafe constructor, "
        "be mindful of the invariant!"
      );
    }

    return LVec3(sse);
  }

  // TODO make private again after fixing tests
  //private:
  union {
    __m128 m_sse;

    struct
    {
      float x;
      float y;
      float z;
      float NOPE;
    };
  };

  // unsafe constructor, doesn't check/enforce that the last element is 0
  explicit LVec3(__m128 sse)
    : m_sse{ sse }
  {}

  friend class Vec4;
  friend class Mat4;

  template<Shuf3 SHUFFLE, FastVector T>
  friend T FSVF_VECCALL shuffle(T v);

  static FSVF_FORCEINLINE __m128 precondition(__m128 sse);

  // public getter
  template<FastVector T>
  friend __m128 m_sse(T v);
};
}    // namespace FSVF
