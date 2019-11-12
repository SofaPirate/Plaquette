/*
 * MovingAverage.cpp
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

#include "MovingAverage.h"

#include <float.h>

namespace pq {

MovingAverage::MovingAverage(float seconds) : _value(0.5f) {
  time(seconds);
  reset();
}

// MovingAverage::MovingAverage(float seconds, float startValue) {
//   window(seconds);
//   reset(startValue);
// }

void MovingAverage::time(float seconds) {
  // Save start status - changing alpha will meddle with it.
  bool started = isStarted();

  // Assign.
  _smoothTime = max(seconds, 0.f);

  // Reset start state.
  _setStarted(started);
}

void MovingAverage::cutoff(float hz) {
  // If hz is null time window is infinite.
  time(hz == 0 ? FLT_MAX : hz);
}

void MovingAverage::reset() {
  _setStarted(false);
}

// void MovingAverage::reset(float startValue) {
//   _value = startValue;
//   _setStarted(true);
// }

float MovingAverage::update(float v, float sampleRate, bool forceAlpha) {
  if (!isStarted()) {
    // Initialize value with first read value -- which is always an unbiased sample.
    _value = v;
    _setStarted(true); // start
    return _value;
  }
  else
    // Exponential moving average.
    return applyUpdate(_value, v, forceAlpha ? sampleRate : alpha(sampleRate));
}

bool MovingAverage::isStarted() const { return _isStarted; }

float MovingAverage::applyUpdate(float& runningValue, float newValue, float alpha) {
  return (runningValue -= alpha * (runningValue - newValue));
}

void MovingAverage::_setStarted(bool start) {
  _isStarted = start;
}

} // namespace pq
