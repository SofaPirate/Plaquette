/*
 * AbstractOscillator.cpp
 *
 * (c) 2025 Sofian Audry        :: info(@)sofianaudry(.)com
 * (c) 2025 Thomas O Fredericks :: tof(@)t-o-f(.)info
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

#include "AbstractOscillator.h"
#include "pq_map.h"
#include "pq_time.h"
#include "pq_wrap.h"

namespace pq {

AbstractOscillator::AbstractOscillator(float period_)
: Timeable(),
  _period(period_+1), // makes sure period is different and will be adjusted
#if PQ_OPTIMIZE_FOR_CPU
  _frequency(FLT_MAX),
#endif
  _phaseShift(0),
  _overflowed(false), _isRunning(false), _isForward(true), _valueNeedsUpdate(true) {
  period(period_);
}

void AbstractOscillator::_stepPhase(float deltaTimeSecondsTimesFixed32Max) {
  // Adjust phase time.
  // NOTE (optimization trick): If isRunning() is false, phase32UpdateFixed32() will not be called.
  _overflowed = (isRunning() &&
                 phase32UpdateFixed32(_phase32, frequency(), deltaTimeSecondsTimesFixed32Max, _isForward));
}

void AbstractOscillator::period(float period) {
  // Assign period.
  if (_period != period) {
    _period = max(period, 0.0f); // Make sure period is positive.

#if PQ_OPTIMIZE_FOR_CPU
    // Assign frequency.
    _frequency = periodToFrequency(_period);
#endif
  }
}

void AbstractOscillator::frequency(float frequency) {
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

void AbstractOscillator::bpm(float bpm) {
  frequency(bpm * BPM_TO_HZ);
}

void AbstractOscillator::phase(float phase) {
  _setPhase32(floatToPhase32(phase));
}

void AbstractOscillator::phaseShift(float phaseShift) {
  if (_phaseShift != phaseShift) {
    // Need to readjust phase time.
    _setPhase32(phase32AddPhase(_phase32, _phaseShift - phaseShift));
    _phaseShift = phaseShift;
  }
}

float AbstractOscillator::timeToPhase(float time) const { return pq::timeToPhase(_period, time); }

void AbstractOscillator::setTime(float time) {
  // Reset phase time to beginning.
  _setPhase32( phase32AddTime(_phaseShift, _period, time) );
}

void AbstractOscillator::addTime(float time) {
  // Perform calculation iff time needs to be added.
  if (time > 0)
    _setPhase32( phase32AddTime(_phase32, _period, time) );
}

void AbstractOscillator::_setPhase32(q0_32u_t phase32) {
  _phase32 = phase32;
  _valueNeedsUpdate = true;
}

void AbstractOscillator::_setRunning(bool isRunning)
{
  _isRunning = isRunning;
}

}
