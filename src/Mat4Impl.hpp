#pragma once

#include "GenericMethodsImpl.hpp"
#include "LMat3.hpp"

namespace FSVF
{

FSVF_FORCEINLINE Mat4::Mat4(LMat3 m)
  : cols{ m.cols }
{}

FSVF_FORCEINLINE Mat4::Mat4(const float* p)
{
  cols[0] = Vec4{ _mm_loadu_ps(&p[0]) };
  cols[1] = Vec4{ _mm_loadu_ps(&p[4]) };
  cols[2] = Vec4{ _mm_loadu_ps(&p[8]) };
  cols[3] = Vec4{ _mm_loadu_ps(&p[12]) };
}

FSVF_FORCEINLINE Mat4::Mat4(float x)
{
  cols[0] = Vec4(x, 0, 0, 0);
  cols[1] = Vec4(0, x, 0, 0);
  cols[2] = Vec4(0, 0, x, 0);
  cols[3] = Vec4(0, 0, 0, x);
}

// clang-format off
FSVF_FORCEINLINE Mat4::Mat4(
    float x0, float x1, float x2, float x3,
    float x4, float x5, float x6, float x7,
    float x8, float x9, float xA, float xB,
    float xC, float xD, float xE, float xF)
// clang-format on
{
  cols[0] = Vec4(x0, x1, x2, x3);
  cols[1] = Vec4(x4, x5, x6, x7);
  cols[2] = Vec4(x8, x9, xA, xB);
  cols[3] = Vec4(xC, xD, xE, xF);
}

FSVF_FORCEINLINE Mat4::Mat4(float d0, float d1, float d2, float d3)
{
  cols[0] = Vec4(d0, 0.f, 0.f, 0.f);
  cols[1] = Vec4(0.f, d1, 0.f, 0.f);
  cols[2] = Vec4(0.f, 0.f, d2, 0.f);
  cols[3] = Vec4(0.f, 0.f, 0.f, d3);
}

FSVF_FORCEINLINE Mat4 FSVF_VECCALL Mat4::transpose() const
{
  // for *this =
  // 0123
  // 4567
  // 89ab
  // cdef

  auto p0 = _mm_unpacklo_ps(cols[0].m_sse, cols[2].m_sse);    // 0819
  auto p1 = _mm_unpacklo_ps(cols[1].m_sse, cols[3].m_sse);    // 4c5d
  auto p2 = _mm_unpackhi_ps(cols[0].m_sse, cols[2].m_sse);    // 2a3b
  auto p3 = _mm_unpackhi_ps(cols[1].m_sse, cols[3].m_sse);    // 6e7f


  return Mat4{
    Vec4{ _mm_unpacklo_ps(p0, p1) },    // 048c
    Vec4{ _mm_unpackhi_ps(p0, p1) },    // 159d
    Vec4{ _mm_unpacklo_ps(p2, p3) },    // 26ae
    Vec4{ _mm_unpackhi_ps(p2, p3) }     // 37bf
  };
}

FSVF_FORCEINLINE float FSVF_VECCALL Mat4::determinant_3x3() const
{
  // cross() guarantees the last component being 0 regardless of input
  LVec3 y = cross(cols[1], cols[2]);

  // the last component of cols[0] won't contribute as it will be multiplied by 0
  return dot(cols[0], y);
}

FSVF_FORCEINLINE Mat4 operator+(const Mat4& m1, const Mat4& m2)
{
  Mat4 res{ m1.cols[0] + m2.cols[0],
            m1.cols[1] + m2.cols[1],
            m1.cols[2] + m2.cols[2],
            m1.cols[3] + m2.cols[3] };
  return res;
}

FSVF_FORCEINLINE Mat4 operator-(const Mat4& m1, const Mat4& m2)
{
  Mat4 res{ m1.cols[0] - m2.cols[0],
            m1.cols[1] - m2.cols[1],
            m1.cols[2] - m2.cols[2],
            m1.cols[3] - m2.cols[3] };
  return res;
}

FSVF_FORCEINLINE Mat4& operator+=(Mat4& m1, const Mat4& m2)
{
  m1.cols[0] += m2.cols[0];
  m1.cols[1] += m2.cols[1];
  m1.cols[2] += m2.cols[2];
  m1.cols[3] += m2.cols[3];
  return m1;
}

FSVF_FORCEINLINE Mat4& operator-=(Mat4& m1, const Mat4& m2)
{
  m1.cols[0] -= m2.cols[0];
  m1.cols[1] -= m2.cols[1];
  m1.cols[2] -= m2.cols[2];
  m1.cols[3] -= m2.cols[3];
  return m1;
}

FSVF_FORCEINLINE Mat4 FSVF_VECCALL Mat4::scale_by(Vec4 v) const
{
  return Mat4{ v * cols[0], v * cols[1], v * cols[2], v * cols[3] };
}

FSVF_FORCEINLINE Mat4 FSVF_VECCALL Mat4::scale_by_inv(Vec4 v) const
{
  return Mat4{ cols[0] / v, cols[1] / v, cols[2] / v, cols[3] / v };
}

FSVF_FORCEINLINE Mat4 FSVF_VECCALL Mat4::scale_by(float x) const
{
  return scale_by(Vec4{ x });
}

FSVF_FORCEINLINE Mat4 FSVF_VECCALL Mat4::scale_by_inv(float x) const
{
  return scale_by_inv(Vec4{ x });
}

FSVF_FORCEINLINE Mat4 FSVF_VECCALL operator*(Mat4 m, float x)
{
  return m.scale_by(x);
}

FSVF_FORCEINLINE Mat4 FSVF_VECCALL operator*(float x, Mat4 m)
{
  return m.scale_by(x);
}

FSVF_FORCEINLINE Mat4 FSVF_VECCALL operator/(Mat4 m, float x)
{
  return m.scale_by_inv(x);
}

FSVF_FORCEINLINE Mat4 FSVF_VECCALL operator/(float x, Mat4 m)
{
  Vec4 v{ x };
  return Mat4{ v / m.cols[0], v / m.cols[1], v / m.cols[2], v / m.cols[3] };
}

template<typename T>
  requires FastVector<T> && (!std::derived_from<T, LVec3>)
FSVF_FORCEINLINE T FSVF_VECCALL Mat4::operator*(T v) const
{
  const Vec4 n0x = shuffle<Shuf4::xxxx>(v);
  const Vec4 n0y = shuffle<Shuf4::yyyy>(v);
  const Vec4 n0z = shuffle<Shuf4::zzzz>(v);
  const Vec4 n0w = shuffle<Shuf4::wwww>(v);

  const Vec4 c0 = cols[0] * n0x;
  const Vec4 c1 = cols[1] * n0y;
  const Vec4 c2 = cols[2] * n0z;
  const Vec4 c3 = cols[3] * n0w;

  const Vec4 res = (c0 + c1) + (c2 + c3);

  return T{ res };
}

// TODO benchmark pass-by-value vs pass-by-reference
// TODO benchmark transpose-first vs direct
FSVF_FORCEINLINE Mat4 FSVF_VECCALL operator*(const Mat4 m, const Mat4 n)
{
  /*
    a00 a01 a02 a03    b00 b01 b02 b03
    a10 a11 a12 a13    b10 b11 b12 b13
    a20 a21 a22 a23    b20 b21 b22 b23
    a30 a31 a32 a33    b30 b31 b32 b33

    matrix:
    a00 b00 + a01 b10 + a02 b20 + a03 b30   a00 b01 + a01 b11 + a02 b21 + a03 b31   a00 b00 + a01 b10 + a02 b20 + a03 b30   a00 b01 + a01 b11 + a02 b21 + a03 b31
    a10 b00 + a11 b10 + a12 b20 + a13 b30   a10 b01 + a11 b11 + a12 b21 + a13 b31   a10 b00 + a11 b10 + a12 b20 + a13 b30   a10 b01 + a11 b11 + a12 b21 + a13 b31
    a20 b00 + a21 b10 + a22 b20 + a23 b30   a20 b01 + a21 b11 + a22 b21 + a23 b31   a20 b00 + a21 b10 + a22 b20 + a23 b30   a20 b01 + a21 b11 + a22 b21 + a23 b31
    a30 b00 + a31 b10 + a32 b20 + a33 b30   a30 b01 + a31 b11 + a32 b21 + a33 b31   a30 b00 + a31 b10 + a32 b20 + a33 b30   a30 b01 + a31 b11 + a32 b21 + a33 b31


    Mcol 0:
    c0 * [b00] + c1 * [b10] + c2 * [b20] + c3 * [b30]
  */

  //const auto t = transpose();

  //const float v00 = dot(t.cols[0], m.cols[0]);
  //const float v10 = dot(t.cols[1], m.cols[0]);
  //const float v20 = dot(t.cols[2], m.cols[0]);
  //const float v30 = dot(t.cols[3], m.cols[0]);

  //const float v01 = dot(t.cols[0], m.cols[1]);
  //const float v11 = dot(t.cols[1], m.cols[1]);
  //const float v21 = dot(t.cols[2], m.cols[1]);
  //const float v31 = dot(t.cols[3], m.cols[1]);

  //const float v02 = dot(t.cols[0], m.cols[2]);
  //const float v12 = dot(t.cols[1], m.cols[2]);
  //const float v22 = dot(t.cols[2], m.cols[2]);
  //const float v32 = dot(t.cols[3], m.cols[2]);

  //const float v03 = dot(t.cols[0], m.cols[3]);
  //const float v13 = dot(t.cols[1], m.cols[3]);
  //const float v23 = dot(t.cols[2], m.cols[3]);
  //const float v33 = dot(t.cols[3], m.cols[3]);

  //return Mat4(
  //      Vec4(v00,v10,v20,v30),
  //      Vec4(v01,v11,v21,v31),
  //      Vec4(v02,v12,v22,v32),
  //      Vec4(v03,v13,v23,v33)
  //  );

  const Vec4 n0x = shuffle<Shuf4::xxxx>(n.cols[0]);
  const Vec4 n0y = shuffle<Shuf4::yyyy>(n.cols[0]);
  const Vec4 n0z = shuffle<Shuf4::zzzz>(n.cols[0]);
  const Vec4 n0w = shuffle<Shuf4::wwww>(n.cols[0]);

  const Vec4 c01 = m.cols[1] * n0y;
  const Vec4 c03 = m.cols[3] * n0w;

  const Vec4 fm00 = fmadd(m.cols[0], n0x, c01);
  const Vec4 fm01 = fmadd(m.cols[2], n0z, c03);

  const Vec4 res0 = fm00 + fm01;

  const Vec4 n1x = shuffle<Shuf4::xxxx>(n.cols[1]);
  const Vec4 n1y = shuffle<Shuf4::yyyy>(n.cols[1]);
  const Vec4 n1z = shuffle<Shuf4::zzzz>(n.cols[1]);
  const Vec4 n1w = shuffle<Shuf4::wwww>(n.cols[1]);

  const Vec4 c11 = m.cols[1] * n1y;
  const Vec4 c13 = m.cols[3] * n1w;

  const Vec4 fm10 = fmadd(m.cols[0], n1x, c11);
  const Vec4 fm11 = fmadd(m.cols[2], n1z, c13);

  const Vec4 res1 = fm10 + fm11;

  const Vec4 n2x = shuffle<Shuf4::xxxx>(n.cols[2]);
  const Vec4 n2y = shuffle<Shuf4::yyyy>(n.cols[2]);
  const Vec4 n2z = shuffle<Shuf4::zzzz>(n.cols[2]);
  const Vec4 n2w = shuffle<Shuf4::wwww>(n.cols[2]);

  const Vec4 c21 = m.cols[1] * n2y;
  const Vec4 c23 = m.cols[3] * n2w;

  const Vec4 fm20 = fmadd(m.cols[0], n2x, c21);
  const Vec4 fm21 = fmadd(m.cols[2], n2z, c23);

  const Vec4 res2 = fm20 + fm21;

  const Vec4 n3x = shuffle<Shuf4::xxxx>(n.cols[3]);
  const Vec4 n3y = shuffle<Shuf4::yyyy>(n.cols[3]);
  const Vec4 n3z = shuffle<Shuf4::zzzz>(n.cols[3]);
  const Vec4 n3w = shuffle<Shuf4::wwww>(n.cols[3]);

  const Vec4 c31 = m.cols[1] * n3y;
  const Vec4 c33 = m.cols[3] * n3w;

  const Vec4 fm30 = fmadd(m.cols[0], n3x, c31);
  const Vec4 fm31 = fmadd(m.cols[2], n3z, c33);

  const Vec4 res3 = fm30 + fm31;

  return Mat4(res0, res1, res2, res3);
}

FSVF_FORCEINLINE float FSVF_VECCALL Mat4::determinant() const
{
  const Vec4 v0p0 = shuffle<Shuf4::yxwz>(cols[0]); /* b a d c */
  const Vec4 v2p0 = shuffle<Shuf4::yxwz>(cols[2]); /* j i l k */
  const Vec4 v3p0 = shuffle<Shuf4::yxwz>(cols[3]); /* n m p o */

  const Vec4 m01 = v0p0 * cols[1];                 /* be af dg ch */
  const Vec4 m02 = v0p0 * cols[2];                 /* bi aj dk cl */
  const Vec4 m03 = v0p0 * cols[3];                 /* bm an do cp */
  const Vec4 m13 = v3p0 * cols[1];                 /* ne mf pg oh */
  const Vec4 m23 = v3p0 * cols[2];                 /* ni mj pk ol */
  const Vec4 m12 = v2p0 * cols[1];                 /* ej fi gl hk */

#if 0
  const Vec4 u01 = shuffle<Shuf4::yyxx>(m01);      /* af af be be */
  const Vec4 u02 = shuffle<Shuf4::yyxx>(m02);      /* aj aj bi bi */
  const Vec4 u03 = shuffle<Shuf4::yyxx>(m03);      /* an an bm bm */
  const Vec4 u13 = shuffle<Shuf4::yyxx>(m13);      /* fm fm en en */
  const Vec4 u23 = shuffle<Shuf4::yyxx>(m23);      /* jm jm in in */
  const Vec4 u12 = shuffle<Shuf4::yyxx>(m12);      /* fi fi ej ej */

  const Vec4 d01 = shuffle<Shuf4::zwzw>(m01);      /* dg ch dg ch */
  const Vec4 d02 = shuffle<Shuf4::zwzw>(m02);      /* dk cl dk cl */
  const Vec4 d03 = shuffle<Shuf4::zwzw>(m03);      /* do cp do cp */
  const Vec4 d13 = shuffle<Shuf4::zwzw>(m13);      /* gp ho gp ho */
  const Vec4 d23 = shuffle<Shuf4::zwzw>(m23);      /* kp lo kp lo */
  const Vec4 d12 = shuffle<Shuf4::zwzw>(m12);      /* gl hk gl hk */

  // afkp - aflo - bekp + belo - (ajgp - ajho - bigp + biho) + angl - anhk - bmgl + bmhk +
  // dofi - cpfi - doej + cpej - (dkfm - clfm - dken + clen) + dgjm - chjm - dgin + chin

  const Vec4 p0123 = u01 * d23; /* afkp aflo bekp belo */
  const Vec4 p0213 = u02 * d13; /* ajgp ajho bigp biho */
  const Vec4 p0312 = u03 * d12; /* angl anhk bmgl bmhk */
  const Vec4 p1203 = u12 * d03; /* dofi cpfi doej cpej */
  const Vec4 p1302 = u13 * d02; /* dkfm clfm dken clen */
  const Vec4 p2301 = u23 * d01; /* dgjm chjm dgin chin */

  const Vec4 mask = Vec4{ 0.f, -0.f, -0.f, 0.f };

  const Vec4 s0123 = mask ^ p0123;
  const Vec4 s0312 = mask ^ p0312;
  const Vec4 s1203 = mask ^ p1203;
  const Vec4 s2301 = mask ^ p2301;
  const Vec4 s0213 = mask ^ p0213;
  const Vec4 s1302 = mask ^ p1302;

  const Vec4 acc = (s0123 + s0312) + (s1203 + s2301) - (s0213 + s1302);

  return sum(acc);
#else
                                              /*  -  -  -  - */
  const Vec4 bl0 = _mm_unpacklo_ps(m01, m12); /* be ej af fi */
  const Vec4 bh0 = _mm_unpackhi_ps(m23, m03); /* kp do lo cp */

                                              /*  +  -  +  - */
  const Vec4 bl1 = _mm_unpacklo_ps(m02, m23); /* bi in aj jm */
  const Vec4 bh1 = _mm_unpackhi_ps(m13, m01); /* gp dg ho ch */

                                              /*  -  +  -  + */
  const Vec4 bl2 = _mm_unpacklo_ps(m03, m13); /* bm en an fm */
  const Vec4 bh2 = _mm_unpackhi_ps(m12, m02); /* gl dk hk cl */

                                               /*  +  +  +  + */
  //                                     (bl0) /* be ej af fi */
  const Vec4 bh0s = shuffle<Shuf4::zwxy>(bh0); /* lo cp kp do */

                                               /*  -  +  -  + */
  //                                     (bl1) /* bi in aj jm */
  const Vec4 bh1s = shuffle<Shuf4::zwxy>(bh1); /* ho ch gp dg */

                                               /*  +  -  +  - */
  //                                     (bh2) /* gl dk hk cl */
  const Vec4 bl2s = shuffle<Shuf4::zwxy>(bl2); /* an fm bm en */

  const Vec4 m0 = bl1 * bh1;
  const Vec4 fm0 = fmaddsub(bl0, bh0s, m0);
  // belo + bigp, cpej - dgin, afkp + ajho, dofi - chjm
  // (correct signs)

  const Vec4 m1 = bl2 * bh2;
  const Vec4 nfm1 = fmaddsub(bl0, bh0, m1);
  // bekp + bmgl, doej - dken, aflo + anhk, cpfi - clfm
  // (inverse signs)

  const float res0 = sum(fm0 - nfm1);

  const Vec4 m2 = bl1 * bh1s;
  const Vec4 sfm2 = fmsub(bh2, bl2s, m2);
  // angl - biho, dkfm - chin, bmhk - ajgp, clen - dgjm
  // (0,2: correct signs & 1,3: inverse signs)

  const Vec4 shuf = shuffle<Shuf4::yyww>(sfm2);
  const float res1 = sum(sfm2 - shuf);
  // angl - biho - dkfm + chin
  // 0
  // bmhk - ajgp - clen + dgjm
  // 0

  return res0 + res1;
#endif
}

FSVF_FORCEINLINE Mat4 FSVF_VECCALL Mat4::adj_transposed() const
{
  /* a e i m
     b f j n
     c g k o
     d h l p */

  const Vec4 v0p0 = shuffle<Shuf4::yxwz>(cols[0]); /* b a d c */
  const Vec4 v1p0 = shuffle<Shuf4::yxwz>(cols[1]); /* f e h g */
  const Vec4 v2p0 = shuffle<Shuf4::yxwz>(cols[2]); /* j i l k */
  const Vec4 v3p0 = shuffle<Shuf4::yxwz>(cols[3]); /* n m p o */

  const Vec4 m01 = v0p0 * cols[1];                 /* be af dg ch */
  const Vec4 m02 = v0p0 * cols[2];                 /* bi aj dk cl */
  const Vec4 m03 = v0p0 * cols[3];                 /* bm an do cp */
  const Vec4 m12 = v2p0 * cols[1];                 /* ej fi gl hk */
  const Vec4 m13 = v3p0 * cols[1];                 /* en fm gp ho */
  const Vec4 m23 = v3p0 * cols[2];                 /* in jm kp lo */

  const Vec4 p0 = shuffle<Shuf4::yxwz>(m01);       /* af be ch dg */
  const Vec4 p1 = shuffle<Shuf4::yxwz>(m02);       /* aj bi cl dk */
  const Vec4 p2 = shuffle<Shuf4::yxwz>(m03);       /* an bm cp do */
  const Vec4 p3 = shuffle<Shuf4::yxwz>(m12);       /* fi ej hk gl */
  const Vec4 p4 = shuffle<Shuf4::yxwz>(m13);       /* fm en ho gp */
  const Vec4 p5 = shuffle<Shuf4::yxwz>(m23);       /* jm in lo kp */

  /* af-be be-af ch-dg dg-ch */
  /* aj-bi bi-aj cl-dk dk-cl */
  /* an-bm bm-an cp-do do-cp */
  /* ej-fi fi-ej gl-hk hk-gl */
  /* en-fm fm-en gp-ho ho-gp */
  /* in-jm jm-in kp-lo lo-kp */

  const Vec4 d0 = p0 - m01;    // s0 -s0 c0 -c0
  const Vec4 d1 = p1 - m02;    // s1 -s1 c1 -c1
  const Vec4 d2 = p2 - m03;    // s2 -s2 c2 -c2
  const Vec4 d3 = m12 - p3;    // s3 -s3 c3 -c3
  const Vec4 d4 = m13 - p4;    // s4 -s4 c4 -c4
  const Vec4 d5 = m23 - p5;    // s5 -s5 c5 -c5

  // convenience aliases to keep the code block below within 80 columns
  using CV4 = const Vec4;

  CV4 sf0 = shuffle<Shuf4::zwxy>(d5) * v1p0;
  CV4 sf2 = shuffle<Shuf4::zwxy>(d4) * v0p0;
  CV4 sf9 = shuffle<Shuf4::wzyx>(d1) * v3p0;
  CV4 sfB = shuffle<Shuf4::wzyx>(d0) * v2p0;

  CV4 sf04 = fmadd(shuffle<Shuf4::zwxy>(d3), v3p0, sf0);
  CV4 sf27 = fmadd(shuffle<Shuf4::wzyx>(d2), v1p0, sf2);
  CV4 sf96 = fmadd(shuffle<Shuf4::zwxy>(d2), v2p0, sf9);
  CV4 sfB5 = fmadd(shuffle<Shuf4::wzyx>(d3), v0p0, sfB);

  CV4 row0 = fmadd(shuffle<Shuf4::wzyx>(d4), v2p0, sf04);
  CV4 row1 = fmadd(shuffle<Shuf4::wzyx>(d5), v0p0, sf96);
  CV4 row2 = fmadd(shuffle<Shuf4::zwxy>(d0), v3p0, sf27);
  CV4 row3 = fmadd(shuffle<Shuf4::zwxy>(d1), v1p0, sfB5);

  return Mat4{ row0, row1, row2, row3 };
}

FSVF_FORCEINLINE Mat4 FSVF_VECCALL Mat4::inverse_transposed() const
{
  return adj_transposed().scale_by_inv(determinant());
}

FSVF_FORCEINLINE Mat4 FSVF_VECCALL Mat4::inverse() const
{
  // funnily enough, MSVC will FSVF_FORCEINLINE the call to `adj()` below, but
  // won't FSVF_FORCEINLINE the call to `inverse_transposed()` in:
  // return inverse_transposed().transpose();
  return adj().scale_by_inv(determinant());
}

FSVF_FORCEINLINE Mat4 FSVF_VECCALL Mat4::adj() const
{
  return adj_transposed().transpose();
}

template<std::derived_from<LVec3> T>
FSVF_FORCEINLINE T FSVF_VECCALL Mat4::transform_3x3(T v) const
{
  const auto vx = shuffle<Shuf3::xxx>(v);
  const auto vy = shuffle<Shuf3::yyy>(v);
  const auto vz = shuffle<Shuf3::zzz>(v);

  const auto c0 = cols[0] * vx;
  const auto c01 = fmadd(cols[1], vy, c0);
  const auto c012 = fmadd(cols[2], vz, c01);

  return T{ c012 };
}

template<std::derived_from<LVec3> T>
FSVF_FORCEINLINE T FSVF_VECCALL Mat4::transform_affine(T v) const
{
  const auto vx = shuffle<Shuf3::xxx>(v);
  const auto vy = shuffle<Shuf3::yyy>(v);
  const auto vz = shuffle<Shuf3::zzz>(v);

  const auto c0 = fmadd(cols[0], vx, LVec3{ cols[3] });
  const auto c01 = fmadd(cols[1], vy, c0);
  const auto c012 = fmadd(cols[2], vz, c01);

  return c012;
}

template<std::derived_from<LVec3> T>
FSVF_FORCEINLINE T FSVF_VECCALL Mat4::transform_projective(T v) const
{
  const auto vx = shuffle<Shuf4::xxxx>(v);
  const auto vy = shuffle<Shuf4::yyyy>(v);
  const auto vz = shuffle<Shuf4::zzzz>(v);

  const auto c0 = fmadd(cols[0], vx, LVec3{ cols[3] });
  const auto c01 = fmadd(cols[1], vy, c0);
  const auto c012 = fmadd(cols[2], vz, c01);

  auto scale = shuffle<Shuf4::wwww>(c012);

  // avoid dividing by zero if the scale is zero
  scale = blend_with_vector_mask(scale, Vec4{1.f}, BitMasks::cmp_equal_f32(scale, Vec4::zeroes()));

  return T{ c012 / scale };
}

// TODO see if it can be optimized
FSVF_FORCEINLINE Mat4 FSVF_VECCALL Mat4::mul_transp(Mat4 m) const
{
  Vec4 p0 = cols[0] * m.cols[0];
  Vec4 p1 = cols[1] * m.cols[0];
  Vec4 p2 = cols[2] * m.cols[0];
  Vec4 p3 = cols[3] * m.cols[0];

  const Vec4 res0{ Internals::reduceAll(
    INTR_WRAP(_mm_add_ps),
    p0.m_sse,
    p1.m_sse,
    p2.m_sse,
    p3.m_sse
  ) };

  p0 = cols[0] * m.cols[1];
  p1 = cols[1] * m.cols[1];
  p2 = cols[2] * m.cols[1];
  p3 = cols[3] * m.cols[1];

  const Vec4 res1{ Internals::reduceAll(
    INTR_WRAP(_mm_add_ps),
    p0.m_sse,
    p1.m_sse,
    p2.m_sse,
    p3.m_sse
  ) };

  p0 = cols[0] * m.cols[2];
  p1 = cols[1] * m.cols[2];
  p2 = cols[2] * m.cols[2];
  p3 = cols[3] * m.cols[2];

  const Vec4 res2{ Internals::reduceAll(
    INTR_WRAP(_mm_add_ps),
    p0.m_sse,
    p1.m_sse,
    p2.m_sse,
    p3.m_sse
  ) };

  p0 = cols[0] * m.cols[3];
  p1 = cols[1] * m.cols[3];
  p2 = cols[2] * m.cols[3];
  p3 = cols[3] * m.cols[3];

  const Vec4 res3{ Internals::reduceAll(
    INTR_WRAP(_mm_add_ps),
    p0.m_sse,
    p1.m_sse,
    p2.m_sse,
    p3.m_sse
  ) };

  return Mat4{ res0, res1, res2, res3 };
}

}    // namespace FSVF
