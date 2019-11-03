/*
 * Metro.cpp
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

#include "Metro.h"

namespace pq {

Metro::Metro(float period_) {
  period(period_);
}

void Metro::begin() {
	_phaseTime = _phase;
}

void Metro::step() {
	// Adjust phase time.
	_phaseTime += 1.0f / (_period * sampleRate());
	// Check if we went over to swtich to "on".
	bool isOn = (_phaseTime > 1);
	if (isOn)
		while (_phaseTime > 1) _phaseTime--; // modulo

	// Register difference between previous and new state.
	_changeState = (int8_t)isOn - (int8_t)_onValue;

	// Register new value.
	_onValue = isOn;
}

Metro& Metro::period(float period) {
	_period = max(period, FLT_MIN);
	return *this;
}

Metro& Metro::frequency(float frequency) {
	return period( frequency == 0 ? FLT_MAX : 1/frequency );
}

Metro& Metro::phase(float phase) {
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
