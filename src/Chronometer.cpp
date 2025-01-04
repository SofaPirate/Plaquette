/*
 * Chronometer.cpp
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

#include "Chronometer.h"
#include "pq_time.h"

namespace pq {

Chronometer::Chronometer() : Unit() {
  stop();
}

void Chronometer::start() {
  // Start.
  _startTime = clock();
  set(0);
  _isRunning = true;
}


void Chronometer::pause() {
  if (_isRunning) {
    _offsetTime = elapsed(); // save current offset
    _isRunning = false;
  }
}

void Chronometer::stop() {
  // Stop.
  _startTime = 0;
  set(0);
  _isRunning = false;
}

void Chronometer::resume() {
  if (!_isRunning) {
    _startTime = clock();
    _isRunning = true;
  }
}

bool Chronometer::hasPassed(float timeout) const
{
  return (elapsed() >= timeout);
}

bool Chronometer::hasPassed(float timeout, bool restartIfPassed) {
  if (hasPassed(timeout)) {
    if (restartIfPassed)
      start();
    return true;
  }
  else {
    return false;
  }
}

void Chronometer::set(float time) {
  _elapsedTime = _offsetTime = time;
}

void Chronometer::addTime(float time) {
  _offsetTime += time;
}

void Chronometer::begin() {
  _begin();
}

void Chronometer::step() {
  // Offset elapsed time.
  _elapsedTime = _offsetTime;

  if (_isRunning) {
    // Add difference to elapsed time.
     _elapsedTime += (clock() - _startTime);
  }
}

}
