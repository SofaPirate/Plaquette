/*
 * SquareWave.cpp
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

#include "SquareWave.h"

namespace pq {

SquareWave::SquareWave(float period_, float width_) : AbstractWave(period_) {
  width(width_);
}

bool SquareWave::isOn() {
  return analogToDigital(_value);
}

float SquareWave::_get(phase_time_t t) {
  return (t <= _width ? 1.0f : 0.0f);
}

void SquareWave::width(float width_) {
  _width = float2phaseTime(constrain(width_, 0, 1));
}

}
