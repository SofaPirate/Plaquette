/*
 * AbstractChronometer.h
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

#ifndef PQ_ABSTRACT_CHRONOMETER_H_
#define PQ_ABSTRACT_CHRONOMETER_H_

#include "PqCore.h"

namespace pq {

class AbstractChronometer {
public:
  /// Constructor.
  AbstractChronometer();
  virtual ~AbstractChronometer() {}

  /// Starts/restarts the chronometer.
  virtual void start();

  /// Interrupts the chronometer and resets to zero.
  virtual void stop();

  /// Interrupts the chronometer.
  virtual void pause();

  /// Resumes process.
  virtual void resume();

  /// Toggles pause/unpause.
  virtual void togglePause();

  /// The time currently elapsed by the chronometer (in seconds).
  virtual float elapsed() const { return _elapsedTime; }

  /// Returns true iff elapsed time has passed given timeout.
  virtual bool hasPassed(float timeout) const;

  /// Forces current time (in seconds).
  virtual void set(float time);

  /**
   * Returns true iff elapsed time has passed given timeout (optional argument to
   * automatically restart if true).
   */
  [[deprecated("Use hasPassed(float) followed by start() instead.")]]
  virtual bool hasPassed(float timeout, bool restartIfPassed);

  /// Adds/subtracts time to the chronometer.
  virtual void add(float time);
  
  [[deprecated("Use add(float) instead.")]]
  virtual void addTime(float time) { add(time); }

  /// Returns true iff the chronometer is currently running.
  bool isRunning() const { return _isRunning; }

  [[deprecated("Use isRunning() instead.")]]
  bool isStarted() const { return isRunning(); }

protected:
  virtual void update();

  // Returns current absolute time (in seconds).
  virtual float clock() const = 0;
  
  // The starting time (in seconds).
  float _startTime;

  // The offset time
  float _offsetTime;

  // The current elapsed time.
  float _elapsedTime;

  // Is the chrono currently started.
  bool _isRunning;
};

}

#endif
