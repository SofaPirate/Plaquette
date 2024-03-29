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

namespace pq {

AnalogOut::AnalogOut(uint8_t pin, uint8_t mode)
  : AnalogSource(), PinUnit(pin, mode)
{}

float AnalogOut::put(float value) {
  // Make sure value is in [0, 1].
  _value = constrain(value, 0, 1);

  // Remap to [0, 255].
  value = _value * 255;
  value = round(value);
  // Write to analog output (inverting if needed).
#if (defined(ESP32) or defined(ARDUINO_ARCH_ESP32)) and defined(SOC_DAC_SUPPORTED)
  dacWrite // use dacWrite if available
#else
  analogWrite
#endif
    (_pin, (_mode == SOURCE ? value : 255 - value));

  return _value;
}

DigitalOut::DigitalOut(uint8_t pin, uint8_t mode)
  : DigitalSource(), PinUnit(pin, mode)
{}

void DigitalOut::mode(uint8_t mode) {
  _mode = mode;
  begin();
}

void DigitalOut::begin() {
  pinMode(_pin, OUTPUT);
}

bool DigitalOut::putOn(bool isOn) {
  // Save value.
  _setOn(isOn);
  // Write to output.
  digitalWrite(_pin, isOn ^ (_mode == SOURCE) ? LOW : HIGH);
  return isOn;
}

} // namespace pq
