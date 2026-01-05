#pragma once

#include <concepts>

namespace FSVF
{

class Vec4;
class Mat4;
class LVec3;
class LMat3;

// any type that inherits publicly from Vec4 or LVec3 qualifies as a FastVector, provided that it's not larger
// than the original type (we want to use them polymorphically while also passing them by value, so we can't
// allow slicing)
template<typename T>
concept FastVector = requires(T v) {
  requires
    sizeof(T) == 16
    &&(std::derived_from<T, Vec4>
    || std::derived_from<T, LVec3>
    || std::is_convertible_v<T, Vec4>
    || std::is_convertible_v<T, LVec3>)
    == true;
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

// analogous to "= delete", but for specific function template instances
template<typename T>
constexpr int impossible_instance() {
  if constexpr (std::is_same_v<T,T>) {
    static_assert(always_false_v<T>);
    return 1;
  } else {
    return 0;
  }
}

// clang-format off
struct Empty {};

template<typename T1, typename T2>
concept V3AncestorAll
  = std::derived_from<T1, LVec3> && std::derived_from<T2, LVec3>;

template<typename T1, typename T2>
concept V3AncestorAny
  = std::derived_from<T1, LVec3> || std::derived_from<T2, LVec3>;
} // Internals

template<typename T1, typename T2>
concept SameDimension
  = FastVector<T1>
 && FastVector<T2>
 && (Internals::V3AncestorAll<T1, T2> || !Internals::V3AncestorAny<T1, T2>);

// templated magic to define the return type of operation on mixed-dimension vectors
// see `LargestDimensionType` and `SmallestDimensionType`
namespace Internals
{
template<FastVector T1, FastVector T2, bool is3D = V3AncestorAll<T1, T2>, bool dameDim = SameDimension<T1, T2>>
struct LargerDimType;

template<FastVector T1, FastVector T2, bool is3D = V3AncestorAny<T1, T2>, bool dameDim = SameDimension<T1, T2>>
struct SmallerDimType;

template<FastVector T1, FastVector T2, bool wantSmaller>
struct SelectType
{
private:
  static constexpr bool both3D = (std::derived_from<T1, LVec3> && std::derived_from<T2, LVec3>);

  static constexpr bool same_dimension =
    both3D || !(std::derived_from<T1, LVec3> || std::derived_from<T2, LVec3>);

  // only works if we already know they have different dimensions
  using smaller_type = std::conditional_t<std::is_base_of_v<LVec3, T1>, T1, T2>;

  // only works if we already know they have different dimensions
  using larger_type = std::conditional_t<std::is_base_of_v<Vec4, T1>, T1, T2>;

  // The smaller/larger of the two according to the parameter; it works
  // correctly only if we already know they have different dimensions
  using requested_type = std::conditional_t<wantSmaller, smaller_type, larger_type>;

  using common_ancestor = std::conditional_t<both3D, LVec3, Vec4>;

public:
  using type =
    std::conditional_t<same_dimension,
      std::conditional_t<std::is_base_of_v<T2, T1>,
        T1,
        std::conditional_t<std::is_base_of_v<T1, T2>,
          T2,
          common_ancestor
        >
      >,
      requested_type
    >;
};

// clang-format on
}    // namespace Internals

// return types for operations that return 4D vectors when mixing 3D and 4D arguments:
// - if both types have dimension 3 but are otherwise unrelated -> LVec3
// - if both types have dimension 4 but are otherwise unrelated -> Vec4
template<FastVector T1, FastVector T2>
using LargestDimensionType = Internals::SelectType<T1, T2, false>::type;
template<FastVector T1, FastVector T2>
using SmallestDimensionType = Internals::SelectType<T1, T2, true>::type;
}    // namespace FSVF
