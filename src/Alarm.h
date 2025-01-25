/*
 * Alarm.h
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

#ifndef PQ_ALARM_H_
#define PQ_ALARM_H_

#include "PqCore.h"
#include "AbstractTimer.h"

namespace pq {

/**
 * Chronometer class which becomes "on" after a given duration.
 */
class Alarm : public DigitalSource, public AbstractTimer {
public:
  Alarm(float duration=1.0f);

  /// Returns true iff the alarm just finished its process this step.
  virtual bool finished() { return rose(); }

  /// Registers event callback on finish event.
  virtual void onFinish(EventCallback callback) { onEvent(callback, EVENT_FINISH); }

protected:
  virtual void begin();
  virtual void step();

  /// Returns true iff an event of a certain type has been triggered.
  virtual bool eventTriggered(EventType eventType) {
    switch (eventType) {
      case EVENT_FINISH: return finished();
      default:           return DigitalSource::eventTriggered(eventType);
    }
  }

  // Returns current absolute time (in seconds).
  virtual float _time() const;
};

}

#endif
