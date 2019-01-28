/*
 * Ramp.cpp
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

#include "Ramp.h"
#include "pq_map_real.h"
#include "pq_time.h"

Ramp::Ramp(float initialValue_) :
	_value(initialValue_), _duration(),
	_from(initialValue_), _change(0),
	_startTime(0), _offsetTime(0),
	_isRunning(false)
{
}

void Ramp::start(float to, float duration) {
	start(_value, to, duration);
}

void Ramp::start(float from, float to, float duration) {
	// Set parameters.
	_duration = duration;
	_from = from;
	_change = (to - from);

	// Start.
	_startTime = seconds();
	_offsetTime = 0;
	_isRunning = true;
}

void Ramp::stop() {
	if (_isRunning) {
		_offsetTime = elapsed();
		_isRunning = false;
	}
}

void Ramp::resume() {
	if (!_isRunning) {
		_startTime = seconds();
		_isRunning = true;
	}
}

float Ramp::progress() const {
	// Compute progress as % of duration.
	float prog = elapsed() / _duration;
	prog = constrain(prog, 0, 1);
	return prog;
}

void Ramp::setup() {
	// Basic reset.
	_startTime = _offsetTime = 0;
	_isRunning = false;
}

void Ramp::update() {
	// Offset elapsed time.
	_elapsedTime = _offsetTime;

	if (_isRunning) {
		// Add difference to elapsed time.
	 	_elapsedTime += (seconds() - _startTime);

		// Compute value if running -- otherwise leave as is.
		// TODO: implement easing functions
	  _value = mapFloat(progress(), 0.0f, 1.0f, _from, (_from + _change));
	}
}