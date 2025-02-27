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

AbstractWave::AbstractWave(float period_, float width_) : AnalogSource(), _period(0), _phase(0), _amplitude(1), _width(0), _isRunning(false), 
                                                         _onValue(0), _prevOnValue(0), _changeState(0), _data(0) { // This is just for SquareWave.
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
    phaseTimeUpdate(_phaseTime, _period, sampleRate());

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
  if (period != _period)
    _period = max(period, 0.0f);
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


void AbstractWave::start() {
  // Start.
  setTime(0);
  _isRunning = true;
}

void AbstractWave::stop() {
  // Stop.
  setTime(0);
  _isRunning = false;
}

void AbstractWave::pause() {
  _isRunning = false;
}

void AbstractWave::resume() {
  _isRunning = true;
}

void AbstractWave::togglePause() {
  _isRunning = !_isRunning;
}

void AbstractWave::setTime(float time) {
  // Set current phase time.
  _phaseTime = floatToPhaseTime(_phase);

  // Perform calculation iff time needs to be added.
  if (time != 0)
    _phaseTime = phaseTimeAddTime(_phaseTime, _period, time);

  // Compute value.
  _value = _getAmplified(_phaseTime);
}

}
