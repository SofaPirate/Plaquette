/*
 * RobustScaler.h
 *
 * (c) 2025 Sofian Audry        :: info(@)sofianaudry(.)com
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
 *
 * (c) 2025 Sofian Audry
 *
 * Adaptive quantile-based scaler using Robbins–Monro updates.
 */

#ifndef SCALER_H_
#define SCALER_H_

#include "PqCore.h"
#include "MovingFilter.h"
#include "MovingAverage.h"

namespace pq {

// Default low quantile level (corresponds to 1% coverage of value in [0, 1]).
#define ROBUST_SCALER_DEFAULT_SPAN 0.99f

/// Regularizes signal into [0,1] using adaptive quantile tracking (robust to outliers).
class RobustScaler : public MovingFilter {
public:
  /// Default constructor.
  RobustScaler(Engine& engine = Engine::primary());

  /**
   * Constructor with custom quantile levels and time window.
   * @param timeWindow The adaptation window in seconds.
   */
  RobustScaler(float timeWindow, Engine& engine = Engine::primary());

  /**
   * Constructor with custom quantile levels and time window.
   * @param timeWindow The adaptation window in seconds.
   * @param span Corresponds to percentage coverage of value in [0, 1].
   */
  RobustScaler(float timeWindow, float span, Engine& engine = Engine::primary());

  virtual ~RobustScaler() {}

  /// Sets the span (in [0, 1]) of the quantile to track.
  virtual void span(float span);

  /// Returns the current span.
  virtual float span() const;

  /// Sets the low quantile level (in [0, 0.5]). Low quantile will automatically be set to 1 - low.
  virtual void lowQuantileLevel(float level);

  /// Returns the current low quantile level.
  virtual float lowQuantileLevel() const { return _quantileLevel; }

  /// Sets the high quantile level (in [0.5, 1]). Low quantile will automatically be set to 1 - high.
  virtual void highQuantileLevel(float level);

  /// Returns the current high quantile level.
  virtual float highQuantileLevel() const { return (1 - _quantileLevel); }

  /// Resets the filter.
  virtual void reset();

  /// Resets the filter with a prior estimate of the mean value.
  virtual void reset(float estimatedMeanValue);

  /// Resets the moving filter with a prior estimate of the min and max values.
  virtual void reset(float estimatedMinValue, float estimatedMaxValue);

  /// Returns value of scaler.
  virtual float get() { return _value; }

  /// Pushes a new value and returns the scaled output.
  virtual float put(float value);

protected:
  virtual void step();

  // Internal quantile update (Robbins–Monro).
  inline void _updateQuantile(float& q, float level, float eta, float x);

  void _initializeRange(float minValue, float maxValue);

protected:
public:
  // Time window (in seconds).
  float _timeWindow;

  // Low quantile level (in [0, 0.5]).
  float _quantileLevel;

  // Quantile estimators.
  float _lowQuantile;  // q_low
  float _highQuantile; // q_high

  // Stddev estimator.
  MovingAverage _stdDev;

  // Variables used to compute current value average during a step (in case of multiple calls to put()).
  float _currentStdDevStep;

  // Number of samples since last reset.
  uint32_t _nSamples;
};

}

#endif
