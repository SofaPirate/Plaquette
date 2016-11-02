/*
 * PqExtra.cpp
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

#include "PqExtra.h"


SquareOsc::SquareOsc(float period, float dutyCycle) {
  // Convert period in ms.
  period *= 1000;
  _period = round(period);
  _period = max(_period, 1.0f); // at least 1ms
  // Convert duty cycle in ms.
  dutyCycle = constrain(dutyCycle, 0, 1);
  dutyCycle *= _period;
  _dutyCyclePeriod = round(dutyCycle);
}

void SquareOsc::setup() {
  _startTime = millis();
}

void SquareOsc::update() {
  // Check where we are.
  _isOn = ((millis() - _startTime) % _period < _dutyCyclePeriod);
}

SerialOut::SerialOut(uint8_t digits) : _digits(digits) {}

float SerialOut::put(float value) {
  // Copy value.
  _value = value;

  // Print it.
  println(_value, _digits);

  // Return it.
  return _value;
}


Smoother::Smoother(float factor)
  : PqPutter(),
    MovingAverage(factor) {
}

float Smoother::put(float value) {
  return MovingAverage::update(value);
}
}

}

Thresholder::Thresholder(float threshold, int8_t dir)
  : PqPutter(),
    _threshold(threshold),
    _dir(dir),
    _value(0) {
}

float Thresholder::put(float value) {
  if (_dir != THRESHOLD_BOTH &&
      ( (_dir == THRESHOLD_HIGH) ^ (value > 0) ))
    _value = 0;
  else
    _value = (abs(value) > _threshold ? 1 : 0);
  return (float)_value;
}
