/*
 * AbstractTimer.cpp
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

#include "AbstractTimer.h"
#include "pq_phase_utils.h"

namespace pq {

AbstractTimer::AbstractTimer(float duration_) : AbstractChronometer(), _duration(0)
#if PQ_OPTIMIZE_FOR_CPU
  , _invDuration(FLT_MAX)
#endif
 {
  duration(duration_);
}

void AbstractTimer::start() {
  AbstractChronometer::start();
}

void AbstractTimer::start(float duration_) {
  duration(duration_);
  start();
}

void AbstractTimer::duration(float duration) {
  // Set parameters.
  if (_duration != duration) {
    _duration = max(duration, 0.0f);
#if PQ_OPTIMIZE_FOR_CPU
    _invDuration = invert(_duration);
#endif
  }
}

float AbstractTimer::progress() const {
  if (_duration <= 0)
    return 1.0;

  // Compute progress as % of duration.
  float prog = elapsed()
#if PQ_OPTIMIZE_FOR_CPU
    * _invDuration // prevents a division
#else
    / _duration
#endif
  ;
  return constrain01(prog);
}

}
