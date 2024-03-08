#pragma once

#include "GenericMethods.hpp"
#include "LVec3.hpp"

namespace FSVF
{
FSVF_FORCEINLINE Vec4::Vec4(LVec3 v)
  : m_sse{ v.m_sse }
{}

FSVF_FORCEINLINE Vec4::Vec4(LVec3 v, float w)
  : m_sse{ v.m_sse }
{
  Vec4 wwww(w);
  m_sse = blend_with_mask<0b1110>(wwww, Vec4(m_sse));
}

FSVF_FORCEINLINE Vec4 Vec4::all_bits_ones()
{
  const auto v = Vec4{ _mm_undefined_ps() };
  return BitMasks::cmp_equal_bitwise(v, v);
}

}
