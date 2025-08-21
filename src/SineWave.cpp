/*
 * SineWave.cpp
 *
 * (c) 2015 Sofian Audry        :: info(@)sofianaudry(.)com
 * (c) 2015 Thomas O Fredericks :: tof(@)t-o-f(.)info
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

#include "SineWave.h"
#include "pq_map.h"
#include "pq_time.h"
#include "pq_wrap.h"
#include "pq_fixed_trig.h"

namespace pq {

SineWave::SineWave(Engine& engine) : AbstractWave(engine) {}
SineWave::SineWave(float period, Engine& engine) : AbstractWave(period, engine) {}
SineWave::SineWave(float period, float skew, Engine& engine) : AbstractWave(period, skew, engine) {}

// Improved version of SineWave::_get with optimizations and typo fixes.
q0_32u_t SineWave::_getFixed(q0_32u_t t) {
#if defined(PQ_ARCH_32BITS)
  // Phasse time remapped and rescaled to 16 bits for use with trigonometric library.
  uint32_t phase32;

  // Special case: skew == 0.5 (default and most common). More efficient.
  if (_skew == HALF_FIXED_MAX) {
    phase32 = t;
  }
  // Rising part of sine wave.
  else if (t < _skew) {
    phase32 = fixedDivide(t, _skew) / 2;
  }
  // Falling part of sine wave.
  else {
    phase32 = fixedDivide(t - _skew, FIXED_MAX - _skew) / 2 + HALF_FIXED_MAX;
  }
  // Serial.print(t); Serial.print(" ");
  // Serial.println(phase32);
  // // Peak of sine wave.
  // else { // t == _skew
  //   phase32 = FIXED_MAX / 2;
  // }

  return static_cast<uint32_t>(HALF_FIXED_MAX - cos32(phase32));
#else
  // Phasse time remapped and rescaled to 16 bits for use with trigonometric library.
  uint16_t phaseTime16;

  // Special case: skew == 0.5 (default and most common). More efficient.
  if (_skew == HALF_FIXED_MAX) {
    phaseTime16 = static_cast<uint16_t>(t >> 16);
  }
  // Rising part of sine wave.
  else if (t < _skew) {
    phaseTime16 = static_cast<uint16_t>( (static_cast<uint64_t>(t) << 15) / _skew);
  }
  // Falling part of sine wave.
  else if (t > _skew) {
    phaseTime16 = static_cast<uint16_t> ((static_cast<uint64_t>(t - _skew) << 15) / (FIXED_MAX - _skew)) + 32768;
  }
  // Peak of sine wave.
  else { // t == _skew
    phaseTime16 = 32768;
  }

  // Convert to [0, 1] with wave shape similar to triangle wave.
  return static_cast<uint32_t>(static_cast<uint16_t>(32767) - cos16(phaseTime16)) << 16;
#endif
}

}
