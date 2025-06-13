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

TriangleWave::TriangleWave(Engine& engine) : AbstractWave(engine), _riseEasing(easeNone), _fallEasing(easeNone) {}
TriangleWave::TriangleWave(float period, Engine& engine) : AbstractWave(period, engine), _riseEasing(easeNone), _fallEasing(easeNone) {}
TriangleWave::TriangleWave(float period, float width, Engine& engine) : AbstractWave(period, width, engine), _riseEasing(easeNone), _fallEasing(easeNone) {}

fixed_t TriangleWave::_getFixed(fixed_t t) {
  return (t <= _width) ?
        fixedDivide(t, _width) :
        fixedDivide(FIXED_MAX - t, FIXED_MAX - _width);
}

float TriangleWave::_getAmplified(fixed_t t) {
  if (t <= _width) {
    fixed_t fixedValue = fixedDivide(t, _width);
    if (_riseEasing == easeNone) {
      return fixedToFloat( amplifyFixed(fixedValue, _amplitude) );
    }
    else {
      return amplifyFloat(_riseEasing(fixedToFloat(fixedValue)), _amplitude);
    }
  }
  else {
    if (_fallEasing == easeNone) {
      return AbstractWave::_getAmplified(t);
    }
    else {
      return amplifyFloat(_fallEasing(fixedToFloat(fixedDivide(FIXED_MAX - t, FIXED_MAX - _width))), _amplitude);
    }
  }
}

// float TriangleWave::_get(fixed_t t) {
//   // Compute triangle depending on raising or falling step.
//   return (t <= _width) ?
//            t / (float(_width) + FLT_MIN) : // + FLT_MIN to avoid 0/0
//            (FIXED_MAX - t) / (float)(FIXED_MAX - _width);
// }

}
