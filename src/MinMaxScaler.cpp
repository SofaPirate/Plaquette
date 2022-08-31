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

#include "float.h"
#include "pq_map_real.h"
#include "MinMaxScaler.h"

namespace pq {

MinMaxScaler::MinMaxScaler(float decayWindow)
 : AnalogSource(0.5f),
   _minValue(FLT_MAX),
   _maxValue(-FLT_MAX),
   _decayWindow(decayWindow)
{
}

void MinMaxScaler::time(float seconds) {
  _decayWindow = ::max(seconds, 0.0f); // make sure it is positive
}

void MinMaxScaler::cutoff(float hz) {
  // If hz is null time window is infinite.
  time(hz == 0 ? FLT_MAX : hz);
}

float MinMaxScaler::put(float value)
{
  // Compute alpha mixing factor.
  float alpha = MovingAverage::alpha(sampleRate(), _decayWindow);

  // Update min. value.
  if (value < _minValue) {
    _minValue = value;
  }
  else {
    MovingAverage::applyUpdate(_minValue, value, alpha);
  }

  // Update max. value.
  if (value > _maxValue) {
    _maxValue = value;
  }
  else {
    MovingAverage::applyUpdate(_maxValue, value, alpha);
  }

  _value = mapTo01(value, _minValue, _maxValue);
  return _value;
}

}
