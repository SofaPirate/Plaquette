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

namespace pq {

/// Returns the square root. Notice that sqrt(0) returns a non-zero, small positive number.
// Source: https://www.gamedev.net/forums/topic/704525-3-quick-ways-to-calculate-the-square-root-in-c/
float fastSqrt(const float& n)
{
    static union {int i; float f;} u;
    u.i = 0x2035AD0C + (*(int*)&n >> 1);
   return n / u.f + u.f * 0.25f;
}

} // namespace pq

#endif
