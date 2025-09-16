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

    // With C++20 one can use std::bit_cast instead
    uint32_t n = static_cast<uint32_t>(x);
    memcpy(&x, &n, 4);
    return x;
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
