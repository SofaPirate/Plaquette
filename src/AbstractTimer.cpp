/*
 * AbstractTimer.cpp
 *
 * (c) 2018 Sofian Audry        :: info(@)sofianaudry(.)com
 * (c) 2018 Thomas O Fredericks :: tof(@)t-o-f(.)info
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

#include "AbstractTimer.h"

namespace pq {

AbstractTimer::AbstractTimer(float duration_) : Chrono() {
	duration(duration_);
}

void AbstractTimer::start() {
	Chrono::start();
}

void AbstractTimer::start(float duration_) {
	duration(duration_);
	start();
}

void AbstractTimer::duration(float duration) {
	// Set parameters.
	_duration = max(duration, 0.f);
}

float AbstractTimer::progress() const {
	if (_duration <= 0)
		return 1.0;

	// Compute progress as % of duration.
	float prog = elapsed() / _duration;
	prog = constrain(prog, 0, 1);
	return prog;
}

}
