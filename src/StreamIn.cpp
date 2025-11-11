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

StreamIn::StreamIn(Engine& engine) : StreamIn(Serial, engine) {}

StreamIn::StreamIn(Stream& stream, Engine& engine) :
   AnalogSource(engine),
  _nextValue(0),
  _nextFraction(1),
  _nextIsValid(false),
  _nextIsNegative(false),
  _nextIsFraction(false),
  _valueUpdated(false),
  _stream(&stream)
{}

void StreamIn::begin() {
  _nextValue = 0;
  _nextIsValid = false;
  _nextIsNegative = false;
  _nextIsFraction = false;
  _valueUpdated = false;
  _nextFraction = 1;
}

void StreamIn::step() {
  // Reset value changed flag.
  _valueUpdated = false;

  // Read stream.
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
          // Apply sign.
          if (_nextIsNegative)
            _nextValue = -_nextValue;
          // Apply fraction.
          if (_nextIsFraction)
            _nextValue *= _nextFraction;
          // Save value.
          _value = _nextValue;
          // Mark as new.
          _valueUpdated = true;
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
