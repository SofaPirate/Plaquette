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

  // Returns value in [0, 1].
float SineWave::_get(phase_time_t t) {
  int16_t sineWave16;
  // Special case: width == 0.5 (default and most common). More efficient.
  if (_width == HALF_PHASE_TIME_MAX) {
    sineWave16 = cos16((uint16_t)(t >> 16));
  }

  // General case.
  else {
    // Remapped phase time (16 bits).
    uint16_t remappedPhaseTime16;
    // Special case (falling sine wave / equivalent of inverted sawtooth).
    if (_width == 0) {
      remappedPhaseTime16 = (uint16_t) ((float)(t) / PHASE_TIME_MAX * 32767.0f) + 32768;
    }
    // Rising part of sine wave.
    else if (t <= _width) {
      remappedPhaseTime16 = (uint16_t) ((float)t / _width * 32767.0f);
    }
    // Falling part of sine wave.
    else {
      phase_time_t widthMinusOne = _width - 1;
      remappedPhaseTime16 = (uint16_t) ((float)(t - widthMinusOne) / (PHASE_TIME_MAX - widthMinusOne) * 32767.0f) + 32768;
    }
    sineWave16 = cos16(remappedPhaseTime16);
  }
  
  // Convert to [0, 1] with wave shape similar to triangle wave.
  return (uint16_t(32767) - sineWave16) / 65534.0f; // = ( 32767 - cos16(remappedPhaseTime16) ) / 65534
}

}
