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
   * Sets left skew ie. how far to the left of current value the ramp starts.
   * @param leftSkew the left skew in [0, 1]
   */
  void leftSkew(float leftSkew) {
    _leftSkew = constrain01(leftSkew);
  }

  /// Removes left skew.
  void noLeftSkew() { leftSkew(0); }

  /**
   * Sets right skew ie. how far to the right of current value the ramp starts.
   * @param leftSkew the right skew in [0, 1]
   */
  void rightSkew(float rightSkew) {
    _rightSkew = constrain01(rightSkew);
  }

  /// Removes right skew.
  void noRightSkew() { rightSkew(0); }

  /// Returns left skew.
  float leftSkew() const { return _leftSkew; }

  /// Returns right skew.
  float rightSkew() const { return _rightSkew; }

  void rising() { _falling = false; }
  void falling() { _falling = true; }

  bool isRising() const { return !_falling; }
  bool isFalling() const { return _falling; }

protected:
  virtual float _read() override
  {
    return _value;
  }

  virtual float _write(float value) override {
    _value = constrain01(value);
    return _value;
  }

protected:
  // The current value.
  float _value;

  // Left and right skew.
  float _leftSkew;
  float _rightSkew;

  // Is the value falling or rising (from left to right)?
  bool _falling;

  // Optional easing function.
  easing_function _easing;
};

};
#endif
