#pragma once

#include "Config.hpp"
#include "LMat3Impl.hpp"
#include "Mat4Impl.hpp"
#include "Vec4Impl.hpp"

#include <array>
#include <concepts>
#include <optional>
#include <stdint.h>

// Fast Small Vectors of Floats

namespace FSVF
{





// TODO USE _mm_insrt_epi32() and similar!!!!
// TODO add missing FSVF_VECCALLs
// TODO FSVF_FORCEINLINE almost everything
}    // namespace FSVF

// cleanup internal macros
#ifdef FSVF_RAW_
  #undef FSVF_RAW_
#endif
