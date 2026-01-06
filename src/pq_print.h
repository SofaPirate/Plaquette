/*
 * pq_print.h
 *
 * Print functions for Plaquette.
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

#ifndef PQ_PRINT_H_
#define PQ_PRINT_H_

#if (defined(ARDUINO) && ARDUINO >= 100) || defined(EPOXY_DUINO)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include "pq_globals.h"

namespace pq {

// Declare class here to prevent circular include issue.
class Monitor;

/**
 * @brief Sets the global/default device used by pq::print()/pq::println().
 *
 * This function does not start Serial or perform any I/O initialization.
 * Device initialization belongs to higher-level objects (e.g. pq::Monitor).
 */
void defaultPrintDevice(Print& device);

/**
 * @brief Sets the global/default device used by pq::print()/pq::println().
 *
 * This function does not start Serial or perform any I/O initialization.
 * Device initialization belongs to higher-level objects (e.g. pq::Monitor).
 */
void defaultPrintDevice(Monitor& device);

/**
 * @brief Disables pq::print()/pq::println() output by routing to a null sink.
 */
void noDefaultPrintDevice();

/**
 * @brief Returns the current global/default print device.
 *
 * If no device has been assigned (or output has been disabled), this returns
 * an internal null sink that discards output.
 */
Print& defaultPrintDevice();

/**
 * @brief Returns true if pq::print()/pq::println() output is enabled.
 */
bool hasDefaultPrintDevice();

// Print functions.
size_t print(const __FlashStringHelper *);
size_t print(const String &);
size_t print(const char[]);
size_t print(char);
size_t print(unsigned char, int = DEC);
size_t print(int, int = DEC);
size_t print(unsigned int, int = DEC);
size_t print(long, int = DEC);
size_t print(unsigned long, int = DEC);
size_t print(double);
size_t print(double, int);
size_t print(const Printable&);

size_t println(const __FlashStringHelper *);
size_t println(const String &s);
size_t println(const char[]);
size_t println(char);
size_t println(unsigned char, int = DEC);
size_t println(int, int = DEC);
size_t println(unsigned int, int = DEC);
size_t println(long, int = DEC);
size_t println(unsigned long, int = DEC);
size_t println(double);
size_t println(double, int);
size_t println(const Printable&);
size_t println(void);

} // namespace pq

#endif
