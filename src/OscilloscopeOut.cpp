/*
 * OscilloscopeOut.cpp
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

#include "OscilloscopeOut.h"
#include "pq_print.h"
#include "pq_map_real.h"

namespace pq {
  
OscilloscopeOut::OscilloscopeOut(float minValue, float maxValue, uint8_t precision)
  : _minValue(minValue), _maxValue(maxValue), _precision(precision) {}

float OscilloscopeOut::put(float value) {
  // Copy value.
  _value = value;

  // Convert to bin.
  float mapped = mapTo01(_value, _minValue, _maxValue);
  int bin = round( mapped * _precision );
  bin = constrain(bin, 0, _precision-1);

  // Print.
  print(_minValue, 2);
  print(" |");
  for (int i=0; i<_precision; i++)
    print(i == bin ? '*' : ' ');
  print("| ");
  print(_maxValue, 2);
  println();

  // Return it.
  return _value;
}

}
