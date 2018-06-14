/*
 * pq_lerp.h
 *
 * Equivalent of Processing lerp() method.
 *
 * (c) 2017 Sofian Audry        :: info(@)sofianaudry(.)com
 * (c) 2017 Thomas O Fredericks :: tof(@)t-o-f(.)info
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

 #if defined(ARDUINO) && ARDUINO >= 100
 #include "Arduino.h"
 #else
 #include "WProgram.h"
 #endif

#include "pq_map_real.h"

namespace pq {

float lerp(float from, float to, float amount) {
  float value = mapFloat(amount, 0, 1, from, to);
  return constrain(value, from, to);
}

} // namespace pq
