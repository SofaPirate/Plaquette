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

#define INFINITE_WINDOW (-1)

MovingAverage::MovingAverage() : _value(0) {
  infiniteTimeWindow();
  reset();
}

MovingAverage::MovingAverage(float timeWindow_) : _value(0) {
  timeWindow(timeWindow_);
  reset();
}

void MovingAverage::infiniteTimeWindow() {
  _smoothTime = INFINITE_WINDOW;
}

void MovingAverage::timeWindow(float seconds) {
  _smoothTime = max(seconds, 0.0f); // make sure it is positive
}

void MovingAverage::cutoff(float hz) {
  // If hz is null time window is infinite.
  if (hz <= 0)
    infiniteTimeWindow();
  else
    timeWindow(1.0f / hz);
}

float MovingAverage::cutoff() const {
  return (timeWindowIsInfinite() ? 0 : 1.0f/_smoothTime);
}

bool MovingAverage::timeWindowIsInfinite() const {
  return _smoothTime == INFINITE_WINDOW;
}

float MovingAverage::alpha(float sampleRate) const {
  return alpha(sampleRate, _smoothTime, _nSamples);
}

void MovingAverage::reset() {
  _nSamples = 1;
}

float MovingAverage::update(float v, float sampleRate, bool forceAlpha) {

  // Exponential moving average.
  v = applyUpdate(_value, v, forceAlpha ? sampleRate : alpha(sampleRate));

  // Increase number of samples.
  if (_nSamples < UINT_MAX)
    _nSamples++;

  return v;
}

float MovingAverage::applyUpdate(float& runningValue, float newValue, float alpha) {
  return (runningValue -= alpha * (runningValue - newValue));
}

float MovingAverage::alpha(float sampleRate, float smoothTime, unsigned int nSamples) {
  // Approximative number of samples in time window.
  float nSamplesTarget = (smoothTime >= 0 ? smoothTime * sampleRate : FLT_MAX);

  nSamplesTarget = min((float)nSamples, nSamplesTarget);

  // In order to do a smooth transition and prevent first values to take too much weight compared to
  // later values, we start by averaging using a non-moving average for the first nSamplesTarget values.
  return (nSamplesTarget > 1.0f ? 1.0f / nSamplesTarget : 1.0f);
}

} // namespace pq
