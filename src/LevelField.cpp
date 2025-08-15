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

LevelField::LevelField() : _value(0), _rampWidth(-1), _rampShift(-1), _falling(true), _easing(easeNone) {
  rampWidth(0);
  rampShift(0.5f);
}

void LevelField::rampWidth(float rampWidth) {
  if (_rampWidth != rampWidth) {
    _rampWidth = constrain01(rampWidth);
#if PQ_OPTIMIZE_FOR_CPU
    _invRampWidth = _rampWidth == 0 ? 0.0f : 1.0f / _rampWidth;
#endif
  }
}

void LevelField::rampShift(float rampShift) {
  if (_rampShift != rampShift) {
    _rampShift = constrain01(rampShift);
#if PQ_OPTIMIZE_FOR_CPU
    _rampShiftFactor = 1.5f - 2*_rampShift;
#endif
  }
}

float LevelField::read(float proportion) {
  // No skew: return square (no ramp) value.
  if (_rampWidth <= 0) {
    return (proportion <= _value ^ _falling ? 0 : 1);
  }

  // Skew: return ramp value.
  else {
    // Compute ramp value.
    // Note: the following code is equivalent to:
    // float center = _value + (2*_rampShift - 1) * _rampWidth
    // float rampValue = mapTo01(proportion, _value - center, _value + center, CONSTRAIN);
#if PQ_OPTIMIZE_FOR_CPU
    float rampValue = (proportion - _value) * _invRampWidth + _rampShiftFactor;
#else
    float rampValue = (proportion - _value) / _rampWidth - 2*_rampShift + 1.5f;
#endif
    // Clamp in [0, 1].
    rampValue = constrain01(rampValue);

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
