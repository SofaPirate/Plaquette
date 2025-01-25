/*
 * Chronometer.h
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

#ifndef PQ_CHRONOMETER_H_
#define PQ_CHRONOMETER_H_

#include "AbstractChronometer.h"

namespace pq {

class Chronometer : public Unit, public AbstractChronometer {
public:
  /// Constructor.
  Chronometer();

  /// Returns elapsed time since start (in seconds).
  virtual float get() { return elapsed(); }

  /// Sets current time in seconds and returns it.
  virtual float put(float value);
  
protected:
  virtual void begin();
  virtual void step();

  // Returns current absolute time (in seconds).
  virtual float _time() const;
};

}

#endif
