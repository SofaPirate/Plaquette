/*
 * LevelField.cpp
 *
 * (c) 2025 Sofian Audry        :: info(@)sofianaudry(.)com
 * (c) 2018 Thomas O Fredericks :: tof(@)t-o-f(.)info
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
#include "LevelField.h"

namespace pq {

LevelField::LevelField() : _value(0), _leftSkew(0), _rightSkew(0), _falling(true), _easing(easeNone) {}

float LevelField::read(float proportion) {
  // No skew: return square (no ramp) value.
  if (_leftSkew == 0 && _rightSkew == 0) {
    return (proportion <= _value ^ _falling ? 0 : 1);
  }

  // Skew: return ramp value.
  else {
    float rampValue = mapTo01(proportion,
                              _value * (1 - _leftSkew),               // ramp start point
                              _value * (1 - _rightSkew) + _rightSkew, // ramp end point
                              CONSTRAIN);

    // Compute easing.
    if (_easing != easeNone)
      rampValue = _easing(rampValue);

    // Invert.
    if (_falling)
      rampValue = 1 - rampValue;

    return rampValue;
  }
}

}
