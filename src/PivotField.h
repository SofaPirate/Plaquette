/*
 * PivotField.h
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
#ifndef PIVOT_FIELD_H_
#define PIVOT_FIELD_H_

#include "AbstractField.h"
#include "pq_easing.h"

namespace pq {

enum PivotFieldMode {
  PIVOT_RISING,
  PIVOT_FALLING,
  PIVOT_BUMP,
  PIVOT_NOTCH
};

class PivotField : public AbstractField
{
public:
  /// Constructor.
  PivotField();
  virtual ~PivotField() {}

  /**
   * Returns value at given proportion in [0, 1].
   * @param proportion the proportion of the field to read
   * @return the value
   */
  virtual float read(float proportion) override;

    /// Returns value.
  virtual float get() override { return _value; }

  /**
   * Pushes value into the unit.
   * @param value the value sent to the unit
   * @return the new value of the unit
   */
  virtual float put(float value) override {
    return (_value = constrain01(value));
  }

  /**
   * Sets mode to use.
   * @param mode the mode to set
   */
  void mode(PivotFieldMode mode) { _mode = mode; }

  /// Returns mode.
  PivotFieldMode mode() const { return _mode; }

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

  /// Returns ramp width.
  float rampWidth() const { return _rampWidth; }

  /**
   * Sets ramp shift in [0, 1] (default: 0.5 = center).
   * @param rampShift the ramp shift in [0, 1]
   */
  void rampShift(float rampShift);

  /// Returns ramp shift.
  float rampShift() const { return _rampShift; }

  /**
   * Sets bump width as % of field range. Only applies to PIVOT_BUMP and PIVOT_NOTCH modes.
   * @param bumpWidth the bump width in [0, 1]
  */
  void bumpWidth(float bumpWidth);

  /// Returns bump width.
  float bumpWidth() const { return 2*_halfBumpWidth; }

  /**
   * Sets center of the ramp in [0, 1].
   * @param center the center in [0, 1]
   */
  void center(float center) { _center = constrain01(center); }

  /// Returns center of the ramp.
  float center() const { return _center; }

protected:
  // Internal use: return ramp value based on proportion compared to value.
  float _ramp(float proportion, float value) {
    return
#if PQ_OPTIMIZE_FOR_CPU
      constrain01( (proportion - value) * _invRampWidth + _rampShiftFactor );
#else
      constrain01( (proportion - value) / _rampWidth - 2*_rampShift + 1.5f );
#endif
  }

protected:
  // The current mode.
  PivotFieldMode _mode;

  // The current value.
  float _value;

  // The ramp width as %.
  float _rampWidth;

  // The ramp shift in [0, 1].
  float _rampShift;

  // The bump width as %.
  float _halfBumpWidth;

  // The center of the ramp in [0, 1].
  float _center;

  // Optional easing function.
  easing_function _easing;

#if PQ_OPTIMIZE_FOR_CPU
  // Precompiled values.
  float _invRampWidth;
  float _rampShiftFactor;
#endif
};

}
#endif
