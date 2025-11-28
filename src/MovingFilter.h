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
#include "TimeWindowable.h"
#include "pq_moving_average.h"

namespace pq {

// Max value of _nValuesStep.
#define MOVING_FILTER_N_VALUES_STEP_MAX 63
constexpr float MOVING_FILTER_N_VALUES_STEP_MAX_MINUS_ONE = MOVING_FILTER_N_VALUES_STEP_MAX - 1;

// Precomputed proportion to add one value to _nValuesStep when it has reached MOVING_FILTER_N_VALUES_STEP_MAX.
constexpr float MOVING_FILTER_VALUES_STEP_ADD_ONE_PROPORTION = MOVING_FILTER_N_VALUES_STEP_MAX / (MOVING_FILTER_N_VALUES_STEP_MAX + 1.0f);

// N. standard deviations considered in full range.
#define MOVING_FILTER_N_STDDEV_RANGE 5.0f

class MovingFilter : public AnalogSource, public TimeWindowable {
protected:
  /**
   * Constructor.
   * @param engine the engine running this unit
   */
  MovingFilter(Engine& engine = Engine::primary());

  virtual ~MovingFilter() {}

public:
  /// Resets the filter.
  virtual void reset();

  /// Resets the filter with a prior estimate of the mean value.
  virtual void reset(float estimatedMeanValue);

  /// Resets the moving filter with a prior estimate of the min and max values.
  virtual void reset(float estimatedMinValue, float estimatedMaxValue);

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

  /// Returns the number of samples that have been processed thus far.
  unsigned int nSamples() const { return _nSamples; }

  /// Returns true if the moving filter has been initialized with a starting range at reset.
  virtual bool isPreInitialized() const { return _isPreInitialized; }

protected:
  virtual void begin();

  // Returns the instantaneous moving average alpha for this filter.
  virtual float alpha() const {
    return movingAverageAlpha(sampleRate(), timeWindow(), nSamples(), isPreInitialized());
  }

  // Start/stop calibration flag.
  bool    _isCalibrating    : 1;
  bool    _isPreInitialized : 1;
  uint8_t _nValuesStep      : 6;

  // Number of samples that have been processed thus far.
  unsigned int _nSamples;
};

}

#endif
