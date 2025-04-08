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
  _period(0), _phase(0), _amplitude(1), _width(0), _isRunning(false), _isForward(true),
  _onValue(0), _prevOnValue(0), _changeState(0), _data(0) {
  period(period_);
  width(width_);
  amplitude(1.0f);
}

void AbstractWave::begin() {
  start();
}

void AbstractWave::step() {
  // Update phase time.
  if (isRunning())
    phaseTimeUpdate(_phaseTime, _period, sampleRate(), _isForward);

  // Compute next value.
  _value = _getAmplified(_phaseTime);

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
  if (_period != period) {
    _period = max(period, 0.0f);
  }
}

void AbstractWave::frequency(float frequency) {
  period( frequency == 0 ? FLT_MAX : 1.0f/frequency );
}

void AbstractWave::bpm(float bpm) {
  period( bpm == 0 ? FLT_MAX : 60.0f/bpm );
}

void AbstractWave::width(float width) {
  _width = floatTofixed(width);
}

void AbstractWave::amplitude(float amplitude)  {
  _amplitude = floatTofixed(amplitude);
}

void AbstractWave::phase(float phase) {
  if (phase != _phase) {
    // Need to readjust _phaseTime.
      _phaseTime = phaseTimeAddPhase(_phaseTime, _phase - phase);
    _phase = phase;
  }
}

float AbstractWave::timeToPhase(float time) const { return pq::timeToPhase(_period, time); }

float AbstractWave::shiftBy(float phaseShift) {
  return _getAmplified(phaseTimeAddPhase(_phaseTime, phaseShift));
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
  _phaseTime = floatToPhaseTime(_phase);

  // Add time.
  addTime(time);
}

void AbstractWave::addTime(float time) {
  // Perform calculation iff time needs to be added.
  if (time > 0)
    _phaseTime = phaseTimeAddTime(_phaseTime, _period, time);

  // Compute value.
  _value = _getAmplified(_phaseTime);
}

void AbstractWave::_setIsRunning(bool isRunning) 
{ 
  _isRunning = isRunning; 
}

}
