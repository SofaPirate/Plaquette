/*
 * SquareOsc.cpp
 *
 * (c) 2015 Sofian Audry        :: info(@)sofianaudry(.)com
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

#include "SquareOsc.h"
#include "pq_map_real.h"
#include "pq_time.h"
#include "pq_wrap.h"
#include "pq_osc_utils.h"

namespace pq {

SquareOsc::SquareOsc(float period_, float dutyCycle_) : AnalogSource() {
  period(period_);
  dutyCycle(dutyCycle_);
	amplitude(1.0f);
}

void SquareOsc::begin() {
	_phaseTime = _phase;
}

void SquareOsc::step() {
  // Update phase time.
  phaseTimeUpdate(_phaseTime, _period, sampleRate());

	// Compute next value.
	_updateValue();

	// // Notice: this computation is not exact but manages naturally changes in the period without
	// // inducing dephasings on Arduino boards.
	// float progress = (seconds() - _startTime) / _period;
	// _isOn = (progress < _dutyCycle);
	// if (progress >= 1) // reset
  // 	_startTime = seconds();
}

void SquareOsc::_updateValue() {
	_value = 0.5f + (_phaseTime < _dutyCycle ? _amplitude : -_amplitude);
}

SquareOsc& SquareOsc::period(float period) {
  if (_period != period)
	  _period = max(period, 0.0f);
	return *this;
}

SquareOsc& SquareOsc::frequency(float frequency) {
	return period( frequency == 0 ? FLT_MAX : 1/frequency );
}

SquareOsc& SquareOsc::dutyCycle(float dutyCycle) {
  _dutyCycle = constrain(dutyCycle, 0, 1);
	return *this;
}

SquareOsc& SquareOsc::phase(float phase) {
	if (phase != _phase) {
		// Need to readjust _phaseTime.
		_phaseTime += (_phase - phase);
    _phaseTime = wrap01(_phaseTime);
		_phase = phase;
	}
	return *this;
}

SquareOsc& SquareOsc::amplitude(float amplitude)  {
	if (amplitude != _amplitude) {
  	_amplitude = constrain(amplitude, 0, 1);
		_amplitude *= 0.5f; // hack: premultiplied
	}
	return *this;
}

}
