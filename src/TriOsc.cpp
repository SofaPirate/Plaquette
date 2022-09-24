/*
 * TriOsc.cpp
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

#include "TriOsc.h"

namespace pq {

TriOsc::TriOsc(float period_, float width_) : Osc(period_), _width(0) {
  width(width_);
}

void TriOsc::_updateValue() {
  // Convert phase time to float in [0, 1].
  float phaseTimeFloat = phaseTime2float(_phaseTime);
	// Compute triangle depending on raising or falling step.
  if (phaseTimeFloat <= _width)
    _value = phaseTimeFloat / _width;
	else
    _value = (1 - phaseTimeFloat) / (1 - _width);
	// Amplify.
  _value = 0.5f + 0.5f * (_amplitude * (2*_value - 1));
}

TriOsc& TriOsc::width(float width) {
	if (width != _width)
  	_width = constrain(width, 0, 1);
	return *this;
}

}
