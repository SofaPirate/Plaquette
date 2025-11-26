/*
 * Timeable.h
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

#ifndef PQ_TIMEABLE_H_
#define PQ_TIMEABLE_H_

namespace pq {

/// Abstract class for time-based objects.
class Timeable {
protected:
  Timeable() {}

public:
  /// Starts/restarts.
  virtual void start();

  /// Interrupts and resets to zero.
  virtual void stop();

  /// Interrupts process.
  virtual void pause();

  /// Resumes process.
  virtual void resume();

  /// Toggles pause/unpause.
  virtual void togglePause();

  /// Forces current time (in seconds).
  virtual void setTime(float time) = 0;

  /// Adds/subtracts time.
  virtual void addTime(float time) = 0;

  /// Returns true iff currently running.
  virtual bool isRunning() const = 0;

protected:
  // Sets running state.
  virtual void _setRunning(bool isRunning) = 0;
};

}

#endif
