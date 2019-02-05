/*
 * SteamOut.cpp
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

#ifndef STREAM_OUT_H_
#define STREAM_OUT_H_

#include "PqCore.h"

namespace pq {
  
/// Stream/serial output. Number of digits of precision is configurable.
class StreamOut : public PqAnalogUnit {
public:
  /**
   * Constructor.
   * @param stream a reference to a Stream object
   */
  StreamOut(Stream& stream=Serial);
  virtual ~StreamOut() {}

  /**
   * Pushes value into the unit.
   * @param value the value sent to the unit
   * @return the new value of the unit
   */
  virtual float put(float value);

  /**
   * Sets precision of the output.
   * @param digits the number of digits to show after decimal point
   */
  virtual void precision(uint8_t digits);

protected:
  // Number of digits of precision.
  uint8_t _digits;

  // The stream.
  Stream* _stream;
};

}

#endif
