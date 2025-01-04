/*
 * AbstractChronometer.cpp
 *
 * (c) 2025 Sofian Audry        :: info(@)sofianaudry(.)com
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

#include "AbstractChronometer.h"

namespace pq {

AbstractChronometer::AbstractChronometer() {
  stop();
}

void AbstractChronometer::start() {
  // Start.
  set(0);
  _isRunning = true;
}


void AbstractChronometer::pause() {
  if (_isRunning) {
    _offsetTime = elapsed(); // save current offset
    _isRunning = false;
  }
}

void AbstractChronometer::stop() {
  // Stop.
  set(0);
  _isRunning = false;
}

void AbstractChronometer::resume() {
  if (!_isRunning) {
    _startTime = clock();
    _isRunning = true;
  }
}

bool AbstractChronometer::hasPassed(float timeout) const
{
  return (elapsed() >= timeout);
}

bool AbstractChronometer::hasPassed(float timeout, bool restartIfPassed) {
  if (hasPassed(timeout)) {
    if (restartIfPassed)
      start();
    return true;
  }
  else {
    return false;
  }
}

void AbstractChronometer::set(float time) {
  _elapsedTime = _offsetTime = time;
  _startTime = clock();
}

void AbstractChronometer::addTime(float time) {
  _offsetTime += time;
}

void AbstractChronometer::update() {
  // Offset elapsed time.
  _elapsedTime = _offsetTime;

  if (_isRunning) {
    // Add difference to elapsed time.
     _elapsedTime += (clock() - _startTime);
  }
}

}
