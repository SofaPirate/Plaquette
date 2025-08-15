/*
 * Wave.cpp
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

#include "Wave.h"

#include "pq_wave_utils.h"

namespace pq
{

Wave::Wave(float period, Engine &engine) : Wave(period, 0.5f, engine) {}
Wave::Wave(WaveShape shape, Engine &engine) : Wave(shape, 1.0f, engine) {}
Wave::Wave(float period, float skew, Engine &engine) : Wave(SQUARE, period, skew, engine) {}
Wave::Wave(WaveShape shape, float period, Engine &engine) : Wave(shape, period, 0.5f, engine) {}
Wave::Wave(WaveShape shape, float period, float skew, Engine &engine) : AbstractWave(period, skew, engine), _shape(shape) {};

void Wave::shape(WaveShape shape) {
  if (_shape != shape) {
    _shape = shape;
    _valueNeedsUpdate = true;
  }
}

fixed_t Wave::_getFixed(fixed_t t)
{
  static fixed_t (*WAVE_FORM_FUNCTIONS[N_SHAPES])(fixed_t, fixed_t) = {squareWave, triangleWave, sineWave};
  // Return appropriate wave function according to shape.
  return WAVE_FORM_FUNCTIONS[_shape](t, _skew);
}

}
