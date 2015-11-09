/*
 * PqOutputs.cpp
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

#include "PqOutputs.h"


PwmOut::PwmOut(uint8_t pin, uint8_t mode)
  : PqPinComponent(pin, mode), PqPutter(),
    _value(0)
{}

float PwmOut::put(float value) {
  return (_value = write(value));
}

float PwmOut::write(float value) {
  // Make sure value is in [0, 1].
  value = constrain(value, 0, 1);
  // Remap as integer in [0, 255].
  float remappedValue = value * 255;
  int rawValue = round(remappedValue);
  // Write to PWM (inverting if needed).
  analogWrite(_pin, (_mode == SOURCE ? rawValue : 255 - rawValue));
  return value;
}

DigitalOut::DigitalOut(uint8_t pin, uint8_t mode)
  : PqPinComponent(pin, mode), PqDigitalPutter()
{}

float DigitalOut::put(float value) {
  // Make sure value is in [0, 1].
  value = constrain(value, 0, 1);
  // Remap as integer : either 0 or 1.
  int value01 = (value > 0.5f ? 1 : 0);
  _isOn = (_mode == SOURCE ? value01 : 1-value01);
  // Write to PWM (inverting if needed).
  digitalWrite(_pin, _isOn ? HIGH : LOW);
  // Return original value.
  return value01;
}

void DigitalOut::setup() {
  pinMode(_pin, OUTPUT);
}
