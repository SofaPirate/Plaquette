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


SquareOsc::SquareOsc(float period_, float dutyCycle_) {
  period(period_);
  dutyCycle(dutyCycle_);
}

void SquareOsc::setup() {
	_startTime = seconds();
}

void SquareOsc::update() {
	// Notice: this computation is not exact but manages naturally changes in the period without
	// inducing dephasings on Arduino boards.
	float totalTime = seconds();
	float progress = (totalTime - _startTime) / _period;
	_isOn = (progress < _dutyCycle);
	if (progress >= 1) // reset
  	_startTime = totalTime;
}

SquareOsc& SquareOsc::period(float period) {
	_period = max(period, 1e-6f);
	return *this;
}

SquareOsc& SquareOsc::dutyCycle(float dutyCycle) {
  _dutyCycle = constrain(dutyCycle, 0, 1);
	return *this;
}
