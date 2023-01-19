/*
 * pq_osc_utils.h
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
#ifndef PQ_OSC_UTILS_H
#define PQ_OSC_UTILS_H

#if (defined(ARDUINO) && ARDUINO >= 100) || defined(EPOXY_DUINO)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include "pq_wrap.h"
#include <stdint.h>

namespace pq {

#define PHASE_TIME_MAX UINT32_MAX
typedef uint32_t phase_time_t;

/// Converts floating point to phase_time_t.
inline phase_time_t float2phaseTime(float x) { return wrap(x) * PHASE_TIME_MAX; }

/// Converts phase_time_t to floating point.
inline float phaseTime2float(phase_time_t v) {
  return float(v) / PHASE_TIME_MAX;
}

/// Adds floating point value in [-1, 1] to phaseTime.
void phaseTimeAdd(phase_time_t& phaseTime, float increment);

/// Computes new phase time for oscillators and returns when phase time overflows.
bool phaseTimeUpdate(phase_time_t& phaseTime, float period, float sampleRate);

}

#endif
