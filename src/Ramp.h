/*
 * Ramp.h
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

#ifndef RAMP_H_
#define RAMP_H_

#include "PqCore.h"
#include "AbstractTimer.h"
#include "pq_easing.h"

namespace pq {

/**
 * Provides a ramping / tweening mechanism that allows smooth transitions between
 * two values.
 */
class Ramp : public Node, public AbstractTimer {
public:
  /**
   * Basic constructor. Use one of the start(...) functions to launch ramps
   * with specific parameters.
   * @param from the value the ramp starts with
   */
  Ramp(float from=0.0f);

  /**
   * Basic constructor. Use one of the start(...) functions to launch ramps
   * with specific parameters.
   * @param from the initial value
   * @param to the final value
   * @param duration the duration of the ramp (in seconds)
   * @param easing the easing function to apply (default: no easing)
   */
  Ramp(float from, float to, float duration, easing_function easing=easeNone);

  /// Returns value of ramp.
  virtual float get() { return _value; }

  /**
   * Forces value in the ramp. If this happens while the ramp is running, it will
   * interrupt the ramp.
   * @param value the value sent to the unit
   * @return the new value of the unit
   */
  virtual float put(float value);

  /**
   * Sets easing function to apply to ramp.
   * @param easing the easing function
   */
  void easing(easing_function easing);

  /// Remove easing function (linear/no easing).
  void noEasing() { easing(easeNone); }

  /**
   * Assign final value of the ramp starting from current value.
   * @param to the final value
   */
  virtual void to(float to);

  /**
   * Assign initial and final values of the ramp.
   * @param from the initial value
   * @param to the final value
   */
  virtual void fromTo(float from, float to);

  /// Starts/restarts the ramp. Will repeat the last ramp.
  virtual void start();

  /**
   * Starts a new ramp, starting from current value.
   * @param to the final value
   * @param duration the duration of the ramp (in seconds)
   * @param easing the easing function (optional)
   */
  virtual void start(float to, float duration, easing_function easing=0);

  /**
   * Starts a new ramp.
   * @param from the initial value
   * @param to the final value
   * @param duration the duration of the ramp (in seconds)
   * @param easing the easing function (optional).
   */
  virtual void start(float from, float to, float duration, easing_function easing=0);

protected:
  // Overrides Node.step().
  virtual void step();

  // The starting point.
  float _from;

  // The end point.
  float _to;

  // The current value.
  float _value;

  // Optional easing function.
  easing_function _easing;
};

}

#endif
