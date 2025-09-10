/*
 * pq_fixed.h
 *
 * Utility functions for fixed-point arithmetic.
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
#ifndef PQ_FIXED_H_
#define PQ_FIXED_H_

#include "pq_globals.h"
#include "pq_constrain.h"

#include <stdint.h>

namespace pq {

typedef uint32_t q0_32u_t;
typedef uint16_t q0_16u_t;
typedef uint8_t  q0_8u_t;

constexpr q0_32u_t FIXED_MAX_32 = static_cast<q0_32u_t>(0xFFFFFFFF);
constexpr q0_32u_t HALF_FIXED_MAX_32 = static_cast<q0_32u_t>(0x80000000);
constexpr float    INV_FIXED_MAX_32 = 1.0f / FIXED_MAX_32;

constexpr q0_16u_t FIXED_MAX_16 = static_cast<q0_16u_t>(0xFFFF);
constexpr q0_16u_t HALF_FIXED_MAX_16 = static_cast<q0_16u_t>(0x8000);
constexpr float    INV_FIXED_MAX_16 = 1.0f / FIXED_MAX_16;

constexpr q0_8u_t FIXED_MAX_8 = static_cast<q0_8u_t>(0xFF);
constexpr q0_8u_t HALF_FIXED_MAX_8 = static_cast<q0_8u_t>(0x80);
constexpr float   INV_FIXED_MAX_8 = 1.0f / FIXED_MAX_8;

/**
 * Re-maps a number in range [0, 1] to a new range [0, toHigh].
 * @param value the number to map (in [0,1])
 * @param toHigh the upper bound of the value’s target range
 * @return the mapped value in [0, toHigh]
 */
template <typename I>
inline I floatToFixed(float value, I toHigh) {
#if defined(PQ_IEEE_754_SUPPORTED)
  uint32_t ui;
  memcpy(&ui, &value, sizeof ui); // safe bit copy

  // Negative -> clamp to 0.0f
  if (ui & 0x80000000u) return static_cast<I>(0);

  // Closed upper bound: x > 1.0f -> 1.0f
  if (ui >= 0x3F800000u) return toHigh;

  return static_cast<I>(value + value * toHigh); // x * (toHigh + 1)
#else
  return (value < 0.0f) ? 0 : (value > 1.0f) ? toHigh : static_cast<I>(value + value * toHigh);
#endif
}

/**
 * Re-maps a number from [0, toHigh] to the [0, 1] range.
 * @param value the number to map
 * @param toHigh the upper bound of the value’s target range
 * @return the mapped value in [0, 1]
 */
template <typename I>
inline float fixedToFloat(I value, float invMax) {
  return constrain01(value * invMax);
}

/// Converts 32-bit fixed32-point value to floating point.
inline float fixed32ToFloat(q0_32u_t x) { return fixedToFloat(x, INV_FIXED_MAX_32); }

/// Converts floating point in range [0, 1] to 32-bit fixed32-point value.
inline q0_32u_t floatToFixed32(float x) { return floatToFixed(x, FIXED_MAX_32); }

/// Converts 16-bit fixed16-point value to floating point.
inline float fixed16ToFloat(q0_16u_t x) { return fixedToFloat(x, INV_FIXED_MAX_16); }

/// Converts floating point in range [0, 1] to 16-bit fixed16-point value.
inline q0_16u_t floatToFixed16(float x) { return floatToFixed(x, FIXED_MAX_16); }

/// Converts 8-bit fixed8-point value to floating point.
inline float fixed8ToFloat(q0_8u_t x) { return fixedToFloat(x, INV_FIXED_MAX_8); }

/// Converts floating point in range [0, 1] to 8-bit fixed8-point value.
inline q0_8u_t floatToFixed8(float x) { return floatToFixed(x, FIXED_MAX_8); }

}

#endif
