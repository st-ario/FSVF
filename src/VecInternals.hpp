#pragma once

#include "LVec3.hpp"

namespace FSVF
{

// public getter for FastVector's intrinsic member
template<FastVector T>
[[nodiscard]] __m128 m_sse(T v)
{
  if constexpr (std::derived_from<T, LVec3>)
  {
    return static_cast<LVec3>(v).m_sse;
  }
  else { return static_cast<Vec4>(v).m_sse; }
}

// shenanigans to allow FastVector's interface functions access to LVec3's unsafe constructor using
// the same interface we use for the other classes, without having to re-declare them all as friends
namespace Internals
{
template<FastVector T>
FSVF_FORCEINLINE T raw_constructor_wrapper(__m128 sse)
{
  if constexpr (std::is_base_of_v<LVec3, T>)
  {
    return LVec3::unsafe_construct<true>(sse);
  }
  else { return T{ sse }; }
}
}

// undef'd at the end of FSVF.hpp
#ifdef FSVF_RAW_
  #error                                                                  \
    "FATAL: FSVF_RAW_ has already been defined outside of this library, \
    please rename this macro"
#endif
#define FSVF_RAW_(T, x) Internals::raw_constructor_wrapper<T>(x)

}    // namespace FSVF
