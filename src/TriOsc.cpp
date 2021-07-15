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

namespace pq {

TriOsc::TriOsc(float period_, float width_) : PqAnalogSource() {
  period(period_);
  width(width_);
	amplitude(1.0f);
}

void TriOsc::begin() {
	_phaseTime = _phase;
  _updateValue();
}

void TriOsc::step() {
  float minPeriod = PLAQUETTE_OSC_MIN_SAMPLE_PERIOD_MULTIPLIER * samplePeriod();
	// Wave needs to compute its own "time" to allow smooth transitions when changing period.
	_phaseTime += 1.0f / (max(_period, minPeriod) * sampleRate());
	while (_phaseTime > 1) _phaseTime--; // modulo
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
	// Compute triangle depending on raising or falling step.
	if (_phaseTime >= _width) _value = (1 - _phaseTime) / (1 - _width);
	else                      _value = _phaseTime / _width;
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
		_phaseTime += (phase - _phase);
		while (_phaseTime > 1) _phaseTime--; // modulo
		while (_phaseTime < 0) _phaseTime++; // modulo
		_phase = phase;
	}
	return *this;
}

}
