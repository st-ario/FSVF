#pragma once

#include <concepts>

namespace FSVF
{

class Vec4;
class Mat4;
class LVec3;
class LMat3;

// any type that inherits publicly from Vec4 or LVec3 qualifies as a FastVector
template<typename T>
concept FastVector = requires(T v) {
  requires std::derived_from<T, Vec4> || std::derived_from<T, LVec3> == true;
};

namespace Internals
{
template<typename T>
struct always_false
{
  constexpr static bool value = std::negation_v<std::is_same<T, T>>;
};

template<typename T>
constexpr bool always_false_v = always_false<T>::value;

// clang-format off
struct Empty {};

// templated magic to define the return type of operation on mixed-dimension vectors
// see `LargestDimensionType` and `SmallestDimensionType`

template<typename T1, typename T2>
concept V3AncestorAll
  = std::derived_from<T1, LVec3> && std::derived_from<T2, LVec3>;

template<typename T1, typename T2>
concept V3AncestorAny
  = std::derived_from<T1, LVec3> || std::derived_from<T2, LVec3>;

template<FastVector T1, FastVector T2, bool is3D = V3AncestorAll<T1, T2>>
struct LargerDimType;

template<FastVector T1, FastVector T2, bool is3D = V3AncestorAny<T1, T2>>
struct SmallerDimType;

template<FastVector T1, FastVector T2> struct LargerDimType<T1, T2, false>{using type = Vec4;};
template<FastVector T1, FastVector T2> struct LargerDimType<T1, T2, true> {using type = LVec3;};
template<FastVector T>                 struct LargerDimType< T,  T, true> {using type = T;};
template<FastVector T>                 struct LargerDimType< T,  T, false>{using type = T;};

template<FastVector T1, FastVector T2> struct SmallerDimType<T1, T2, false>{using type = Vec4;};
template<FastVector T1, FastVector T2> struct SmallerDimType<T1, T2, true> {using type = LVec3;};
template<FastVector T>                 struct SmallerDimType< T,  T, true> {using type = T;};
template<FastVector T>                 struct SmallerDimType< T,  T, false>{using type = T;};

// clang-format on
}    // namespace Internals

// return type for operations that return 4D vectors when mixing 3D and 4D arguments:
// - original type if the two arguments have the same type
// - LVec3 if it's a common ancestor of both argument types
// - Vec4 otherwise
template<FastVector T1, FastVector T2>
using LargestDimensionType = Internals::LargerDimType<T1, T2>::type;

// return type for operations that return 3D vectors when mixing 3D and 4D arguments:
// - original type if the two arguments have the same type
// - LVec3 if it's the ancestor of any of the two argument types
// - Vec4 otherwise
template<FastVector T1, FastVector T2>
using SmallestDimensionType = Internals::SmallerDimType<T1, T2>::type;

template<typename T1, typename T2>
concept SameDimension
  = FastVector<T1>
 && FastVector<T2>
 && (Internals::V3AncestorAll<T1, T2> || !Internals::V3AncestorAny<T1, T2>);
}    // namespace FSVF
