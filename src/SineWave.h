/*
 * SineWave.h
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

#ifndef SINE_WAVE_H_
#define SINE_WAVE_H_

#include "Wave.h"

namespace pq {

  /**
 * @deprecated
 * Sine oscillator. Phase is expressed as % of period.
 */
class [[deprecated("Use Wave(SINE) instead.")]] SineWave : public Wave {
public:
  SineWave(Engine& engine = Engine::primary()) : Wave(SINE, engine) {}
  SineWave(float period, Engine& engine = Engine::primary()) : Wave(SINE, period, engine) {}
  SineWave(float period, float skew, Engine& engine = Engine::primary()) : Wave(SINE, period, skew, engine) {}
  virtual ~SineWave() {}
};

/// @deprecated
[[deprecated("Use Wave(SINE) instead.")]]
typedef SineWave SineOsc;

}

#endif
