/*
 * Monitor.cpp
 *
 * (c) 2025 Sofian Audry        :: info(@)sofianaudry(.)com
 * (c) 2025 Thomas O Fredericks :: tof(@)t-o-f(.)info
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

#include "Monitor.h"
#include "pq_print.h"
#include "pq_serial.h"

namespace pq {

Monitor::Monitor(unsigned long baudRate, Engine& engine)
  : Monitor(PLAQUETTE_DEFAULT_SERIAL, baudRate, engine)
{}

Monitor::Monitor(SerialType& device, unsigned long baudRate, Engine& engine)
  : Unit(engine),
    Print(),
    _device(&device),
    _isSerial(true),
    _begun(false),
    _digits(PLAQUETTE_PRINT_DEFAULT_DIGITS),
    _baudRate(baudRate),
    _value(0)
{}

Monitor::Monitor(Print& device, Engine& engine)
  : Unit(engine),
    Print(),
    _device(&device),
    _isSerial(false),
    _begun(false),
    _digits(PLAQUETTE_PRINT_DEFAULT_DIGITS),
    _baudRate(0),
    _value(0)
{}

void Monitor::begin() {
  if (_begun) return;

  // Sensible fallback if constructed incorrectly.
  if (!_device) {
    _device   = &Serial;
    _isSerial = true;
  }

  if (_isSerial) {
    // NOTE: This relies on the guarantee that _device actually points to a
    // PlaquetteSerialType (Serial, Serial1, etc.). This mirrors pq_print.cpp.
    SerialType* serial = static_cast<SerialType*>(_device);

    // Safely start serial.
    beginSerial(*serial, _baudRate);
  }

  // Assign as default device.
  if (!hasDefaultPrintDevice())
    defaultPrintDevice(*this);

  // Flag begin.
  _begun = true;
}

size_t Monitor::write(uint8_t b) {
  return _device ? _device->write(b) : 0;
}

size_t Monitor::print(double v) {
  return Print::print(v, _digits);
}

size_t Monitor::println(double v) {
  return Print::println(v, _digits);
}

void Monitor::precision(uint8_t digits) {
  _digits = min(digits, PLAQUETTE_PRINT_MAX_DIGITS);
}

float Monitor::put(float value) {
  // Default behavior: print value.
  // Uses a higher default precision than Arduino's default (2 digits),
  // consistent with StreamOut.
  if (_device) {
    _device->print(static_cast<double>(value), _digits);
  }

  _value = value;

  return _value;
}

} // namespace pq
