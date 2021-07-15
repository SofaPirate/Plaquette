/*
 * SineOsc.cpp
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

#include "SineOsc.h"
#include "pq_map_real.h"
#include "pq_time.h"
#include "pq_trig8.h"

namespace pq {

SineOsc::SineOsc(float period_) : _phase(0) {
  period(period_);
	phase(0);
	amplitude(1.0f);
}

#define _PQ_SINE_OSC_PHASE_TIME_PREMULTIPLIER (65535.5f)
#define _PQ_SINE_OSC_PHASE_TIME_MAX           (65535)

void SineOsc::begin() {
	_phaseTime = _PQ_SINE_OSC_PHASE_TIME_PREMULTIPLIER * _phase;
  _updateValue();
}

void SineOsc::step() {
  float minPeriod = PLAQUETTE_OSC_MIN_SAMPLE_PERIOD_MULTIPLIER * samplePeriod();
	// Wave needs to compute its own "time" to allow smooth transitions when changing period.
	_phaseTime += _PQ_SINE_OSC_PHASE_TIME_PREMULTIPLIER / (max(_period, minPeriod) * sampleRate());
	while (_phaseTime > _PQ_SINE_OSC_PHASE_TIME_MAX) _phaseTime-=_PQ_SINE_OSC_PHASE_TIME_MAX; // modulo
	// Compute next value.
	_updateValue();

	// // Notice: this computation is not exact but manages naturally changes in the period without
	// // inducing dephasings on Arduino boards and slow processors.
	// float relativeTime = seconds() - _startTime;
	// uint16_t theta = ((uint64_t)(65535UL * (_phase + relativeTime / _period))) % 65536;
	// _value = ((cos16(theta) * _amplitude) + 1) / 2;
	// if (relativeTime >= _period)
	// 	_startTime = seconds();
}

void SineOsc::_updateValue() {
	_value = ((cos16((uint16_t)(_phaseTime)) * _amplitude) + 1) / 2;
}

SineOsc& SineOsc::period(float period) {
	if (_period != period)
		_period = max(period, 0.0f);
	return *this;
}

SineOsc& SineOsc::frequency(float frequency) {
	return period( frequency == 0 ? FLT_MAX : 1/frequency );
}

SineOsc& SineOsc::amplitude(float amplitude)  {
	if (amplitude != _amplitude)
  	_amplitude = constrain(amplitude, 0, 1);
	_amplitude /= -32767.0f; // hack: precompute value
	return *this;
}

SineOsc& SineOsc::phase(float phase) {
	if (phase != _phase) {
		phase = constrain(phase, 0, 1);
		_phaseTime += _PQ_SINE_OSC_PHASE_TIME_PREMULTIPLIER * (phase - _phase);
		while (_phaseTime > _PQ_SINE_OSC_PHASE_TIME_MAX)
			_phaseTime -= _PQ_SINE_OSC_PHASE_TIME_MAX; // modulo
		while (_phaseTime < 0)
		 	_phaseTime += _PQ_SINE_OSC_PHASE_TIME_MAX; // modulo
		_phase = phase;
	}
	return *this;
}

}
