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

// ----------------------------------------------------------------------------
// Global constants.
// ----------------------------------------------------------------------------

// Max. components that can be added. Can be pre-defined. Notice that the use of a
// hybrid array list requires a fixed size at compile time but the size will automatically be
// adjusted at runtime if necessary using dynamically-allocated memory.
#ifndef PLAQUETTE_MAX_UNITS
#define PLAQUETTE_MAX_UNITS 32
#endif

// Smoothing.
#ifndef PLAQUETTE_DEFAULT_SMOOTH_WINDOW
#define PLAQUETTE_DEFAULT_SMOOTH_WINDOW 0.1f
#endif

// Debouncing.
#ifndef PLAQUETTE_DEFAULT_DEBOUNCE_WINDOW
#define PLAQUETTE_DEFAULT_DEBOUNCE_WINDOW 0.02f // 20 ms
#endif

// Sample rate.
#define PLAQUETTE_MAX_SAMPLE_RATE FLT_MAX

#define INFINITE_TIME_WINDOW (-1)
#define NO_TIME_WINDOW       0

// Printing / serial.
#define PLAQUETTE_PRINT_DEFAULT_DIGITS 2
#define PLAQUETTE_PRINT_MAX_DIGITS     6

// ----------------------------------------------------------------------------
// Default analog write and read values.
// ----------------------------------------------------------------------------

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


// ----------------------------------------------------------------------------
// Optimization flags.
// ----------------------------------------------------------------------------

// 32-bit vs 8-bit platforms.
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
  #define PQ_IEEE_754_SUPPORTED
#else
  #warning "IEEE 754 floating point not supported on this architecture. This may cause unexpected behavior."
#endif

// Enable CPU optimization (as opposed to memory optimization).
#ifndef PQ_OPTIMIZE_FOR_CPU

// Disable CPU optimization for low-RAM AVR chips (≤ 2 KB SRAM)
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)  || \
    defined(__AVR_ATmega88__)   || defined(__AVR_ATmega48__)   || \
    defined(__AVR_ATmega8__)    || defined(__AVR_ATmega16__)   || \
    defined(__AVR_ATmega32__)   || defined(__AVR_ATtiny85__)   || \
    defined(__AVR_ATtiny84__)   || defined(__AVR_ATmega168PB__)|| \
    defined(__AVR_AT90S2313__)  || defined(__AVR_AT90S4433__)

    #define PQ_OPTIMIZE_FOR_CPU 0

// All other platforms: enable optimization by default
#else
    #define PQ_OPTIMIZE_FOR_CPU 1
#endif

#endif

#endif
