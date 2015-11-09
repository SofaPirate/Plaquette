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
    _value(-1) {
  // TODO: repetitive code here (see AnalogIn).
  factor = max(factor, 0); // make sure factor >= 0
  _alpha = (factor > 1 ?
      2 / (factor + 1) :
      factor);
}

float Smoother::put(float value) {
  // Update value.
  if (_value < 0) // value never initialized
    return (_value = value);
  else // moving average
    return (_value -= _alpha * (_value - value));
}

SquareOsc::SquareOsc(float period, float dutyCycle) {
  // Convert period in ms.
  period *= 1000;
  _period = round(period);
  _period = max(_period, 1); // at least 1ms
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
