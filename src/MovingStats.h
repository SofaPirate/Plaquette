/*
 * MovingStats.h
 *
 * Computes floating-point mean and variance statistics over time using an exponential moving average.
 * This class computes the following statistics: mean, variance and standard deviation.
 *
 * (c) 2014 Sofian Audry -- info(@)sofianaudry(.)com
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

#ifndef MOVINGSTATS_H_
#define MOVINGSTATS_H_

#include "MovingAverage.h"

namespace pq {

class MovingStats {
public:
  // Moving average over values (ie. mean).
  MovingAverage _avg;

  // Moving average of variance.
  float _var;

  /// Default constructor (infinite time window).
  MovingStats();

  /// Default constructor (finite time window).
  MovingStats(float timeWindow);

  virtual ~MovingStats() {}

  /// Sets to "infinite" smoothing window.
  void infiniteTimeWindow() { _avg.infiniteTimeWindow(); }

  /// Changes the smoothing window (expressed in seconds).
  void timeWindow(float seconds);

  /// Returns the smoothing window (expressed in seconds).
  float timeWindow() const { return _avg.timeWindow(); }

  /// Returns true if time window is infinite.
  bool timeWindowIsInfinite() const { return _avg.timeWindowIsInfinite(); }

  /// Changes the smoothing window cutoff frequency (expressed in Hz).
  void cutoff(float hz);

  /// Returns the smoothing window cutoff frequency (expressed in Hz).
  float cutoff() const { return _avg.cutoff(); }

  /// Resets the statistics.
  virtual void reset();

  /// Adds a value to the statistics (returns the mean).
  virtual float update(float value, float sampleRate=1);

  /// Returns an exponential moving average of the samples.
  virtual float mean() const { return _avg.constGet(); }

  /// Returns an exponential moving variance of the samples.
  virtual float var() const { return _var; }

  /// Returns the standard deviation of the samples.
  virtual float stddev() const;

  /// Returns the normalized value according N(0, 1).
  float normalize(float value) const;

  /// Returns the normalized value according to the computed statistics (mean and variance).
  float normalize(float value, float mean, float stddev) const;
};

} // namespace pq

#endif /* MOVINGSTATS_H_ */
