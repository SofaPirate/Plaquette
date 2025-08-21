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

} // namespace pq

#endif
