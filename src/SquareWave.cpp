/*
 * SquareWave.cpp
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

#include "SquareWave.h"

namespace pq {

SquareWave::SquareWave(Engine& engine) : AbstractWave(engine) {}
SquareWave::SquareWave(float period, Engine& engine) : AbstractWave(period, engine) {}
SquareWave::SquareWave(float period, float skew, Engine& engine) : AbstractWave(period, skew, engine) {}

// bool SquareWave::isOn() {
//   return _onValue;
// }

// bool SquareWave::shiftByIsOn(float phaseShift) {
//   return (phase32AddPhase(_phase32, phaseShift) <= _skew32);
// }

// bool SquareWave::atPhaseIsOn(float phase) {
//   return (floatToPhase32(phase) <= _skew32);
// }

void SquareWave::step() {
  AbstractWave::step();

  // // Force compute digital value.
  _onValue = (_phase32 <= _skew32);

  // // Update change state.
  // _changeState = (int8_t)_onValue - (int8_t)_prevOnValue;
  // _prevOnValue = _onValue;
}

q0_32u_t SquareWave::_getFixed32(q0_32u_t t) {
  return (_onValue ? FIXED_MAX : 0);
}

}
