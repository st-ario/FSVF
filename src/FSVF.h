#pragma once

#include <array>
#include <bit>
#include <cmath>
#include <concepts>
#include <cstdint>
#include <cstring>
#include <immintrin.h>
#include <optional>
#include <stdint.h>
#include <type_traits>
#include <utility>

// Fast Small Vectors of Floats

namespace FSVF
{

/* ########################################## Shuffles ########################################## */
// generated with generate_shuffles.hs

// compile-time arguments to shuffle 3D vectors
enum class Shuf3 : uint8_t
{
  /* xyzw components */
  xxx = 0b11'00'00'00, xxy = 0b11'01'00'00, xxz = 0b11'10'00'00, xyx = 0b11'00'01'00,
  xyy = 0b11'01'01'00, xyz = 0b11'10'01'00, xzx = 0b11'00'10'00, xzy = 0b11'01'10'00,
  xzz = 0b11'10'10'00, yxx = 0b11'00'00'01, yxy = 0b11'01'00'01, yxz = 0b11'10'00'01,
  yyx = 0b11'00'01'01, yyy = 0b11'01'01'01, yyz = 0b11'10'01'01, yzx = 0b11'00'10'01,
  yzy = 0b11'01'10'01, yzz = 0b11'10'10'01, zxx = 0b11'00'00'10, zxy = 0b11'01'00'10,
  zxz = 0b11'10'00'10, zyx = 0b11'00'01'10, zyy = 0b11'01'01'10, zyz = 0b11'10'01'10,
  zzx = 0b11'00'10'10, zzy = 0b11'01'10'10, zzz = 0b11'10'10'10,

#if USE_RGBA_COMPONENTS
  /* rgba components */
  rrr = 0b11'00'00'00, rrg = 0b11'01'00'00, rrb = 0b11'10'00'00, rgr = 0b11'00'01'00,
  rgg = 0b11'01'01'00, rgb = 0b11'10'01'00, rbr = 0b11'00'10'00, rbg = 0b11'01'10'00,
  rbb = 0b11'10'10'00, grr = 0b11'00'00'01, grg = 0b11'01'00'01, grb = 0b11'10'00'01,
  ggr = 0b11'00'01'01, ggg = 0b11'01'01'01, ggb = 0b11'10'01'01, gbr = 0b11'00'10'01,
  gbg = 0b11'01'10'01, gbb = 0b11'10'10'01, brr = 0b11'00'00'10, brg = 0b11'01'00'10,
  brb = 0b11'10'00'10, bgr = 0b11'00'01'10, bgg = 0b11'01'01'10, bgb = 0b11'10'01'10,
  bbr = 0b11'00'10'10, bbg = 0b11'01'10'10, bbb = 0b11'10'10'10,
#endif
};

// compile-time arguments to shuffle 4D vectors
enum class Shuf4 : uint8_t
{
  /* xyzw components */
  xxxx = 0b00'00'00'00, xxxy = 0b01'00'00'00, xxxz = 0b10'00'00'00, xxxw = 0b11'00'00'00, xxyx = 0b00'01'00'00, xxyy = 0b01'01'00'00, xxyz = 0b10'01'00'00, xxyw = 0b11'01'00'00,
  xxzx = 0b00'10'00'00, xxzy = 0b01'10'00'00, xxzz = 0b10'10'00'00, xxzw = 0b11'10'00'00, xxwx = 0b00'11'00'00, xxwy = 0b01'11'00'00, xxwz = 0b10'11'00'00, xxww = 0b11'11'00'00,
  xyxx = 0b00'00'01'00, xyxy = 0b01'00'01'00, xyxz = 0b10'00'01'00, xyxw = 0b11'00'01'00, xyyx = 0b00'01'01'00, xyyy = 0b01'01'01'00, xyyz = 0b10'01'01'00, xyyw = 0b11'01'01'00,
  xyzx = 0b00'10'01'00, xyzy = 0b01'10'01'00, xyzz = 0b10'10'01'00, xyzw = 0b11'10'01'00, xywx = 0b00'11'01'00, xywy = 0b01'11'01'00, xywz = 0b10'11'01'00, xyww = 0b11'11'01'00,
  xzxx = 0b00'00'10'00, xzxy = 0b01'00'10'00, xzxz = 0b10'00'10'00, xzxw = 0b11'00'10'00, xzyx = 0b00'01'10'00, xzyy = 0b01'01'10'00, xzyz = 0b10'01'10'00, xzyw = 0b11'01'10'00,
  xzzx = 0b00'10'10'00, xzzy = 0b01'10'10'00, xzzz = 0b10'10'10'00, xzzw = 0b11'10'10'00, xzwx = 0b00'11'10'00, xzwy = 0b01'11'10'00, xzwz = 0b10'11'10'00, xzww = 0b11'11'10'00,
  xwxx = 0b00'00'11'00, xwxy = 0b01'00'11'00, xwxz = 0b10'00'11'00, xwxw = 0b11'00'11'00, xwyx = 0b00'01'11'00, xwyy = 0b01'01'11'00, xwyz = 0b10'01'11'00, xwyw = 0b11'01'11'00,
  xwzx = 0b00'10'11'00, xwzy = 0b01'10'11'00, xwzz = 0b10'10'11'00, xwzw = 0b11'10'11'00, xwwx = 0b00'11'11'00, xwwy = 0b01'11'11'00, xwwz = 0b10'11'11'00, xwww = 0b11'11'11'00,
  yxxx = 0b00'00'00'01, yxxy = 0b01'00'00'01, yxxz = 0b10'00'00'01, yxxw = 0b11'00'00'01, yxyx = 0b00'01'00'01, yxyy = 0b01'01'00'01, yxyz = 0b10'01'00'01, yxyw = 0b11'01'00'01,
  yxzx = 0b00'10'00'01, yxzy = 0b01'10'00'01, yxzz = 0b10'10'00'01, yxzw = 0b11'10'00'01, yxwx = 0b00'11'00'01, yxwy = 0b01'11'00'01, yxwz = 0b10'11'00'01, yxww = 0b11'11'00'01,
  yyxx = 0b00'00'01'01, yyxy = 0b01'00'01'01, yyxz = 0b10'00'01'01, yyxw = 0b11'00'01'01, yyyx = 0b00'01'01'01, yyyy = 0b01'01'01'01, yyyz = 0b10'01'01'01, yyyw = 0b11'01'01'01,
  yyzx = 0b00'10'01'01, yyzy = 0b01'10'01'01, yyzz = 0b10'10'01'01, yyzw = 0b11'10'01'01, yywx = 0b00'11'01'01, yywy = 0b01'11'01'01, yywz = 0b10'11'01'01, yyww = 0b11'11'01'01,
  yzxx = 0b00'00'10'01, yzxy = 0b01'00'10'01, yzxz = 0b10'00'10'01, yzxw = 0b11'00'10'01, yzyx = 0b00'01'10'01, yzyy = 0b01'01'10'01, yzyz = 0b10'01'10'01, yzyw = 0b11'01'10'01,
  yzzx = 0b00'10'10'01, yzzy = 0b01'10'10'01, yzzz = 0b10'10'10'01, yzzw = 0b11'10'10'01, yzwx = 0b00'11'10'01, yzwy = 0b01'11'10'01, yzwz = 0b10'11'10'01, yzww = 0b11'11'10'01,
  ywxx = 0b00'00'11'01, ywxy = 0b01'00'11'01, ywxz = 0b10'00'11'01, ywxw = 0b11'00'11'01, ywyx = 0b00'01'11'01, ywyy = 0b01'01'11'01, ywyz = 0b10'01'11'01, ywyw = 0b11'01'11'01,
  ywzx = 0b00'10'11'01, ywzy = 0b01'10'11'01, ywzz = 0b10'10'11'01, ywzw = 0b11'10'11'01, ywwx = 0b00'11'11'01, ywwy = 0b01'11'11'01, ywwz = 0b10'11'11'01, ywww = 0b11'11'11'01,
  zxxx = 0b00'00'00'10, zxxy = 0b01'00'00'10, zxxz = 0b10'00'00'10, zxxw = 0b11'00'00'10, zxyx = 0b00'01'00'10, zxyy = 0b01'01'00'10, zxyz = 0b10'01'00'10, zxyw = 0b11'01'00'10,
  zxzx = 0b00'10'00'10, zxzy = 0b01'10'00'10, zxzz = 0b10'10'00'10, zxzw = 0b11'10'00'10, zxwx = 0b00'11'00'10, zxwy = 0b01'11'00'10, zxwz = 0b10'11'00'10, zxww = 0b11'11'00'10,
  zyxx = 0b00'00'01'10, zyxy = 0b01'00'01'10, zyxz = 0b10'00'01'10, zyxw = 0b11'00'01'10, zyyx = 0b00'01'01'10, zyyy = 0b01'01'01'10, zyyz = 0b10'01'01'10, zyyw = 0b11'01'01'10,
  zyzx = 0b00'10'01'10, zyzy = 0b01'10'01'10, zyzz = 0b10'10'01'10, zyzw = 0b11'10'01'10, zywx = 0b00'11'01'10, zywy = 0b01'11'01'10, zywz = 0b10'11'01'10, zyww = 0b11'11'01'10,
  zzxx = 0b00'00'10'10, zzxy = 0b01'00'10'10, zzxz = 0b10'00'10'10, zzxw = 0b11'00'10'10, zzyx = 0b00'01'10'10, zzyy = 0b01'01'10'10, zzyz = 0b10'01'10'10, zzyw = 0b11'01'10'10,
  zzzx = 0b00'10'10'10, zzzy = 0b01'10'10'10, zzzz = 0b10'10'10'10, zzzw = 0b11'10'10'10, zzwx = 0b00'11'10'10, zzwy = 0b01'11'10'10, zzwz = 0b10'11'10'10, zzww = 0b11'11'10'10,
  zwxx = 0b00'00'11'10, zwxy = 0b01'00'11'10, zwxz = 0b10'00'11'10, zwxw = 0b11'00'11'10, zwyx = 0b00'01'11'10, zwyy = 0b01'01'11'10, zwyz = 0b10'01'11'10, zwyw = 0b11'01'11'10,
  zwzx = 0b00'10'11'10, zwzy = 0b01'10'11'10, zwzz = 0b10'10'11'10, zwzw = 0b11'10'11'10, zwwx = 0b00'11'11'10, zwwy = 0b01'11'11'10, zwwz = 0b10'11'11'10, zwww = 0b11'11'11'10,
  wxxx = 0b00'00'00'11, wxxy = 0b01'00'00'11, wxxz = 0b10'00'00'11, wxxw = 0b11'00'00'11, wxyx = 0b00'01'00'11, wxyy = 0b01'01'00'11, wxyz = 0b10'01'00'11, wxyw = 0b11'01'00'11,
  wxzx = 0b00'10'00'11, wxzy = 0b01'10'00'11, wxzz = 0b10'10'00'11, wxzw = 0b11'10'00'11, wxwx = 0b00'11'00'11, wxwy = 0b01'11'00'11, wxwz = 0b10'11'00'11, wxww = 0b11'11'00'11,
  wyxx = 0b00'00'01'11, wyxy = 0b01'00'01'11, wyxz = 0b10'00'01'11, wyxw = 0b11'00'01'11, wyyx = 0b00'01'01'11, wyyy = 0b01'01'01'11, wyyz = 0b10'01'01'11, wyyw = 0b11'01'01'11,
  wyzx = 0b00'10'01'11, wyzy = 0b01'10'01'11, wyzz = 0b10'10'01'11, wyzw = 0b11'10'01'11, wywx = 0b00'11'01'11, wywy = 0b01'11'01'11, wywz = 0b10'11'01'11, wyww = 0b11'11'01'11,
  wzxx = 0b00'00'10'11, wzxy = 0b01'00'10'11, wzxz = 0b10'00'10'11, wzxw = 0b11'00'10'11, wzyx = 0b00'01'10'11, wzyy = 0b01'01'10'11, wzyz = 0b10'01'10'11, wzyw = 0b11'01'10'11,
  wzzx = 0b00'10'10'11, wzzy = 0b01'10'10'11, wzzz = 0b10'10'10'11, wzzw = 0b11'10'10'11, wzwx = 0b00'11'10'11, wzwy = 0b01'11'10'11, wzwz = 0b10'11'10'11, wzww = 0b11'11'10'11,
  wwxx = 0b00'00'11'11, wwxy = 0b01'00'11'11, wwxz = 0b10'00'11'11, wwxw = 0b11'00'11'11, wwyx = 0b00'01'11'11, wwyy = 0b01'01'11'11, wwyz = 0b10'01'11'11, wwyw = 0b11'01'11'11,
  wwzx = 0b00'10'11'11, wwzy = 0b01'10'11'11, wwzz = 0b10'10'11'11, wwzw = 0b11'10'11'11, wwwx = 0b00'11'11'11, wwwy = 0b01'11'11'11, wwwz = 0b10'11'11'11, wwww = 0b11'11'11'11,

#if USE_RGBA_COMPONENTS
  /* rgba components */
  rrrr = 0b00'00'00'00, rrrg = 0b01'00'00'00, rrrb = 0b10'00'00'00, rrra = 0b11'00'00'00, rrgr = 0b00'01'00'00, rrgg = 0b01'01'00'00, rrgb = 0b10'01'00'00, rrga = 0b11'01'00'00,
  rrbr = 0b00'10'00'00, rrbg = 0b01'10'00'00, rrbb = 0b10'10'00'00, rrba = 0b11'10'00'00, rrar = 0b00'11'00'00, rrag = 0b01'11'00'00, rrab = 0b10'11'00'00, rraa = 0b11'11'00'00,
  rgrr = 0b00'00'01'00, rgrg = 0b01'00'01'00, rgrb = 0b10'00'01'00, rgra = 0b11'00'01'00, rggr = 0b00'01'01'00, rggg = 0b01'01'01'00, rggb = 0b10'01'01'00, rgga = 0b11'01'01'00,
  rgbr = 0b00'10'01'00, rgbg = 0b01'10'01'00, rgbb = 0b10'10'01'00, rgba = 0b11'10'01'00, rgar = 0b00'11'01'00, rgag = 0b01'11'01'00, rgab = 0b10'11'01'00, rgaa = 0b11'11'01'00,
  rbrr = 0b00'00'10'00, rbrg = 0b01'00'10'00, rbrb = 0b10'00'10'00, rbra = 0b11'00'10'00, rbgr = 0b00'01'10'00, rbgg = 0b01'01'10'00, rbgb = 0b10'01'10'00, rbga = 0b11'01'10'00,
  rbbr = 0b00'10'10'00, rbbg = 0b01'10'10'00, rbbb = 0b10'10'10'00, rbba = 0b11'10'10'00, rbar = 0b00'11'10'00, rbag = 0b01'11'10'00, rbab = 0b10'11'10'00, rbaa = 0b11'11'10'00,
  rarr = 0b00'00'11'00, rarg = 0b01'00'11'00, rarb = 0b10'00'11'00, rara = 0b11'00'11'00, ragr = 0b00'01'11'00, ragg = 0b01'01'11'00, ragb = 0b10'01'11'00, raga = 0b11'01'11'00,
  rabr = 0b00'10'11'00, rabg = 0b01'10'11'00, rabb = 0b10'10'11'00, raba = 0b11'10'11'00, raar = 0b00'11'11'00, raag = 0b01'11'11'00, raab = 0b10'11'11'00, raaa = 0b11'11'11'00,
  grrr = 0b00'00'00'01, grrg = 0b01'00'00'01, grrb = 0b10'00'00'01, grra = 0b11'00'00'01, grgr = 0b00'01'00'01, grgg = 0b01'01'00'01, grgb = 0b10'01'00'01, grga = 0b11'01'00'01,
  grbr = 0b00'10'00'01, grbg = 0b01'10'00'01, grbb = 0b10'10'00'01, grba = 0b11'10'00'01, grar = 0b00'11'00'01, grag = 0b01'11'00'01, grab = 0b10'11'00'01, graa = 0b11'11'00'01,
  ggrr = 0b00'00'01'01, ggrg = 0b01'00'01'01, ggrb = 0b10'00'01'01, ggra = 0b11'00'01'01, gggr = 0b00'01'01'01, gggg = 0b01'01'01'01, gggb = 0b10'01'01'01, ggga = 0b11'01'01'01,
  ggbr = 0b00'10'01'01, ggbg = 0b01'10'01'01, ggbb = 0b10'10'01'01, ggba = 0b11'10'01'01, ggar = 0b00'11'01'01, ggag = 0b01'11'01'01, ggab = 0b10'11'01'01, ggaa = 0b11'11'01'01,
  gbrr = 0b00'00'10'01, gbrg = 0b01'00'10'01, gbrb = 0b10'00'10'01, gbra = 0b11'00'10'01, gbgr = 0b00'01'10'01, gbgg = 0b01'01'10'01, gbgb = 0b10'01'10'01, gbga = 0b11'01'10'01,
  gbbr = 0b00'10'10'01, gbbg = 0b01'10'10'01, gbbb = 0b10'10'10'01, gbba = 0b11'10'10'01, gbar = 0b00'11'10'01, gbag = 0b01'11'10'01, gbab = 0b10'11'10'01, gbaa = 0b11'11'10'01,
  garr = 0b00'00'11'01, garg = 0b01'00'11'01, garb = 0b10'00'11'01, gara = 0b11'00'11'01, gagr = 0b00'01'11'01, gagg = 0b01'01'11'01, gagb = 0b10'01'11'01, gaga = 0b11'01'11'01,
  gabr = 0b00'10'11'01, gabg = 0b01'10'11'01, gabb = 0b10'10'11'01, gaba = 0b11'10'11'01, gaar = 0b00'11'11'01, gaag = 0b01'11'11'01, gaab = 0b10'11'11'01, gaaa = 0b11'11'11'01,
  brrr = 0b00'00'00'10, brrg = 0b01'00'00'10, brrb = 0b10'00'00'10, brra = 0b11'00'00'10, brgr = 0b00'01'00'10, brgg = 0b01'01'00'10, brgb = 0b10'01'00'10, brga = 0b11'01'00'10,
  brbr = 0b00'10'00'10, brbg = 0b01'10'00'10, brbb = 0b10'10'00'10, brba = 0b11'10'00'10, brar = 0b00'11'00'10, brag = 0b01'11'00'10, brab = 0b10'11'00'10, braa = 0b11'11'00'10,
  bgrr = 0b00'00'01'10, bgrg = 0b01'00'01'10, bgrb = 0b10'00'01'10, bgra = 0b11'00'01'10, bggr = 0b00'01'01'10, bggg = 0b01'01'01'10, bggb = 0b10'01'01'10, bgga = 0b11'01'01'10,
  bgbr = 0b00'10'01'10, bgbg = 0b01'10'01'10, bgbb = 0b10'10'01'10, bgba = 0b11'10'01'10, bgar = 0b00'11'01'10, bgag = 0b01'11'01'10, bgab = 0b10'11'01'10, bgaa = 0b11'11'01'10,
  bbrr = 0b00'00'10'10, bbrg = 0b01'00'10'10, bbrb = 0b10'00'10'10, bbra = 0b11'00'10'10, bbgr = 0b00'01'10'10, bbgg = 0b01'01'10'10, bbgb = 0b10'01'10'10, bbga = 0b11'01'10'10,
  bbbr = 0b00'10'10'10, bbbg = 0b01'10'10'10, bbbb = 0b10'10'10'10, bbba = 0b11'10'10'10, bbar = 0b00'11'10'10, bbag = 0b01'11'10'10, bbab = 0b10'11'10'10, bbaa = 0b11'11'10'10,
  barr = 0b00'00'11'10, barg = 0b01'00'11'10, barb = 0b10'00'11'10, bara = 0b11'00'11'10, bagr = 0b00'01'11'10, bagg = 0b01'01'11'10, bagb = 0b10'01'11'10, baga = 0b11'01'11'10,
  babr = 0b00'10'11'10, babg = 0b01'10'11'10, babb = 0b10'10'11'10, baba = 0b11'10'11'10, baar = 0b00'11'11'10, baag = 0b01'11'11'10, baab = 0b10'11'11'10, baaa = 0b11'11'11'10,
  arrr = 0b00'00'00'11, arrg = 0b01'00'00'11, arrb = 0b10'00'00'11, arra = 0b11'00'00'11, argr = 0b00'01'00'11, argg = 0b01'01'00'11, argb = 0b10'01'00'11, arga = 0b11'01'00'11,
  arbr = 0b00'10'00'11, arbg = 0b01'10'00'11, arbb = 0b10'10'00'11, arba = 0b11'10'00'11, arar = 0b00'11'00'11, arag = 0b01'11'00'11, arab = 0b10'11'00'11, araa = 0b11'11'00'11,
  agrr = 0b00'00'01'11, agrg = 0b01'00'01'11, agrb = 0b10'00'01'11, agra = 0b11'00'01'11, aggr = 0b00'01'01'11, aggg = 0b01'01'01'11, aggb = 0b10'01'01'11, agga = 0b11'01'01'11,
  agbr = 0b00'10'01'11, agbg = 0b01'10'01'11, agbb = 0b10'10'01'11, agba = 0b11'10'01'11, agar = 0b00'11'01'11, agag = 0b01'11'01'11, agab = 0b10'11'01'11, agaa = 0b11'11'01'11,
  abrr = 0b00'00'10'11, abrg = 0b01'00'10'11, abrb = 0b10'00'10'11, abra = 0b11'00'10'11, abgr = 0b00'01'10'11, abgg = 0b01'01'10'11, abgb = 0b10'01'10'11, abga = 0b11'01'10'11,
  abbr = 0b00'10'10'11, abbg = 0b01'10'10'11, abbb = 0b10'10'10'11, abba = 0b11'10'10'11, abar = 0b00'11'10'11, abag = 0b01'11'10'11, abab = 0b10'11'10'11, abaa = 0b11'11'10'11,
  aarr = 0b00'00'11'11, aarg = 0b01'00'11'11, aarb = 0b10'00'11'11, aara = 0b11'00'11'11, aagr = 0b00'01'11'11, aagg = 0b01'01'11'11, aagb = 0b10'01'11'11, aaga = 0b11'01'11'11,
  aabr = 0b00'10'11'11, aabg = 0b01'10'11'11, aabb = 0b10'10'11'11, aaba = 0b11'10'11'11, aaar = 0b00'11'11'11, aaag = 0b01'11'11'11, aaab = 0b10'11'11'11, aaaa = 0b11'11'11'11,
#endif
};


/* ############################################################################################## */
/* ################################# platform-specific details ################################## */
/* ############################################################################################## */

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

/* ############################################################################################## */
/* ################################### preprocessor variables ################################### */
/* ############################################################################################## */

/* Exactly one of the following must be defined (it's the user's responsibility to set compilation
 * flags accordingly):
 *
 * FSVF_FLOAT_FINITE_MATH   (finite arithmetic, no signed zeroes)
 * FSVF_FLOAT_DEFAULT_MATH  (INFs, NaNs, signed zeroes, no denormals)
 * FSVF_FLOAT_DENORMAL_MATH (denormals allowed; EXPERIMENTAL)
 *
 * NOTE that regardless of these settings, we always assume:
 * - no trapping of floating point exceptions (-fno-trapping-math, /fp:except-)
 * - no usage of errno (-fno-math-errno)
 * - same rounding mode throughout the client's application runtime (although no particular choice
 *   of rounding mode is required)
 *
 * NOTE the precision of the library's algorithms is currently undocumented and might change across
 * releases (and even across commits, as long as the major release verrsion is still 0);
 * in particular, some optimization techniques that compilers would generate only when specific
 * flags (-fassociative-math, -funsafe-math, ...) are set are, sometimes but not always,
 * used; other such optimizations;
 * in the future some of these optimizations might be made less aggressive when
 * FSVF_FLOAT_DENORMAL_MATH is defined, but currently there are no such guarantees.
 * Some of the optimization that compilers would generate under the -freciprocal-math are controlled
 * through the definition (or lack thereof) of the preprocessor variables
 * FSVF_NO_RECIPROCAL_MATH and FSFV_RCPPS (see documentation below) */

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
#if !defined(FSVF_FLOAT_FINITE_MATH)   \
 && !defined(FSVF_FLOAT_DEFAULT_MATH)  \
 && !defined(FSVF_FLOAT_DENORMAL_MATH)
#define FSVF_FLOAT_DEFAULT_MATH
#endif

/* Reciprocal math optimizations:
 * by default, divisions are approximated (in code) by multiplications with the reciprocal of the
 * divisor, whenever it is convenient to do so; users needing extra precision can set the flag
 * FSVF_NO_RECIPROCAL_MATH, which ensures that actual division intrinsics are used in code to
 * perform vectorized divisions of floats (this behavior might still be overwritten by compilers
 * when using certain compilation flags)
 *
 * users willing to sacrifice precision for speed can also set the FSVF_RCPPS preprocessor variable
 * (provided that FSVF_FLOAT_DENORMAL_MATH is NOT set), which replaces the usage of floating point
 * division intrinsics with multiplications with RCPPS (https://www.felixcloutier.com/x86/rcpps)
 * of the divisor (undefined by default)
 *
 * another possible speed/accuracy tradeoff can be made through FSFV_RSQRT_NORMALIZE; defining it
 * will use RSQRT in order to compute length and normalization of vectors (disabled by default, but
 * implied by FSFV_RCPPS) */

// #define FSVF_NO_RECIPROCAL_MATH                  /* undefined by default */
// #define FSVF_RCPPS                               /* undefined by default */
// #define FSVF_RSQRT_NORMALIZE                     /* undefined by default */

/* Setting FTZ and DAZ automatically at application startup:
 * unless either FSFV_DENORMAL_MATH or FSVF_NO_AUTOMATIC_FTZ_DAZ are defined, the library will try
 * to set FTZ and DAZ at program startup, through the constructor of a static object defined ad-hoc
 * for this purpose */

// #define FSVF_NO_AUTOMATIC_FTZ_DAZ                /* undefined by default */

/* ######################################### Internals ########################################## */

namespace Internals
{
/* Try to automatically set FTZ and DAZ if FSVF_FLOAT_DENORMAL_MATH is not set */
#if !defined(FSVF_NO_AUTOMATIC_FTZ_DAZ) && !defined(FSVF_FLOAT_DENORMAL_MATH)
struct FlagSetter
{
  FlagSetter() { _mm_setcsr(_mm_getcsr() | 0x8040); /* FTZ: 0x8000, DAZ: 0x0040 */ }
};

static inline FlagSetter s_FlagSetter{};
#endif

/* missing from C++20 */

template<typename E>
  requires  std::is_enum_v<E>
constexpr FSVF_FORCEINLINE auto to_underlying(E e)
{
  return static_cast<std::underlying_type_t<E>>(e);
}

template <typename T>
struct always_false
{
  constexpr static bool value = std::negation_v<std::is_same<T, T>>;
};

template <typename T>
constexpr bool always_false_v = always_false<T>::value;

struct Empty {};
} // namespace Internals

// bitcast utilities for SSE types (compiled to no-ops)
FSVF_FORCEINLINE __m128   bits_to_float(__m128i bits) { return _mm_castsi128_ps(bits); }
FSVF_FORCEINLINE __m128i  float_to_bits(__m128  bits) { return _mm_castps_si128(bits); }

// bitcast utilities for 32-bit values, constrained to prevent implicit type conversion
constexpr FSVF_FORCEINLINE
float bits_to_float(std::same_as<uint32_t> auto bits)
{
  return std::bit_cast<float, uint32_t>(bits);
}
constexpr FSVF_FORCEINLINE
uint32_t float_to_bits(std::same_as<float> auto f)
{
  return std::bit_cast<uint32_t, float>(f);
}

/* ############################################################################################## */
/* ###################################### FSVF Interfaces ####################################### */
/* ############################################################################################## */

// Forward declarations
class Vec4;
class Mat4;
class LVec3;
class LMat3;

/* ####################################### `FastVector`s ######################################## */

// any type that inherits publicly from Vec4 or LVec3 qualifies as a FastVector
template<typename T>
concept FastVector = requires (T v)
{
  requires std::derived_from<T, Vec4> || std::derived_from<T, LVec3> == true;
};

namespace Internals
{
  // templated magic to define the return type of operation on mixed-dimension vectors
  // see `LargestDimensionType` and `SmallestDimensionType`

  template<typename T1, typename T2>
  concept V3AncestorAll = std::derived_from<T1, LVec3> && std::derived_from<T2, LVec3>;

  template<typename T1, typename T2>
  concept V3AncestorAny = std::derived_from<T1, LVec3> || std::derived_from<T2, LVec3>;

  template<FastVector T1, FastVector T2, bool is3D = V3AncestorAll<T1, T2>> struct LargerDimType;
  template<FastVector T1, FastVector T2, bool is3D = V3AncestorAny<T1, T2>> struct SmallerDimType;

  template<FastVector T1, FastVector T2> struct LargerDimType<T1, T2, false>{using type = Vec4;};
  template<FastVector T1, FastVector T2> struct LargerDimType<T1, T2, true> {using type = LVec3;};
  template<FastVector T>                 struct LargerDimType< T,  T, true> {using type = T;};
  template<FastVector T>                 struct LargerDimType< T,  T, false>{using type = T;};

  template<FastVector T1, FastVector T2> struct SmallerDimType<T1, T2, false>{using type = Vec4;};
  template<FastVector T1, FastVector T2> struct SmallerDimType<T1, T2, true> {using type = LVec3;};
  template<FastVector T>                 struct SmallerDimType< T,  T, true> {using type = T;};
  template<FastVector T>                 struct SmallerDimType< T,  T, false>{using type = T;};
} // namespace Internals

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
concept SameDimension = FastVector<T1> && FastVector<T2> &&
  (Internals::V3AncestorAll<T1, T2> || !Internals::V3AncestorAny<T1, T2>);


// returns the sum of all its argument's components
template<FastVector T> float   FSVF_VECCALL sum(T v);

// returns the product of all its argument's components
template<FastVector T> float   FSVF_VECCALL prod(T v);

// returns the unit vector of the argument
template<FastVector T> T       FSVF_VECCALL normalize(T v);

// returns the (Euclidean) length of the argument
template<FastVector T> float   FSVF_VECCALL len(T v);

// returns the square of the (Euclidean) length of the argument
// effectively equivalent to dot(v,v)
template<FastVector T> float   FSVF_VECCALL len_squared(T v);

template<FastVector T> uint8_t FSVF_VECCALL max_component_index(T v);
template<FastVector T> uint8_t FSVF_VECCALL min_component_index(T v);

// returns a vector whose components are all the largest value in the argument
template<FastVector T> Vec4    FSVF_VECCALL max_component_value(T v);

// returns a vector whose components are all the smallest value in the argument
template<FastVector T> Vec4    FSVF_VECCALL min_component_value(T v);

// returns the largest component of the argument
template<FastVector T> float   FSVF_VECCALL max_component_value_f32(T v);

// returns the smallest component of the argument
template<FastVector T> float   FSVF_VECCALL min_component_value_f32(T v);

// sort components (smaller to larger)
template<FastVector T> T       FSVF_VECCALL sort(T v);

// reverse-sort components (larger to smaller)
template<FastVector T> T       FSVF_VECCALL reverse_sort(T v);

// component-wise operations
template<FastVector T> T       FSVF_VECCALL floor(T v);
template<FastVector T> T       FSVF_VECCALL ceil(T v);
template<FastVector T> T       FSVF_VECCALL abs(T v);
template<FastVector T> T       FSVF_VECCALL sqrt(T v);

// component-wise reciprocal of the square root; faster but less precise than doing `1.f/sqrt(v)`
template<FastVector T> T       FSVF_VECCALL rsqrt(T v);

// returns a vector with the component-wise maximum of the two arguments
template<FastVector T1, FastVector T2>
auto FSVF_VECCALL merge_max(T1 v, T2 w) -> LargestDimensionType<T1, T2>;

// returns a vector with the component-wise minimum of the two arguments
template<FastVector T1, FastVector T2>
auto FSVF_VECCALL merge_min(T1 v, T2 w) -> LargestDimensionType<T1, T2>;

template<FastVector T1, FastVector T2>
float FSVF_VECCALL dot(T1 v, T2 w);

template<FastVector T1, FastVector T2>
LVec3 FSVF_VECCALL cross(T1 v, T2 w);

// returns a vector whose i-th component is the horizontal sum of the i-th argument
// logically equivalent to (but faster than) Vec4(sum(v1), sum(v2), sum(v3), sum(v4))
template<FastVector T1, FastVector T2, FastVector T3, FastVector T4>
Vec4 FSVF_VECCALL parallel_sum(T1 v1, T2 v2, T3 v3, T4 v4);

// returns a vector whose i-th component is the horizontal product of the i-th argument
// logically equivalent to (but faster than) Vec4(prod(v1), prod(v2), prod(v3), prod(v4))
template<FastVector T1, FastVector T2, FastVector T3, FastVector T4>
Vec4 FSVF_VECCALL parallel_prod(T1 v1, T2 v2, T3 v3, T4 v4);

// returns a vector obtained by blending the two arguments according to `BinaryMask`
// 0 -> keep corresponding first's argument component
// 1 -> keep corresponding second's argument component
// e.g. blend_with_mask<0b1011>(a, b) == {b.x, a.y, b.z, b.w}
// (note for the experts: unlike the corresponding intrinsic, this function processes the mask going
// most -> least significant bit)
template<uint8_t BinaryMask, FastVector T1, FastVector T2>
auto FSVF_VECCALL blend_with_mask(T1 v, T2 w) -> LargestDimensionType<T1, T2>;

// component-wise clamp
template<FastVector T1, FastVector T2> T1
FSVF_VECCALL clamp (T1 v, T2 min, T2 max);

// comparisons returning 4 bitmasks, 0xFFFF'FFFF if the condition holds, 0x0 otherwise
template<FastVector T> Vec4 FSVF_VECCALL operator<=(T v, T w);
template<FastVector T> Vec4 FSVF_VECCALL operator>=(T v, T w);
template<FastVector T> Vec4 FSVF_VECCALL operator< (T v, T w);
template<FastVector T> Vec4 FSVF_VECCALL operator> (T v, T w);

// comparison returning a boolean; use BitMasks::cmp_equal_f32() for the 4 bitmasks equivalent
template<FastVector T1, FastVector T2>
bool FSVF_VECCALL operator==(T1 v, T2 w);

// comparison returning a boolean; use BitMasks::cmp_not_equal_f32() for the 4 bitmasks equivalent
template<FastVector T1, FastVector T2>
bool FSVF_VECCALL operator!=(T1 v, T2 w);

// returns true if all [VectorDimensions] components of the argument are 0xFFFF'FFFF
// valid values for `VectorDimensions` are 3 and 4
template<uint8_t VectorDimensions>
bool FSVF_VECCALL all(Vec4 bitmask);

// returns true if any of the first [VectorDimensions] components of the argument are 0xFFFF'FFFF
// valid values for `VectorDimensions` are 3 and 4
template<uint8_t VectorDimensions>
bool FSVF_VECCALL any(Vec4 bitmask);

// returns true if none of the first [VectorDimensions] components of the argument are 0xFFFF'FFFF
template<uint8_t VectorDimensions>
bool FSVF_VECCALL none(Vec4 bitmask);

// shuffles the vector according to the shuffle passed as template argument;
// the shuffle argument is templated because `_mm_shuffle_ps()` expects a compile-time constant
// (each shuffle instruction has a different opcode)
// shuffling a 3-dimensional vector with a Shuf4 will return a 4D vector (the 4th component of the
// argument being == 0.f by type contract)
template<Shuf4 SHUFFLE, FastVector T>
auto FSVF_VECCALL shuffle(T v) -> LargestDimensionType<T, Vec4>;

// shuffles the vector according to the shuffle passed as template argument;
// the shuffle argument is templated because `_mm_shuffle_ps()` expects a compile-time constant
// (each shuffle instruction has a different opcode)
// shuffling 4-dimensional vector with a Shuf3 will keep the last component in place
template<Shuf3 SHUFFLE, FastVector T>
T FSVF_VECCALL shuffle(T v);

namespace BitMasks
{
  template<FastVector T1, FastVector T2>
  Vec4 FSVF_VECCALL cmp_equal_f32(T1 v, T2 w);

  template<FastVector T1, FastVector T2>
  auto FSVF_VECCALL cmp_not_equal_f32(T1 v, T2 w) -> LargestDimensionType<T1, T2>;

  template<FastVector T1, FastVector T2>
  Vec4 FSVF_VECCALL cmp_equal_bitwise(T1 v, T2 w);
}

/* ######################################### Operators ########################################## */

template<FastVector T> T FSVF_VECCALL operator*(float x, T v);
template<FastVector T> T FSVF_VECCALL operator/(float x, T v);
template<FastVector T> T FSVF_VECCALL operator*(T v, float x);
template<FastVector T> T FSVF_VECCALL operator/(T v, float x);

template<FastVector T> T FSVF_VECCALL operator-(T v);
template<FastVector T> T FSVF_VECCALL operator~(T v);

template<FastVector T1, FastVector T2> auto FSVF_VECCALL operator^(T1 v, T2 w) -> LargestDimensionType<T1, T2>;
template<FastVector T1, FastVector T2> auto FSVF_VECCALL operator|(T1 v, T2 w) -> LargestDimensionType<T1, T2>;
template<FastVector T1, FastVector T2> auto FSVF_VECCALL operator&(T1 v, T2 w) -> SmallestDimensionType<T1, T2>;

template<FastVector T1, FastVector T2> Vec4 FSVF_VECCALL and_not(T1 v, T2 w);

template<FastVector T1, FastVector T2> auto FSVF_VECCALL operator+(T1 v, T2 w) -> LargestDimensionType<T1, T2>;
template<FastVector T1, FastVector T2> auto FSVF_VECCALL operator-(T1 v, T2 w) -> LargestDimensionType<T1, T2>;
template<FastVector T1, FastVector T2> auto FSVF_VECCALL operator/(T1 v, T2 w) -> LargestDimensionType<T1, T2>;
template<FastVector T1, FastVector T2> auto FSVF_VECCALL operator*(T1 v, T2 w) -> SmallestDimensionType<T1, T2>;

template<FastVector T1, FastVector T2> T1& FSVF_VECCALL operator-=(T1& v, T2 w);
template<FastVector T1, FastVector T2> T1& FSVF_VECCALL operator+=(T1& v, T2 w);
template<FastVector T1, FastVector T2> T1& FSVF_VECCALL operator/=(T1& v, T2 w);
template<FastVector T1, FastVector T2> T1& FSVF_VECCALL operator*=(T1& v, T2 w);

template<FastVector T> T& FSVF_VECCALL operator/=(T& v, float x);
template<FastVector T> T& FSVF_VECCALL operator*=(T& v, float x);

template<FastVector T1, FastVector T2>
  requires (SameDimension<T1, T2>) T1& FSVF_VECCALL operator&=(T1& v, T2 w);

template<FastVector T1, FastVector T2>
  requires (SameDimension<T1, T2>) T1& FSVF_VECCALL operator|=(T1& v, T2 w);

template<FastVector T1, FastVector T2>
  requires (SameDimension<T1, T2>) T1& FSVF_VECCALL operator^=(T1& v, T2 w);


/* ############################################################################################## */
/* #################################### Class Declarations ###################################### */
/* ############################################################################################## */

using stdVec3 = std::array<float, 3>;
using stdVec4 = std::array<float, 4>;

/* ######################################## Vector types: ####################################### */

/* Vec4  = 4 packed 32-bit floats
 * LVec3 = "Large" Vec3, 4 packed 32-bit floats, the last component is guaranteed to be +0.0f, and
 *         access to it is hidden from the user;
 *         IMPORTANT: the interface is designed to make it nearly impossible to break this invariant
 *         accidentally, but still gives the user the possibility to do so willingly for local,
 *         in-scope, optimizations;
 *         if you decide to use this power, be extra careful with your responsibility:
 *                              !!! don't break the type's invariant !!!
 *
 * when storing significantly-sized buffers in memory, the user should measure and evaluate on a
 * case-by-case basis the tradeoffs of taking 25% extra space to store `LVec3`s directly vs. storing
 * contiguous floats grouped by 3 and then loading them into `LVec3`s according to some strategy. */

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
  Vec4() noexcept : m_sse(_mm_undefined_ps()) {}

  Vec4(const Vec4& m) noexcept = default;

  // constructs a Vec4 with the same value in all components
  explicit Vec4(float x) : m_sse{_mm_set_ps1(x)} {}

  // constructs a Vec4 with the specified components;
  // prefer using another constructor if the data is already available in a contiguous buffer
  Vec4(float x, float y, float z, float w) : m_sse{_mm_set_ps(w, z, y, x)} {}

  // loads 4 floats from a float buffer into a Vec4 object
  // this constructor deals properly with alignment and it's
  // the only way a Vec4 should be created to access buffers
  explicit Vec4(const float* p) : m_sse{_mm_loadu_ps(p)} {}

  /* implicit */
  Vec4(LVec3 v);

  Vec4(LVec3 v, float w);

  // implicit conversions to/from intrinsic type
  FSVF_FORCEINLINE Vec4(__m128 sse) : m_sse{sse} {}
  FSVF_FORCEINLINE operator __m128() const { return m_sse; }

  FSVF_FORCEINLINE
  static Vec4 zeroes() { return _mm_setzero_ps(); }

  // returns a Vec4 with all its bits set to 1
  static Vec4 all_bits_ones();

  // returns a Vec4 with all its bits set to 1
  // it might generate less instructions than all_bits_ones() if the argument is a Vec4 that is
  // already in some CPU register when this function is called
  static Vec4 all_bits_ones(Vec4 v);

  Vec4& FSVF_VECCALL operator=(const Vec4& m) noexcept { m_sse = m.m_sse; return *this; }

  FSVF_FORCEINLINE
  stdVec4 components() const { return std::bit_cast<stdVec4, __m128>(m_sse); }

private:
  __m128 m_sse;

  friend class LVec3;
  friend class Mat4;

  // public getter
  template<FastVector T>
  friend __m128 m_sse(T v);
};

/* ########################################## LVec3 ############################################# */

/* restricted component-wise access, see Vec4's documentation
 *
 * "Large" Vec3, 4 packed 32-bit floats, the last component is guaranteed to be == 0.0f, and access
 * to it is hidden from the user
 */
class alignas(__m128) LVec3
{
public:
  LVec3() : m_sse{_mm_setzero_ps()} {}

  LVec3(const LVec3& v) = default;

  // constructs a LVec3 with the same value in all components
  explicit LVec3(float x) : m_sse{_mm_set_ps1(x)} { m_sse = precondition(m_sse); }

  // constructs a LVec3 with the specified components;
  // prefer using another constructor if the data is already available in a contiguous buffer
  LVec3(float x, float y, float z) : m_sse{_mm_set_ps(0.0f, z, y, x)} {}

  // loads 3 floats from a float buffer into a LVec3 object
  // this constructor deals properly with alignment and it's
  // the only way a LVec3 should be created to access buffers
  explicit LVec3(const float* p) : m_sse{_mm_loadu_ps(p)} { m_sse = precondition(m_sse); }

  // create a Vec3 from a Vec4 (zeroes out the last component, NOT a memcpy/no-op)
  explicit LVec3(Vec4 v) : m_sse{v.m_sse} { m_sse = precondition(m_sse); }

  LVec3& FSVF_VECCALL operator=(const LVec3& m) { m_sse = m.m_sse; return *this; }

  FSVF_FORCEINLINE stdVec3 components() const
  {
    stdVec3 res;
    std::memcpy(&res, &m_sse, sizeof(res));
    return res;
  }

  // unsafe construction: doesn't check/enforce that the last element is 0
  // the user needs to pass a `true` template argument for the call to compile
  // this is just a small measure taken to make users think twice about correctness
  template <bool EnableUnsafeConstruction = false, typename T = Internals::Empty>
  static FSVF_FORCEINLINE LVec3 unsafe_construct(__m128 sse)
  {
    // the template parameter T is only there to enable using `always_false``
    if constexpr (!EnableUnsafeConstruction)
    {
      static_assert(Internals::always_false_v<T>,
        "ERROR: trying to use LVec3's unsafe constructor, be mindful of the invariant!");
    }

    return LVec3(sse);
  }

private:
  __m128 m_sse;

  // unsafe constructor, doesn't check/enforce that the last element is 0
  explicit LVec3(__m128 sse) : m_sse{sse} {}

  friend class Vec4;
  friend class Mat4;

  template<Shuf3 SHUFFLE, FastVector T>
  friend T FSVF_VECCALL shuffle(T v);

  static FSVF_FORCEINLINE __m128 precondition(__m128 sse)
  {
    return blend_with_mask<0b1110>(Vec4::zeroes(), Vec4(sse));
  }

  // public getter
  template<FastVector T>
  friend __m128 m_sse(T v);
};

/* ######################################## Matrix types: ####################################### */

/* Mat4  = column-major representation of a 4x4 matrix; implemented as an array of 4 Vec4s;
 *         aligned to cache lines (any object actually takes up a whole one)
 * LMat3 = "Large" Mat3, internally stored as an affine 4x4 matrix (i.e. with the last row and last
 *         column being [0,0,0,1]); as for LVec3, the invariant is guaranteed
 *         by the interface and the user should not be able to break it accidentally, but can decide
 *         to do so willingly at their own risk;
 *         NOTICE that being internally represented as a 4x4 matrix means this type takes 178%
 *         the memory space of an array of 9 floats
 *
 * the tradeoff for storing `LMat3`s in memory (e.g. as object members) should be measured and
 * evaluated on a case-by-case basis; in case of multiple store/fetch operations, the extra space
 * might cause performance degradations due to an increased number of evictions from L2 and L3
 * caches, while on the other end "small" 3x3 matrices can (and about half of the times will, if
 * stored contiguously) cross L1 cache boundaries. As always, the only objective answer comes from
 * measurements. */

/* ########################################### Mat4 ############################################# */

// aligned to cache line, column-major representation
class alignas(64) Mat4
{
public:
  // constructs a Mat4 with uninitialized values
  Mat4() noexcept :
    cols{
      _mm_undefined_ps(),
      _mm_undefined_ps(),
      _mm_undefined_ps(),
      _mm_undefined_ps()
    }
  {}

  Mat4(const Mat4& m)            noexcept = default;
  Mat4& operator=(const Mat4& m) noexcept = default;

  // constructs a matrix with the value x on the main diagonal
  explicit Mat4(float x);

  // loads 16 floats from a float buffer into a matrix object (column-major representation)
  // this constructor deals properly with alignment and it's
  // the only way a matrix should be created to access buffers
  explicit Mat4(const float* p);

  // constructs a matrix having the given arguments as columns
  Mat4(Vec4 v0, Vec4 v1, Vec4 v2, Vec4 v3) : cols{ v0, v1, v2, v3} {}

  // constructs a matrix with the specified components (column-major representation);
  // prefer using another constructor if the data is already available in a contiguous buffer
  Mat4(float x0, float x1, float x2, float x3,
       float x4, float x5, float x6, float x7,
       float x8, float x9, float xA, float xB,
       float xC, float xD, float xE, float xF);

  // constructs a matrix having the given arguments as main diagonal entries
  Mat4(float d0, float d1, float d2, float d3);

  bool operator==(const Mat4& m) const = default;
  bool operator!=(const Mat4& m) const = default;

  Mat4 FSVF_VECCALL operator*(Mat4 m) const;

  // multiply with a 4D vector
  template <typename T>
    requires FastVector<T> && (!std::derived_from<T, LVec3>)
  T FSVF_VECCALL operator*(T v) const;

  // (deleted) multiply with a 3D vector
  // deleted as potentially ambiguous, use `transform_3x3()`, `transform_affine()` or
  // `transform_projective()` instead
  template <std::derived_from<LVec3> T>
  T FSVF_VECCALL operator*(T m) const = delete;

  // multiply the (top-left) 3x3 submatrix of `*this` with `v`
  template <std::derived_from<LVec3> T>
  T FSVF_VECCALL transform_3x3(T v) const;

  // transform `v` by `*this`, when `*this` is an affine transformation
  // the result is undefined if the last _row_ of `*this` is != (0 0 0 1)
  //
  // when the preconditions are met, this function is logically equivalent to
  // `(*this) * Vec4{v, 1.f}`
  template <std::derived_from<LVec3> T>
  T FSVF_VECCALL transform_affine(T m) const;


  // transform `v` by `*this`, when `*this` is a projective transformation
  // unlike transform_affine(), this method has no preconditions
  //
  // this function is logically equivalent to
  // ```(pseudo-code)
  // res = (*this) * {v.x, v.y, v.z, 1.f}
  // scale = res.wwww
  // return Vec3(res / res.wwww)
  // ```
  template <std::derived_from<LVec3> T>
  T FSVF_VECCALL transform_projective(T m) const;

  // TODO transposed variants for transform_*()?

  // access by column
  Vec4 FSVF_VECCALL operator[](int i) const { return cols[i]; }
  Vec4& FSVF_VECCALL operator[](int i) { return cols[i]; }


  Mat4 FSVF_VECCALL transpose() const;
  Mat4 FSVF_VECCALL adj_transposed() const;
  Mat4 FSVF_VECCALL inverse_transposed() const;

  // multiply all entries of `*this` by x
  Mat4 FSVF_VECCALL scale_by(float x) const;

  // multiply each column of `*this` by v
  Mat4 FSVF_VECCALL scale_by(Vec4  v) const;

  // divide all entries of `*this` by x
  Mat4 FSVF_VECCALL scale_by_inv(float x) const;

  // divide each column of `*this` by v
  Mat4 FSVF_VECCALL scale_by_inv(Vec4  v) const;

  // TODO rename
  // TODO needed?
  Mat4 FSVF_VECCALL mul_transp(Mat4 m) const;

  // returns the adjunct matrix of `*this`
  // NOTE that `adj_transposed()` is faster than `adj()`;
  // avoid doing things like `adj().transpose()`
  Mat4 FSVF_VECCALL adj() const { return adj_transposed().transpose(); }

  // returns the inverse matrix of `*this`
  // NOTE that `inverse_transposed()` is faster than `inverse()`;
  // avoid doing things like `inverse().transpose()`
  Mat4 FSVF_VECCALL inverse() const;

  // returns the inverse matrix of `*this`, if the determinant is !=0, or a `nullopt` otherwise
  // NOTE that `inverse_transposed_safe()` is faster than `inverse_transposed()`;
  std::optional<Mat4> FSVF_VECCALL inverse_safe() const;

  // if the determinant is 0, returns the inverse transposed of `*this`, otherwise a `nullopt`
  std::optional<Mat4> FSVF_VECCALL inverse_transposed_safe() const;

  float FSVF_VECCALL determinant() const;

  // returns the determinant of the (top-left) 3x3 submatrix of `*this`, ignoring whatever is
  // contained in the last row and column
  float FSVF_VECCALL affine_determinant() const;

public:
  std::array<Vec4, 4> cols;
};

class alignas(64) LMat3 : protected Mat4
{
  // TODO documentation

public:
  explicit LMat3(Mat4 m) :
    Mat4{
      LVec3{m[0]},
      LVec3{m[1]},
      LVec3{m[2]},
      Vec4{},
    } {}

  // constructs a matrix by loading its components from a contiguous buffer
  explicit LMat3(const float* m);

  // constructs a matrix with the value x on the main diagonal
  explicit LMat3(float x) : Mat4{x, x, x, 1.f} {}

  // constructs a matrix with the specified components (column-major representation);
  // prefer using another constructor if the data is already available in a contiguous buffer
  LMat3(float x0, float x1, float x2,
        float x3, float x4, float x5,
        float x6, float x7, float x8);

  // constructs a matrix having the given arguments as main diagonal entries
  LMat3(float d0, float d1, float d2) : Mat4(d0, d1, d2, 1.f) {}

  // constructs a matrix having the given arguments as columns
  LMat3(LVec3 v0, LVec3 v1, LVec3 v2) : Mat4{v0, v1, v2, Vec4{0.f, 0.f, 0.f, 1.f}} {}

  bool operator==(const LMat3& m) const = default;
  bool operator!=(const LMat3& m) const = default;

  // unsafe constructor, doesn't check/enforce the class' invariants
  // the user needs to pass a `true` template argument for the call to compile
  // this is just a small measure taken to make users think twice about correctness
  template <bool EnableUnsafeConstruction = false, typename T = Internals::Empty>
  static LMat3 FSVF_VECCALL unsafe_construct(Mat4 m);

  using Mat4::transpose;

  LMat3 FSVF_VECCALL adj_transposed() const;
  LMat3 FSVF_VECCALL inverse_transposed() const;

  // if the determinant is 0, returns the inverse transposed of `*this`, otherwise a `nullopt`
  std::optional<LMat3> FSVF_VECCALL inverse_transposed_safe() const;

  // returns the adjunct matrix of `*this`
  // NOTE that `adj_transposed()` is faster than `adj()`;
  // avoid doing things like `adj().transpose()`
  LMat3 FSVF_VECCALL adj() const;

  // returns the inverse matrix of `*this`
  // NOTE that `inverse_transposed()` is faster than `inverse()`;
  // avoid doing things like `inverse().transpose()`
  LMat3 FSVF_VECCALL inverse() const;

  // returns the inverse matrix of `*this`, if the determinant is !=0, or a `nullopt` otherwise
  // NOTE that `inverse_transposed_safe()` is faster than `inverse_transposed()`;
  std::optional<LMat3> FSVF_VECCALL inverse_safe() const;

  FSVF_FORCEINLINE
  float FSVF_VECCALL determinant() const { return Mat4::affine_determinant(); }

private:
  // unsafe constructor, doesn't check/enforce the class' invariants
  LMat3(__m128 c0, __m128 c1, __m128 c2, __m128 c3)
    : Mat4(Vec4{c0}, Vec4{c1}, Vec4{c2}, Vec4{c3}) {}
};

/* ######################################### Internals ########################################## */

// public getter for FastVector's intrinsic member
template<FastVector T>
[[nodiscard]] __m128 m_sse(T v)
{
  if constexpr (std::derived_from<T, LVec3>)
  {
    return static_cast<LVec3>(v).m_sse;
  }
  else
  {
    return static_cast<Vec4>(v).m_sse;
  }
}

// shenanigans to allow FastVector's interface functions access to LVec3's unsafe constructor using
// the same interface we use for the other classes, without having to re-declare them all as friends
namespace Internals
{
template<FastVector T>
FSVF_FORCEINLINE T raw_constructor_wrapper(__m128 sse)
{
  if constexpr(std::is_base_of_v<LVec3, T>)
  {
    return LVec3::unsafe_construct<true>(sse);
  }
  else
  {
    return T{sse};
  }
}
}

// undef'd at the end of the file
#ifdef FSVF_RAW_
#error "FATAL: FSVF_RAW_ has already been defined outside of this library, please rename this macro"
#endif
#define FSVF_RAW_(T, x) Internals::raw_constructor_wrapper<T>(x)

/* ############################################################################################## */
/* ################################### Class Implementations #################################### */
/* ############################################################################################## */

FSVF_FORCEINLINE Vec4::Vec4(LVec3 v) : m_sse{v.m_sse} {}

FSVF_FORCEINLINE Vec4::Vec4(LVec3 v, float w)
  : m_sse{v.m_sse}
{
  Vec4 wwww(w);
  m_sse = blend_with_mask<0b1110>(wwww, Vec4(m_sse));
}

template<FastVector T1, FastVector T2>
FSVF_FORCEINLINE auto FSVF_VECCALL operator|(T1 v, T2 w) -> LargestDimensionType<T1, T2>
{
  using Res = LargestDimensionType<T1, T2>;
  return FSVF_RAW_(Res, _mm_or_ps(m_sse(v),  m_sse(w)));
}

template<FastVector T1, FastVector T2>
FSVF_FORCEINLINE auto FSVF_VECCALL operator&(T1 v, T2 w) -> SmallestDimensionType<T1, T2>
{
  using Res = SmallestDimensionType<T1, T2>;
  return FSVF_RAW_(Res, _mm_and_ps(m_sse(v), m_sse(w)));
}

template<FastVector T1, FastVector T2>
FSVF_FORCEINLINE Vec4 FSVF_VECCALL and_not(T1 v, T2 w)
{
  // we want v AND (NOT w), so the arguments are flipped in the intrinsic call
  return _mm_andnot_ps(m_sse(w), m_sse(v));
}

template<FastVector T1, FastVector T2>
FSVF_FORCEINLINE auto FSVF_VECCALL operator^(T1 v, T2 w) -> LargestDimensionType<T1, T2>
{
  using Res = LargestDimensionType<T1, T2>;

  // cleanup of last component might be needed for XOR
  return Res{Vec4{_mm_xor_ps(m_sse(v), m_sse(w))}};
}

template<FastVector T> T FSVF_VECCALL operator~(T v)
{
  if constexpr (std::is_base_of_v<LVec3, T>)
  {
    static_assert(Internals::always_false_v<T>,
        "Using operator~() on a 3D vector is ambiguous in terms of what should happen in the (hidden) fourth component;"
        "please use this operator on a 4D vector, making all intended casts explicit");
  }

  return v ^ Vec4::all_bits_ones(v);
}

template<FastVector T1, FastVector T2>
  requires (SameDimension<T1, T2>)
T1& FSVF_VECCALL operator&=(T1& v, T2 w)
{
  v = v & w; return v;
}

template<FastVector T1, FastVector T2>
  requires (SameDimension<T1, T2>)
T1& FSVF_VECCALL operator|=(T1& v, T2 w)
{
  v = v | w; return v;
}

template<FastVector T1, FastVector T2>
  requires (SameDimension<T1, T2>)
T1& FSVF_VECCALL operator^=(T1& v, T2 w)
{
  v = v ^ w; return v;
}

template<FastVector T>
FSVF_FORCEINLINE T FSVF_VECCALL operator-(T v)
{
  if constexpr (!std::is_base_of_v<LVec3, T>)
  {
    const __m128i vb = float_to_bits(v);
    const __m128i ones = _mm_cmpeq_epi32(vb, vb);
    const __m128 sign_mask = bits_to_float(_mm_slli_epi32(ones, 31));
    return FSVF_RAW_(T, _mm_xor_ps(m_sse(v), sign_mask));
  }
  else
  {
    return LVec3(-Vec4(v));
  }
}

namespace Internals
{
// bitwise magic to reverse a 4-bit number in binary representation
// requires: x <= 0xF
FSVF_FORCEINLINE constexpr uint8_t magic_flip(uint8_t x)
{
  int32_t res = x;
  res *= 0x222u;
  res &= 0x1284u;
  uint32_t magic = res * 0x2083;
  magic >>= 19;
  magic *= 0xC1;
  res += magic;
  return static_cast<uint8_t>(res);
}
}  // namespace Internals

template<FastVector T>
FSVF_FORCEINLINE bool FSVF_VECCALL bitwise_equal(T v, T w)
{
  // bitwise equality <=> a ^ b == 0
  __m128i xorRes = _mm_castps_si128(_mm_xor_ps(m_sse(v), m_sse(w)));
  return _mm_testz_si128(xorRes, xorRes);
}

template<FastVector T1, FastVector T2>
FSVF_FORCEINLINE bool FSVF_VECCALL operator==(T1 v, T2 w)
{
  __m128i res = _mm_castps_si128(_mm_cmpneq_ps(m_sse(v), m_sse(w)));
  return static_cast<bool>(_mm_testz_si128(res, res));
}

template<FastVector T1, FastVector T2>
FSVF_FORCEINLINE bool FSVF_VECCALL operator!=(T1 v, T2 w)
{
  return !(v == w);
}

template <uint8_t BinaryMask, FastVector T1, FastVector T2>
FSVF_FORCEINLINE auto FSVF_VECCALL blend_with_mask(T1 v, T2 w) -> LargestDimensionType<T1, T2>
{
  using Res = LargestDimensionType<T1, T2>;
  const __m128 blended = _mm_blend_ps(m_sse(v), m_sse(w), Internals::magic_flip(BinaryMask));
  return FSVF_RAW_(Res, blended);
}

namespace BitMasks
{
  template<FastVector T1, FastVector T2>
  FSVF_FORCEINLINE Vec4 FSVF_VECCALL cmp_equal_f32(T1 v, T2 w)
  {
    return Vec4{_mm_cmpeq_ps(m_sse(v), m_sse(w))};
  }

  template<FastVector T1, FastVector T2>
  FSVF_FORCEINLINE auto FSVF_VECCALL cmp_not_equal_f32(T1 v, T2 w) -> LargestDimensionType<T1, T2>
  {
    using Res = LargestDimensionType<T1, T2>;
    return FSVF_RAW_(Res, _mm_cmpneq_ps(m_sse(v), m_sse(w)));
  }

  template<FastVector T1, FastVector T2>
  FSVF_FORCEINLINE Vec4 FSVF_VECCALL cmp_equal_bitwise(T1 v, T2 w)
  {
    if constexpr (std::is_base_of_v<LVec3, T1> || std::is_base_of_v<LVec3, T2>)
    {
      static_assert(Internals::always_false_v<T1>,
          "Using a bitwise comparison on a 3D vector is bug-prone due to the (hidden) fourth component;"
          "please use this operator on 4D vectors, making all intended casts explicit");
    }
    return bits_to_float(
      _mm_cmpeq_epi32(
        float_to_bits(m_sse(v)),
        float_to_bits(m_sse(w))
      )
    );
  }

  FSVF_FORCEINLINE bool all_ones_bitwise(Vec4 v)
  {
    const auto s = float_to_bits(m_sse(v));
    const auto ones = _mm_cmpeq_epi32(s, s);
    return _mm_testc_si128(s, ones);
  }

  FSVF_FORCEINLINE bool all_zeroes_bitwise(Vec4 v)
  {
    // bit-wise zero <=> x & x == 0
    const __m128i bits = float_to_bits(m_sse(v));
    return _mm_testz_si128(bits, bits);
  }

  // isolate and pack bits of index 8j-1 in Vec4 for 1 <= j <= 16
  // can be interpreted as the sign-bits of 8-bit signed integers packed in 128 bits
  FSVF_FORCEINLINE uint16_t hi_bits8(Vec4 v)
  {
    return _mm_movemask_epi8(float_to_bits(m_sse(v)));
  }

  // isolate and pack bits of index 32j-1 in Vec4 for 1 <= j <= 4
  // can be interpreted as the sign-bits of its components
  FSVF_FORCEINLINE uint8_t  hi_bits32(Vec4 v)
  {
    return _mm_movemask_ps(m_sse(v));
  }

  // isolate and pack bits 63 and 17 in Vec4
  // (most-significant bits of each half of the object)
  FSVF_FORCEINLINE uint8_t  hi_bits64(Vec4 v)
  {
    return _mm_movemask_pd(_mm_castps_pd(m_sse(v)));
  }
} // namespace BitMasks

FSVF_FORCEINLINE Vec4 Vec4::all_bits_ones(Vec4 v)
{
  return BitMasks::cmp_equal_bitwise(v, v);
}

FSVF_FORCEINLINE Vec4 Vec4::all_bits_ones()
{
  const auto v = Vec4{_mm_undefined_ps()};
  return all_bits_ones(v);
}

template<Shuf4 SHUFFLE, FastVector T>
FSVF_FORCEINLINE auto FSVF_VECCALL shuffle(T v) -> LargestDimensionType<T, Vec4>
{
  using Res = LargestDimensionType<T, Vec4>;
  return Res{_mm_shuffle_ps(m_sse(v), m_sse(v), Internals::to_underlying(SHUFFLE))};
}

template<Shuf3 SHUFFLE, FastVector T>
FSVF_FORCEINLINE T FSVF_VECCALL shuffle(T v)
{
  return FSVF_RAW_(T, _mm_shuffle_ps(m_sse(v), m_sse(v), Internals::to_underlying(SHUFFLE)));
}

template<FastVector T1, FastVector T2>
FSVF_FORCEINLINE auto FSVF_VECCALL operator+(T1 v, T2 w) -> LargestDimensionType<T1, T2>
{
  using Res = LargestDimensionType<T1, T2>;
  return FSVF_RAW_(Res, _mm_add_ps(m_sse(v), m_sse(w)));
}

template<FastVector T1, FastVector T2>
FSVF_FORCEINLINE auto FSVF_VECCALL operator-(T1 v, T2 w) -> LargestDimensionType<T1, T2>
{
  using Res = LargestDimensionType<T1, T2>;
  return FSVF_RAW_(Res, _mm_sub_ps(m_sse(v), m_sse(w)));
}

// TODO document on declaration
template<FastVector T1, FastVector T2> FSVF_FORCEINLINE auto FSVF_VECCALL operator/(T1 v, T2 w) -> LargestDimensionType<T1, T2>
{
  using Res = LargestDimensionType<T1, T2>;

  // if both are 3D vectors -> set the last component to 0
  // if only the divisor is a 3D vector -> the last component will be an infinite with the same sign of the dividend
#if defined(FSVF_RCPPS)
  return Res{v * Vec4{_mm_rcp_ps(m_sse(w))}};
#else
  return Res{Vec4{_mm_div_ps(m_sse(v), m_sse(w))}};
#endif
}

template<FastVector T1, FastVector T2>
FSVF_FORCEINLINE auto FSVF_VECCALL operator*(T1 v, T2 w) -> SmallestDimensionType<T1, T2>
{
  using Res = SmallestDimensionType<T1, T2>;
  return FSVF_RAW_(Res, _mm_mul_ps(m_sse(v), m_sse(w)));
}

template<FastVector T> FSVF_FORCEINLINE T FSVF_VECCALL operator*(float x, T v) { return FSVF_RAW_(T, _mm_mul_ps(_mm_set_ps1(x), m_sse(v))); }
template<FastVector T> FSVF_FORCEINLINE T FSVF_VECCALL operator*(T v, float x) { return x * v; }

// TODO document on declaration
template<FastVector T> FSVF_FORCEINLINE T FSVF_VECCALL operator/(float x, T v)
{
  return T{Vec4{x} / v};
}

// TODO document on declaration
template<FastVector T> FSVF_FORCEINLINE T FSVF_VECCALL operator/(T v, float x)
{
  return T{v / Vec4{x}};
}

template<FastVector T1, FastVector T2> T1& FSVF_VECCALL operator-=(T1& v, T2 w) { v = v - w; return v; }
template<FastVector T1, FastVector T2> T1& FSVF_VECCALL operator+=(T1& v, T2 w) { v = v + w; return v; }
template<FastVector T1, FastVector T2> T1& FSVF_VECCALL operator/=(T1& v, T2 w) { v = v / w; return v; } // TODO document on declaration
template<FastVector T1, FastVector T2> T1& FSVF_VECCALL operator*=(T1& v, T2 w) { v = v * w; return v; }
template<FastVector T> T& FSVF_VECCALL operator/=(T& v, float x) { v = v / x; return v; } // TODO specialize to avoid dividing by 0 in LVec3s
template<FastVector T> T& FSVF_VECCALL operator*=(T& v, float x) { v = v * x; return v; }

template<FastVector T> Vec4 FSVF_VECCALL operator<=(T v, T w) { return _mm_cmple_ps(m_sse(v), m_sse(w)); }
template<FastVector T> Vec4 FSVF_VECCALL operator>=(T v, T w) { return _mm_cmpge_ps(m_sse(v), m_sse(w)); }
template<FastVector T> Vec4 FSVF_VECCALL operator< (T v, T w) { return _mm_cmplt_ps(m_sse(v), m_sse(w)); }
template<FastVector T> Vec4 FSVF_VECCALL operator> (T v, T w) { return _mm_cmpgt_ps(m_sse(v), m_sse(w)); }

template<uint8_t VectorDimensions>
FSVF_FORCEINLINE bool FSVF_VECCALL all(Vec4 bitmask)
{
  if constexpr (VectorDimensions != 3 && VectorDimensions != 4)
  {
    static_assert(std::is_same_v<decltype(VectorDimensions), float>,
        "ERROR: VectorDimensions must be either 3 or 4");
  }

  if constexpr (VectorDimensions == 3)
  {
    bitmask |= Vec4{0.f, 0.f, 0.f, bits_to_float(0xFFFF'FFFF)};
  }

  return BitMasks::all_ones_bitwise(bitmask);
}

template<uint8_t VectorDimensions>
FSVF_FORCEINLINE bool FSVF_VECCALL none(Vec4 bitmask)
{
  if constexpr (VectorDimensions != 3 && VectorDimensions != 4)
  {
    static_assert(std::is_same_v<decltype(VectorDimensions), float>,
        "ERROR: VectorDimensions must be either 3 or 4");
  }

  if constexpr (VectorDimensions == 3)
  {
    bitmask = LVec3(bitmask);
  }

  return BitMasks::all_zeroes_bitwise(bitmask);
}

template<uint8_t VectorDimensions>
FSVF_FORCEINLINE bool FSVF_VECCALL any (Vec4 bitmask)
{
  return !none<VectorDimensions>(bitmask);
}

namespace Internals
{
  template<typename Fn>
  concept Cumulable = std::is_invocable_r_v<__m128, Fn, __m128, __m128>;

  // callable that can be accumulated // TODO document better
  // TODO add associativity requirement (unlike std::reduce(), commutativity is not required)
  template<Cumulable Fn>
  __m128 FSVF_FORCEINLINE FSVF_VECCALL reduce0(Fn f, __m128 v)
  {
    // see https://stackoverflow.com/a/35270026
    const __m128 shuf    = _mm_movehdup_ps(v);          // y y w w
    const __m128 couple  = f(v, shuf);                  // f(x,y) f(y,y) f(z,w) f(w,w)
    const __m128 partial = _mm_movehl_ps(shuf, couple); // f(z,w) f(w,w) w w
    return f(couple, partial);                          // f(fxy,fzw) f(fyy,fww) f(fzw,w) f(fww,w)
  }

  // TODO document
  template<Cumulable Fn>
  __m128 FSVF_FORCEINLINE FSVF_VECCALL reduceAll(Fn f, __m128 v0, __m128 v1, __m128 v2, __m128 v3)
  {
    // see Intel(R) 64 and IA-32 Architectures Optimization Reference Manual, 7.5.1.4
    const __m128 t0 = _mm_movelh_ps(v0, v1);
    const __m128 t1 = _mm_movehl_ps(v0, v1);
    const __m128 t2 = _mm_movelh_ps(v2, v3);
    const __m128 t3 = _mm_movehl_ps(v2, v3);

    const __m128 a0 = f(t0, t1);
    const __m128 a1 = f(t2, t3);

    const __m128 s0 = _mm_shuffle_ps(a0, a1, to_underlying(Shuf4::xzxz));
    const __m128 s1 = _mm_shuffle_ps(a0, a1, to_underlying(Shuf4::ywyw));

    return f(s0, s1);
  }
}

template<FastVector T>
FSVF_FORCEINLINE float FSVF_VECCALL sum(T v)
{
  auto sums = Internals::reduce0(INTR_WRAP(_mm_add_ps), m_sse(v));
  return _mm_cvtss_f32(sums);
}

template<FastVector T>
FSVF_FORCEINLINE float FSVF_VECCALL prod(T v)
{
  if constexpr (std::is_base_of_v<LVec3, T>)
  {
    const Vec4 adjusted = v | Vec4(0, 0, 0, 1.0f);
    auto prods = Internals::reduce0(INTR_WRAP(_mm_mul_ps), m_sse(adjusted));
    return _mm_cvtss_f32(prods);
  }

  auto prods = Internals::reduce0(INTR_WRAP(_mm_mul_ps), m_sse(v));
  return _mm_cvtss_f32(prods);
}

template<FastVector T1, FastVector T2>
FSVF_FORCEINLINE float FSVF_VECCALL dot(T1 v, T2 w)
{
  __m128 res = _mm_mul_ps(m_sse(v), m_sse(w));
  return sum(Vec4{res});
}

template<FastVector T1, FastVector T2>
FSVF_FORCEINLINE LVec3 FSVF_VECCALL cross(T1 v, T2 w)
{
  // v = x y z n
  // w = a b c m
  auto vp = shuffle<Shuf3::zxy>(v); // z x y n
  auto wp = shuffle<Shuf3::zxy>(w); // c a b m
  auto m1 = vp * w;                 // az bx cy mn
  auto m2 = v  * wp;                // cx ay bz mn
  auto s  = m1 - m2;                // az-cx bx-ay cy-bz 0
  auto sh = shuffle<Shuf3::zxy>(s); // cy-bz az-cx bx-ay 0
  return FSVF_RAW_(LVec3, m_sse(sh));
}

template<FastVector T>
T FSVF_VECCALL abs(T v)
{
  constexpr uint32_t f32_sign_mask = 0x8000'0000u;
  __m128 abs_mask = _mm_set_ps1(bits_to_float(~f32_sign_mask));
  return FSVF_RAW_(T, _mm_and_ps(abs_mask, m_sse(v)));
}

template<FastVector T>
FSVF_FORCEINLINE float FSVF_VECCALL len_squared(T v)
{
  return dot(v, v);
}

template<FastVector T>
FSVF_FORCEINLINE float FSVF_VECCALL len(T v)
{
  // using that len(v) = abs(max) * len(v/abs(max)) to improve numerical stability
  const Vec4 maxVal = max_component_value<T>(abs(v));

  // avoid dividing by zero
  v &= BitMasks::cmp_not_equal_f32(Vec4::zeroes(), maxVal);

  const Vec4 scaled = v / maxVal;

  const float lenScaled = sqrtf(len_squared(scaled));
  const Vec4 res = maxVal * lenScaled;

  return _mm_cvtss_f32(res);
}

template<FastVector T>
T FSVF_VECCALL normalize(T v)
{
  // using that norm(v) = norm(v/abs(max)) to improve numerical stability
  const Vec4 maxVal = max_component_value<T>(abs(v));

  Vec4 scaled = v / maxVal;

  // handle the case maxVal == 0
  const Vec4 nonzeroMask = BitMasks::cmp_not_equal_f32(Vec4::zeroes(), maxVal);
  scaled &= nonzeroMask;

  Vec4 lenScaledSquared{_mm_dp_ps(scaled, scaled, 0xFF)};

#if defined(FSVF_RSQRT_NORMALIZE) || defined(FSVF_RCPPS)
  const Vec4 invLenScaled = rsqrt(lenScaledSquared);
  Vec4 res = scaled * invLenScaled;
#else
  const Vec4 lenScaled = sqrt(lenScaledSquared);
  Vec4 res = scaled / lenScaled;
#endif

  // handle the case lenScaled == 0
  res &= nonzeroMask;

  return FSVF_RAW_(T, res);
}

template<FastVector T1, FastVector T2>
FSVF_FORCEINLINE auto FSVF_VECCALL merge_max(T1 v, T2 w) -> LargestDimensionType<T1, T2>
{
  using Res = LargestDimensionType<T1, T2>;
  return FSVF_RAW_(Res, _mm_max_ps(m_sse(v), m_sse(w)));
}

template<FastVector T1, FastVector T2>
FSVF_FORCEINLINE auto FSVF_VECCALL merge_min(T1 v, T2 w) -> LargestDimensionType<T1, T2>
{
  using Res = LargestDimensionType<T1, T2>;
  return FSVF_RAW_(Res, _mm_min_ps(m_sse(v), m_sse(w)));
}

template<FastVector T>
FSVF_FORCEINLINE uint8_t FSVF_VECCALL min_component_index(T v)
{
  if constexpr (!std::is_base_of_v<LVec3, T>)
  {
    auto const wshuf = _mm_shuffle_ps(m_sse(v), m_sse(v), static_cast<uint8_t>(Shuf4::wwxy));
    auto const zshuf = _mm_shuffle_ps(m_sse(v), m_sse(v), static_cast<uint8_t>(Shuf4::zzxy));

    // check if we're gonna keep z or w (w means adding 1 if later we pick "Z")
    auto const wz = _mm_ucomilt_ss(wshuf, zshuf);

#ifdef FSVF_FLOAT_FINITE_MATH
    // the rest of the function can operates on integer lanes, saving a few cycles and possibly
    // with higher throughput; worth it even on CPUs with domain transition penalties
    auto izshuf = bits_to_float(zshuf);
    auto iwshuf = bits_to_float(wshuf);
    auto w = _mm_min_epi32(iwshuf, izshuf);
#else
    auto w = _mm_min_ps(wshuf, zshuf);
#endif

    // from this point on, Z refers to whichever member survived between m_sse[2] and m_sse[3]

    // w = ZZxy, so with respect to m_sse, the permutations are
    // a = xxyy
    // b = yZZZ
#ifdef FSVF_FLOAT_FINITE_MATH
    auto const a = _mm_shuffle_epi32(w, static_cast<uint8_t>(Shuf4::zzww));
    auto const b = _mm_shuffle_epi32(w, static_cast<uint8_t>(Shuf4::wxxx));
    auto const cmp = _mm_cmplt_epi32(a, b);
#else
    auto const a = _mm_shuffle_ps(w, w, static_cast<uint8_t>(Shuf4::zzww));
    auto const b = _mm_shuffle_ps(w, w, static_cast<uint8_t>(Shuf4::wxxx));
    auto const cmp = _mm_cmplt_ps(a, b);
#endif

    std::array<uint32_t, 4> arr;
    std::memcpy(&arr, &cmp, sizeof(arr));

    /* arr[0] = 0xFFFF'FFFF * (x < y)
    *  arr[1] = 0xFFFF'FFFF * (x < Z)
    *  arr[2] = 0xFFFF'FFFF * (y < Z) */

    /* branch-free implementation of:
    return (x < y) ?
      (x < Z) ? 0 : (2 + wz) :
      (y < Z) ? 1 : (2 + wz);
    */

    return /* x < y ? */ ( arr[0] &
           /* x < Z ? */ (!arr[1] * (2 + wz)) ) /* ":" */ | ( ~arr[0] &
           /* y < Z ? */ (!arr[2] * (1 + wz) + 1));
  }
  else
  {
    // 3D case
    // TODO finish optimizing for the FINITE_MATH case

    // make all desired comparisons at once:
    auto const a = _mm_shuffle_ps(m_sse(v), m_sse(v), Internals::to_underlying(Shuf3::xxy));
    auto const b = _mm_shuffle_ps(m_sse(v), m_sse(v), Internals::to_underlying(Shuf3::yzz));
    auto const cmp = _mm_cmplt_ps(a, b);

    std::array<uint32_t, 4> arr;
    std::memcpy(&arr, &cmp, sizeof(arr));


    // the branch-free logic is the same as the 4-dimensional case
    // without the extra complexity of having to deal with `wz`

    return /* x < y ? */ ( arr[0] &
           /* x < z ? */ (!arr[1] * 2) ) /* ":" */ | ( ~arr[0] &
           /* y < z ? */ (!arr[2] + 1) );
  }
}

template<FastVector T>
inline uint8_t FSVF_VECCALL max_component_index(T v)
{
  // see min_component_index() for documentation about the implementation

  if constexpr (!std::is_base_of_v<LVec3, T>)
  {
    auto const wshuf = _mm_shuffle_ps(m_sse(v), m_sse(v), static_cast<uint8_t>(Shuf4::wwxy));
    auto const zshuf = _mm_shuffle_ps(m_sse(v), m_sse(v), static_cast<uint8_t>(Shuf4::zzxy));

    auto const wz = _mm_ucomigt_ss(wshuf, zshuf);

#ifdef FSVF_FLOAT_FINITE_MATH
    auto izshuf = _mm_castps_si128(zshuf);
    auto iwshuf = _mm_castps_si128(wshuf);
    auto w = _mm_max_epi32(iwshuf, izshuf);

    auto const a = _mm_shuffle_epi32(w, static_cast<uint8_t>(Shuf4::zzww));
    auto const b = _mm_shuffle_epi32(w, static_cast<uint8_t>(Shuf4::wxxx));
    auto const cmp = _mm_cmpgt_epi32(a, b);
#else
    auto w = _mm_max_ps(wshuf, zshuf);

    auto const a = _mm_shuffle_ps(w, w, static_cast<uint8_t>(Shuf4::zzww));
    auto const b = _mm_shuffle_ps(w, w, static_cast<uint8_t>(Shuf4::wxxx));
    auto const cmp = _mm_cmpgt_ps(a, b);
#endif

    std::array<uint32_t, 4> arr;
    std::memcpy(&arr, &cmp, sizeof(arr));

    return (arr[0] & (!arr[1] * (2 + wz))) | (~arr[0] & (!arr[2] * (1 + wz) + 1));
  }
  else
  {
    // 3D case
    // TODO finish optimizing for the FINITE_MATH case

    auto const a = _mm_shuffle_ps(m_sse(v), m_sse(v), static_cast<uint8_t>(Shuf3::xxy));
    auto const b = _mm_shuffle_ps(m_sse(v), m_sse(v), static_cast<uint8_t>(Shuf3::yzz));
    auto const cmp = _mm_cmpgt_ps(a, b);

    std::array<uint32_t, 4> arr;
    std::memcpy(&arr, &cmp, sizeof(arr));

    return (arr[0] & (!arr[1] * 2)) | (~arr[0] & (!arr[2] + 1));
  }
}

template<FastVector T>
FSVF_FORCEINLINE Vec4 FSVF_VECCALL min_component_value(T v)
{

  // TODO optimize
  Vec4 vm;
  if constexpr (std::is_base_of_v<LVec3, T>)
    vm = shuffle<Shuf4::xyzz>(v);
  else
    vm = v;

  vm = merge_min(vm, shuffle<Shuf4::wxyz>(vm));
  vm = merge_min(vm, shuffle<Shuf4::zwxy>(vm));

  return vm;
}

template<FastVector T>
FSVF_FORCEINLINE Vec4 FSVF_VECCALL max_component_value(T v)
{
  // TODO optimize
  Vec4 vm;
  if constexpr (std::is_base_of_v<LVec3, T>)
    vm = shuffle<Shuf4::xyzz>(v);
  else
    vm = v;

  vm = merge_max(vm, shuffle<Shuf4::wxyz>(vm));
  vm = merge_max(vm, shuffle<Shuf4::zwxy>(vm));

  return vm;
}

template<FastVector T>
FSVF_FORCEINLINE float FSVF_VECCALL min_component_value_f32(T v)
{
  // TODO optimize
  __m128 vm;
  if constexpr (std::is_base_of_v<LVec3, T>)
    vm = m_sse(shuffle<Shuf4::xyzz>(v));
  else
    vm = m_sse(v);

  __m128 reduced = Internals::reduce0(INTR_WRAP(_mm_min_ps), vm);
  return _mm_cvtss_f32(reduced);
}

template<FastVector T>
FSVF_FORCEINLINE float FSVF_VECCALL max_component_value_f32(T v)
{
  // TODO optimize
  __m128 vm;
  if constexpr (std::is_base_of_v<LVec3, T>)
    vm = m_sse(shuffle<Shuf4::xyzz>(v));
  else
    vm = m_sse(v);

  __m128 reduced = Internals::reduce0(INTR_WRAP(_mm_max_ps), vm);
  return _mm_cvtss_f32(reduced);
}

template<FastVector T1, FastVector T2>
FSVF_FORCEINLINE T1 FSVF_VECCALL clamp(T1 v, T2 min, T2 max)
{
  auto min_mask = Vec4{v} < Vec4{min};
  auto max_mask = Vec4{v} > Vec4{max};
  auto change_mask = min_mask | max_mask;

  auto max_filler = max_mask & max;
  auto min_filler = min_mask & min;

  auto res = and_not(v, change_mask);
  res |= max_filler;
  res |= min_filler;

  return LargestDimensionType<T1, T2>{res};
}

template<FastVector T>
FSVF_FORCEINLINE T FSVF_VECCALL sqrt(T v)
{
  return FSVF_RAW_(T, _mm_sqrt_ps(m_sse(v)));
}

template<FastVector T>
FSVF_FORCEINLINE T FSVF_VECCALL rsqrt(T v)
{
  Vec4 res = FSVF_RAW_(Vec4, _mm_rsqrt_ps(m_sse(v)));
  // for types deriving from LVec3
  // the constructor will mask out possible junk coming from the last component
  return T{res};
}

template<FastVector T>
FSVF_FORCEINLINE T FSVF_VECCALL sort(T v)
{
  if constexpr (!std::is_base_of_v<LVec3, T>)
  {
    // bitonic sort

    /* sort upper and lower half */

    auto yyww = _mm_movehdup_ps(m_sse(v));
    auto xxzz = _mm_moveldup_ps(m_sse(v));
    auto min = _mm_min_ps(m_sse(v), yyww); // (min(x,y), y, min(z,w), w)
    auto max = _mm_max_ps(m_sse(v), xxzz); // (x, max(x,y), z, max(z,w))
    auto upup = _mm_blend_ps(min, max, 0b10'10); // (min(x,y), max(x,y), min(z,w), max(z,w))

    /* swap last two entries to make the sequence bitonic */

    auto bitonic = _mm_shuffle_ps(upup, upup, static_cast<uint8_t>(Shuf4::xywz)); // (min(x,y), max(x,y), max(z,w), min(z,w))

    /* conditionally swap odd/even entries if they're not ordered */

    auto shuf = _mm_shuffle_ps(upup, upup, static_cast<uint8_t>(Shuf4::wzxy));
    auto swap = _mm_cmpgt_ps(bitonic, shuf);
    // repeat the upper half to make the swapper/keeper trick work (the second half is already the negation of the first)
    swap = _mm_shuffle_ps(swap, swap, static_cast<uint8_t>(Shuf4::xyxy));
    auto swapper = _mm_and_ps(swap, shuf);
    auto keeper = _mm_andnot_ps(swap, bitonic);
    auto merger = _mm_or_ps(swapper, keeper);

    /* sort again lower and upper half */

    yyww = _mm_movehdup_ps(merger);
    xxzz = _mm_moveldup_ps(merger);
    min = _mm_min_ps(merger, yyww);
    max = _mm_max_ps(merger, xxzz);

    return FSVF_RAW_(Vec4, _mm_blend_ps(min, max, 0b10'10));
  }
  else
  {
    // 3D case

    /* sort upper elements */
    auto yyz = _mm_shuffle_ps(m_sse(v), m_sse(v), static_cast<uint8_t>(Shuf3::yyz));
    auto xxz = _mm_shuffle_ps(m_sse(v), m_sse(v), static_cast<uint8_t>(Shuf3::xxz));
    auto min = _mm_min_ps(m_sse(v), yyz); // (mxy, y, z)
    auto max = _mm_max_ps(m_sse(v), xxz); // (x, Mxy, z)
    auto mxy = _mm_blend_ps(min, max, 0b00'10); // (mxy, Mxy, z)

    /* sort lower elements */
    auto sh1 = _mm_shuffle_ps(mxy, mxy, static_cast<uint8_t>(Shuf3::xzz)); // (mxy, z, z)
    auto sh2 = _mm_shuffle_ps(mxy, mxy, static_cast<uint8_t>(Shuf3::xyy)); // (mxy, Mxy, Mxy)
    min = _mm_min_ps(mxy, sh1); // (mxy, min(Mxy,z), z)
    max = _mm_max_ps(mxy, sh2); // (mxy, Mxy, max(Mxy,z))
    auto myz = _mm_blend_ps(min, max, 0b11'00); // (mxy, min(z,Mxy), max(z,Mxy))

    /* sort upper elements again */
    yyz = _mm_shuffle_ps(myz, myz, static_cast<uint8_t>(Shuf3::yyz));
    xxz = _mm_shuffle_ps(myz, myz, static_cast<uint8_t>(Shuf3::xxz));
    min = _mm_min_ps(myz, yyz);
    max = _mm_max_ps(myz, xxz);

    return FSVF_RAW_(LVec3, _mm_blend_ps(min, max, 0b00'10));
  }
}

template<FastVector T>
FSVF_FORCEINLINE T FSVF_VECCALL reverse_sort(T v)
{
  if constexpr (!std::is_base_of_v<LVec3, T>)
  {
    // bitonic sort, same logic as sort()

    /* sort upper and lower half */

    auto yyww = _mm_movehdup_ps(m_sse(v));
    auto xxzz = _mm_moveldup_ps(m_sse(v));
    auto max = _mm_max_ps(m_sse(v), yyww);
    auto min = _mm_min_ps(m_sse(v), xxzz);
    auto upup = _mm_blend_ps(max, min, 0b10'10);

    /* swap last two entries to make the sequence bitonic */

    auto bitonic = _mm_shuffle_ps(upup, upup, static_cast<uint8_t>(Shuf4::xywz));

    /* conditionally swap odd/even entries if they're not ordered */

    auto shuf = _mm_shuffle_ps(upup, upup, static_cast<uint8_t>(Shuf4::wzxy));
    auto swap = _mm_cmplt_ps(bitonic, shuf);
    swap = _mm_shuffle_ps(swap, swap, static_cast<uint8_t>(Shuf4::xyxy));
    auto swapper = _mm_and_ps(swap, shuf);
    auto keeper = _mm_andnot_ps(swap, bitonic);
    auto merger = _mm_or_ps(swapper, keeper);

    /* sort again lower and upper half */

    yyww = _mm_movehdup_ps(merger);
    xxzz = _mm_moveldup_ps(merger);
    max = _mm_max_ps(merger, yyww);
    min = _mm_min_ps(merger, xxzz);

    return FSVF_RAW_(Vec4, _mm_blend_ps(max, min, 0b10'10));
  }
  else
  {
    // 3D case

    /* sort upper elements */
    auto yyz = _mm_shuffle_ps(m_sse(v), m_sse(v), static_cast<uint8_t>(Shuf3::yyz));
    auto xxz = _mm_shuffle_ps(m_sse(v), m_sse(v), static_cast<uint8_t>(Shuf3::xxz));
    auto min = _mm_max_ps(m_sse(v), yyz); // (Mxy, y, z)
    auto max = _mm_min_ps(m_sse(v), xxz); // (x, mxy, z)
    auto mxy = _mm_blend_ps(min, max, 0b00'10); // (Mxy, mxy, z)

    /* sort lower elements */
    auto sh1 = _mm_shuffle_ps(mxy, mxy, static_cast<uint8_t>(Shuf3::xzz)); // (Mxy, z, z)
    auto sh2 = _mm_shuffle_ps(mxy, mxy, static_cast<uint8_t>(Shuf3::xyy)); // (Mxy, mxy, mxy)
    min = _mm_max_ps(mxy, sh1); // (Mxy, max(mxy,z), z)
    max = _mm_min_ps(mxy, sh2); // (Mxy, mxy, min(mxy,z))
    auto myz = _mm_blend_ps(min, max, 0b11'00); // (Mxy, max(mxy,z), min(mxy,z))

    /* sort upper elements again */
    yyz = _mm_shuffle_ps(myz, myz, static_cast<uint8_t>(Shuf3::yyz));
    xxz = _mm_shuffle_ps(myz, myz, static_cast<uint8_t>(Shuf3::xxz));
    min = _mm_max_ps(myz, yyz);
    max = _mm_min_ps(myz, xxz);

    return FSVF_RAW_(LVec3, _mm_blend_ps(min, max, 0b00'10));
  }
}

template<FastVector T> T
FSVF_FORCEINLINE FSVF_VECCALL floor(T v)
{
  return FSVF_RAW_(T, _mm_floor_ps(m_sse(v)));
}

template<FastVector T> T
FSVF_FORCEINLINE FSVF_VECCALL ceil(T v)
{
  return FSVF_RAW_(T, _mm_ceil_ps(m_sse(v)));
}

inline Mat4::Mat4(const float* p)
{
  cols[0] = Vec4{_mm_loadu_ps(&p[0])};
  cols[1] = Vec4{_mm_loadu_ps(&p[4])};
  cols[2] = Vec4{_mm_loadu_ps(&p[8])};
  cols[3] = Vec4{_mm_loadu_ps(&p[12])};
}

inline Mat4::Mat4(float x)
{
  cols[0] = Vec4(x,0,0,0);
  cols[1] = Vec4(0,x,0,0);
  cols[2] = Vec4(0,0,x,0);
  cols[3] = Vec4(0,0,0,x);
}

inline Mat4::Mat4(float x0, float x1, float x2, float x3,
                  float x4, float x5, float x6, float x7,
                  float x8, float x9, float xA, float xB,
                  float xC, float xD, float xE, float xF)
{
  cols[0] = Vec4(x0,x1,x2,x3);
  cols[1] = Vec4(x4,x5,x6,x7);
  cols[2] = Vec4(x8,x9,xA,xB);
  cols[3] = Vec4(xC,xD,xE,xF);
}

inline Mat4::Mat4(float d0, float d1, float d2, float d3)
{
  cols[0] = Vec4( d0,0.f,0.f,0.f);
  cols[1] = Vec4(0.f, d1,0.f,0.f);
  cols[2] = Vec4(0.f,0.f, d2,0.f);
  cols[3] = Vec4(0.f,0.f,0.f, d3);
}

inline LMat3::LMat3(float x0, float x1, float x2,
                    float x3, float x4, float x5,
                    float x6, float x7, float x8)
{
  cols[0] = Vec4(x0,x1,x2,0);
  cols[1] = Vec4(x3,x4,x5,0);
  cols[2] = Vec4(x6,x7,x8,0);
  cols[3] = Vec4(0, 0, 0, 1.f);
}

inline LMat3::LMat3(const float* m)
{
  cols[0] = Vec4(m[0]);
  cols[1] = Vec4(m[3]);
  cols[2] = Vec4(m[6]);
  cols[3] = Vec4(0, 0, 0, 1.f);
}

FSVF_FORCEINLINE Mat4 FSVF_VECCALL Mat4::transpose() const
{
  // for *this =
  // 0123
  // 4567
  // 89ab
  // cdef

  auto p0 = _mm_unpacklo_ps(cols[0].m_sse, cols[2].m_sse); // 0819
  auto p1 = _mm_unpacklo_ps(cols[1].m_sse, cols[3].m_sse); // 4c5d
  auto p2 = _mm_unpackhi_ps(cols[0].m_sse, cols[2].m_sse); // 2a3b
  auto p3 = _mm_unpackhi_ps(cols[1].m_sse, cols[3].m_sse); // 6e7f


  return Mat4{
    Vec4{_mm_unpacklo_ps(p0, p1)}, // 048c
    Vec4{_mm_unpackhi_ps(p0, p1)}, // 159d
    Vec4{_mm_unpacklo_ps(p2, p3)}, // 26ae
    Vec4{_mm_unpackhi_ps(p2, p3)}  // 37bf
  };
}

FSVF_FORCEINLINE float FSVF_VECCALL Mat4::affine_determinant() const
{
  // cross() guarantees the last component being 0 regardless of input
  LVec3 y = cross(cols[1],  cols[2]);

  // the last component of cols[0] won't contribute as it will be multiplied by 0
  return dot(cols[0], y);
}

FSVF_FORCEINLINE LMat3 FSVF_VECCALL LMat3::adj_transposed() const
{
    auto m0 = LVec3{cols[0]};
    auto m1 = LVec3{cols[1]};
    auto m2 = LVec3{cols[2]};

    auto s0 = shuffle<Shuf3::yzx>(m1);
    auto s1 = shuffle<Shuf3::zxy>(m2);
    auto s2 = shuffle<Shuf3::zxy>(m1);
    auto s3 = shuffle<Shuf3::yzx>(m2);

    const auto c0 = s0 * s1 - s2 * s3;

    s0 = shuffle<Shuf3::zxy>(m0);
    s1 = shuffle<Shuf3::yzx>(m2);
    s2 = shuffle<Shuf3::yzx>(m0);
    s3 = shuffle<Shuf3::zxy>(m2);

    const auto c1 = s0 * s1 - s2 * s3;

    s0 = shuffle<Shuf3::yzx>(m0);
    s1 = shuffle<Shuf3::zxy>(m1);
    s2 = shuffle<Shuf3::zxy>(m0);
    s3 = shuffle<Shuf3::yzx>(m1);

    const auto c2 = s0 * s1 - s2 * s3;

    return LMat3{c0, c1, c2};
}

inline LMat3 FSVF_VECCALL LMat3::adj() const
{
  return unsafe_construct<true>(adj_transposed().transpose());
}

inline LMat3 FSVF_VECCALL LMat3::inverse() const
{
    const float inv_det = 1.f / determinant();
    return unsafe_construct<true>(adj().scale_by(inv_det));
}

inline LMat3 FSVF_VECCALL LMat3::inverse_transposed() const
{
    const float inv_det = 1.f / determinant();
    return unsafe_construct<true>(adj_transposed().scale_by(inv_det));
}

inline std::optional<LMat3> FSVF_VECCALL LMat3::inverse_safe() const
{
  const float det = determinant();

  if (det == 0.f)
      return {};

  const float inv_det = 1.f / det;

  auto res = adj();
  res.cols[0] *= inv_det;
  res.cols[1] *= inv_det;
  res.cols[2] *= inv_det;
  return res;
}

inline std::optional<LMat3> FSVF_VECCALL LMat3::inverse_transposed_safe() const
{
  const float det = determinant();

  if (det == 0.f)
      return {};

  const float inv_det = 1.f / det;

  auto res = adj_transposed();
  res.cols[0] *= inv_det;
  res.cols[1] *= inv_det;
  res.cols[2] *= inv_det;
  return res;
}

// developed using Laplace expansion,
// see https://www.geometrictools.com/Documentation/LaplaceExpansionTheorem.pdf
// the notations c# and s# (for # = 0..5) follow those of the article above
//
// a b c d   |   a00 a01 a02 a03
// e f g h   |   a10 a11 a12 a13
// i j k l   |   a20 a21 a22 a23
// m n o p   |   a30 a31 a32 a33
#define LAPLACE_FACTORS \
const Vec4 c0p = shuffle<Shuf4::yxwz>(cols[0]); /* e a m i  |  a10 a00 a30 a20 */ \
const Vec4 c1p = shuffle<Shuf4::yxwz>(cols[1]); /* f b n j  |  a11 a01 a31 a21 */ \
const Vec4 c2p = shuffle<Shuf4::yxwz>(cols[2]); /* g c o k  |  a12 a02 a32 a22 */ \
const Vec4 c3p = shuffle<Shuf4::yxwz>(cols[3]); /* h d p l  |  a13 a03 a33 a23 */ \
                                                                                  \
const Vec4 m0 = cols[0] * c1p;                  /* af be in jm  |  s0, c0 */      \
const Vec4 m1 = cols[2] * c0p;                  /* ce ag km io  | -s1,-c1 */      \
const Vec4 m2 = cols[0] * c3p;                  /* ah de ip lm  |  s2, c2 */      \
const Vec4 m3 = cols[2] * c1p;                  /* cf bg kn jo  | -s3,-c3 */      \
const Vec4 m4 = cols[3] * c1p;                  /* df bh ln jp  | -s4,-c4 */      \
const Vec4 m5 = cols[2] * c3p;                  /* ch dg kp lo  |  s5, c5 */      \
                                                                                  \
const Vec4 s23a = _mm_shuffle_ps(m2, m3, Internals::to_underlying(Shuf4::xzwy));  /* ah ip jo bg */ \
const Vec4 s23b = _mm_shuffle_ps(m2, m3, Internals::to_underlying(Shuf4::ywzx));  /* de lm kn cf */ \
const Vec4 s54a = _mm_shuffle_ps(m5, m4, Internals::to_underlying(Shuf4::zxwy));  /* kp ch jp bh */ \
const Vec4 s54b = _mm_shuffle_ps(m5, m4, Internals::to_underlying(Shuf4::wyzx));  /* lo dg ln df */ \
                                                                                  \
const Vec4 d2d3 = s23a - s23b; /* ah-de  ip-lm  jo-kn  bg-cf  |  s2 c2 c3 s3 */   \
const Vec4 d5d4 = s54a - s54b; /* kp-lo  ch-dg  jp-ln  bh-df  |  c5 s5 c4 s4 */   \


FSVF_FORCEINLINE float FSVF_VECCALL Mat4::determinant() const
{
  LAPLACE_FACTORS

  const Vec4 s01a = _mm_shuffle_ps(m0, m1, Internals::to_underlying(Shuf4::xzxz)); // af in ce km
  const Vec4 s01b = _mm_shuffle_ps(m0, m1, Internals::to_underlying(Shuf4::ywyw)); // be jm ag io

  const Vec4 d0d1 = s01a - s01b; // af-be  in-jm  ce-ag  km-io  // s0 c0 -s1 -c1
                                 //
  const Vec4 aux23 = _mm_movehl_ps(Vec4::zeroes(), d2d3); // c3 s3 0 0

  const Vec4 s0 = d0d1 * d5d4;  // s0c5       s5c0        -s1c4  -s4c1
  const Vec4 s1 = d2d3 * aux23; // s2c3       s3c2         0      0
  const Vec4 s2 = s0 + s1;      // s0c5+s2c3  s5c0+s3c2   -s1s4  -s4c1

  return sum(s2);
}

FSVF_FORCEINLINE Mat4 FSVF_VECCALL Mat4::adj_transposed() const
{
  LAPLACE_FACTORS

  const Vec4 s01a = _mm_shuffle_ps(m0, m1, Internals::to_underlying(Shuf4::xzyw)); // af in ag io
  const Vec4 s01b = _mm_shuffle_ps(m0, m1, Internals::to_underlying(Shuf4::ywxz)); // be jm ce km

  const Vec4 d0d1 = s01a - s01b; // af-be  in-jm  ag-ce  io-km  | s0 c0 s1 c1

  const Vec4 nd0d1 = -d0d1; // -s0 -c0 -s1 -c1
  const Vec4 nd2d3 = -d2d3; // -s2 -c2 -c3 -s3
  const Vec4 nd5d4 = -d5d4; // -c5 -s5 -c4 -s4

  const Vec4 d0 = _mm_shuffle_ps(d0d1, nd0d1, Internals::to_underlying(Shuf4::yxyz)); // c0 s0 -c0 -s0
  const Vec4 d1 = _mm_shuffle_ps(d0d1, nd0d1, Internals::to_underlying(Shuf4::wzwz)); // c1 s1 -c1 -s1
  const Vec4 d2 = _mm_shuffle_ps(d2d3, nd2d3, Internals::to_underlying(Shuf4::yxyx)); // c2 s2 -c2 -s2
  const Vec4 d3 = _mm_shuffle_ps(d2d3, nd2d3, Internals::to_underlying(Shuf4::zwzw)); // c3 s3 -c3 -s3
  const Vec4 d4 = _mm_shuffle_ps(d5d4, nd5d4, Internals::to_underlying(Shuf4::zwzw)); // c4 s4 -c4 -s4
  const Vec4 d5 = _mm_shuffle_ps(d5d4, nd5d4, Internals::to_underlying(Shuf4::xyxy)); // c5 s5 -c5 -s5

  const Vec4 r0a = c1p * d5; // a11*c5, -a01*c5, a31*s5, -a21*s5
  const Vec4 r0b = c2p * d4; // a12*c4, -a02*c4, a32*s4, -a22*s4
  const Vec4 r0c = c3p * d3; // a13*c3, -a03*c3, a33*s3, -a23*s3

  const Vec4 row0 = r0a - r0b + r0c;

  const Vec4 r1a = c0p * d5; // a10*c5 -a00*c5 a30*s5 -a20*s5
  const Vec4 r1b = c2p * d2; // a12*c2 -a02*c2 a32*s2 -a22*s2
  const Vec4 r1c = c3p * d1; // a13*c1 -a03*c1 a33*s1 -a23*s1

  const Vec4 row1 = r1b - r1a - r1c;

  const Vec4 r2a = c0p * d4; // a10*c4 -a00*c4 a30*s4 -a20*s4
  const Vec4 r2b = c1p * d2; // a11*c2 -a01*c2 a31*s2 -a21*s2
  const Vec4 r2c = c3p * d0; // a13*c0 -a03*c0 a33*s0 -a23*s0

  const Vec4 row2 = r2a - r2b + r2c;

  const Vec4 r3a = c0p * d3; // a10*c3 -a00*c3 a30*s3 -a20*s3
  const Vec4 r3b = c1p * d1; // a11*c1 -a01*c1 a31*s1 -a21*s1
  const Vec4 r3c = c2p * d0; // a12*c0 -a02*c0 a32*s0 -a22*s0

  const Vec4 row3 = r3b - r3a - r3c;

  return Mat4{ row0, row1, row2, row3 };
}

FSVF_FORCEINLINE Mat4 FSVF_VECCALL Mat4::scale_by(Vec4 v) const
{
  return Mat4{ v * cols[0]
             , v * cols[1]
             , v * cols[2]
             , v * cols[3]};
}

FSVF_FORCEINLINE Mat4 FSVF_VECCALL Mat4::scale_by_inv(Vec4 v) const
{
  return Mat4{ v / cols[0]
             , v / cols[1]
             , v / cols[2]
             , v / cols[3]};
}

inline Mat4 FSVF_VECCALL Mat4::scale_by(float x) const
{
  return scale_by(Vec4{x});
}

inline Mat4 FSVF_VECCALL Mat4::scale_by_inv(float x) const
{
  return scale_by_inv(Vec4{x});
}

inline Mat4 FSVF_VECCALL Mat4::inverse_transposed() const
{
#ifdef FSVF_NO_RECIPROCAL_MATH
  return adj_transposed().scale_by_inv(determinant());
#else
  return adj_transposed().scale_by(1.f / determinant());
#endif
}

inline Mat4 FSVF_VECCALL Mat4::inverse() const {
  // funnily enough, MSVC will inline the call to `adj()` below, but
  // won't inline the call to `inverse_transposed()` in:
  // return inverse_transposed().transpose();
#ifdef FSVF_NO_RECIPROCAL_MATH
  return adj().scale_by_inv(determinant());
#else
  return adj().scale_by(1.f / determinant());
#endif
}

inline std::optional<Mat4> FSVF_VECCALL Mat4::inverse_safe() const
{
  const float det = determinant();

  if (det == 0.f)
      return {};

#ifdef FSVF_NO_RECIPROCAL_MATH
  return std::optional<Mat4>(adj().scale_by_inv(det));
#else
  const float inv_det = 1.f / det;
  return std::optional<Mat4>(adj().scale_by(inv_det));
#endif
}

inline std::optional<Mat4> FSVF_VECCALL Mat4::inverse_transposed_safe() const
{
  const float det = determinant();

  if (det == 0.f)
      return {};

#ifdef FSVF_NO_RECIPROCAL_MATH
  return std::optional<Mat4>(adj_transposed().scale_by_inv(det));
#else
  const float inv_det = 1.f / det;
  return std::optional<Mat4>(adj_transposed().scale_by(inv_det));
#endif
}

inline Mat4 FSVF_VECCALL Mat4::operator*(Mat4 m) const
{
  const Vec4 w0x = shuffle<Shuf4::xxxx>(m.cols[0]);
  const Vec4 w0y = shuffle<Shuf4::yyyy>(m.cols[0]);
  const Vec4 w0z = shuffle<Shuf4::zzzz>(m.cols[0]);
  const Vec4 w0w = shuffle<Shuf4::wwww>(m.cols[0]);

  const Vec4 c00 = cols[0] * w0x;
  const Vec4 c01 = cols[1] * w0y;
  const Vec4 c02 = cols[2] * w0z;
  const Vec4 c03 = cols[3] * w0w;

  const Vec4 res0 = (c00 + c01) + (c02 + c03);

  const Vec4 w1x = shuffle<Shuf4::xxxx>(m.cols[1]);
  const Vec4 w1y = shuffle<Shuf4::yyyy>(m.cols[1]);
  const Vec4 w1z = shuffle<Shuf4::zzzz>(m.cols[1]);
  const Vec4 w1w = shuffle<Shuf4::wwww>(m.cols[1]);

  const Vec4 c10 = cols[0] * w1x;
  const Vec4 c11 = cols[1] * w1y;
  const Vec4 c12 = cols[2] * w1z;
  const Vec4 c13 = cols[3] * w1w;

  const Vec4 res1 = (c10 + c11) + (c12 + c13);

  const Vec4 w2x = shuffle<Shuf4::xxxx>(m.cols[2]);
  const Vec4 w2y = shuffle<Shuf4::yyyy>(m.cols[2]);
  const Vec4 w2z = shuffle<Shuf4::zzzz>(m.cols[2]);
  const Vec4 w2w = shuffle<Shuf4::wwww>(m.cols[2]);

  const Vec4 c20 = cols[0] * w2x;
  const Vec4 c21 = cols[1] * w2y;
  const Vec4 c22 = cols[2] * w2z;
  const Vec4 c23 = cols[3] * w2w;

  const Vec4 res2 = (c20 + c21) + (c22 + c23);

  const Vec4 w3x = shuffle<Shuf4::xxxx>(m.cols[3]);
  const Vec4 w3y = shuffle<Shuf4::yyyy>(m.cols[3]);
  const Vec4 w3z = shuffle<Shuf4::zzzz>(m.cols[3]);
  const Vec4 w3w = shuffle<Shuf4::wwww>(m.cols[3]);

  const Vec4 c30 = cols[0] * w3x;
  const Vec4 c31 = cols[1] * w3y;
  const Vec4 c32 = cols[2] * w3z;
  const Vec4 c33 = cols[3] * w3w;

  const Vec4 res3 = (c30 + c31) + (c32 + c33);

  return Mat4(res0, res1, res2, res3);
}

template <typename T>
  requires FastVector<T> && (!std::derived_from<T, LVec3>)
inline T FSVF_VECCALL Mat4::operator*(T v) const {
  // transpose-multiply:
  const Mat4 tm = transpose();
  const Vec4 c0 = tm.cols[0] * v;
  const Vec4 c1 = tm.cols[1] * v;
  const Vec4 c2 = tm.cols[2] * v;
  const Vec4 c3 = tm.cols[3] * v;

  const Vec4 res{Internals::reduceAll(INTR_WRAP(_mm_add_ps), c0, c1, c2, c3)};

  return T{res};
}

template <std::derived_from<LVec3> T>
inline T FSVF_VECCALL Mat4::transform_3x3(T v) const
{
  // TODO check best approach (transpose-mul, using FMA and combinations of these)
  const auto vx = shuffle<Shuf3::xxx>(v);
  const auto vy = shuffle<Shuf3::yyy>(v);
  const auto vz = shuffle<Shuf3::zzz>(v);

  const auto c0 = cols[0] * vx;
  const auto c1 = cols[1] * vy;
  const auto c2 = cols[2] * vz;

  return T{c0 + c1 + c2};
}

template <std::derived_from<LVec3> T>
inline T FSVF_VECCALL Mat4::transform_affine(T v) const
{
  // TODO check best
  const auto vx = shuffle<Shuf3::xxx>(v);
  const auto vy = shuffle<Shuf3::yyy>(v);
  const auto vz = shuffle<Shuf3::zzz>(v);

  const auto c0 = cols[0] * vx;
  const auto c1 = cols[1] * vy;
  /* TODO try FMA? */
  const auto c2 = cols[2] * vz;
  const auto res = (c0 + c1) + (c2 + LVec3{cols[3]});
  /******************/

  return res;
}

template <std::derived_from<LVec3> T>
inline T FSVF_VECCALL Mat4::transform_projective(T v) const
{
  // TODO check best
  const Vec4 vx = shuffle<Shuf4::xxxx>(v);
  const Vec4 vy = shuffle<Shuf4::yyyy>(v);
  const Vec4 vz = shuffle<Shuf4::zzzz>(v);

  const Vec4 c0 = cols[0] * vx;
  const Vec4 c1 = cols[1] * vy;
  const Vec4 c2 = cols[2] * vz;
  const Vec4 res = (c0 + c1) + (c2 + cols[3]);
  /******************/
  const Vec4 scale = shuffle<Shuf4::wwww>(res);

  // TODO conditional vs unconditional division?
  // TODO optimize constant
  if (scale == Vec4(1.f))
    return LVec3{res};

  return LVec3{res / scale};
}

inline Mat4 FSVF_VECCALL Mat4::mul_transp(Mat4 m) const
{
  Vec4 p0 = cols[0] * m.cols[0];
  Vec4 p1 = cols[1] * m.cols[0];
  Vec4 p2 = cols[2] * m.cols[0];
  Vec4 p3 = cols[3] * m.cols[0];

  const Vec4 res0{Internals::reduceAll(INTR_WRAP(_mm_add_ps),
      p0.m_sse, p1.m_sse, p2.m_sse, p3.m_sse)};

  p0 = cols[0] * m.cols[1];
  p1 = cols[1] * m.cols[1];
  p2 = cols[2] * m.cols[1];
  p3 = cols[3] * m.cols[1];

  const Vec4 res1{Internals::reduceAll(INTR_WRAP(_mm_add_ps),
      p0.m_sse, p1.m_sse, p2.m_sse, p3.m_sse)};

  p0 = cols[0] * m.cols[2];
  p1 = cols[1] * m.cols[2];
  p2 = cols[2] * m.cols[2];
  p3 = cols[3] * m.cols[2];

  const Vec4 res2{Internals::reduceAll(INTR_WRAP(_mm_add_ps),
      p0.m_sse, p1.m_sse, p2.m_sse, p3.m_sse)};

  p0 = cols[0] * m.cols[3];
  p1 = cols[1] * m.cols[3];
  p2 = cols[2] * m.cols[3];
  p3 = cols[3] * m.cols[3];

  const Vec4 res3{Internals::reduceAll(INTR_WRAP(_mm_add_ps),
      p0.m_sse, p1.m_sse, p2.m_sse, p3.m_sse)};

  return Mat4{res0, res1, res2, res3};
}

inline Mat4 operator+(const Mat4& m1, const Mat4& m2)
{
  Mat4 res{
    m1.cols[0] + m2.cols[0],
    m1.cols[1] + m2.cols[1],
    m1.cols[2] + m2.cols[2],
    m1.cols[3] + m2.cols[3]
  };
  return res;
}

inline Mat4 operator-(const Mat4& m1, const Mat4& m2)
{
  Mat4 res{
    m1.cols[0] - m2.cols[0],
    m1.cols[1] - m2.cols[1],
    m1.cols[2] - m2.cols[2],
    m1.cols[3] - m2.cols[3]
  };
  return res;
}

inline Mat4& operator+=(Mat4& m1, const Mat4& m2)
{
  Mat4 res{
    m1.cols[0] += m2.cols[0],
    m1.cols[1] += m2.cols[1],
    m1.cols[2] += m2.cols[2],
    m1.cols[3] += m2.cols[3]
  };
  return m1;
}

inline Mat4& operator-=(Mat4& m1, const Mat4& m2)
{
  Mat4 res{
    m1.cols[0] -= m2.cols[0],
    m1.cols[1] -= m2.cols[1],
    m1.cols[2] -= m2.cols[2],
    m1.cols[3] -= m2.cols[3]
  };
  return m1;
}

template <bool EnableUnsafeConstruction, typename T>
inline LMat3 FSVF_VECCALL LMat3::unsafe_construct(Mat4 m)
{
  return LMat3{
    m_sse(m[0]),
    m_sse(m[1]),
    m_sse(m[2]),
    m_sse(m[3])
  };
}

// TODO USE _mm_insrt_epi32() and similar!!!!
// TODO add missing FSVF_VECCALLs
// TODO FSVF_FORCEINLINE almost everything
} // namespace FSVF

// cleanup internal macros
#ifdef FSVF_RAW_
#undef FSVF_RAW_
#endif
