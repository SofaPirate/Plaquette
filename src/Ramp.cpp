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

namespace pq {

Ramp::Ramp(float initialValue, easing_function easing) :
	AbstractTimer(),
	_from(initialValue), _to(initialValue), _easing(easing)
{
}

Ramp::Ramp(easing_function easing) :
	AbstractTimer(),
	_from(0.0f), _to(0.0f), _easing(easing)
{
}

void Ramp::easing(easing_function easing) {
	_easing = easing;
}

void Ramp::to(float to) {
	fromTo(_value, to);
}

void Ramp::to(float to, easing_function easing_) {
	fromTo(_value, to, easing_);
}

void Ramp::fromTo(float from, float to) {
	_from = from;
	_to   = to;
}

void Ramp::fromTo(float from, float to, easing_function easing_) {
	fromTo(from, to);
	easing(easing_);

}

void Ramp::start() {
	AbstractTimer::start();
}

void Ramp::start(float to, float duration) {
	start(_value, to, duration);
}

void Ramp::start(float to, float duration, easing_function easing_) {
	start(_value, to, duration, easing_);
}

void Ramp::start(float from, float to, float duration_) {
	fromTo(from, to);

	// Initialize duration.
	duration(duration_);

	// Start chronometer.
	start();
}

void Ramp::start(float from, float to, float duration_, easing_function easing_) {
	fromTo(from, to, easing_);

	// Initialize duration.
	duration(duration_);

	// Start chronometer.
	start();
}

void Ramp::step() {
	AbstractTimer::step();

	if (_isStarted) {
		// Compute value if running -- otherwise leave as is.
		// TODO: implement easing functions
		_value = mapFrom01(_easing(progress()), _from, _to);
	}
}

}
