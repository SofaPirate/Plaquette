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

AnalogOut::AnalogOut(uint8_t pin, uint8_t mode)
  : PqPinComponent(pin, mode), PqPutter(),
    _value(0)
{}

float AnalogOut::put(float value) {
  return (_value = write(value));
}

float AnalogOut::write(float value) {
  // Make sure value is in [0, 1] and remap to [0, 255].
  value = constrain(value, 0, 1) * 255;
  int rawValue = round(value);
  // Write to analog output (inverting if needed).
#if defined(ESP32) or defined(ARDUINO_ARCH_ESP32)
  dacWrite
#else
  analogWrite
#endif
    (_pin, (_mode == SOURCE ? rawValue : 255 - rawValue));
  return value;
}

DigitalOut::DigitalOut(uint8_t pin, uint8_t mode)
  : PqPinComponent(pin, mode), PqDigitalPutter()
{}

float DigitalOut::put(float value) {
  // Make sure value is in [0, 1].
	value = constrain(value, 0, 1);
  // Set ON status depending on value: invert if mode is SINK.
  _isOn = analogToDigital(value);
  // Write to output.
  digitalWrite(_pin, _isOn ^ (_mode == SOURCE) ? LOW : HIGH);
  // Return value.
  return value;
}

void DigitalOut::setup() {
  pinMode(_pin, OUTPUT);
}
