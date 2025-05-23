/*
 * pq_osc_utils.h
 *
 * Utility functions for phase quantization oscillators.
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
#ifndef PQ_OSC_UTILS_H
#define PQ_OSC_UTILS_H

#if (defined(ARDUINO) && ARDUINO >= 100) || defined(EPOXY_DUINO)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include "pq_globals.h"
#include "pq_fixed_math.h"
#include "pq_wrap.h"

#include <stdint.h>
#include <float.h>

namespace pq {

typedef uint32_t fixed_t;

#define FIXED_MAX static_cast<fixed_t>(0xFFFFFFFF)
#define HALF_FIXED_MAX static_cast<fixed_t>(0x80000000)
//constexpr fixed_t HALF_FIXED_MAX = 0.5f * FIXED_MAX;
constexpr float INV_FIXED_MAX = 1.0f / FIXED_MAX;

/// Applies amplitude scaling to 32-bit fixed-point value interpreted as a signal centered at UINT32_MAX/2.
inline fixed_t amplifyFixed(fixed_t x, fixed_t amplitude) {
  // Shift to signed range (-UINT32_MAX/2 to UINT32_MAX/2).
  int32_t centered = (int32_t)(x ^ 0x80000000);

  // Apply amplitude scaling (keeping within 32-bit range).
  centered = ((int64_t)centered * amplitude) >> 32;

  // Convert back to unsigned range.
  return (fixed_t)(centered ^ 0x80000000);
}

/// Converts 32-bit fixed-point value to floating point.
inline float fixedToFloat(uint32_t x) {
  return (x * INV_FIXED_MAX);
}

/// Converts floating point in range [0, 1] to 32-bit fixed-point value.
inline fixed_t floatTofixed(float x) {
  // The x >= 1 instruction prevents overflow issues when x is close to 1.
  if      (x <= 0) return 0;
  else if (x >= 1) return FIXED_MAX;
  else             return static_cast<fixed_t>(x * FIXED_MAX);
}

inline float constrain01(float x) {
#if defined(IEEE_754_SUPPORTED)
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
  return constrain(x, 0, 1);
#endif
}

/// Converts floating point to fixed_t.
inline fixed_t floatToPhaseTime(float x) { return wrap01(x) * FIXED_MAX; }

/// Phase-time division.
inline fixed_t fixedDivide(fixed_t x, fixed_t y) { return divide_32div32(x, y); }

/// Converts time in seconds to phase in %.
inline float timeToPhase(float period, float time) { return period == 0 ? 0 : time / period; }

/// Returns phase time value with offset.
fixed_t phaseTimeAddPhase(fixed_t phaseTime, float phase);

/// Returns phase time value with time offset.
fixed_t phaseTimeAddTime(fixed_t phaseTime, float period, float time);

/// Computes new phase time for oscillators and returns true when phase time overflows.
bool phaseTimeUpdate(fixed_t& phaseTime, float period, float sampleRate);

}

#endif
