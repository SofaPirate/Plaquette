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

MovingStats::MovingStats(float smoothWindow) : _avg(smoothWindow), _var(0) { }

void MovingStats::time(float seconds) {
  _avg.time(seconds);
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
  float alpha = _avg.alpha(sampleRate);
  _avg.update(value, alpha, true); // force alpha
  if (!isStarted())
    _var = 0;
  else {
    float diff = value - _avg.get();
    MovingAverage::applyUpdate(_var, sq(diff), alpha);
  }

  return normalize(value);
}

bool MovingStats::isStarted() const {
  return _avg.isStarted();
}

} // namespace pq
