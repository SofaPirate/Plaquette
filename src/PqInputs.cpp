/*
 * PqInputs.cpp
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

#include "PqInputs.h"

namespace pq {

PqSmoothable::PqSmoothable(float smoothTime) : _avg(smoothTime) {}

void PqSmoothable::_begin() {
}

void PqSmoothable::_step() {
  _avg.update( _read(), _avg.alpha(sampleRate()) );
}

AnalogIn::AnalogIn(uint8_t pin, uint8_t mode)
  : PqPinUnit(pin, mode)
{}

#ifdef ESP8266
#define ANALOG_READ_MAX_VALUE 1024
#elif defined(ESP32) or defined(ARDUINO_ARCH_ESP32)
#define ANALOG_READ_MAX_VALUE 4095 // applies only to default resolution of 12 bits
#else
#define ANALOG_READ_MAX_VALUE 1023
#endif
float AnalogIn::_read() {
  // Convert
  int rawValue = analogRead(_pin);
  if (_mode == ANALOG_INVERTED)
    rawValue = ANALOG_READ_MAX_VALUE - rawValue;
  return rawValue / float(ANALOG_READ_MAX_VALUE);
}

void AnalogIn::begin() {
  _begin();
}

void AnalogIn::step() {
  _step();
}

DigitalIn::DigitalIn(uint8_t pin, uint8_t mode)
  : PqPinUnit(pin, mode), PqDigitalSource(), _changeState(0)
{}

bool DigitalIn::_isOn() {
  bool isHigh = (digitalRead(_pin) == HIGH);
  if (_mode == INTERNAL_PULLUP || _mode == EXTERNAL_PULLUP) // inverted
    isHigh = !isHigh;
  return isHigh;
}

void DigitalIn::begin() {
  pinMode(_pin, _mode == INTERNAL_PULLUP ? INPUT_PULLUP : INPUT);
  _changeState = 0;
}

void DigitalIn::step() {
  // Perform basic step.
  _step();
  // Read state.
	bool isOn = analogToDigital(_debounced());
  // Register difference between previous and new state.
  _changeState = (int8_t)isOn - (int8_t)_onValue;
  // Save state.
  _onValue = isOn;
}

} // namespace pq
