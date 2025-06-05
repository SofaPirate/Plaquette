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
SquareWave::SquareWave(float period, float width, Engine& engine) : AbstractWave(period, width, engine) {}
  
bool SquareWave::isOn() {
  return _onValue;
}

bool SquareWave::shiftByIsOn(float phaseShift) {
  return (phaseTimeAddPhase(_phaseTime, phaseShift) <= _width);
}

bool SquareWave::atPhaseIsOn(float phase) {
  return (floatToPhaseTime(phase) <= _width);
}

void SquareWave::step() {
  AbstractWave::step();

  // Update change state.
  _changeState = (int8_t)_onValue - (int8_t)_prevOnValue;
  _prevOnValue = _onValue;
}

fixed_t SquareWave::_getFixed(fixed_t t) {
  return ((_onValue = (t <= _width)) ? FIXED_MAX : 0); // XXX small trick here: we set _onValue at the same time to deal with state changes
}

}
