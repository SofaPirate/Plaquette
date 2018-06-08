/*
 * SimpleStats.cpp
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

#include "SimpleStats.h"
#include <limits.h>

namespace pq {

SimpleStats::SimpleStats() {
  reset();
}

void SimpleStats::reset() {
  _mean = _mean2 = 0;
  _nSamples = 0;
//  _min = FLT_MAX;
//  _max = -FLT_MAX;
}

float SimpleStats::update(float value, float sampleRate) {
  // Simple trick that makes sure we don't overflow.
  if (_nSamples < ULONG_MAX)
    _nSamples++;

  // Update mean and mean2.
  // float prop = (float)(_nSamples-1) / (float)_nSamples;
  // _mean  = _mean  * prop + value     / _nSamples;
  // _mean2 = _mean2 * prop + sq(value) / _nSamples;
  float alpha = 1.f / _nSamples;
  _mean  += alpha * (value     - _mean );
  _mean2 += alpha * (sq(value) - _mean2);

  // Update min and max
//  _min = min(_min, value);
//  _max = max(_max, value);

  return normalize(value);
}

float SimpleStats::var() const {
  float v = (_mean2 - sq(_mean));
  return max(v, 0.0f); // make sure the result is >= 0
}

} // namespace pq
