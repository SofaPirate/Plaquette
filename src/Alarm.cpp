/*
 * Alarm.cpp
 *
 * (c) 2018 Sofian Audry        :: info(@)sofianaudry(.)com
 * (c) 2018 Thomas O Fredericks :: tof(@)t-o-f(.)info
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

#include "Alarm.h"

namespace pq {


Alarm::Alarm(Engine& engine) : Alarm(1.0f, engine) {}

Alarm::Alarm(float duration, Engine& engine) :
    DigitalSource(engine),
    AbstractTimer(duration),
    _engine(engine)
  {
  }
  
void Alarm::begin() {
  setTime(0);
}

void Alarm::step() {
  // Call parent update.
  update();

  if (_isRunning) {
    // Compute value if running -- otherwise leave as is.
    _onValue = isFinished();
  }

  // Update change state.
  _updateChangeState();
}

void Alarm::setTime(float time) {
  AbstractTimer::setTime(time);
  _onValue = isFinished();
}

float Alarm::_time() const {
  return Unit::seconds();
}

}
