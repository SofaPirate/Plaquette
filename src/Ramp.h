/*
 * Ramp.h
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

#ifndef RAMP_H_
#define RAMP_H_

#include "PqCore.h"

// TODO: implement a floating-point version of Chrono in Plaquette and make
// ramp a subclass of that class.
/**
 * Provides a ramping / tweening mechanism that allows smooth transitions between
 * two values.
 */
class Ramp : public PqGetter {
public:
  /**
   * Constructor.
   * @param initialValue the value the ramp starts with
   */
  Ramp(float initialValue=0.0f);
  virtual ~Ramp() {}

  /// Returns value in [0, 1].
  virtual float get() { return _value; }

  /**
   * Starts a new ramp (starting from current value).
   * @param to the final value
   * @param duration the duration of the ramp (in seconds)
   */
  virtual void start(float to, float duration);

  /**
   * Starts a new ramp.
   * @param from the initial value
   * @param to the final value
   * @param duration the duration of the ramp (in seconds)
   */
  virtual void start(float from, float to, float duration);

  /// Interrupts the ramp.
  virtual void stop();

  /// Resumes process.
  virtual void resume();

  /// The time currently elapsed by the ramp (in seconds).
  virtual float elapsed() const { return _elapsedTime; }

  /// The progress of the ramping process (in %).
  virtual float progress() const;

  /// Returns true iff the ramp is currently running.
  bool isRunning() const { return _isRunning; }

  /// Returns true iff the ramp has completed its process.
  bool isComplete() const { return progress() >= 1.0; }

protected:
  virtual void setup();
  virtual void update();

  // The current value of the ramp.
  float _value;

  // The duration.
  float _duration;

  // The starting point.
  float _from;

  // The variation from starting point targetted.
  float _change;

  // The starting time (in seconds).
  float _startTime;

  // The offset time
  float _offsetTime;

  // The current elapsed time.
  float _elapsedTime;

  // Is th ramp running or not?
  bool _isRunning;
};

#endif