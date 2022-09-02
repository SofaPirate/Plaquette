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
  /// Constructor.
  MinMaxScaler();
  MinMaxScaler(float decayWindow);
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

  /// Resets the moving filter.
  virtual void reset();

  /**
   * Pushes value into the unit. If isStarted() is false the filter will not be
   * updated but will just return the filtered value.
   * @param value the value sent to the unit
   * @return the new value of the unit
   */
  virtual float put(float value);

protected:
  // Minmum value ever put.
  float _minValue;

  // Maximum value ever put.
  float _maxValue;
};

}

#endif
