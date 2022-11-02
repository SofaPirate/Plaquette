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

#define NORMALIZER_DEFAULT_MEAN   0.5f
#define NORMALIZER_DEFAULT_STDDEV 0.15f

/**
 * Adaptive normalizer: normalizes values on-the-run using exponential moving
 * averages over mean and standard deviation.
 */
class Normalizer : public MovingFilter, public MovingStats {
public:
  /**
   * Default constructor. Will renormalize data around a mean of 0.5 and a standard deviation of 0.15.
   */
  Normalizer();

  /**
   * Will renormalize data around a mean of 0.5 and a standard deviation of 0.15.
   * @param smoothWindow specifies the approximate "time window" over which the normalization applies(in seconds)
   */
  Normalizer(float timeWindow);

  /**
   * Constructor with infinite time window.
   * @param mean the target mean
   * @param stdDev the target standard deviation
   * @param smoothWindow specifies the approximate "time window" over which the normalization applies(in seconds)
   */
  Normalizer(float mean, float stdDev);

  /**
   * Constructor.
   * @param mean the target mean
   * @param stdDev the target standard deviation
   * @param smoothWindow specifies the approximate "time window" over which the normalization applies(in seconds)
   */
  Normalizer(float mean, float stdDev, float timeWindow);

  virtual ~Normalizer() {}

  /**
   * Sets target mean of normalized values.
   * @param mean the target mean
   */
  Normalizer& targetMean(float mean) { _targetMean = mean; return *this; }

  /// Returns target mean.
  float targetMean() const { return _targetMean; }

  /**
   * Sets target standard deviation of normalized values.
   * @param stdDev the target standard deviation
   */
  Normalizer& targetStdDev(float stdDev) { _targetStdDev = abs(stdDev); return *this; }

  /// Returns target standard deviation.
  float targetStdDev() const { return _targetStdDev; }

  /// Sets time window to infinite.
  virtual void infiniteTimeWindow();

  /// Changes the time window (expressed in seconds).
  virtual void timeWindow(float seconds);

  /// Returns the time window (expressed in seconds).
  virtual float timeWindow() const;

  /// Returns true if time window is infinite.
  virtual bool timeWindowIsInfinite() const;

  virtual void reset();

  /**
   * Pushes value into the unit. If isStarted() is false the filter will not be
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

  }

protected:
  // Target normalization parameters.
  float _targetMean;
  float _targetStdDev;
};

}

#endif
