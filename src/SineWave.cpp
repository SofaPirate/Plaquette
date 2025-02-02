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
#include "pq_map_real.h"
#include "pq_time.h"
#include "pq_wrap.h"
#include "pq_trig8.h"

namespace pq {

#define PQ_SINE_OSC_AMPLITUDE_DIVIDER (-32767.0f)

SineWave::SineWave(float period_, float width_) : AbstractWave(period_, width_) {
}

// Improved version of SineWave::_get with optimizations and typo fixes.

float SineWave::_get(phase_time_t t) {
  // Precompiled constant for faster operation.
  constexpr float INV_65534 = 1.0f / 65534.0f;
  constexpr float SAFE_PHASE_TIME_MAX = PHASE_TIME_MAX + FLT_MIN; // this is to avoid division by zero when width = 1.0

  // Phasse time remapped and rescaled to 16 bits for use with trigonometric library.
  uint16_t phaseTime16;

  // Special case: width == 0.5 (default and most common). More efficient.
  if (_width == HALF_PHASE_TIME_MAX) {
    phaseTime16 = static_cast<uint16_t>(t >> 16);
  }
  // Rising part of sine wave.
  else if (t < _width) {
    phaseTime16 = static_cast<uint16_t>( (static_cast<uint64_t>(t) << 15) / _width);
  }
  // Falling part of sine wave.
  else if (t > _width) {
    phaseTime16 = static_cast<uint16_t> ((static_cast<uint64_t>(t - _width) << 15) / (PHASE_TIME_MAX - _width)) + 32768;
  }
  // Peak of sine wave.
  else { // t == _width
    phaseTime16 = 32768;
  }
  
  // Convert to [0, 1] with wave shape similar to triangle wave.
  return (static_cast<uint16_t>(32767) - cos16(phaseTime16)) * INV_65534; // = ( 32767 - cos16(phaseTime16) ) / 65534
}

}
