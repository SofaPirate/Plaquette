/*
 * MovingFilter.h
 *
 * (c) 2022 Sofian Audry        :: info(@)sofianaudry(.)com
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

#ifndef MOVING_FILTER_H_
#define MOVING_FILTER_H_

#include "PqCore.h"

#define MOVING_FILTER_INFINITE_TIME_WINDOW (-1)

namespace pq {

class MovingFilter : public AnalogSource {
protected:
  /**
   * Constructor.
   * @param engine the engine running this unit
   */
  MovingFilter(Engine& engine = Engine::primary());

  virtual ~MovingFilter() {}

public:
  /// Sets time window to infinite.
  virtual void infiniteTimeWindow() = 0;

  /// Changes the time window (expressed in seconds).
  virtual void timeWindow(float seconds) = 0;

  /// Returns the time window (expressed in seconds).
  virtual float timeWindow() const = 0;

  /// Returns true if time window is infinite.
  virtual bool timeWindowIsInfinite() const = 0;

  /// Changes the time window cutoff frequency (expressed in Hz).
  virtual void cutoff(float hz);

  /// Returns the time window cutoff frequency (expressed in Hz).
  virtual float cutoff() const;

  /// Resets the moving filter.
  virtual void reset();

  /**
   * Switches to calibration mode (default). Calls to put(value) will return filtered 
   * value AND update the normalization statistics.
   */
  virtual void resumeCalibrating();

  /**
   * Switches to non-calibration mode: calls to put(value) will return filtered value 
   * without updating the normalization statistics.
   */
  virtual void pauseCalibrating();

  /// Toggles calibration mode.
  virtual void toggleCalibrating();

  /// Returns true iff the moving filter is in calibration mode.
  virtual bool isCalibrating() const;

protected:
  // Start/stop calibration flag.
  bool    _isCalibrating : 1;
  uint8_t _nValuesStep   : 7;
};

}

#endif
