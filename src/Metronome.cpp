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
Metronome::Metronome(float period_, Engine& engine) : DigitalUnit(engine), Timeable(),
  _period(0),
#if PQ_OPTIMIZE_FOR_CPU
  _frequency(FLT_MAX),
#endif
  _phaseShift(0), _onValue(0), _isRunning(false) {
  period(period_);
}

void Metronome::begin() {
  _phaseTime = floatToPhaseTime(_phaseShift);
  _onValue = 0;
  _isRunning = true;
}

void Metronome::step() {
  // Adjust phase time.
  _onValue = phaseTimeUpdate(_phaseTime, _period, sampleRate());
}


void Metronome::period(float period) {
  // Assign period.
  if (_period != period) {
    _period = max(period, 0.0f); // Make sure period is positive.

#if PQ_OPTIMIZE_FOR_CPU
    // Assign frequency.
    _frequency = periodToFrequency(_period);
#endif
  }
}

void Metronome::frequency(float frequency) {
#if PQ_OPTIMIZE_FOR_CPU
  // Assign period.
  if (_frequency != frequency) {
    _frequency = max(frequency, 0.0f); // Make sure frequency is positive.

    // Assign period.
    _period = frequencyToPeriod(_frequency);
  }
#else
  period( frequencyToPeriod(frequency) );
#endif
}

void Metronome::bpm(float bpm) {
  frequency(bpm * BPM_TO_HZ);
}

void Metronome::phase(float phase) {
  _phaseTime = floatToPhaseTime(phase);
}

void Metronome::phaseShift(float phaseShift) {
  if (_phaseShift != phaseShift) {
    // Need to readjust phase time.
    _phaseTime = phaseTimeAddPhase(_phaseTime, _phaseShift - phaseShift);
    _phaseShift = phaseShift;
  }
}

void Metronome::onBang(EventCallback callback) {
  onEvent(callback, EVENT_BANG);
}

bool Metronome::eventTriggered(EventType eventType) {
  if (eventType == EVENT_BANG) return _onValue;
  else return DigitalUnit::eventTriggered(eventType);
}


void Metronome::setTime(float time) {
  // Reset phase time to beginning.
  _phaseTime = floatToPhaseTime(_phaseShift);

  // Add time.
  addTime(time);
}

void Metronome::addTime(float time) {
  // Perform calculation iff time needs to be added.
  if (time > 0)
    _phaseTime = phaseTimeAddTime(_phaseTime, _period, time);
}

void Metronome::_setIsRunning(bool isRunning)
{
  _isRunning = isRunning;
}
}
