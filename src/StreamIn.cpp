/*
 * Steamin.cpp
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

#include "StreamIn.h"

namespace pq {
	
StreamIn::StreamIn(Stream& stream) :  _nextValue(0), _nextFraction(1), _nextIsValid(false), _nextIsNegative(false), _nextIsFraction(false), _stream(&stream) {}

void StreamIn::begin() {
	_nextValue = 0;
	_nextIsValid = false;
	_nextIsNegative = false;
	_nextIsFraction = false;
	_nextFraction = 1;
}

void StreamIn::step() {
	while (_stream->available()) {

		int c = _stream->read();
		if (c == '-') {
			_nextIsNegative = true;
		}
		else if (c == '.') {
			_nextIsFraction = true;
		}
		else if (c >= '0' && c <= '9') {
			_nextIsValid = true;
			_nextValue = _nextValue * 10 + (c - '0');
			if (_nextIsFraction)
				_nextFraction *= 0.1;
		}
		else {
			// Newline or carriage return: save value.
			if (c == '\n' || c == '\r') {
				// Save if valid.
				if (_nextIsValid) {
					if (_nextIsNegative)
						_nextValue = -_nextValue;
				  if (_nextIsFraction)
						_nextValue *= _nextFraction;
					_value = _nextValue;
				}
			}

			// Reset everything.
			_nextValue = 0;
			_nextIsValid = false;
			_nextIsNegative = false;
			_nextIsFraction = false;
			_nextFraction = 1;
		}
	}
}

}
