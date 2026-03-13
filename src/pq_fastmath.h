/*
 * pq_fastmath.h
 *
 * Optimized mathematical functions.
 *
 * (c) 2022 Sofian Audry        :: info(@)sofianaudry(.)com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef PQ_FAST_MATH_H_
#define PQ_FAST_MATH_H_

#include "pq_globals.h"
#include "pq_fixed32_trig.h"
#include "pq_wrap.h"

#include <stdint.h>

namespace pq {

/// Returns the square root. Notice that sqrt(0) returns a non-zero, small positive number.
// Source: https://www.gamedev.net/forums/topic/704525-3-quick-ways-to-calculate-the-square-root-in-c/
inline float fastSqrt(const float& n)
{
    static union {int32_t i; float f;} u;
    u.i = 0x2035AD0C + (*(int32_t*)&n >> 1);
    return n / u.f + u.f * 0.25f;
}

inline float fastSin(float x) {
#if defined(PQ_ARCH_32BITS)
    x = wrap01(x/TWO_PI) * 4294967295ULL;
    return sin32( (uint32_t)x ) / 2147483532.0f;
#else
    x = wrap01(x/TWO_PI) * 65535;
    return sin16( (uint16_t)x ) / 32767.0f;
#endif
}

inline float fastCos(float x) {
    return fastSin(HALF_PI - x);
}

//Source: https://martin.ankerl.com/2007/10/04/optimized-pow-approximation-for-java-and-c-c/
inline double fastPow(double a, double b) {
    union {
        double d;
        int32_t x[2];
    } u = { a };
    u.x[1] = (int32_t)(b * (u.x[1] - 1072632447) + 1072632447);
    u.x[0] = 0;
    return u.d;
}

inline float fastPow(float a, float b) {
    return (float)fastPow((double)a, (double)b);
}

// Computes 2^b using a bit-manipulation trick optimized for base 2.
// Exact for integer exponents; ~6% max error for fractional exponents.
inline double fastPow2(double b) {
    union {
        double d;
        int32_t x[2];
    } u;
    u.x[0] = 0;
    u.x[1] = (int32_t)(b * (1 << 20) + (1023 << 20));
    return u.d;
}

inline float fastPow2(float b) {
    return (float)fastPow2((double)b);
}

// Fast natural logarithm approximation using IEEE 754 bit manipulation.
// Inverse of the Schraudolph fastExp trick below. Max relative error ~14%
// (worst case at m = sqrt(2) within a binade). Suitable for non-critical uses
// where a single multiply-and-shift is sufficient.
inline float fastLog(float x) {
  union { float f; uint32_t u; } pun = { x };
  // log(x) ≈ (bits(x) / 2^23 - 127) * ln2
  return (float)pun.u * (0.693147f / (float)(1 << 23)) - 88.029691f;
  // 88.029691f = 127 * ln(2)
}

// More precise fast natural logarithm for x ∈ (0, 1].
// Splits x = 2^k * m (m ∈ [1,2)), then approximates log(m) using the Euler
// substitution s = (m-1)/(m+1) where s ∈ [0, 1/3), which converges rapidly:
//   log(m) = 2*(s + s³/3 + s⁵/5 + ...)
// Two terms give max error ~0.003 (~0.4% relative), vs ~14% for the bit trick.
// All float ops (div, mul, add) are already linked in any floating-point sketch.
inline float fastLog01(float x) {
  union { float f; uint32_t u; } pun = { x };

  // Integer exponent: log(x) = k*ln2 + log(m).
  int32_t k = (int32_t)(pun.u >> 23) - 127;

  // Mantissa m ∈ [1, 2) as float (replace exponent field with 127).
  pun.u = (pun.u & 0x007FFFFF) | 0x3F800000;

  // log(m) via s = (m-1)/(m+1): two terms of 2*(s + s³/3).
  float s  = (pun.f - 1.0f) / (pun.f + 1.0f);
  float s2 = s * s;
  float logm = s * (2.0f + (2.0f / 3.0f) * s2);

  return (float)k * 0.693147f + logm;
}

// Directly computes -log(r / 2^32) from a raw uint32 random value r,
// skipping the fixed32ToFloat step and the max(u, FLT_MIN) guard.
// Builds the IEEE 754 representation of x = r/2^32 directly from the integer
// bits (exponent = 126 - clz(r), mantissa = top 23 bits below the leading 1),
// then delegates to fastLog01 and negates. Same precision as fastLog01.
// For r=0 (probability 2^-32), returns a large capped value.
inline float fastNegLog32(uint32_t r) {
  if (r == 0) return 32.0f * 0.693147f;

  const uint32_t z = __builtin_clz(r);

  // Construct IEEE 754 bits for x = r/2^32 ∈ (0, 1]:
  //   exponent field = 126 - z  (biased exponent for the interval [2^(-1-z), 2^(-z)))
  //   mantissa field = top 23 bits of r below the leading 1
  union { float f; uint32_t u; } pun;
  pun.u = ((r << (z + 1)) >> 9) | ((126 - z) << 23);

  return -fastLog01(pun.f);
}

// Source: https://gist.github.com/jrade/293a73f89dfef51da6522428c857802d
// N. Schraudolph, “A Fast, Compact Approximation of the Exponential Function”,
// Neural Computation 11, 853–862 (1999).
// (available at https://nic.schraudolph.org/pubs/Schraudolph99.pdf)
inline float fastExp(float x)
{
    constexpr float a = (1 << 23) / 0.69314718f;
    constexpr float b = (1 << 23) * (127 - 0.043677448f);
    x = a * x + b;

    // Remove these lines if bounds checking is not needed
    constexpr float c = (1 << 23);
    constexpr float d = (1 << 23) * 255;
    if (x < c || x > d)
        x = (x < c) ? 0.0f : d;

    union { float f; uint32_t u; } pun;
    pun.u = static_cast<uint32_t>(x);
    return pun.f;
}

// inline double fastExp(double x)
// {
//     constexpr double a = (1ll << 52) / 0.6931471805599453;
//     constexpr double b = (1ll << 52) * (1023 - 0.04367744890362246);
//     x = a * x + b;

//     // Remove these lines if bounds checking is not needed
//     constexpr double c = (1ll << 52);
//     constexpr double d = (1ll << 52) * 2047;
//     if (x < c || x > d)
//         x = (x < c) ? 0.0 : d;

//     // With C++20 one can use std::bit_cast instead
//     uint64_t n = static_cast<uint64_t>(x);
//     memcpy(&x, &n, 8);
//     return x;
// }

} // namespace pq

#endif
