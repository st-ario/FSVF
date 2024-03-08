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

FSVF_FORCEINLINE Mat4 FSVF_VECCALL operator/(Mat4 m, float x)
{
  return m.scale_by_inv(x);
}

FSVF_FORCEINLINE Mat4 FSVF_VECCALL operator/(float x, Mat4 m)
{
  Vec4 v{ x };
  return Mat4{ v / m.cols[0], v / m.cols[1], v / m.cols[2], v / m.cols[3] };
}

// TODO LVec3 ?
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

  const Vec4 c00 = m.cols[0] * n0x;
  const Vec4 c01 = m.cols[1] * n0y;
  const Vec4 c02 = m.cols[2] * n0z;
  const Vec4 c03 = m.cols[3] * n0w;

  const Vec4 res0 = (c00 + c01) + (c02 + c03);

  const Vec4 n1x = shuffle<Shuf4::xxxx>(n.cols[1]);
  const Vec4 n1y = shuffle<Shuf4::yyyy>(n.cols[1]);
  const Vec4 n1z = shuffle<Shuf4::zzzz>(n.cols[1]);
  const Vec4 n1w = shuffle<Shuf4::wwww>(n.cols[1]);

  const Vec4 c10 = m.cols[0] * n1x;
  const Vec4 c11 = m.cols[1] * n1y;
  const Vec4 c12 = m.cols[2] * n1z;
  const Vec4 c13 = m.cols[3] * n1w;

  const Vec4 res1 = (c10 + c11) + (c12 + c13);

  const Vec4 n2x = shuffle<Shuf4::xxxx>(n.cols[2]);
  const Vec4 n2y = shuffle<Shuf4::yyyy>(n.cols[2]);
  const Vec4 n2z = shuffle<Shuf4::zzzz>(n.cols[2]);
  const Vec4 n2w = shuffle<Shuf4::wwww>(n.cols[2]);

  const Vec4 c20 = m.cols[0] * n2x;
  const Vec4 c21 = m.cols[1] * n2y;
  const Vec4 c22 = m.cols[2] * n2z;
  const Vec4 c23 = m.cols[3] * n2w;

  const Vec4 res2 = (c20 + c21) + (c22 + c23);

  const Vec4 n3x = shuffle<Shuf4::xxxx>(n.cols[3]);
  const Vec4 n3y = shuffle<Shuf4::yyyy>(n.cols[3]);
  const Vec4 n3z = shuffle<Shuf4::zzzz>(n.cols[3]);
  const Vec4 n3w = shuffle<Shuf4::wwww>(n.cols[3]);

  const Vec4 c30 = m.cols[0] * n3x;
  const Vec4 c31 = m.cols[1] * n3y;
  const Vec4 c32 = m.cols[2] * n3z;
  const Vec4 c33 = m.cols[3] * n3w;

  const Vec4 res3 = (c30 + c31) + (c32 + c33);

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

  const __m128i vb     = float_to_bits(_mm_setzero_ps());
  const __m128i ones_i = _mm_cmpeq_epi32(vb, vb);
  const Vec4    ones   = bits_to_float(_mm_slli_epi32(ones_i, 31));
  const Vec4    zeroes = _mm_setzero_ps();
  const Vec4    mask   = blend_with_mask<0b0110>(zeroes, ones);

  const Vec4 s0123 = mask ^ p0123;
  const Vec4 s0312 = mask ^ p0312;
  const Vec4 s1203 = mask ^ p1203;
  const Vec4 s2301 = mask ^ p2301;
  const Vec4 s0213 = mask ^ p0213;
  const Vec4 s1302 = mask ^ p1302;

  const Vec4 acc = (s0123 + s0312) + (s1203 + s2301) - (s0213 + s1302);

  return sum(acc);
}

FSVF_FORCEINLINE Mat4 FSVF_VECCALL Mat4::adj_transposed() const
{
  /*
    a00 a01 a02 a03
    a10 a11 a12 a13
    a20 a21 a22 a23
    a30 a31 a32 a33
    */

  const Vec4 v0p0 = shuffle<Shuf4::yxwz>(cols[0]); /* a10 a00 a30 a20 */
  const Vec4 v2p0 = shuffle<Shuf4::yxwz>(cols[2]); /* a12 a02 a32 a22 */
  const Vec4 v3p0 = shuffle<Shuf4::yxwz>(cols[3]); /* a13 a03 a33 a23 */

  const Vec4 m01 = v0p0 * cols[1];           /* a10a01 a00a11 a30a21 a20a31 */
  const Vec4 m02 = v0p0 * cols[2];           /* a10a02 a00a12 a30a22 a20a32 */
  const Vec4 m03 = v0p0 * cols[3];           /* a10a03 a00a13 a30a23 a20a33 */
  const Vec4 m12 = v2p0 * cols[1];           /* a12a01 a02a11 a32a21 a22a31 */
  const Vec4 m13 = v3p0 * cols[1];           /* a13a01 a03a11 a33a21 a23a31 */
  const Vec4 m23 = v3p0 * cols[2];           /* a13a02 a03a12 a33a22 a23a32 */

  const Vec4 p0 = shuffle<Shuf4::yxwz>(m01); /* a00a11 a10a01 a20a31 a30a21 */
  const Vec4 p1 = shuffle<Shuf4::yxwz>(m02); /* a00a12 a10a02 a20a32 a30a22 */
  const Vec4 p2 = shuffle<Shuf4::yxwz>(m03); /* a00a13 a10a03 a20a33 a30a23 */
  const Vec4 p3 = shuffle<Shuf4::yxwz>(m12); /* a02a11 a12a01 a22a31 a32a21 */
  const Vec4 p4 = shuffle<Shuf4::yxwz>(m13); /* a03a11 a13a01 a23a31 a33a21 */
  const Vec4 p5 = shuffle<Shuf4::yxwz>(m23); /* a03a12 a13a02 a23a32 a33a22 */

  /* a00a11-a10a01 a10a01-a00a11 a20a31-a30a21 a30a21-a20a31 */
  /* a00a12-a10a02 a10a02-a00a12 a20a32-a30a22 a30a22-a20a32 */
  /* a00a13-a10a03 a10a03-a00a13 a20a33-a30a23 a30a23-a20a33 */
  /* a12a01-a02a11 a02a11-a12a01 a32a21-a22a31 a22a31-a32a21 */
  /* a13a01-a03a11 a03a11-a13a01 a33a21-a23a31 a23a31-a33a21 */
  /* a13a02-a03a12 a03a12-a13a02 a33a22-a23a32 a23a32-a33a22 */

  const Vec4 d0 = p0 - m01;    // s0 -s0 c0 -c0
  const Vec4 d1 = p1 - m02;    // s1 -s1 c1 -c1
  const Vec4 d2 = p2 - m03;    // s2 -s2 c2 -c2
  const Vec4 d3 = m12 - p3;    // s3 -s3 c3 -c3
  const Vec4 d4 = m13 - p4;    // s4 -s4 c4 -c4
  const Vec4 d5 = m23 - p5;    // s5 -s5 c5 -c5

  // convenience aliases to keep the code block below within 80 columns
  auto& c   = cols;
  using CV4 = const Vec4;

  CV4 f0 = d5 * shuffle<Shuf4::wzyx>(c[1]);    // a31*s5 -a21*s5 a11*c5 -a01*c5
  CV4 f1 = d5 * shuffle<Shuf4::zwxy>(c[0]);    // a20*s5 -a30*s5 a00*c5 -a10*c5
  CV4 f2 = d4 * shuffle<Shuf4::wzyx>(c[0]);    // a30*s4 -a20*s4 a10*c4 -a00*c4
  CV4 f3 = d4 * shuffle<Shuf4::zwxy>(c[2]);    // a22*s4 -a32*s4 a02*c4 -a12*c4
  CV4 f4 = d3 * shuffle<Shuf4::wzyx>(c[3]);    // a33*s3 -a23*s3 a13*c3 -a03*c3
  CV4 f5 = d3 * shuffle<Shuf4::zwxy>(c[0]);    // a20*s3 -a30*s3 a00*c3 -a10*c3
  CV4 f6 = d2 * shuffle<Shuf4::wzyx>(c[2]);    // a32*s2 -a22*s2 a12*c2 -a02*c2
  CV4 f7 = d2 * shuffle<Shuf4::zwxy>(c[1]);    // a21*s2 -a31*s2 a01*c2 -a11*c2
  CV4 f8 = d1 * shuffle<Shuf4::wzyx>(c[1]);    // a31*s1 -a21*s1 a11*c1 -a01*c1
  CV4 f9 = d1 * shuffle<Shuf4::zwxy>(c[3]);    // a23*s1 -a33*s1 a03*c1 -a13*c1
  CV4 fA = d0 * shuffle<Shuf4::wzyx>(c[3]);    // a33*s0 -a23*s0 a13*c0 -a03*c0
  CV4 fB = d0 * shuffle<Shuf4::zwxy>(c[2]);    // a22*s0 -a32*s0 a02*c0 -a12*c0

  // we're working on the transposed adjoint

  // x = f0.z + f3.w + f4.z
  // y = f0.w + f3.z + f4.w
  // z = f0.x + f3.y + f4.x
  // w = f0.y + f3.x + f4.y

  // row 1: f0.zwxy + f3.wzyx + f4.zwxy
  // row 2: f1.wzyx + f6.zwxy + f9.wzyx
  // row 3: f2.zwxy + f7.wzyx + fA.zwxy
  // row 4: f5.wzyx + f8.zwxy + fB.wzyx

  const Vec4 row1 = shuffle<Shuf4::zwxy>(f0)
                  + shuffle<Shuf4::wzyx>(f3)
                  + shuffle<Shuf4::zwxy>(f4);

  const Vec4 row2 = shuffle<Shuf4::wzyx>(f1)
                  + shuffle<Shuf4::zwxy>(f6)
                  + shuffle<Shuf4::wzyx>(f9);

  const Vec4 row3 = shuffle<Shuf4::zwxy>(f2)
                  + shuffle<Shuf4::wzyx>(f7)
                  + shuffle<Shuf4::zwxy>(fA);

  const Vec4 row4 = shuffle<Shuf4::wzyx>(f5)
                  + shuffle<Shuf4::zwxy>(f8)
                  + shuffle<Shuf4::wzyx>(fB);

  return Mat4{ row1, row2, row3, row4 };

  //  LAPLACE_FACTORS
  //
  //  // determinants
  //  const Vec4 da0 = lhs0 - rhs0;               // s0 s1 c0 c1
  //  const Vec4 da1 = lhs1 - rhs1;               // s2 s3 c2 c3
  //  // careful: da2 goes 5-4-5-4
  //  const Vec4 da2 = lhs2 - rhs2;               // s5 s4 c5 c4
  //
  //  const Vec4 dd0 = shuffle<Shuf4::zzxx>(da0); // c0 c0 s0 s0
  //  const Vec4 dd1 = shuffle<Shuf4::wwyy>(da0); // c1 c1 s1 s1
  //  const Vec4 dd2 = shuffle<Shuf4::zzxx>(da1); // c2 c2 s2 s2
  //  const Vec4 dd3 = shuffle<Shuf4::wwyy>(da1); // c3 c3 s3 s3
  //  const Vec4 dd4 = shuffle<Shuf4::wwyy>(da2); // c4 c4 s4 s4
  //  const Vec4 dd5 = shuffle<Shuf4::zzxx>(da2); // c5 c5 s5 s5
  //
  //  // flip alternatingly
  //  const Vec4 mask{bits_to_float(_mm_set_epi32(f32_sign_mask, 0, f32_sign_mask, 0))};
  //
  //  const Vec4 d0 = dd0 ^ mask;                 // c0 -c0 s0 -s0
  //  const Vec4 d1 = dd1 ^ mask;                 // c1 -c1 s1 -s1
  //  const Vec4 d2 = dd2 ^ mask;                 // c2 -c2 s2 -s2
  //  const Vec4 d3 = dd3 ^ mask;                 // c3 -c3 s3 -s3
  //  const Vec4 d4 = dd4 ^ mask;                 // c4 -c4 s4 -s4
  //  const Vec4 d5 = dd5 ^ mask;                 // c5 -c5 s5 -s5
  //
  //  // TODO with the third set of permutations this looks bad, check if there's a better way
  //  const Vec4 r0a = v1p2 * d5; // a11*c5, -a01*c5, a31*s5, -a21*s5
  //  const Vec4 r0b = v2p2 * d4; // a12*c4, -a02*c4, a32*s4, -a22*s4
  //  const Vec4 r0c = v3p2 * d3; // a13*c3, -a03*c3, a33*s3, -a23*s3
  //
  //  const Vec4 row0 = r0a - r0b + r0c;
  //
  //  const Vec4 r1a = v0p2 * d5; // a10*c5 -a00*c5 a30*s5 -a20*s5
  //  const Vec4 r1b = v2p2 * d2; // a12*c2 -a02*c2 a32*s2 -a22*s2
  //  const Vec4 r1c = v3p2 * d1; // a13*c1 -a03*c1 a33*s1 -a23*s1
  //
  //  const Vec4 row1 = r1b - r1a - r1c;
  //
  //  const Vec4 r2a = v0p2 * d4; // a10*c4 -a00*c4 a30*s4 -a20*s4
  //  const Vec4 r2b = v1p2 * d2; // a11*c2 -a01*c2 a31*s2 -a21*s2
  //  const Vec4 r2c = v3p2 * d0; // a13*c0 -a03*c0 a33*s0 -a23*s0
  //
  //  const Vec4 row2 = r2a - r2b + r2c;
  //
  //  const Vec4 r3a = v0p2 * d3; // a10*c3 -a00*c3 a30*s3 -a20*s3
  //  const Vec4 r3b = v1p2 * d1; // a11*c1 -a01*c1 a31*s1 -a21*s1
  //  const Vec4 r3c = v2p2 * d0; // a12*c0 -a02*c0 a32*s0 -a22*s0
  //
  //  const Vec4 row3 = r3b - r3a - r3c;
  //
  //  return Mat4{ row0, row1, row2, row3 };
}

FSVF_FORCEINLINE Mat4 FSVF_VECCALL Mat4::inverse_transposed() const
{
#ifdef FSVF_NO_RECIPROCAL_MATH
  return adj_transposed().scale_by_inv(determinant());
#else
  return adj_transposed().scale_by(1.f / determinant());
#endif
}

FSVF_FORCEINLINE Mat4 FSVF_VECCALL Mat4::inverse() const
{
  // funnily enough, MSVC will FSVF_FORCEINLINE the call to `adj()` below, but
  // won't FSVF_FORCEINLINE the call to `inverse_transposed()` in:
  // return inverse_transposed().transpose();
#ifdef FSVF_NO_RECIPROCAL_MATH
  return adj().scale_by_inv(determinant());
#else
  return adj().scale_by(1.f / determinant());
#endif
}

FSVF_FORCEINLINE std::optional<Mat4> FSVF_VECCALL Mat4::inverse_safe() const
{
  const float det = determinant();

  if (det == 0.f) return {};

#ifdef FSVF_NO_RECIPROCAL_MATH
  return std::optional<Mat4>(adj().scale_by_inv(det));
#else
  const float inv_det = 1.f / det;
  return std::make_optional(adj().scale_by(inv_det));
#endif
}

FSVF_FORCEINLINE std::optional<Mat4> FSVF_VECCALL Mat4::inverse_transposed_safe() const
{
  const float det = determinant();

  if (det == 0.f) return {};

#ifdef FSVF_NO_RECIPROCAL_MATH
  return std::optional<Mat4>(adj_transposed().scale_by_inv(det));
#else
  const float inv_det = 1.f / det;
  return std::optional<Mat4>(adj_transposed().scale_by(inv_det));
#endif
}

template<std::derived_from<LVec3> T>
FSVF_FORCEINLINE T FSVF_VECCALL Mat4::transform_3x3(T v) const
{
  // TODO check best approach (transpose-mul, using FMA and combinations of these)
  const auto vx = shuffle<Shuf3::xxx>(v);
  const auto vy = shuffle<Shuf3::yyy>(v);
  const auto vz = shuffle<Shuf3::zzz>(v);

  const auto c0 = cols[0] * vx;
  const auto c1 = cols[1] * vy;
  const auto c2 = cols[2] * vz;

  return T{ c0 + c1 + c2 };
}

template<std::derived_from<LVec3> T>
FSVF_FORCEINLINE T FSVF_VECCALL Mat4::transform_affine(T v) const
{
  // TODO check best
  const auto vx = shuffle<Shuf3::xxx>(v);
  const auto vy = shuffle<Shuf3::yyy>(v);
  const auto vz = shuffle<Shuf3::zzz>(v);

  const auto c0 = cols[0] * vx;
  const auto c1 = cols[1] * vy;
  /* TODO try FMA? */
  const auto c2  = cols[2] * vz;
  const auto res = (c0 + c1) + (c2 + LVec3{ cols[3] });
  /******************/

  return res;
}

template<std::derived_from<LVec3> T>
FSVF_FORCEINLINE T FSVF_VECCALL Mat4::transform_projective(T v) const
{
  // TODO check best
  const Vec4 vx = shuffle<Shuf4::xxxx>(v);
  const Vec4 vy = shuffle<Shuf4::yyyy>(v);
  const Vec4 vz = shuffle<Shuf4::zzzz>(v);

  const Vec4 c0  = cols[0] * vx;
  const Vec4 c1  = cols[1] * vy;
  const Vec4 c2  = cols[2] * vz;
  const Vec4 res = (c0 + c1) + (c2 + cols[3]);
  /******************/
  const Vec4 scale = shuffle<Shuf4::wwww>(res);

  // TODO conditional vs unconditional division?
  // TODO optimize constant
  if (scale == Vec4(1.f)) return LVec3{ res };

  return LVec3{ res / scale };
}

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
