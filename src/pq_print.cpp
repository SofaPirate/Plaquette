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

struct {
  Print* device = nullptr;
  bool isSerial = true;
  unsigned long baudRate = 0;
} monitorInstance;

class NullPrint : public Print {
public:
  virtual size_t write(uint8_t) override { return 1; }
  virtual size_t write(const uint8_t *buffer, size_t size) override { return size; }
};

static NullPrint nullPrint;

void monitorBegin(unsigned long baudRate) {

  if (!monitorInstance.device)
    monitorInstance.device = &Serial;

  monitorInstance.baudRate = baudRate;

  if (monitorInstance.isSerial) {
    PlaquetteSerialType* serial = static_cast<PlaquetteSerialType*>(monitorInstance.device);
    // Wait for last transmitted data to be sent.
    serial->flush();
    // Start serial with new baudrate.
    serial->begin(monitorInstance.baudRate);
    // Empty  out possible garbage from input buffer.
    while (serial->available()) serial->read();
  }
}

void monitorBegin(PlaquetteSerialType& monitor, unsigned long baudRate) {
  monitorInstance.device = &monitor;
  monitorInstance.isSerial = true;
  monitorBegin(baudRate);
}

void monitorBegin(Print& monitor) {
  monitorInstance.device = &monitor;
  monitorInstance.isSerial = false;
}

void monitorEnd() {
  monitorInstance.device = &nullPrint;
}

Print& monitor() {
  return *monitorInstance.device;
}

size_t print(const __FlashStringHelper *ifsh)
{
  return monitorInstance.device->print(ifsh);
}

size_t print(const String &s)
{
  return monitorInstance.device->print(s);
}

size_t print(const char str[])
{
  return monitorInstance.device->print(str);
}

size_t print(char c)
{
  return monitorInstance.device->print(c);
}

size_t print(unsigned char b, int base)
{
  return monitorInstance.device->print(b, base);
}

size_t print(int n, int base)
{
  return monitorInstance.device->print(n, base);
}

size_t print(unsigned int n, int base)
{
  return monitorInstance.device->print(n, base);
}

size_t print(long n, int base)
{
  return monitorInstance.device->print(n, base);
}

size_t print(unsigned long n, int base)
{
  return monitorInstance.device->print(n, base);
}

size_t print(double n, int digits)
{
  return monitorInstance.device->print(n, digits);
}

size_t println(const __FlashStringHelper *ifsh)
{
  return monitorInstance.device->println(ifsh);
}

size_t print(const Printable& x)
{
  return monitorInstance.device->println(x);
}

size_t println(void)
{
  return monitorInstance.device->println();
}

size_t println(const String &s)
{
  return monitorInstance.device->println(s);
}

size_t println(const char c[])
{
  return monitorInstance.device->println(c);
}

size_t println(char c)
{
  return monitorInstance.device->println(c);
}

size_t println(unsigned char b, int base)
{
  return monitorInstance.device->println(b, base);
}

size_t println(int num, int base)
{
  return monitorInstance.device->println(num, base);
}

size_t println(unsigned int num, int base)
{
  return monitorInstance.device->println(num, base);
}

size_t println(long num, int base)
{
  return monitorInstance.device->println(num, base);
}

size_t println(unsigned long num, int base)
{
  return monitorInstance.device->println(num, base);
}

size_t println(double num, int digits)
{
  return monitorInstance.device->println(num, digits);
}

size_t println(const Printable& x)
{
  return monitorInstance.device->println(x);
}

} // namespace pq
