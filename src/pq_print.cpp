/*
 * pq_print.cpp
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

#include "pq_print.h"

namespace pq {

SERIAL_CLASS_NAME& defaultSerialInstance = Serial;
unsigned long defaultSerialBaudRate = PLAQUETTE_SERIAL_BAUD_RATE;

void beginSerial() {
  beginSerial(defaultSerialBaudRate);
}

void beginSerial(unsigned long baudRate) {
  // Wait for last transmitted data to be sent.
  defaultSerialInstance.flush();
  // Start serial with new baudrate.
  defaultSerialInstance.begin(baudRate);
  // Empty  out possible garbage from input buffer.
  while (defaultSerialInstance.available()) defaultSerialInstance.read();
}

void defaultSerial(SERIAL_CLASS_NAME& serial) {
  defaultSerialInstance = serial;
}

void defaultSerial(SERIAL_CLASS_NAME& serial, unsigned long baudRate) {
  defaultSerialInstance = serial;
  defaultSerialBaudRate = baudRate;
}

void defaultSerial(unsigned long baudRate) {
  defaultSerialBaudRate = baudRate;
}

size_t print(const __FlashStringHelper *ifsh)
{
  return defaultSerialInstance.print(ifsh);
}

size_t print(const String &s)
{
  return defaultSerialInstance.print(s);
}

size_t print(const char str[])
{
  return defaultSerialInstance.print(str);
}

size_t print(char c)
{
  return defaultSerialInstance.print(c);
}

size_t print(unsigned char b, int base)
{
  return defaultSerialInstance.print(b, base);
}

size_t print(int n, int base)
{
  return defaultSerialInstance.print(n, base);
}

size_t print(unsigned int n, int base)
{
  return defaultSerialInstance.print(n, base);
}

size_t print(long n, int base)
{
  return defaultSerialInstance.print(n, base);
}

size_t print(unsigned long n, int base)
{
  return defaultSerialInstance.print(n, base);
}

size_t print(double n, int digits)
{
  return defaultSerialInstance.print(n, digits);
}

size_t println(const __FlashStringHelper *ifsh)
{
  return defaultSerialInstance.println(ifsh);
}

size_t print(const Printable& x)
{
  return defaultSerialInstance.println(x);
}

size_t println(void)
{
  return defaultSerialInstance.println();
}

size_t println(const String &s)
{
  return defaultSerialInstance.println(s);
}

size_t println(const char c[])
{
  return defaultSerialInstance.println(c);
}

size_t println(char c)
{
  return defaultSerialInstance.println(c);
}

size_t println(unsigned char b, int base)
{
  return defaultSerialInstance.println(b, base);
}

size_t println(int num, int base)
{
  return defaultSerialInstance.println(num, base);
}

size_t println(unsigned int num, int base)
{
  return defaultSerialInstance.println(num, base);
}

size_t println(long num, int base)
{
  return defaultSerialInstance.println(num, base);
}

size_t println(unsigned long num, int base)
{
  return defaultSerialInstance.println(num, base);
}

size_t println(double num, int digits)
{
  return defaultSerialInstance.println(num, digits);
}

size_t println(const Printable& x)
{
  return defaultSerialInstance.println(x);
}

} // namespace pq
