/*
 * TriangleWave.cpp
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

#include "TriangleWave.h"

namespace pq {

TriangleWave::TriangleWave(float period_, float width_) : AbstractWave(period_, width_) {
}

fixed_t TriangleWave::_getFixed(fixed_t t) {
  return (t <= _width) ? 
        fixedDivide(t, _width) :
        fixedDivide(FIXED_MAX - t, FIXED_MAX - _width);
}

// float TriangleWave::_get(fixed_t t) {
//   // Compute triangle depending on raising or falling step.
//   return (t <= _width) ? 
//            t / (float(_width) + FLT_MIN) : // + FLT_MIN to avoid 0/0
//            (FIXED_MAX - t) / (float)(FIXED_MAX - _width);
// }

}
