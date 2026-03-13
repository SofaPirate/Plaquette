/*
 * SquareWave.h
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

#ifndef SQUARE_WAVE_H_
#define SQUARE_WAVE_H_

#include "Wave.h"

namespace pq {

/**
 * @deprecated
 * Square oscillator. Duty cycle is expressed as % of period.
 */
class [[deprecated("Use Wave(SQUARE) instead.")]] SquareWave : public Wave {
public:
  SquareWave(Engine& engine = Engine::primary()) : Wave(SQUARE, engine) {}
  SquareWave(float period, Engine& engine = Engine::primary()) : Wave(SQUARE, period, engine) {}
  SquareWave(float period, float skew, Engine& engine = Engine::primary()) : Wave(SQUARE, period, skew, engine) {}
  virtual ~SquareWave() {}

  /// @deprecated
  [[deprecated("Use skew(float) instead.")]]
  virtual void dutyCycle(float dutyCycle) { skew(dutyCycle); }

  /// @deprecated
  [[deprecated("Use skew() instead.")]]
  virtual float dutyCycle() const { return skew(); }
};

/// @deprecated
[[deprecated("Use Wave(SQUARE) instead.")]]
typedef SquareWave SquareOsc;

}

#endif
