# FSVF (Fast Small Vectors of Floats)

A SIMD library for single-precision 3D and 4D linear algebra, heavily optimized for desktop x86_64 architectures.

FSVF is designed for game engines, graphics applications, and numerical code where small-vector math is a bottleneck. It's designed to make it natural to write code that compilers can optimize better. Moreover, it often outperforms other state-of-the-art numerical libraries like Eigen on common operations like 4x4 matrix inverse and determinant on consumer CPUs.

## Features

- **Fast**: Consistently faster than Eigen for small matrix/vector operations on desktop CPUs
- **SIMD-native API**: Designed to help you write performant code that compilers can optimize well
- **Configurable**: Control floating-point behavior and precision/performance tradeoffs via compile-time macros
- **Header-only**: Single include, no build system integration required

## Quick Start

```cpp
#include <FSVF.h>
using namespace FSVF;

Vec4 v{1.f, 2.f, 3.f, 4.f};
Mat4 m{1.f};  // identity matrix

Vec4 result = m * v;
float d = m.determinant();
Mat4 inv = m.inverse();
```

## Why FSVF?

### Performance

FSVF competes with and often outperforms state-of-the-art libraries like Eigen on key operations like computing determinants and inverses (see benchmarks below).

### An API That Guides You Toward Fast Code

SIMD code is fastest when data stays in vector registers. Moving individual floats in and out of SIMD lanes kills performance.

Most vector math APIs make it easy to write code that's hard for the compiler to optimize, and sometimes make it cumbersome to write code that can be optimized well. FSVF's goal is to do the opposite: SIMD-native operations are the natural, concise path. Scalar access is available but intentionally verbose.

The result is that even if you're not a SIMD expert, you'll naturally write code that compilers can optimize well. This matters most in complex algorithms: when you structure your code around shuffles, FMAs, and blends instead of component access, compilers can often generate vectorized code paths that would be impossible with a "naive" scalar implementation, even if that scalar code looks cleaner on paper.

**The problem with typical vector APIs:**

```cpp
vec.x = other.y;
vec.z = some_float;
float f = vec.w;
```

Each component access can force data out of SIMD registers, through memory, and back. Compilers *can* optimize this away in the simplest cases, but often aren't able to do so for complex algorithms.

**FSVF's approach:**

FSVF makes you reach for SIMD-native operations first. Component access is available via `.components()` but is intentionally verbose—a reminder that you're leaving SIMD-land.

```cpp
// Shuffles: rearrange components without leaving SIMD registers
Vec4 v{1.f, 2.f, 3.f, 4.f};
Vec4 swizzled = shuffle<Shuf4::wzyx>(v);  // {4, 3, 2, 1}
Vec4 broadcast = shuffle<Shuf4::xxxx>(v); // {1, 1, 1, 1}

// FMA: fused multiply-add stays in registers
Vec4 a, b, c;
Vec4 result = fmadd(a, b, c);  // a*b + c, single instruction

// Blend: combine vectors based on a mask
Vec4 blended = blend_with_mask<0b1010>(a, b);  // {a.x, b.y, a.z, b.w}

// When you really need scalar access
auto [x, y, z, w] = v.components();
```

**Example: transforming a 3D vector**

```cpp
// SIMD-friendly: one shuffle per component, then FMAs
LVec3 transform_point(const Mat4& m, LVec3 v) {
    auto vx = shuffle<Shuf3::xxx>(v);
    auto vy = shuffle<Shuf3::yyy>(v);
    auto vz = shuffle<Shuf3::zzz>(v);
    
    auto result = fmadd(m[0], vx, m[3]);  // col0 * vx + col3
    result = fmadd(m[1], vy, result); // += col1 * vy
    result = fmadd(m[2], vz, result); // += col2 * vz
    return LVec3{result};
}
```

This compiles to tight SIMD code because all operations work on full 128-bit registers. No scalar extraction, no lane-crossing penalties.

### Benchmarks

FSVF's core operations are fast. Operations like 4x4 matrix inverse and determinant are already heavily optimized in mature libraries like Eigen. Yet FSVF manages to be competitive or faster in most configurations, delivering improvements on operations that are already near the limits of what the hardware can do.

#### 4x4 Matrix Determinant

| CPU | Compiler | FSVF (ns/op) | Eigen (ns/op) | FSVF vs Eigen |
|-----|----------|--------------|---------------|---------------|
| 13th Gen i7-13850HX (Laptop) | Clang 18 | 4.25 ± 0.2% | 6.14 ± 0.4% | 44.4% faster |
| Ryzen 9 7950X3D, V-Cache CCD | Clang 21 | 3.73 ± 0.9% | 4.41 ± 0.3% | 18.3% faster |
| Ryzen 9 7950X3D, standard CCD | Clang 21 | 3.62 ± 0.1% | 4.21 ± 0.2% | 16.6% faster |
| Ryzen 9 7950X3D, V-Cache CCD | GCC 15 | 3.14 ± 0.1% | 3.31 ± 0.1% | 5.5% faster |
| Ryzen 9 7950X3D, standard CCD | GCC 15 | 2.92 ± 0.1% | 3.06 ± 0.1% | 5.1% faster |
| Ryzen 7 PRO 7840U (Laptop) | Clang 21 | 6.73 ± 0.9% | 7.00 ± 0.0% | 4.0% faster |
| Ryzen 7 PRO 7840U (Laptop) | GCC 15 | 4.89 ± 0.1% | 5.12 ± 0.2% | 4.8% faster |
| 13th Gen i7-13850HX (Laptop) | GCC 13 | 4.11 ± 0.2% | 3.88 ± 0.0% | 5.6% slower |

#### 4x4 Matrix Inverse

| CPU | Compiler | FSVF (ns/op) | Eigen (ns/op) | FSVF vs Eigen |
|-----|----------|--------------|---------------|---------------|
| Ryzen 7 PRO 7840U (Laptop) | Clang 21 | 14.71 ± 2.2% | 18.50 ± 2.0% | 25.8% faster |
| Ryzen 9 7950X3D, standard CCD | Clang 21 | 7.92 ± 2.0% | 9.37 ± 2.1% | 18.2% faster |
| Ryzen 9 7950X3D, V-Cache CCD | Clang 21 | 8.61 ± 1.2% | 9.89 ± 2.2% | 14.8% faster |
| 13th Gen i7-13850HX (Laptop) | GCC 13 | 13.21 ± 0.0% | 14.04 ± 0.1% | 5.9% faster |
| Ryzen 7 PRO 7840U (Laptop) | GCC 15 | 16.64 ± 0.1% | 15.97 ± 0.0% | 4.0% slower |
| Ryzen 9 7950X3D, V-Cache CCD | GCC 15 | 10.49 ± 0.1% | 10.03 ± 0.0% | 4.3% slower |
| Ryzen 9 7950X3D, standard CCD | GCC 15 | 9.65 ± 0.2% | 9.21 ± 0.1% | 4.6% slower |
| 13th Gen i7-13850HX (Laptop) | Clang 18 | 10.95 ± 0.0% | 10.04 ± 0.1% | 8.3% slower |

Benchmarks run using [nanobench](https://github.com/martinus/nanobench). See `test/Benchmark.cpp` for the benchmark code.

## Configuration

FSVF behavior is controlled through preprocessor macros. Define these *before* including `FSVF.h`.

### Floating-Point Modes

Exactly one of these should be defined (defaults to `FSVF_FLOAT_DEFAULT_MATH`):

| Macro | Behavior |
|-------|----------|
| `FSVF_FLOAT_FINITE_MATH` | Assumes no infinities, NaNs, or signed zeros. Fastest. |
| `FSVF_FLOAT_DEFAULT_MATH` | Handles INF/NaN/signed zeros, but not denormals. Default. |
| `FSVF_FLOAT_DENORMAL_MATH` | Full denormal support. Slowest. (Not fully supported yet) |

### Precision vs Performance

| Macro | Effect |
|-------|--------|
| `FSVF_NO_RECIPROCAL_MATH` | Use actual division instead of multiply-by-reciprocal. More precise. |
| `FSVF_RSQRT_NORMALIZE` | Use `rsqrt` for vector normalization. Faster but less precise. |

### DAZ/FTZ Control

| Macro | Effect |
|-------|--------|
| `FSVF_NO_AUTOMATIC_FTZ_DAZ` | Don't automatically set FTZ/DAZ flags at startup. |

By default (unless `FSVF_FLOAT_DENORMAL_MATH` is defined), FSVF sets the FTZ (Flush-to-Zero) and DAZ (Denormals-Are-Zero) CPU flags at program startup. This prevents denormal numbers from causing slowdowns in SIMD code.

---

# Reference

## Types

### `Vec4`

4 packed single-precision floats. The fundamental SIMD type.

```cpp
Vec4 v;                          // uninitialized
Vec4 v{1.f};                     // all components = 1
Vec4 v{1.f, 2.f, 3.f, 4.f};      // explicit components
Vec4 v{float_ptr};               // load from memory (handles alignment)
Vec4 v = Vec4::zeroes();         // all zeros
```

Implicit conversion to/from `__m128` allows seamless interop with raw intrinsics.

### `LVec3`

"Large" 3D vector—uses 4 floats internally, with the 4th guaranteed to be zero. Useful for 3D points and directions where the extra storage doesn't matter.

```cpp
LVec3 v;                         // zero-initialized (unlike Vec4)
LVec3 v{1.f};                    // all components = 1
LVec3 v{1.f, 2.f, 3.f};          // explicit components
LVec3 v{float_ptr};              // load from memory
```

### `Mat4`

4x4 matrix, column-major, cache-line aligned (64 bytes).

```cpp
Mat4 m;                          // uninitialized
Mat4 m{1.f};                     // identity matrix (diagonal = 1)
Mat4 m{v0, v1, v2, v3};          // from column vectors
Mat4 m{float_ptr};               // load 16 floats from memory
Mat4 m{d0, d1, d2, d3};          // diagonal matrix
```

Key operations:
```cpp
m.transpose()
m.determinant()
m.inverse()
m.adj()                          // adjugate matrix

m * v                            // matrix-vector multiply
m * n                            // matrix-matrix multiply
m.transform_3x3(v)               // transform by upper-left 3x3
m.transform_affine(v)            // affine transform (assumes last row is 0,0,0,1)
m.transform_projective(v)        // projective transform with perspective divide
```

### `LMat3`

3x3 matrix stored as a 4x4 (last row/column maintained as identity). Inherits from `Mat4`.

## Operations

### Vector Operations

```cpp
float dot(v, w);                 // dot product
LVec3 cross(v, w);               // cross product (3D only)

float len(v);                    // Euclidean length
float len_squared(v);            // squared length
T normalize(v);                  // unit vector

float sum(v);                    // sum of components
float prod(v);                   // product of components

T abs(v);                        // component-wise absolute value
T floor(v);                      // component-wise floor
T ceil(v);                       // component-wise ceil
T sqrt(v);                       // component-wise square root
T rsqrt(v);                      // fast reciprocal square root

T merge_max(v, w);               // component-wise max
T merge_min(v, w);               // component-wise min
T clamp(v, min, max);            // component-wise clamp
T saturate(v);                   // clamp to [0, 1]
```

### Shuffles

Rearrange vector components at compile time. Each shuffle compiles to a single instruction.

```cpp
// 4D shuffles
shuffle<Shuf4::xyzw>(v);         // identity (no-op)
shuffle<Shuf4::wzyx>(v);         // reverse
shuffle<Shuf4::xxxx>(v);         // broadcast x to all lanes
shuffle<Shuf4::xxyy>(v);         // duplicate pairs

// 3D shuffles (preserve 4th component as 0)
shuffle<Shuf3::xyz>(v);          // identity
shuffle<Shuf3::yzx>(v);          // rotate left
shuffle<Shuf3::zxy>(v);          // rotate right

// Blend two vectors (shuffle + merge)
shuffle_blend<Shuf4::xyzw>(v, w); // components 0-1 from v, 2-3 from w
```

### FMA (Fused Multiply-Add)

Single-instruction multiply-add operations. More accurate than separate multiply and add.

```cpp
fmadd(a, b, c);                  // a*b + c
fmsub(a, b, c);                  // a*b - c
fnmadd(a, b, c);                 // -(a*b) + c = c - a*b
fnmsub(a, b, c);                 // -(a*b) - c

fmaddsub(a, b, c);               // {a*b+c, a*b-c, a*b+c, a*b-c}
fmsubadd(a, b, c);               // {a*b-c, a*b+c, a*b-c, a*b+c}

addsub(v, w);                    // {v+w, v-w, v+w, v-w}
subadd(v, w);                    // {v-w, v+w, v-w, v+w}
```

### Comparisons

Most comparisons return **bitmasks** (`Vec4` with `0xFFFFFFFF` or `0x00000000` per component), not booleans. This is the SIMD-native representation and allows further vector operations on the result.

```cpp
// Relational operators return bitmasks
Vec4 mask = v < w;               // 0xFFFFFFFF where v[i] < w[i], else 0
Vec4 mask = v > w;
Vec4 mask = v <= w;
Vec4 mask = v >= w;

// Equality operators are the exception: they return bool
// (component-wise float comparison of all entries)
bool eq = (v == w);              // true if all components are equal
bool ne = (v != w);              // true if any component differs

// Explicit comparison functions return bitmasks
BitMasks::cmp_equal_f32(v, w);       // bitmask: float equality per component
BitMasks::cmp_not_equal_f32(v, w);
BitMasks::cmp_equal_bitwise(v, w);   // bitmask: bitwise equality per component
BitMasks::cmp_not_equal_bitwise(v, w);
```

### Blending and Masking

```cpp
// Compile-time blend (single instruction)
blend_with_mask<0b1010>(a, b);   // {a.x, b.y, a.z, b.w}

// Runtime blend using a bitmask from comparisons
Vec4 mask = a > b;
Vec4 result = blend_with_vector_mask(c, b, mask);  // select from c where mask is set
// or simply:
Vec4 result = blend_with_vector_mask(c, b, a > b);

// Check masks (reduce bitmask to bool)
all<4>(mask);                    // true if all 4 components are 0xFFFFFFFF
any<4>(mask);                    // true if any component has non-zero bits
none<4>(mask);                   // true if all components are zero
```

### Float Classification (Bitmasks)

Classification functions return **bitmasks**, not booleans. Use `all`/`any`/`none` to reduce to bool if needed.

```cpp
// These return Vec4 bitmasks (0xFFFFFFFF or 0x0 per component)
Vec4 mask = is_inf(v);
Vec4 mask = is_nan(v);
Vec4 mask = is_finite(v);
Vec4 mask = is_inf_or_nan(v);
Vec4 mask = is_denormal(v);      // (only with FSVF_FLOAT_DENORMAL_MATH)

// Convenience functions that return bool directly
bool b = any_is_inf(v);          // true if any component is infinity
bool b = any_is_nan(v);
bool b = any_is_finite(v);
bool b = any_is_inf_or_nan(v);

// Example: replace infinities with zero
v = blend_with_vector_mask(v, Vec4::zeroes(), is_inf(v));
```

### Float Comparison Utilities

```cpp
// Comparison with tolerance (return bool)
almost_equal(x, y);              // combined ULP + epsilon comparison
almost_equal_ulp<threshold>(x, y);
almost_equal_eps<epsilon>(x, y);

// Distance measurement
ulps_distance(x, y);             // ULPs between two floats (scalar or vector)

// Adjacent floats
next_float_up(x);                // next representable float toward +inf
next_float_down(x);              // next representable float toward -inf
```

### Parallel Reductions

Compute reductions on 4 vectors simultaneously, more efficiently than 4 separate calls.

```cpp
// Returns Vec4 where each component is the sum of the corresponding input vector
Vec4 sums = parallel_sum(v1, v2, v3, v4);  // {sum(v1), sum(v2), sum(v3), sum(v4)}
Vec4 prods = parallel_prod(v1, v2, v3, v4);
```

## Requirements

- C++23 compiler
- x86_64 with SSE4.1 and FMA support (virtually all desktop CPUs since ~2013)

## Building

FSVF is header-only. Just add `src/` to your include path and `#include <FSVF.h>`.

For running tests:
```bash
mkdir build && cd build
cmake ..
make
ctest
```