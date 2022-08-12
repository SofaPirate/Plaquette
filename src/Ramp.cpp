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

Ramp::Ramp(float initialValue) :
	AbstractTimer(),
	_from(initialValue), _to(initialValue)
{
}

void Ramp::to(float to) {
	fromTo(_value, to);
}

void Ramp::fromTo(float from, float to) {
	_from = from;
	_to   = to;
}

void Ramp::start() {
	AbstractTimer::start();
}

void Ramp::start(float to, float duration) {
	start(_value, to, duration);
}

void Ramp::start(float from, float to, float duration_) {
	fromTo(from, to);

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
		_value = mapFrom01(progress(), _from, _to);
	}
}

}
