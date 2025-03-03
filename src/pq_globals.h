/*
 * pq_globals.h
 *
 * Global constants.
 *
 * (c) 2025 Sofian Audry        :: info(@)sofianaudry(.)com
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

#ifndef PQ_GLOBALS_H_
#define PQ_GLOBALS_H_

// Platform type.

// 32-bit platforms
#if defined(ARDUINO_ARCH_SAMD) || defined(ARDUINO_ARCH_STM32) || defined(ESP32) || defined(TEENSYDUINO) || defined(__arm__) || defined(__riscv)
  #define PQ_ARCH_32BITS
// 8-bit AVR boards (Uno, Mega, Nano, ATtiny)
#elif defined(__AVR__)
  #define PQ_ARCH_8BITS
#else
  #define PQ_ARCH_8BITS
#endif

// IEEE 754 floating point supported.
#if defined(__STDC_IEC_559__) || defined(__IEEE754__) || defined(ESP_PLATFORM) || defined(TEENSYDUINO) || defined(__AVR__) || defined(__ARM_FP)
  #define IEEE_754_SUPPORTED
#endif


#if defined(ARDUINO_ARCH_AVR)
    // Arduino Uno, Nano, Mega (8-bit PWM, 10-bit ADC)
    #define ANALOG_WRITE_MAX_VALUE 255
    #define ANALOG_READ_MAX_VALUE 1023

#elif defined(ARDUINO_ARCH_SAMD)
    // Arduino Zero, MKR series (12-bit PWM, 12-bit ADC)
    #define ANALOG_WRITE_MAX_VALUE 4095
    #define ANALOG_READ_MAX_VALUE 4095

#elif defined(ARDUINO_ARCH_ESP8266)
    // ESP8266 (10-bit PWM, 10-bit ADC)
    #define ANALOG_WRITE_MAX_VALUE 1023
    #define ANALOG_READ_MAX_VALUE 1024 // this is specific to ESP8266, the max is 1024 NOT 1023

#elif defined(ARDUINO_ARCH_ESP32)
    // ESP32 (8-bit default PWM, 12-bit default ADC, configurable)
    #define ANALOG_WRITE_MAX_VALUE 255   // Can be configured up to 16-bit
    #define ANALOG_READ_MAX_VALUE 4095   // Default 12-bit, configurable to 13-bit (8191)

#elif defined(ARDUINO_ARCH_SAM)
    // Arduino Due (12-bit PWM, 12-bit ADC)
    #define ANALOG_WRITE_MAX_VALUE 4095
    #define ANALOG_READ_MAX_VALUE 4095

#elif defined(ARDUINO_ARCH_STM32)
    // STM32 boards (12-bit PWM, 12-bit ADC)
    #define ANALOG_WRITE_MAX_VALUE 4095
    #define ANALOG_READ_MAX_VALUE 4095

#else
    // Fallback if architecture is unknown (assume standard 8-bit PWM, 10-bit ADC)
    #define ANALOG_WRITE_MAX_VALUE 255
    #define ANALOG_READ_MAX_VALUE 1023
#endif


#endif