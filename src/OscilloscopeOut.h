/*
 * OscilloscopeOut.h
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

#ifndef OSCILLOSCOPE_OUT_H_
#define OSCILLOSCOPE_OUT_H_

#include "PqCore.h"

namespace pq {
  
/// Generates a simple ASCII-based representation of a signal.
/// Precision represents the number of columns used to represent the signal.
class OscilloscopeOut : public PqAnalogUnit {
public:
  OscilloscopeOut(float minValue=0, float maxValue=1, uint8_t precision=100);
  virtual ~OscilloscopeOut() {}

  virtual float put(float value);

protected:
  float _minValue;
  float _maxValue;

  // Number of spaces of precision.
  uint8_t _precision;
};

}

#endif
