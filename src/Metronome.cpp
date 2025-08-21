/*
 * Metronome.cpp
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

#include "Metronome.h"
#include "pq_wrap.h"

namespace pq {

Metronome::Metronome(Engine& engine) : Metronome(1.0f, engine) {}
Metronome::Metronome(float period_, Engine& engine) : DigitalUnit(engine), AbstractOscillator(period_) {}

void Metronome::begin() {
  start();
  _onValue = 0;
}

void Metronome::step() {
  // Adjust phase time.
  _onValue = (isRunning() &&
              phaseTimeUpdateFixed(_phaseTime, frequency(), engine()->deltaTimeSecondsTimesFixedMax(), _isForward));
}

void Metronome::onBang(EventCallback callback) {
  onEvent(callback, EVENT_BANG);
}

bool Metronome::eventTriggered(EventType eventType) {
  if (eventType == EVENT_BANG) return _onValue;
  else return DigitalUnit::eventTriggered(eventType);
}

}
