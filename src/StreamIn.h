/*
 * PqExtra.h
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

#ifndef STREAM_IN_H_
#define STREAM_IN_H_

#include "PqCore.h"

namespace pq {

/// Stream/serial input. Reads float values using Arduino built-in parseFloat().
class StreamIn : public PqAnalogSource {
public:
  /**
   * Constructor.
   * @param stream a reference to a Stream object
   */
  StreamIn(Stream& stream=Serial);
  virtual ~StreamIn() {}

protected:
  // Core Plaquette methods.
  virtual void begin();
  virtual void step();

  // Internal use: keep track of next incoming value in a non-blocking way.
  float _nextValue;
  float _nextFraction;
  bool _nextIsValid : 1;
  bool _nextIsNegative : 1;
  bool _nextIsFraction : 1;

  // The stream.
  Stream* _stream;
};

}

#endif
