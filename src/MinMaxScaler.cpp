/*
 * MinMaxScaler.cpp
 *
 * (c) 2015 Sofian Audry        :: info(@)sofianaudry(.)com
 * (c) 2015 Thomas O Fredericks :: tof(@)t-o-f(.)info
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

#include "MinMaxScaler.h"

#include "float.h"
#include "pq_map_real.h"
#include "MovingAverage.h"

namespace pq {

MinMaxScaler::MinMaxScaler()
  : MovingFilter(),
    _currentValueStep(0)
{
  infiniteTimeWindow();
  reset();
}

MinMaxScaler::MinMaxScaler(float decayWindow)
  : MovingFilter(),
    _currentValueStep(0)
{
  timeWindow(decayWindow);
  reset();
}

void MinMaxScaler::infiniteTimeWindow() {
  _timeWindow = MOVING_FILTER_INFINITE_TIME_WINDOW;
}

void MinMaxScaler::timeWindow(float seconds) {
  _timeWindow = max(seconds, 0.0f); // make sure it is positive
}

float MinMaxScaler::timeWindow() const { return _timeWindow; }

bool MinMaxScaler::timeWindowIsInfinite() const {
  return _timeWindow == MOVING_FILTER_INFINITE_TIME_WINDOW;
}

void MinMaxScaler::reset() {
  MovingFilter::reset();
  _minValue =  FLT_MAX;
  _maxValue = -FLT_MAX;
  _value = 0.5f;
  _currentValueStep = 0;
  _nValuesStep = 0;
}

float MinMaxScaler::put(float value)
{
  if (isRunning()) {

    // Update min. value.
    if (value < _minValue) {
      _minValue = value;
    }

    // Update max. value.
    if (value > _maxValue) {
      _maxValue = value;
    }

    // Increment n. values.
    if (_nValuesStep < 128)
      _nValuesStep++;

    if (_nValuesStep == 1) {
      // Save current value.
      _currentValueStep = value;
    }
    else {
      // Update current step average value.
      MovingAverage::applyUpdate(_currentValueStep, value, 1.0f/_nValuesStep);
    }
  }

  // Compute rescaled value.
  _value = mapTo01(value, _minValue, _maxValue, CONSTRAIN);

  return _value;
}


void MinMaxScaler::step() {
  // If no values were added during this step, update using previous value.
  if (_nValuesStep > 0 ||      // if at least one value was recorded this step ...
      _minValue != FLT_MAX) {  // ... or at least one value was ever recorded since reset

    // Reset (but keep _currentValueStep).
    _nValuesStep = 0;

    // Apply decay.
    if (!timeWindowIsInfinite()) {
      float alpha = MovingAverage::alpha(sampleRate(), _timeWindow);
      // Decay towards value.
      MovingAverage::applyUpdate(_minValue, _currentValueStep, alpha);
      MovingAverage::applyUpdate(_maxValue, _currentValueStep, alpha);
    }
  }
}

}
