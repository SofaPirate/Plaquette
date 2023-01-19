/*
 * Osc.cpp
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

#include "Osc.h"
#include "pq_map_real.h"
#include "pq_time.h"
#include "pq_wrap.h"

namespace pq {

Osc::Osc(float period_) : AnalogSource(), _period(0), _phase(0), _amplitude(1) {
  period(period_);
  amplitude(1.0f);
}

void Osc::begin() {
  // Offset phaseTime to current phase.
  _phaseTime = float2phaseTime(_phase);

  // Compute initial value.
  _updateValue();
}

void Osc::step() {
  // Update phase time.
  phaseTimeUpdate(_phaseTime, _period, sampleRate());

	// Compute next value.
	_updateValue();

	// // Notice: this computation is not exact but manages naturally changes in the period without
	// // inducing dephasings on Arduino boards.
	// float relativeTime = seconds() - _startTime;
	//
  // // Check where we are.
	// float progress = relativeTime / _period;
	// if (progress >= 1) {
	// 	_value = 0;
	// 	_startTime = seconds();
	// }
	// else if (progress >= _width) _value = (1 - progress) / (1 - _width);
	// else                         _value = progress / _width;
	//
	// // Amplify.
  // _value = _amplitude * (_value - 0.5f) + 0.5f;
}


void Osc::period(float period) {
	if (period != _period)
		_period = max(period, 0.0f);
}

void Osc::frequency(float frequency) {
	period( frequency == 0 ? FLT_MAX : 1/frequency );
}

void Osc::amplitude(float amplitude)  {
	if (amplitude != _amplitude)
  	_amplitude = constrain(amplitude, 0, 1);
}

void Osc::phase(float phase) {
	if (phase != _phase) {
		// Need to readjust _phaseTime.
    phaseTimeAdd(_phaseTime, _phase - phase);
		_phase = phase;
	}
}

float Osc::timeToPhase(float time) const { return pq::timeToPhase(_period, time); }

float Osc::shiftBy(float phase) {
	return _getAmplified(phaseTimeAddPhase(_phaseTime, phase));
}

}
