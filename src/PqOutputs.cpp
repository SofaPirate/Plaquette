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
#include "pq_globals.h"

namespace pq {

AnalogOut::AnalogOut(uint8_t pin, Engine& engine)
  : AnalogSource(engine), PinConfig(pin, DIRECT)
{}

AnalogOut::AnalogOut(uint8_t pin, uint8_t mode, Engine& engine)
  : AnalogSource(engine), PinConfig(pin, mode)
{}

float AnalogOut::put(float value) {
  // Make sure value is in [0, 1].
  _value = constrain(value, 0, 1);

  return _value;
}

#if (defined(ESP32) or defined(ARDUINO_ARCH_ESP32)) and defined(SOC_DAC_SUPPORTED)
#define analogWriteFunction dacWrite // use dacWrite if available
#else
#define analogWriteFunction analogWrite
#endif

void AnalogOut::write(float value) {
  // Make sure value is in [0, 1].
  value = constrain(value, 0, 1);

  // Remap to [0, ANALOG_WRITE_MAX_VALUE].
  value = value * ANALOG_WRITE_MAX_VALUE;
  value = round(value);
  rawWrite(value);
}

void AnalogOut::rawWrite(int value) {
  value = constrain(value, 0, ANALOG_WRITE_MAX_VALUE);
  _value = (_mode == DIRECT ? value : ANALOG_WRITE_MAX_VALUE - value) / (float)ANALOG_WRITE_MAX_VALUE;
  analogWriteFunction(_pin, value);
}

void AnalogOut::step() {
  float value = (_mode == DIRECT ? _value : 1 - _value) * ANALOG_WRITE_MAX_VALUE;
  analogWriteFunction(_pin, round(value));
}

DigitalOut::DigitalOut(uint8_t pin, Engine& engine)
  : DigitalSource(engine), PinConfig(pin, DIRECT)
{}

DigitalOut::DigitalOut(uint8_t pin, uint8_t mode, Engine& engine)
  : DigitalSource(engine), PinConfig(pin, mode)
{}


void DigitalOut::mode(uint8_t mode) {
  _mode = mode;
  _init();
}

void DigitalOut::_init() {
  pinMode(_pin, OUTPUT);
}

void DigitalOut::write(bool value) {
  rawWrite(value ? HIGH : LOW);
}

void DigitalOut::write(float value) {
  rawWrite(analogToDigital(value) ? HIGH : LOW);
}

void DigitalOut::rawWrite(int value) {
  digitalWrite(_pin, value);
}

void DigitalOut::begin() {
  _init();
}

void DigitalOut::step() {
  // Update change state.
  _updateChangeState();
  // Write to output.
  digitalWrite(_pin, _onValue ^ (_mode == DIRECT) ? LOW : HIGH);
}

} // namespace pq
