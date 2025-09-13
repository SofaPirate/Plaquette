/*
 * pq_wave_utils.h
 *
 * Utility functions for waves.
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
#ifndef PQ_WAVE_UTILS_H_
#define PQ_WAVE_UTILS_H_

#include "pq_phase_utils.h"
#include "pq_fixed32_trig.h"

namespace pq {

inline q0_32u_t squareWave(const q0_32u_t& t, const q0_32u_t& skew)
{
  return (t <= skew) ? FIXED_32_MAX : 0;
}

inline const q0_32u_t& triangleWave(const q0_32u_t& t, const q0_32u_t& skew)
{
  return (t <= skew) ?
            fixed32Divide(t, skew) :
            fixed32Divide(FIXED_32_MAX - t, FIXED_32_MAX - skew);
}

inline q0_32u_t sineWave(const q0_32u_t& t, const q0_32u_t& skew)
{
#if defined(PQ_ARCH_32BITS)
  // Phasse time remapped and rescaled to 16 bits for use with trigonometric library.
  q0_32u_t phase32;

  // Special case: skew == 0.5 (default and most common). More efficient.
  if (skew == HALF_FIXED_32_MAX)
  {
      phase32 = t;
  }
  // Rising part of sine wave.
  else if (t < skew)
  {
      phase32 = fixed32Divide(t, skew) / 2;
  }
  // Falling part of sine wave.
  else
  {
      phase32 = fixed32Divide(t - skew, FIXED_32_MAX - skew) / 2 + HALF_FIXED_32_MAX;
  }

  return static_cast<uint32_t>(HALF_FIXED_32_MAX - cos32(phase32));
#else
  // Phasse time remapped and rescaled to 16 bits for use with trigonometric library.
  uint16_t phase16;

  // Special case: skew == 0.5 (default and most common). More efficient.
  if (skew == HALF_FIXED_32_MAX)
  {
      phase16 = static_cast<uint16_t>(t >> 16);
  }
  // Rising part of sine wave.
  else if (t < skew)
  {
      phase16 = static_cast<uint16_t>((static_cast<uint64_t>(t) << 15) / skew);
  }
  // Falling part of sine wave.
  else if (t > skew)
  {
      phase16 = static_cast<uint16_t>((static_cast<uint64_t>(t - skew) << 15) / (FIXED_32_MAX - skew)) + HALF_FIXED_16_MAX;
  }
  // Peak of sine wave.
  else
  { // t == _skew32
      phase16 = 32768;
  }

  // Convert to [0, 1] with wave shape similar to triangle wave.
  return static_cast<uint32_t>(static_cast<uint16_t>(HALF_FIXED_16_MAX-1) - cos16(phase16)) << 16;
#endif
}

}

#endif
