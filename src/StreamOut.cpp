/*
 * StreamOut.cpp
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

#include "StreamOut.h"

namespace pq {

StreamOut::StreamOut(Stream& stream) : _digits(4), _stream(&stream) {}

float StreamOut::put(float value) {
  // Copy value.
  _value = value;

  // Print it.
  _stream->println(_value, _digits);

  // Return it.
  return _value;
}

void StreamOut::precision(uint8_t digits) {
	_digits = digits;
}

}
