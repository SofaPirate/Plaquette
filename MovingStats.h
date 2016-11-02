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

#include "Stats.h"
#include "MovingAverage.h"

class MovingStats : public Stats {
public:
  MovingAverage avg;
  float _var;

  /**
   * Constructs the moving statistics, starting with #startMean# and #startVar# as initial mean and
   * variance. The #alphaOrN# argument has two options:
   * - if <= 1 then it's used directly as the alpha value
   * - if > 1 then it's used as the "number of items that are considered from the past" (*)
   * (*) Of course this is an approximation. It actually sets the alpha value to 2 / (n - 1)
   */
  MovingStats(float alphaOrN=1);
  MovingStats(float alphaOrN, float startMean, float startVar);
  virtual ~MovingStats() {}

  /// Resets the statistics.
  virtual void reset();

  /// Resets the statistics.
  virtual void reset(float startMean, float startVar);

  /// Adds a value to the statistics (returns the mean).
  virtual float update(float value);

  /// The statistics.
  virtual float mean() const { return avg.get(); }
  virtual float var() const { return _var; }

  virtual bool isStarted() const;
};

#endif /* MOVINGSTATS_H_ */
