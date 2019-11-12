/*
 * Chrono.cpp
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

#include "Chrono.h"
#include "pq_time.h"

namespace pq {

Chrono::Chrono() : PqUnit() {
	begin();
}

void Chrono::start() {
	// Start.
	_startTime = seconds();
	_offsetTime = _elapsedTime = 0;
	_isStarted = true;
}

void Chrono::addTime(float time) {
	_offsetTime += time;
}

void Chrono::stop() {
	if (_isStarted) {
		_offsetTime = elapsed();
		_isStarted = false;
	}
}

void Chrono::resume() {
	if (!_isStarted) {
		_startTime = seconds();
		_isStarted = true;
	}
}

bool Chrono::hasPassed(float timeout) const
{
  return (elapsed() >= timeout);
}

bool Chrono::hasPassed(float timeout, bool restartIfPassed) {
  if (hasPassed(timeout)) {
    if (restartIfPassed)
      start();
    return true;
  }
  else {
    return false;
  }
}

void Chrono::begin() {
	// Basic reset.
	_startTime = _offsetTime = _elapsedTime = 0;
	_isStarted = false;
}

void Chrono::step() {
	// Offset elapsed time.
	_elapsedTime = _offsetTime;

	if (_isStarted) {
		// Add difference to elapsed time.
	 	_elapsedTime += (seconds() - _startTime);
	}
}

}
