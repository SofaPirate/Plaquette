/*
 * AbstractTimer.h
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

#ifndef ABSTRACT_TIMER_H_
#define ABSTRACT_TIMER_H_

#include "PqCore.h"
#include "Chronometer.h"

namespace pq {

class AbstractTimer : public AbstractChronometer {
public:
  typedef ParameterSlot<AbstractTimer> Parameter;

protected:
  AbstractTimer(float duration);
  virtual ~AbstractTimer() {}

public:
  /// Starts/restarts the chronometer.
  virtual void start();

  /// Starts/restarts the chronometer with specific duration.
  virtual void start(float duration);

  /// Sets the duration of the chronometer.
  virtual void duration(float duration);

  /// Returns duration.
  virtual float duration() const { return _duration; }

  /// Returns duration as a parameter.
  virtual Parameter duration() { return Parameter(this, &AbstractTimer::duration, &AbstractTimer::duration); }

  /// The progress of the timer process (in %).
  virtual float progress() const;

  /// Returns true iff the chronometer has finished its process.
  virtual bool isFinished() const { return _elapsedTime >= _duration; }

  /// @deprecated
  [[deprecated("Use isFinished() instead.")]]
  virtual bool isComplete() const { return isFinished(); }

protected:
  // The duration.
  float _duration;

#if PQ_OPTIMIZE_FOR_CPU
  // Precomputed 1/duration.
  float _invDuration;
#endif
};

}

#endif
