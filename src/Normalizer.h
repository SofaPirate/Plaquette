/*
 * Normalizer.h
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

#ifndef NORMALIZER_H_
#define NORMALIZER_H_

#include "PqCore.h"
#include "MovingFilter.h"
#include "MovingStats.h"

namespace pq {

// Default mean and standard deviation.
constexpr float NORMALIZER_DEFAULT_MEAN   = 0.5f;
constexpr float NORMALIZER_DEFAULT_STDDEV = 1.0f / MOVING_FILTER_N_STDDEV_RANGE;
// sum_i x_i^2 = stddev^2 + (sum_i x_i)^2
constexpr float NORMALIZER_DEFAULT_MEAN2 = sq(NORMALIZER_DEFAULT_STDDEV) + sq(NORMALIZER_DEFAULT_MEAN);

// This is so that the basic normalizer will stay within range [0, 1].
constexpr float NORMALIZER_DEFAULT_CLAMP_STDDEV = 0.5f * MOVING_FILTER_N_STDDEV_RANGE - FLT_MIN;

// Label for no clamping.
#define NORMALIZER_NO_CLAMP 0

/**
 * Adaptive normalizer: normalizes values on-the-run using exponential moving
 * averages over mean and standard deviation.
 */
class Normalizer : public MovingFilter, public MovingStats {
public:
  /**
   * Default constructor. Assigns infinite time window.
   * Will renormalize data around a mean of 0.5 and a standard deviation of 0.15.
   * @param engine the engine running this unit
   */
  Normalizer(Engine& engine = Engine::primary());

  /**
   * Constructor with time window.
   * Will renormalize data around a mean of 0.5 and a standard deviation of 0.15.
   * @param timeWindow the time window over which the normalization applies (in seconds)
   * @param engine the engine running this unit
   */
  Normalizer(float timeWindow, Engine& engine = Engine::primary());

  /**
   * Constructor with infinite time window.
   * @param mean the target mean
   * @param stdDev the target standard deviation
   */
  Normalizer(float mean, float stdDev, Engine& engine = Engine::primary());

  /**
   * Constructor with time window.
   * @param mean the target mean
   * @param stdDev the target standard deviation
   * @param timeWindow the time window over which the normalization applies (in seconds)
   */
  Normalizer(float mean, float stdDev, float timeWindow, Engine& engine = Engine::primary());

  virtual ~Normalizer() {}

  /**
   * Sets target mean of normalized values.
   * @param mean the target mean
   */
  void targetMean(float mean) { _targetMean = mean; }

  /// Returns target mean.
  float targetMean() const { return _targetMean; }

  /**
   * Sets target standard deviation of normalized values.
   * @param stdDev the target standard deviation
   */
  void targetStdDev(float stdDev) { _targetStdDev = abs(stdDev); }

  /// Returns target standard deviation.
  float targetStdDev() const { return _targetStdDev; }


  /// Resets the statistics.
  virtual void reset();

  /// Resets the filter with a prior estimate of the mean value.
  virtual void reset(float estimatedMeanValue);

  /// Resets the moving filter with a prior estimate of the min and max values.
  virtual void reset(float estimatedMinValue, float estimatedMaxValue);

  /**
   * Pushes value into the unit. If isRunning() is false the filter will not be
   * updated but will just return the filtered value.
   * @param value the value sent to the unit
   * @return the new value of the unit
   */
  virtual float put(float value);

  /**
   * Returns value above which value is considered to be a low outler (below average).
   * @param nStdDev the number of standard deviations (typically between 1 and 3); low values = more sensitive
   */
  virtual float lowOutlierThreshold(float nStdDev=1.5f) const;

  /**
   * Returns value above which value is considered to be a high outler (above average).
   * @param nStdDev the number of standard deviations (typically between 1 and 3); low values = more sensitive
   */
  virtual float highOutlierThreshold(float nStdDev=1.5f) const;

  /// Return true iff the normalized value is clamped within reasonable range.
  bool isClamped() const;

  /**
   * Assign clamping value. Values will then be clamped between reasonable range
   * (targetMean() +/- nStdDev * targetStdDev()).
   * @param nStdDev the number of standard deviations (default: 3.333333333)
   */
  void clamp(float nStdDev=NORMALIZER_DEFAULT_CLAMP_STDDEV);

  /// Remove clamping.
  void noClamp();

  /// Maps value to new range.
  virtual float mapTo(float toLow, float toHigh);

protected:
  virtual void step();

  // Helper function for constructors.
  void _init(float mean, float stdDev);

  // Returns clamped value.
  float _clamp(float value) const;

  // Target normalization parameters.
  float _targetMean;
  float _targetStdDev;

  // Clamped standard deviation (if 0 = no clamp).
  float _clampStdDev;

  // Variables used to compute current value average during a step (in case of multiple calls to put()).
  float _currentMeanStep;
  float _currentMean2Step;
};

}

#endif
