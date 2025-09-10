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
TriangleWave::TriangleWave(float period, float skew, Engine& engine) : AbstractWave(period, skew, engine), _riseEasing(easeNone), _fallEasing(easeNone) {}

q0_32u_t TriangleWave::_getFixed32(q0_32u_t t) {
  return (t <= _skew32) ?
        fixed32Divide(t, _skew32) :
        fixed32Divide(FIXED_32_MAX - t, FIXED_32_MAX - _skew32);
}

float TriangleWave::_getAmplified(q0_32u_t t) {
  if (t <= _skew32) {
    q0_32u_t fixed32Value = fixed32Divide(t, _skew32);
    if (_riseEasing == easeNone) {
      return fixed32ToFloat( amplifyFixed32(fixed32Value, _amplitude) );
    }
    else {
      return amplifyFloat(_riseEasing(fixed32ToFloat(fixed32Value)), _amplitude);
    }
  }
  else {
    if (_fallEasing == easeNone) {
      return AbstractWave::_getAmplified(t);
    }
    else {
      return amplifyFloat(_fallEasing(fixed32ToFloat(fixed32Divide(FIXED_32_MAX - t, FIXED_32_MAX - _skew32))), _amplitude);
    }
  }
}

// float TriangleWave::_get(q0_32u_t t) {
//   // Compute triangle depending on raising or falling step.
//   return (t <= _skew32) ?
//            t / (float(_skew32) + FLT_MIN) : // + FLT_MIN to avoid 0/0
//            (FIXED_32_MAX - t) / (float)(FIXED_32_MAX - _skew32);
// }

}
