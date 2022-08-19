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

MovingAverage::MovingAverage(float seconds, float initValue) : _value(initValue) {
  time(seconds);
  reset();
}

// MovingAverage::MovingAverage(float seconds, float startValue) {
//   window(seconds);
//   reset(startValue);
// }

void MovingAverage::time(float seconds) {
  _smoothTime = max(seconds, 0.0f); // make sure it is positive
}

void MovingAverage::cutoff(float hz) {
  // If hz is null time window is infinite.
  time(hz == 0 ? FLT_MAX : hz);
}

float MovingAverage::alpha(float sampleRate) const {
  
  float nSamplesTarget = _smoothTime * sampleRate;

  // In order to do a smooth transition and prevent first values to take too much weight compared to
  // later values, we start by averaging using a non-moving average for the first series of valus.
  if (_nSamples < nSamplesTarget && _nSamples < UINT_MAX)
    nSamplesTarget = _nSamples;

  // Rule of thumb: alpha = 2 / (nSamplesTarget + 1).
  float alpha = 2 / (_smoothTime*sampleRate + 1);
  return min(alpha, 1.f); // make sure it does not get over 1
}

void MovingAverage::reset(float initValue) {
  _nSamples = 1;
  _value = initValue;
}

float MovingAverage::update(float v, float sampleRate, bool forceAlpha) {

  // Exponential moving average.
  v = applyUpdate(_value, v, forceAlpha ? sampleRate : alpha(sampleRate));

  // Increase number of samples.
  if (_nSamples < UINT_MAX)
    _nSamples++;
  
  return v;
}

bool MovingAverage::isStarted() const { return _nSamples > 1; }

float MovingAverage::applyUpdate(float& runningValue, float newValue, float alpha) {
  return (runningValue -= alpha * (runningValue - newValue));
}

} // namespace pq
