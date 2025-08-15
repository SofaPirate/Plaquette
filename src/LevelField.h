/*
 * LevelField.h
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
#ifndef LEVEL_FIELD_H_
#define LEVEL_FIELD_H_

#include "AbstractField.h"
#include "pq_easing.h"

namespace pq {

class LevelField : public AbstractField
{
public:
  /// Constructor.
  LevelField();
  virtual ~LevelField() {}

  /**
   * Returns value at given proportion in [0, 1].
   * @param proportion the proportion of the field to read
   * @return the value
   */
  virtual float read(float proportion) override;

    /**
   * Sets easing function to apply to ramp.
   * @param easing the easing function
   */
  void easing(easing_function easing) { _easing = easing; }

  /// Remove easing function (linear/no easing).
  void noEasing() { easing(easeNone); }

  /**
   * Sets ramp width as % of field range.
   * @param rampWidth the ramp width in [0, 1]
   */
  void rampWidth(float rampWidth);

  /// Removes ramp width.
  void noRampWidth() { rampWidth(0); }

  /**
   * Sets ramp shift in [0, 1] (default: 0.5 = center).
   * @param rampShift the ramp shift in [0, 1]
   */
  void rampShift(float rampShift);

  /// Returns ramp width.
  float rampWidth() const { return _rampWidth; }

  /// Returns ramp shift.
  float rampShift() const { return _rampShift; }

  /// Sets falling or rising (from left to right).
  void rising() { _falling = false; }
  void falling() { _falling = true; }

  bool isRising() const { return !_falling; }
  bool isFalling() const { return _falling; }

protected:
  virtual float _read() override { return _value; }

  virtual float _write(float value) override {
    return (_value = constrain01(value));
  }

protected:
  // The current value.
  float _value;

  // The ramp width as %.
  float _rampWidth;

  // The ramp shift in [0, 1].
  float _rampShift;

  // Is the value falling or rising (from left to right)?
  bool _falling;

  // Optional easing function.
  easing_function _easing;

#if PQ_OPTIMIZE_FOR_CPU
  // Precompiled values.
  float _invRampWidth;
  float _rampShiftFactor;
#endif
};

};
#endif
