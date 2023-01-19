/*
 * SquareOsc.cpp
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

#include "SquareOsc.h"

namespace pq {

SquareOsc::SquareOsc(float period_, float dutyCycle_) : Osc(period_) {
  dutyCycle(dutyCycle_);
}

bool SquareOsc::isOn() {
  return analogToDigital(_value);
}

void SquareOsc::_updateValue() {
	_value = 0.5f + 0.5f * (_phaseTime <= _dutyCycle ? _amplitude : -_amplitude);
}

void SquareOsc::dutyCycle(float dutyCycle) {
  _dutyCycle = float2phaseTime(constrain(dutyCycle, 0, 1));
}

}
