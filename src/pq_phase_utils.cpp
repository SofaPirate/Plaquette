/*
 * pq_phase_utils.cpp
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
#include "pq_phase_utils.h"
#include "pq_time.h"

namespace pq {

/// Returns phase time value with offset.
q0_32u_t phase32AddPhase(q0_32u_t phase32, float phase) {
  return phase32 + floatToPhase32(phase);
}

/// Returns phase time value with offset.
q0_32u_t phase32AddTime(q0_32u_t phase32, float period, float time) {
  return phase32AddPhase(phase32, timeToPhase(period, time));
}

// Helper function.
bool _phase32Update(q0_32u_t& phase32, q0_32u_t increment, bool forward) {
  // Forward case.
  if (forward) {
    // Check if increment will overflow.
    bool overflow = (increment > FIXED_MAX_32 - phase32);
    // Add increment (will overflow when reaching max).
    phase32 += increment;
    return overflow;
  }

  // Backwards case.
  else {
    // Check if increment will underflow.
    bool underflow = (increment > phase32);
    // Add increment (will overflow when reaching max).
    phase32 -= increment;
    return underflow;
  }
}

bool phase32UpdateFixed32(q0_32u_t& phase32, float frequency, float deltaTimeSecondsTimesFixed32Max, bool forward) {
  return _phase32Update(phase32, round(frequency * deltaTimeSecondsTimesFixed32Max), forward);
}


/// Computes new phase time for oscillators and returns when phase time overflows or underflows.
bool phase32Update(q0_32u_t& phase32, float period, float sampleRate, bool forward) {
  // Premultiply period.
  period *= sampleRate;

  return (period == 0) ? true : _phase32Update(phase32, floatToPhase32(1.0f / period), forward);
}

}
