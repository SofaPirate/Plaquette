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
    uint32_t ui;
    memcpy(&ui, &x, sizeof(ui)); // safe bit copy

    if (ui & 0x80000000u) return 0.0f; // check sign: negative => return 0
    if (ui > 0x3F800000u) return 1.0f; // check if > 1.0f => return 1
    return x;                          // already in [0,1]
#else
    return (x < 0.0f) ? 0.0f : (x > 1.0f ? 1.0f : x);
#endif
}

#endif
