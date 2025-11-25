/*
 * TimeWindowable.h
 *
 * (c) 2025 Sofian Audry        :: info(@)sofianaudry(.)com
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

#ifndef PQ_TIME_WINDOWABLE_H_
#define PQ_TIME_WINDOWABLE_H_

#include "PqCore.h"

namespace pq {

/// Abstract class for time-based objects.
class TimeWindowable {
protected:
  TimeWindowable() {}

public:
  /// Sets time window to infinite.
  virtual void infiniteTimeWindow();

  /// Sets time window to no time window.
  virtual void noTimeWindow();

  /// Changes the time window (expressed in seconds).
  virtual void timeWindow(float seconds);

  /// Returns the time window (expressed in seconds).
  virtual float timeWindow() const { return _timeWindow; }

  /// Returns true if time window is infinite.
  virtual bool timeWindowIsInfinite() const { return _timeWindow == INFINITE_TIME_WINDOW; }

  /// Changes the time window cutoff frequency (expressed in Hz).
  virtual void cutoff(float hz);

  /// Returns the time window cutoff frequency (expressed in Hz).
  virtual float cutoff() const;

protected:
  float _timeWindow;
};

}

#endif
