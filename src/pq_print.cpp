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


size_t print(const __FlashStringHelper *ifsh)
{
  return Serial.print(ifsh);
}

size_t print(const String &s)
{
  return Serial.print(s);
}

size_t print(const char str[])
{
  return Serial.print(str);
}

size_t print(char c)
{
  return Serial.print(c);
}

size_t print(unsigned char b, int base)
{
  return Serial.print(b, base);
}

size_t print(int n, int base)
{
  return Serial.print(n, base);
}

size_t print(unsigned int n, int base)
{
  return Serial.print(n, base);
}

size_t print(long n, int base)
{
  return Serial.print(n, base);
}

size_t print(unsigned long n, int base)
{
  return Serial.print(n, base);
}

size_t print(double n, int digits)
{
  return Serial.print(n, digits);
}

size_t println(const __FlashStringHelper *ifsh)
{
  return Serial.println(ifsh);
}

size_t print(const Printable& x)
{
  return Serial.println(x);
}

size_t println(void)
{
  return Serial.println();
}

size_t println(const String &s)
{
  return Serial.println(s);
}

size_t println(const char c[])
{
  return Serial.println(c);
}

size_t println(char c)
{
  return Serial.println(c);
}

size_t println(unsigned char b, int base)
{
  return Serial.println(b, base);
}

size_t println(int num, int base)
{
  return Serial.println(num, base);
}

size_t println(unsigned int num, int base)
{
  return Serial.println(num, base);
}

size_t println(long num, int base)
{
  return Serial.println(num, base);
}

size_t println(unsigned long num, int base)
{
  return Serial.println(num, base);
}

size_t println(double num, int digits)
{
  return Serial.println(num, digits);
}

size_t println(const Printable& x)
{
  return Serial.println(x);
}
