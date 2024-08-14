/*
 * pq_map_real.cpp
 *
 * Equivalent of Arduino map() method but for floats and doubles.
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

#include "pq_map_real.h"
#include "pq_wrap.h"

namespace pq {

float _mapConvert(float value, float toLow, float toHigh, uint8_t mode) {
  // Convert -0.0f to 0.0f
  if (value == 0.0f)
    value = 0.0f;

  // Apply mode and return.
  switch (mode) {
    case UNCONSTRAIN: return value;
    case CONSTRAIN:   return toLow <= toHigh ? constrain(value, toLow, toHigh) : constrain(value, toHigh, toLow);
    case WRAP:        return wrap(value, toLow, toHigh);
    default:          return value;
  }
}

float mapFloat(double value, double fromLow, double fromHigh, double toLow, double toHigh, uint8_t mode)
{
  // Avoid divisions by zero.
  if (fromLow == fromHigh)
    value = (toLow + toHigh) / 2.0f; // dummy value
  else
    value = (value - fromLow) * (toHigh - toLow) / (fromHigh - fromLow) + toLow;

  return _mapConvert(value, toLow, toHigh, mode);
}

float mapFrom01(double value, double toLow, double toHigh, uint8_t mode) {
  // Compute value.
  value = (value * (toHigh - toLow)) + toLow;

  // Return and constrain.
  return _mapConvert(value, toLow, toHigh, mode);
}

float mapTo01(double value, double fromLow, double fromHigh, uint8_t mode) {
  // Avoid divisions by zero.
  if (fromLow == fromHigh)
    value = 0.5f; // dummy value
  else
    value = (value - fromLow) / (fromHigh - fromLow);

  // Return and constrain.
  return _mapConvert(value, 0.0, 1.0, mode);
}

} // namespace pq
