/*
 * MovingAverage.h
 *
 * Tool for moving averages.
 *
 * This file is part of Qualia https://github.com/sofian/qualia
 *
 * (c) 2011 Sofian Audry -- info(@)sofianaudry(.)com
 * Inspired by code by Karsten Kutza
 * http://www.ip-atlas.com/pub/nap/nn-src/bpn.txt
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
#ifndef MOVINGAVERAGE_H_
#define MOVINGAVERAGE_H_

#include "PqCore.h"
#include "pq_moving_average.h"

namespace pq {

/// An exponential moving average class.
class MovingAverage {
public:
  /// Default constructor (infinite time window).
  MovingAverage();
  virtual ~MovingAverage() {}

  /// Resets the moving average.
  void reset();

  /// Resets the moving average with initial value.
  void reset(float initialValue);

  /// Updates the moving average with new value #v# (also returns the current value).
  virtual float update(float v, float alpha);

  /// Returns the value of the moving average. This is undefined if isValid() == false.
  float get() { return _value; }
  float constGet() const { return _value; }

protected:
  // The current value of the exponential moving average.
  float _value;
};

} // namespace pq

#endif
