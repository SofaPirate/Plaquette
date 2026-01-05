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
: AnalogSource(engine), AbstractOscillator(period), _amplitude(0) {
  skew(skew_);
  amplitude(1.0f);
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
  _preSkew = _isPreSkew();
  _passedSkew = false;
}

void AbstractWave::step() {
  // Update phase time.
  _stepPhase(engine()->deltaTimeSecondsTimesFixed32Max());

  // Serial.printf("pre:%i passed:%i over:%i\n", _preSkew, _passedSkew, _overflowed);
  // Update passed-skew flag.
  _updatePassedSkew();
  // Serial.printf("--> pre:%i passed:%i over:%i\n", _preSkew, _passedSkew, _overflowed);

  // Set flag to indicate value is out of sync.
  _valueNeedsUpdate = true;
}

float AbstractWave::_getAmplified(q0_32u_t t) {
  return fixed32ToFloat( amplifyFixed32(_getFixed32(t), _amplitude) );
}

void AbstractWave::_updatePassedSkew() {
  // Check if we moved from pre to post or vice versa.
  bool newPreSkew = _isPreSkew();
  // _passedSkew = (_preSkew != newPreSkew && !_overflowed); // check change
  if (_skew32 == 0 ||_skew32 == FIXED_32_MAX) // skew() == 0 || skew() == 1
    _passedSkew = _overflowed;
  else
    _passedSkew = (_isForward ? (_preSkew && !newPreSkew) : (!_preSkew && newPreSkew)); // check change
  _preSkew = newPreSkew; // set new value
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

void AbstractWave::amplitude(float amplitude)  {
  _amplitude = floatToFixed32(amplitude);
}

void AbstractWave::skew(float skew) {
  _skew32 = floatToFixed32(skew);
  _updatePassedSkew();
  _valueNeedsUpdate = true;
}


void AbstractWave::onPassPeriod(EventCallback callback) {
  onEvent(callback, EVENT_CUSTOM_1);
}

void AbstractWave::onPassSkew(EventCallback callback) {
  onEvent(callback, EVENT_CUSTOM_2);
}

bool AbstractWave::eventTriggered(EventType eventType) {
  if (eventType == EVENT_CUSTOM_1) return passedPeriod();
  else if (eventType == EVENT_CUSTOM_2) return passedSkew();
  else return AnalogSource::eventTriggered(eventType);
}

// void AbstractWave::_setRunning(bool isRunning)
// {
//   _isRunning = isRunning;
// }

}
