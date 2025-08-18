/*
 * PivotField.cpp
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
#include "PivotField.h"

namespace pq {

PivotField::PivotField() : _mode(LEVEL_FALLING), _value(0), _rampWidth(-1), _rampShift(-1), _halfBumpWidth(-1), _easing(easeNone) {
  rampWidth(0);
  rampShift(0.5f);
  bumpWidth(0.25f);
}

void PivotField::rampWidth(float rampWidth) {
  if (_rampWidth != rampWidth) {
    _rampWidth = constrain01(rampWidth);
#if PQ_OPTIMIZE_FOR_CPU
    _invRampWidth = _rampWidth == 0 ? 0.0f : 1.0f / _rampWidth;
#endif
  }
}

void PivotField::rampShift(float rampShift) {
  if (_rampShift != rampShift) {
    _rampShift = constrain01(rampShift);
#if PQ_OPTIMIZE_FOR_CPU
    _rampShiftFactor = 1.5f - 2*_rampShift;
#endif
  }
}

void PivotField::bumpWidth(float bumpWidth) {
  _halfBumpWidth = constrain01(bumpWidth) / 2;
}

float remapFromCenter(float proportion, float center) {
  return mapTo01(proportion, center, (proportion < center ? 0 : 1));
}


float PivotField::read(float proportion) {
  bool bumpMode = (_mode == LEVEL_BUMP || _mode == LEVEL_NOTCH);
  bool falling = (_mode == LEVEL_FALLING || _mode == LEVEL_BUMP);

  // Remap proportion relative to center.
  proportion = remapFromCenter(proportion, _center);

  // No ramp.
  if (_rampWidth <= 0) {
    if (bumpMode) {
      float diff = proportion - _value;
      return (abs(diff) < _halfBumpWidth ^ falling ? 0 : 1);
    }
    else
      return (proportion <= _value ^ falling ? 0 : 1);
  }

  // Ramp.
  else {
    float rampValue;
    if (bumpMode) {
      proportion = remapFromCenter(proportion, _value);
      rampValue = _ramp(proportion, _halfBumpWidth);
    }
    else {
      // Compute ramp value.
      // Note: the following code is mathematically equivalent to:
      // float center = _value + (2*_rampShift - 1) * _rampWidth;
      // float rampValue = mapTo01(proportion, center - _rampWidth / 2, center + _rampWidth / 2, CONSTRAIN);
      rampValue = _ramp(proportion, _value);
    }

    // Compute easing.
    if (_easing != easeNone)
      rampValue = _easing(rampValue);

    // Invert.
    if (falling)
      rampValue = 1 - rampValue;

    return rampValue;
  }
}

}
