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

namespace pq {

/**
 * Provides a ramping / tweening mechanism that allows smooth transitions between
 * two values.
 */
class Ramp : public PqAnalogSource, public AbstractTimer {
public:
  /**
   * Constructor.
   * @param initialValue the value the ramp starts with
   */
  Ramp(float initialValue=0.0f);

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

  /// Starts/restarts the chronometer.
  virtual void start();

  /**
   * Starts a new ramp (starting from current value).
   * @param to the final value
   * @param duration the duration of the ramp (in seconds)
   */
  virtual void start(float to, float duration);

  /**
   * Starts a new ramp.
   * @param from the initial value
   * @param to the final value
   * @param duration the duration of the ramp (in seconds)
   */
  virtual void start(float from, float to, float duration);

protected:

  virtual void step();

  // The starting point.
  float _from;

  // The variation from starting point targetted.
  float _change;
};

}

#endif
