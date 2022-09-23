/*
 * TriOsc.cpp
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

#include "TriOsc.h"
#include "pq_map_real.h"
#include "pq_time.h"
#include "pq_wrap.h"

namespace pq {

TriOsc::TriOsc(float period_, float width_) : AnalogSource() {
  period(period_);
  width(width_);
  amplitude(1.0f);
}

void TriOsc::begin() {
  _phaseTime = float2phaseTime(_phase);
  _updateValue();
}

void TriOsc::step() {
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

void TriOsc::_updateValue() {
  // Convert phase time to float in [0, 1].
  float phaseTimeFloat = phaseTime2float(_phaseTime);
	// Compute triangle depending on raising or falling step.
	if (phaseTimeFloat >= _width) _value = (1 - phaseTimeFloat) / (1 - _width);
	else                          _value = phaseTimeFloat / _width;
	// Amplify.
  _value = _amplitude * (_value - 0.5f) + 0.5f;
}

TriOsc& TriOsc::period(float period) {
	if (period != _period)
		_period = max(period, 0.0f);
	return *this;
}

TriOsc& TriOsc::frequency(float frequency) {
	return period( frequency == 0 ? FLT_MAX : 1/frequency );
}

TriOsc& TriOsc::width(float width) {
	if (width != _width)
  	_width = constrain(width, 0, 1);
	return *this;
}

TriOsc& TriOsc::amplitude(float amplitude)  {
	if (amplitude != _amplitude)
  	_amplitude = constrain(amplitude, 0, 1);
	return *this;
}

TriOsc& TriOsc::phase(float phase) {
	if (phase != _phase) {
		// Need to readjust _phaseTime.
    phaseTimeAdd(_phaseTime, _phase - phase);
		_phase = phase;
	}
	return *this;
}

}
