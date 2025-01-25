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

#define USE_FAST_SQRT 1

#if USE_FAST_SQRT
#include "pq_fastmath.h"
#endif

namespace pq {

MovingStats::MovingStats() : _avg(), _mean2(0) { }
MovingStats::MovingStats(float timeWindow) : _avg(timeWindow), _mean2(0) { }

void MovingStats::timeWindow(float seconds) {
  _avg.timeWindow(seconds);
}

void MovingStats::cutoff(float hz) {
  _avg.cutoff(hz);
}

void MovingStats::reset() {
  _avg.reset();
  _mean2 = 0;
}

float MovingStats::update(float value, float sampleRate)
 {
  // Get alpha.
  float alpha = _avg.alpha(sampleRate);

  // Update average.
  _avg.update(value, alpha, true); // force alpha

  // Update mean of squares.
  MovingAverage::applyUpdate(_mean2, sq(value), alpha);

  return normalize(value);
}

float MovingStats::stdDev() const {
  return
#if USE_FAST_SQRT
  fastSqrt
#else
  sqrt
#endif
  (var());
}

float MovingStats::normalize(float value) const {
  float s = stdDev();
  return ( value - mean() ) /
#if USE_FAST_SQRT
  s // Saves a call to max() since fastSqrt(0) returns small positive values
#else
  (max(s, FLT_MIN)) // Need to cover the case where s = 0.
  #endif
  ;
}

float MovingStats::normalize(float value, float mean_, float stdDev_) const {
  return normalize(value) * stdDev_ + mean_;
}

bool MovingStats::isOutlier(float value, float nStdDev) const {
  float z = normalize(value);
  return abs(z) >= abs(nStdDev);
}

bool MovingStats::isLowOutlier(float value, float nStdDev) const {
  return normalize(value) <= (-abs(nStdDev));
}

bool MovingStats::isHighOutlier(float value, float nStdDev) const {
  return normalize(value) >= abs(nStdDev);
}

} // namespace pq
