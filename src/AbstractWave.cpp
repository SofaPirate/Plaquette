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
    _value = _getAmplified(_phase32);
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
    _overflowed = phase32UpdateFixed32(_phase32, frequency(), engine()->deltaTimeSecondsTimesFixed32Max(), _isForward);
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
  // else if (progress >= _skew32) _value = (1 - progress) / (1 - _skew32);
  // else                         _value = progress / _skew32;
  //
  // // Amplify.
  // _value = _amplitude * (_value - 0.5f) + 0.5f;
}

float AbstractWave::_getAmplified(q0_32u_t t) {
  return fixed32ToFloat( amplifyFixed32(_getFixed32(t), _amplitude) );
}

float AbstractWave::shiftBy(float phaseShift) {
  return _getAmplified(phase32AddPhase(_phase32, phaseShift));
}

float AbstractWave::shiftByTime(float timeShift) {
  return _getAmplified(phase32AddPhase(_phase32, frequencyAndTimeToPhase(frequency(), timeShift)));
}

float AbstractWave::atPhase(float phase) {
  return _getAmplified(floatToPhase32(phase));
}

void AbstractWave::skew(float skew) {
  _skew32 = floatToFixed32(skew);
  _valueNeedsUpdate = true;
}

// void AbstractWave::_setRunning(bool isRunning)
// {
//   _isRunning = isRunning;
// }

}
