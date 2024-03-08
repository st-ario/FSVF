#pragma once

#include <immintrin.h>

#ifdef _WIN64
  #define FSVF_VECCALL __vectorcall
  #define FSVF_FORCEINLINE __forceinline
  // MSVC doesn't define addresses for purely intrinsic functions
  #define INTR_WRAP(f) [](__m128 v, __m128 w){ return f(v, w); }
#else
  #define FSVF_VECCALL
  #define FSVF_FORCEINLINE __attribute__((always_inline)) inline
  // MSVC doesn't define addresses for purely intrinsic functions
  #define INTR_WRAP(f) f
#endif
