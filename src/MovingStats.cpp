/*
 * MovingStats.cpp
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

#include "MovingStats.h"

#include <float.h>

namespace pq {

MovingStats::MovingStats() : _avg(), _var(0) { }
MovingStats::MovingStats(float timeWindow) : _avg(timeWindow), _var(0) { }

void MovingStats::timeWindow(float seconds) {
  _avg.timeWindow(seconds);
}

void MovingStats::cutoff(float hz) {
  _avg.cutoff(hz);
}

void MovingStats::reset() {
  _avg.reset();
  _var = 0;
}

float MovingStats::update(float value, float sampleRate)
 {
  // Get alpha.
  float alpha = _avg.alpha(sampleRate);

  // Update average.
  _avg.update(value, alpha, true); // force alpha

  // Update variance.
  float diff = value - _avg.get();
  MovingAverage::applyUpdate(_var, sq(diff), alpha);

  return normalize(value);
}

float MovingStats::stddev() const { return sqrt(var()); }

float MovingStats::normalize(float value, float mean_, float stddev_) const {
  float s = stddev();
  return ( value - mean() ) / (max(s, FLT_MIN)) * stddev_ + mean_;
}

} // namespace pq
