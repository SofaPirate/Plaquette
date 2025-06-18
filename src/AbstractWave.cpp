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
#include "pq_map.h"
#include "pq_time.h"
#include "pq_wrap.h"

namespace pq {

AbstractWave::AbstractWave(Engine& engine) : AbstractWave(1.0f, 0.5f, engine) {}
AbstractWave::AbstractWave(float period, Engine& engine) : AbstractWave(period, 0.5f, engine) {}
AbstractWave::AbstractWave(float period, float skew_, Engine& engine)
: AnalogSource(engine), AbstractOscillator(period) {
  skew(skew_);
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
}

void AbstractWave::step() {
  // Update phase time.
  if (isRunning())
    _overflowed = phaseTimeUpdateFixed(_phaseTime, frequency(), engine()->deltaTimeSecondsTimesFixedMax(), _isForward);
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
  // else if (progress >= _skew) _value = (1 - progress) / (1 - _skew);
  // else                         _value = progress / _skew;
  //
  // // Amplify.
  // _value = _amplitude * (_value - 0.5f) + 0.5f;
}

float AbstractWave::_getAmplified(fixed_t t) {
  return fixedToFloat( amplifyFixed(_getFixed(t), _amplitude) );
}

float AbstractWave::shiftBy(float phaseShift) {
  return _getAmplified(phaseTimeAddPhase(_phaseTime, phaseShift));
}

float AbstractWave::getShiftedByTime(float time) {
  return _getAmplified(phaseTimeAddPhase(_phaseTime, pq::frequencyAndTimeToPhase(frequency(), time)));
}

float AbstractWave::atPhase(float phase) {
  return _getAmplified(floatToPhaseTime(phase));
}

void AbstractWave::skew(float skew) {
  _skew = floatTofixed(skew);
  _valueNeedsUpdate = true;
}

// void AbstractWave::_setIsRunning(bool isRunning)
// {
//   _isRunning = isRunning;
// }

}
