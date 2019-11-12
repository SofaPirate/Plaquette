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
#include "Chrono.h"

namespace pq {

class AbstractTimer : public Chrono {
protected:
  AbstractTimer(float duration=0.0f);

public:
  /// Starts/restarts the chronometer.
  virtual void start();

  /// Starts/restarts the chronometer with specific duration.
  virtual void start(float duration);

  /// Sets the duration of the chronometer.
  virtual void duration(float duration);

  /// Returns duration.
  virtual float duration() const { return _duration; }

  /// The progress of the chrono process (in %).
  virtual float progress() const;

  /// Returns true iff the chronometer has completed its process.
  bool isComplete() const { return progress() >= 1.0; }

protected:
  // The duration.
  float _duration;
};

}

#endif
