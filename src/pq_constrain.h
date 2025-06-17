/*
 * pq_constrain.h
 *
 * Utility functions clamping.
 *
 * (c) 2025 Sofian Audry        :: info(@)sofianaudry(.)com
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
#ifndef PQ_CONSTRAIN_H_
#define PQ_CONSTRAIN_H_

#if (defined(ARDUINO) && ARDUINO >= 100) || defined(EPOXY_DUINO)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

/// Clamps floating point value in range [0, 1].
inline float constrain01(float x) {
#if defined(PQ_IEEE_754_SUPPORTED)
    union {
        float f;
        uint32_t i;
    } u;

    u.f = x;
    u.i &= 0x7FFFFFFF;               // Force non-negative
    if (u.i > 0x3F800000)            // Clamp to 1.0
        u.i = 0x3F800000;

    return u.f;
#else
  return constrain(x, 0.0f, 1.0f);
#endif
}

#endif
