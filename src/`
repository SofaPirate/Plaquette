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

/// Returns phase time value with offset.
fixed_t phaseTimeAddPhase(fixed_t phaseTime, float phase) {
  return phaseTime + floatToPhaseTime(phase);
}

/// Returns phase time value with offset.
fixed_t phaseTimeAddTime(fixed_t phaseTime, float period, float time) {
  return phaseTimeAddPhase(phaseTime, timeToPhase(period, time));
}

/// Computes new phase time for oscillators and returns when phase time overflows or underflows.
bool phaseTimeUpdate(fixed_t& phaseTime, float period, float sampleRate, bool forward) {
  // Premultiply period.
  period *= sampleRate;

  // Extreme case: infinite increment.
  if (period == 0)
    return true;
  
  // Forward case.
  else if (forward) {
    // Increment to add to phaseTime.
    fixed_t increment = floatToPhaseTime(1.0f / period);
    // Check if increment will overflow.
    bool overflow = (increment > FIXED_MAX - phaseTime);
    // Add increment (will overflow when reaching max).
    phaseTime += increment;
    return overflow;

  }

  // Backwards case.
  else {
    // Decrement to subtract from phaseTime.
    fixed_t decrement = floatToPhaseTime(1.0f / period);
    // Check if increment will underflow.
    bool underflow = (decrement > phaseTime);
    // Add increment (will overflow when reaching max).
    phaseTime -= decrement;
    return underflow;
  }
}

}
