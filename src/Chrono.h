/*
 * Chrono.h
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

#ifndef PQ_CHRONO_H_
#define PQ_CHRONO_H_

#include "PqCore.h"

namespace pq {

class Chrono : public PqUnit {
public:
  /// Constructor.
  Chrono();

  /// Starts/restarts the chronometer.
  virtual void start();

  /// Adds/subtracts time to the chronometer.
  virtual void addTime(float time);

  /// Interrupts the chronometer.
  virtual void stop();

  /// Resumes process.
  virtual void resume();

  /// The time currently elapsed by the chronometer (in seconds).
  virtual float elapsed() const { return _elapsedTime; }

  /// Returns true iff elapsed time has passed given timeout.
  virtual bool hasPassed(float timeout) const;

  /**
   * Returns true iff elapsed time has passed given timeout (optional argument to
   * automatically restart if true).
   */
  virtual bool hasPassed(float timeout, bool restartIfPassed);

  /// Returns true iff the chronometer is currently running.
  bool isStarted() const { return _isStarted; }

protected:
  virtual void begin();
  virtual void step();

  // The starting time (in seconds).
  float _startTime;

  // The offset time
  float _offsetTime;

  // The current elapsed time.
  float _elapsedTime;

  // Is the chrono currently started.
  bool _isStarted;
};

}

#endif
