/*
 * pq_osc_utils.cpp
 *
 * (c) 2022 Sofian Audry        :: info(@)sofianaudry(.)com
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
#include "pq_osc_utils.h"

namespace pq {

void phaseTimeAdd(phase_time_t& phaseTime, float increment) {
  if (increment >= 0)
    phaseTime += float2phaseTime(increment);
  else
    phaseTime -= float2phaseTime(-increment);
}

/// Computes new phase time for oscillators and returns when phase time overflows.
bool phaseTimeUpdate(phase_time_t& phaseTime, float period, float sampleRate) {
  // Premultiply period.
  period *= sampleRate;

  // Extreme case: infinite increment.
  if (period == 0)
    return true;

  // Normal case.
  else {
    // Increment to add to phaseTime.
    phase_time_t increment = float2phaseTime(1.0f / period);
    // Check if increment will overflow.
    bool overflow = (increment > PHASE_TIME_MAX - phaseTime);
    // Add increment (will overflow when reaching max).
    phaseTime += increment;
    return overflow;
  }
}

}
