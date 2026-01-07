/*
 * pq_serial.h
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
#ifndef PQ_SERIAL_H_
#define PQ_SERIAL_H_

#include "PqCore.h"

namespace pq {

// Serial definitions.
#define PLAQUETTE_DEFAULT_SERIAL Serial

// Platform-dependent using SerialType = for.
#if defined(ARDUINO_ARCH_AVR)
  #if defined(USBCON)
    using SerialType = Serial_;  // Leonardo, Micro (USB CDC)
  #else
    using SerialType = HardwareSerial;  // Uno, Mega, Nano
  #endif
#elif defined(ARDUINO_ARCH_ESP32) || defined(ARDUINO_ARCH_ESP8266)
  using SerialType = HardwareSerial;
#elif defined(ARDUINO_ARCH_SAMD)
  using SerialType = Uart;
#elif defined(EPOXY_DUINO)
  using SerialType = StdioSerial;
#else
  using SerialType = HardwareSerial;  // Fallback
#endif

/// Safely starts serial.
static inline void beginSerial(SerialType& serial, unsigned long baudrate) {
    // Wait for last transmitted data to be sent.
    serial.flush();

    // Start serial with baudrate.
    if (baudrate > 0) {
      serial.begin(baudrate);
    }

    // Wait until serial is ready.
    while (!serial) {}
}

}

#endif
