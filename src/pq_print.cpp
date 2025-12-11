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

Print* consoleOutputInstance = &Serial;
bool consoleOutputIsSerial = true;
unsigned long consoleOutputBaudRate = PLAQUETTE_SERIAL_BAUD_RATE;

void consoleBegin() {
  if (consoleOutputIsSerial) {
    PlaquetteSerialType* serial = static_cast<PlaquetteSerialType*>(consoleOutputInstance);
    // Wait for last transmitted data to be sent.
    serial->flush();
    // Start serial with new baudrate.
    serial->begin(consoleOutputBaudRate);
    // Empty  out possible garbage from input buffer.
    while (serial->available()) serial->read();
  }
}

void consoleBegin(unsigned long baudRate) {
  // Make sure to set default baudrate if not set.
  consoleOutput(baudRate);
  // Start serial.
  consoleBegin();
}

void consoleOutput(Print& console) {
  consoleOutputInstance = &console;
  consoleOutputIsSerial = false;
}

void consoleOutput(PlaquetteSerialType& console) {
  consoleOutputInstance = &console;
  consoleOutputIsSerial = true;
}

void consoleOutput(PlaquetteSerialType& console, unsigned long baudRate) {
  consoleOutputInstance = &console;
  consoleOutputBaudRate = baudRate;
  consoleOutputIsSerial = true;
}

void consoleOutput(unsigned long baudRate) {
  consoleOutputBaudRate = baudRate;
}

size_t print(const __FlashStringHelper *ifsh)
{
  return consoleOutputInstance->print(ifsh);
}

size_t print(const String &s)
{
  return consoleOutputInstance->print(s);
}

size_t print(const char str[])
{
  return consoleOutputInstance->print(str);
}

size_t print(char c)
{
  return consoleOutputInstance->print(c);
}

size_t print(unsigned char b, int base)
{
  return consoleOutputInstance->print(b, base);
}

size_t print(int n, int base)
{
  return consoleOutputInstance->print(n, base);
}

size_t print(unsigned int n, int base)
{
  return consoleOutputInstance->print(n, base);
}

size_t print(long n, int base)
{
  return consoleOutputInstance->print(n, base);
}

size_t print(unsigned long n, int base)
{
  return consoleOutputInstance->print(n, base);
}

size_t print(double n, int digits)
{
  return consoleOutputInstance->print(n, digits);
}

size_t println(const __FlashStringHelper *ifsh)
{
  return consoleOutputInstance->println(ifsh);
}

size_t print(const Printable& x)
{
  return consoleOutputInstance->println(x);
}

size_t println(void)
{
  return consoleOutputInstance->println();
}

size_t println(const String &s)
{
  return consoleOutputInstance->println(s);
}

size_t println(const char c[])
{
  return consoleOutputInstance->println(c);
}

size_t println(char c)
{
  return consoleOutputInstance->println(c);
}

size_t println(unsigned char b, int base)
{
  return consoleOutputInstance->println(b, base);
}

size_t println(int num, int base)
{
  return consoleOutputInstance->println(num, base);
}

size_t println(unsigned int num, int base)
{
  return consoleOutputInstance->println(num, base);
}

size_t println(long num, int base)
{
  return consoleOutputInstance->println(num, base);
}

size_t println(unsigned long num, int base)
{
  return consoleOutputInstance->println(num, base);
}

size_t println(double num, int digits)
{
  return consoleOutputInstance->println(num, digits);
}

size_t println(const Printable& x)
{
  return consoleOutputInstance->println(x);
}

} // namespace pq
