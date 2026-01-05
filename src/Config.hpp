#pragma once

#include <immintrin.h>

// TODO add and document FSVF_EXTRA_PRECISION_CROSS, FSVF_ONE_SIDED_STICKY_INFINITY_UPDOWN
// and FSVF_NO_STICKY_INFINITY_UPDOWN

/* Exactly one of the following must be defined (it's the user's responsibility
 * to set compilation flags accordingly):
 *
 * FSVF_FLOAT_FINITE_MATH   (finite arithmetic, no signed zeroes)
 * FSVF_FLOAT_DEFAULT_MATH  (INFs, NaNs, signed zeroes, no denormals)
 * FSVF_FLOAT_DENORMAL_MATH (denormals allowed; NOT YET FULLY SUPPORTED)
 *
 * NOTE that regardless of these settings, we always assume:
 * - no trapping of floating point exceptions (-fno-trapping-math, /fp:except-)
 * - no usage of errno (-fno-math-errno)
 * - same rounding mode throughout the client's application runtime (although no
 *   particular choice of rounding mode is required)
 *
 * NOTE the precision of the library's algorithms is currently undocumented and
 * might change across releases (and even across commits, as long as the major
 * release verrsion is still 0);
 * in particular, some optimization techniques that compilers would generate
 * only when specific flags (-fassociative-math, -funsafe-math, ...) are set
 * are, sometimes but not always, used for SIMD code;
 * in the future some of these optimizations might be made less aggressive when
 * FSVF_FLOAT_DENORMAL_MATH is defined, but currently there are no such
 * guarantees.
 * Some of the optimization that compilers would generate under the
 * -freciprocal-math are controlled through the definition (or lack thereof) of
 * the preprocessor variables FSVF_NO_RECIPROCAL_MATH and FSFV_RCPPS (see
 * documentation below) */

// check at most one floating point behavior flag is set
#ifdef FSVF_FLOAT_FINITE_MATH
  #if defined(FSVF_FLOAT_DEFAULT_MATH) || defined(FSVF_FLOAT_DENORMAL_MATH)
    #error "only one FLOAT_*_MATH flag can be defined at any time"
  #endif
#endif
#ifdef FSVF_FLOAT_DEFAULT_MATH
  #if defined(FSVF_FLOAT_FINITE_MATH) || defined(FSVF_FLOAT_DENORMAL_MATH)
    #error "only one FLOAT_*_MATH flag can be defined at any time"
  #endif
#endif
#ifdef FSVF_FLOAT_DENORMAL_MATH
  #if defined(FSVF_FLOAT_DEFAULT_MATH) || defined(FSVF_FLOAT_FINITE_MATH)
    #error "only one FLOAT_*_MATH flag can be defined at any time"
  #endif
#endif

// set the default floating point behavior flag if none has been set
#if !defined(FSVF_FLOAT_FINITE_MATH)    \
  && !defined(FSVF_FLOAT_DEFAULT_MATH)  \
  && !defined(FSVF_FLOAT_DENORMAL_MATH)
  #define FSVF_FLOAT_DEFAULT_MATH
#endif

/* Reciprocal math optimizations:
 * By default, whenever it is convenient to do so, divisions are approximated
 * (in code) by multiplications with the reciprocal of the divisor; users needing
 * extra precision can set the flag FSVF_NO_RECIPROCAL_MATH, which ensures
 * that actual division intrinsics are used in code to perform vectorized
 * divisions of floats (this behavior might still be overwritten by compilers
 * when using certain compilation flags).
 *
 * Users willing to sacrifice precision for speed can also set the (unset by
 * default) FSVF_RCPPS preprocessor variable (provided that
 * FSVF_FLOAT_DENORMAL_MATH is NOT set), which replaces *all* floating point
 * division intrinsics with multiplications with RCPPS
 * (https://www.felixcloutier.com/x86/rcpps) of the divisor.
 *
 * Another possible speed/accuracy tradeoff can be made through
 * FSFV_RSQRT_NORMALIZE; defining it will use RSQRT in order to compute length
 * and normalization of vectors (disabled by default, but implied by
 * FSFV_RCPPS) */

// increased precision at a performance cost
// #define FSVF_NO_RECIPROCAL_MATH                  /* undefined by default */

// TODO document dangerous

// increased performance at a precision cost
// #define FSVF_DANGEROUS_OPTIMIZATIONS             /* undefined by default */
// #define FSVF_RCPPS                               /* undefined by default */
// #define FSVF_RSQRT_NORMALIZE                     /* undefined by default */

#ifdef FSVF_DANGEROUS_OPTIMIZATIONS
  #define FSVF_RCPPS
  #define FSVF_RSQRT_NORMALIZE
#endif

#if defined(FSVF_NO_RECIPROCAL_MATH)
#if defined(FSVF_RCPPS) || defined(FSVF_RSQRT_NORMALIZE) || defined(FSVF_RSQRT_DANGEROUS)
  #error "conflicting preprocessor definitions"
#endif
#endif

/* Setting FTZ and DAZ automatically at application startup:
 * unless either FSFV_DENORMAL_MATH or FSVF_NO_AUTOMATIC_FTZ_DAZ are defined,
 * the library will try to set FTZ and DAZ at program startup, through the
 * constructor of a static object defined for this purpose */

// #define FSVF_NO_AUTOMATIC_FTZ_DAZ                /* undefined by default */

namespace FSVF::Internals
{
/* Try to set FTZ and DAZ before entering `main()` if FSVF_FLOAT_DENORMAL_MATH
 * is not set */
#if !defined(FSVF_NO_AUTOMATIC_FTZ_DAZ) && !defined(FSVF_FLOAT_DENORMAL_MATH)
struct FlagSetter
{
  FlagSetter()
  {
    _mm_setcsr(_mm_getcsr() | 0x8040); /* FTZ: 0x8000, DAZ: 0x0040 */
  }
};

static inline Internals::FlagSetter s_FlagSetter{};
#endif
}
