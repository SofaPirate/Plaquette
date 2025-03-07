/*
 * MinMaxScaler.h
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

#ifndef MIN_MAX_SCALER_H_
#define MIN_MAX_SCALER_H_

#include "PqCore.h"
#include "MovingFilter.h"

namespace pq {

/// Regularizes signal into [0,1] by rescaling it using the min and max values.
class MinMaxScaler : public MovingFilter {
public:
  /**
   * Default constructor. Assigns infinite time window.
   * @param engine the engine running this unit
   */
  MinMaxScaler(Engine& engine = Engine::singleton());

  /**
   * Constructor with time window.
   * @param timeWindow the time window (in seconds)
   * @param engine the engine running this unit
   */
  MinMaxScaler(float timeWindow, Engine& engine = Engine::singleton());

  virtual ~MinMaxScaler() {}

  /// Returns the current min. value.
  float minValue() const { return _minValue; }

  /// Returns the current max. value.
  float maxValue() const { return _maxValue; }

  /// Sets time window to infinite.
  virtual void infiniteTimeWindow();

  /// Changes the time window (expressed in seconds).
  virtual void timeWindow(float seconds);

  /// Returns the time window (expressed in seconds).
  virtual float timeWindow() const;

  /// Returns true if time window is infinite.
  virtual bool timeWindowIsInfinite() const;

  /// Resets the moving filter.
  virtual void reset();

  /**
   * Pushes value into the unit. If isRunning() is false the filter will not be
   * updated but will just return the filtered value.
   * @param value the value sent to the unit
   * @return the new value of the unit
   */
  virtual float put(float value);

protected:
  virtual void step();

  // Time window (in seconds).
  float _timeWindow;

  // Minimum value ever put (decays over time if time window is finite).
  float _minValue;

  // Maximum value ever put (decays over time if time window is finite).
  float _maxValue;

  // Variables used to compute current value average during a step (in case of multiple calls to put()).
  float _currentValueStep;
};

}

#endif
