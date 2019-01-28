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

SineOsc::SineOsc(float period_) : _value(0.5f), _phase(0) {
  period(period_);
}

void SineOsc::setup() {
  _startTime = seconds();
  _update(0);
}

void SineOsc::update() {
	// Notice: this computation is not exact but manages naturally changes in the period without
	// inducing dephasings on Arduino boards.
	float totalTime = seconds();
	float relativeTime = totalTime - _startTime;
	_update(relativeTime);
	if (relativeTime >= _period)
  	_startTime = totalTime;
}

void SineOsc::_update(float t) {
  _value = (sin( (_phase + (t / _period)) * TWO_PI) + 1) / 2;
}

SineOsc& SineOsc::period(float period) {
	_period = max(period, 1e-6f);
	return *this;
}

SineOsc& SineOsc::phase(float phase) {
  _phase = phase;
	return *this;
}