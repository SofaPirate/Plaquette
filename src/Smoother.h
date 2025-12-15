/*
 * Smoother.h
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

#ifndef SMOOTHER_H_
#define SMOOTHER_H_

#include "PqCore.h"
#include "MovingFilter.h"

namespace pq {

/// Simple moving average transform filter.
class Smoother : public MovingFilter {
public:
  /**
   * Constructor with default smoothing.
   * @param engine the engine running this unit
   */
  Smoother(Engine& engine = Engine::primary());

  /**
   * Constructor with smoothing window.
   * @param timeWindow the time window over which the smoothing applies (in seconds)
   * @param engine the engine running this unit
   */
  Smoother(float timeWindow, Engine& engine = Engine::primary());
  virtual ~Smoother() {}

  /// Resets the filter.
  virtual void reset();

  /// Resets the filter with a prior estimate of the mean value.
  virtual void reset(float estimatedMeanValue);

  /// Resets the moving filter with a prior estimate of the min and max values.
  virtual void reset(float estimatedMinValue, float estimatedMaxValue);

  /**
   * Pushes value into the unit.
   * @param value the value sent to the unit
   * @return the new value of the unit
   */
  virtual float put(float value);

  /// Returns the filtered value (without calibrating).
  virtual float filter(float value);

protected:
  virtual void step();

  // Variables used to compute current value average during a step (in case of multiple calls to put()).
  float _currentValueStep;
};

}

#endif
