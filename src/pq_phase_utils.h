/*
 * pq_phase_utils.h
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
#ifndef PQ_PHASE_UTILS_H_
#define PQ_PHASE_UTILS_H_

#if (defined(ARDUINO) && ARDUINO >= 100) || defined(EPOXY_DUINO)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include "pq_globals.h"
#include "pq_fixed.h"
#include "pq_fixed32_math.h"
#include "pq_wrap.h"

#include <stdint.h>
#include <float.h>

namespace pq {

/// Applies amplitude scaling to 32-bit fixed32-point value interpreted as a signal centered at UINT32_MAX/2.
inline q0_32u_t amplifyFixed32(q0_32u_t x, q0_32u_t amplitude) {
  // Shift to signed range (-UINT32_MAX/2 to UINT32_MAX/2).
  int32_t centered = (int32_t)(x ^ HALF_FIXED_MAX_32);

  // Apply amplitude scaling (keeping within 32-bit range).
  centered = ((int64_t)centered * amplitude) >> 32;

  // Convert back to unsigned range.
  return (q0_32u_t)(centered ^ HALF_FIXED_MAX_32);
}

/// Applies amplitude scaling to float using fixed32-point amplitude.
inline float amplifyFloat(float x, q0_32u_t amplitude) {
  return fixed32ToFloat(amplitude) * (x - 0.5f) + 0.5f;
}

/// Converts floating point to q0_32u_t.
inline q0_32u_t floatToPhase32(float x) { return floatToFixed32(x); }

/// Fixed-point division.
inline q0_32u_t fixed32Divide(q0_32u_t x, q0_32u_t y) { return divide_32div32(x, y); }

/// Fixed-point multiplication.
inline q0_32u_t fixed32Multiply(q0_32u_t x, q0_32u_t y) { return multiply_32x32_rshift32(x,y) ;}

/// Converts time in seconds to phase in %.
inline float timeToPhase(float period, float time) { return period == 0 ? 0 : time / period; }

/// Converts time in seconds an frequency in Hz to phase in %.
inline float frequencyAndTimeToPhase(float frequency, float time) { return time * frequency; }

/// Safely returns 1/x.
inline float invert(float x) { return (x == 0) ? FLT_MAX : 1.0f / x; }

/// Converts period in seconds to frequency in Hz.
inline float periodToFrequency(float period) { return invert(period); }

/// Converts frequency in Hz to period in seconds.
inline float frequencyToPeriod(float frequency) { return invert(frequency); }

// https://www.programiz.com/online-compiler/1hIyxD51PqRYE
// inline float stepIntervalForNormalizedValue(int count) { return (count <= 1) ? 1.0f : 1.0f / (count-1); }

/// Returns phase time value with offset.
q0_32u_t phase32AddPhase(q0_32u_t phase32, float phase);

/// Returns phase time value with time offset.
q0_32u_t phase32AddTime(q0_32u_t phase32, float period, float time);

/// Computes new phase time for oscillators and returns true when phase time overflows (uses precompiled deltaTimeSecondsTimesFixedMax).
bool phase32UpdateFixed32(q0_32u_t& phase32, float frequency, float deltaTimeSecondsTimesFixed32Max, bool forward = true);

/// Computes new phase time for oscillators and returns true when phase time overflows.
bool phase32Update(q0_32u_t& phase32, float period, float sampleRate, bool forward = true);

}

#endif
