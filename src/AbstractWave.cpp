/*
 * AbstractWave.cpp
 *
 * (c) 2022 Sofian Audry        :: info(@)sofianaudry(.)com
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

#include "AbstractWave.h"
#include "pq_map_real.h"
#include "pq_time.h"
#include "pq_wrap.h"

namespace pq {

AbstractWave::AbstractWave(Engine& engine) : AbstractWave(1.0f, 0.5f, engine) {}
AbstractWave::AbstractWave(float period, Engine& engine) : AbstractWave(period, 0.5f, engine) {}
AbstractWave::AbstractWave(float period_, float width_, Engine& engine)
: AnalogSource(engine), Timeable(),
  _period(0),
#if PQ_OPTIMIZE_FOR_CPU
  _frequency(FLT_MAX),
#endif
  _amplitude(1), _width(0), _phaseShift(0),
  _overflowed(false), _isRunning(false), _isForward(true), _valueNeedsUpdate(true),
  _onValue(0), _prevOnValue(0), _changeState(0) {
  period(period_);
  width(width_);
  amplitude(1.0f);
}

float AbstractWave::get() {
  // Prevents unnecessary computations in the step() function by updating the value on a need basis.
  if (_valueNeedsUpdate) {
      // Compute next value.
    _value = _getAmplified(_phaseTime);
    _valueNeedsUpdate = false; // reset flag
  }

  return _value;
}

void AbstractWave::begin() {
  start();
  _valueNeedsUpdate = true;
}

void AbstractWave::step() {
  // Update phase time.
  if (isRunning())
    _overflowed = phaseTimeUpdate(_phaseTime, _period, sampleRate(), _isForward);
  else
    _overflowed = false;

  // Set flag to indicate value is out of sync.
  _valueNeedsUpdate = true;
  // // Notice: this computation is not exact but manages naturally changes in the period without
  // // inducing dephasings on Arduino boards.
  // float relativeTime = seconds() - _startTime;
  //
  // // Check where we are.
  // float progress = relativeTime / _period;
  // if (progress >= 1) {
  //   _value = 0;
  //   _startTime = seconds();
  // }
  // else if (progress >= _width) _value = (1 - progress) / (1 - _width);
  // else                         _value = progress / _width;
  //
  // // Amplify.
  // _value = _amplitude * (_value - 0.5f) + 0.5f;
}

float AbstractWave::_getAmplified(fixed_t t) {
  return fixedToFloat( amplifyFixed(_getFixed(t), _amplitude) );
}

void AbstractWave::period(float period) {
  // Assign period.
  if (_period != period) {
    _period = max(period, 0.0f); // Make sure period is positive.

#if PQ_OPTIMIZE_FOR_CPU
    // Assign frequency.
    _frequency = periodToFrequency(_period);
#endif
  }
}

void AbstractWave::frequency(float frequency) {
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

void AbstractWave::bpm(float bpm) {
  frequency(bpm * BPM_TO_HZ);
}

void AbstractWave::width(float width) {
  _width = floatTofixed(width);
}

void AbstractWave::amplitude(float amplitude)  {
  _amplitude = floatTofixed(amplitude);
}

void AbstractWave::phase(float phase) {
  _phaseTime = floatToPhaseTime(phase);
}

void AbstractWave::phaseShift(float phaseShift) {
  if (_phaseShift != phaseShift) {
    // Need to readjust phase time.
    _phaseTime = phaseTimeAddPhase(_phaseTime, _phaseShift - phaseShift);
    _phaseShift = phaseShift;
  }
}

float AbstractWave::timeToPhase(float time) const { return pq::timeToPhase(_period, time); }

float AbstractWave::shiftBy(float phaseShift) {
  return _getAmplified(phaseTimeAddPhase(_phaseTime, phaseShift));
}

float AbstractWave::getShiftedByTime(float time) {
  return _getAmplified(phaseTimeAddPhase(_phaseTime, pq::frequencyAndTimeToPhase(frequency(), time)));
}

float AbstractWave::atPhase(float phase) {
  return _getAmplified(floatToPhaseTime(phase));
}

void AbstractWave::forward() {
  _isForward = true;
}

void AbstractWave::reverse() {
  _isForward = false;
}

void AbstractWave::toggleReverse() {
  _isForward = !_isForward;
}

void AbstractWave::setTime(float time) {
  // Reset phase time to beginning.
  _phaseTime = floatToPhaseTime(_phaseShift);

  // Add time.
  addTime(time);
}

void AbstractWave::addTime(float time) {
  // Perform calculation iff time needs to be added.
  if (time > 0)
    _phaseTime = phaseTimeAddTime(_phaseTime, _period, time);

  // Compute value.
  _valueNeedsUpdate = true;
}

void AbstractWave::_setIsRunning(bool isRunning)
{
  _isRunning = isRunning;
}

}
