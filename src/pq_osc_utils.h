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

#include <stdint.h>

#include "pq_wrap.h"

namespace pq {

#define PHASE_TIME_MAX UINT32_MAX
typedef uint32_t phase_time_t;
const phase_time_t HALF_PHASE_TIME_MAX = 0.5f * PHASE_TIME_MAX;

/// Converts floating point to phase_time_t.
inline phase_time_t float2phaseTime(float x) { return wrap01(x) * PHASE_TIME_MAX; }

/// Converts phase_time_t to floating point.
inline float phaseTime2float(phase_time_t x) { return float(x) / PHASE_TIME_MAX; }

/// Converts time in seconds to phase in %.
inline float timeToPhase(float period, float time) { return period == 0 ? 0 : time / period; }

/// Returns phase time value with offset.
phase_time_t phaseTimeAddPhase(phase_time_t phaseTime, float phase);

/// Returns phase time value with time offset.
phase_time_t phaseTimeAddTime(phase_time_t phaseTime, float period, float time);

/// Computes new phase time for oscillators and returns true when phase time overflows.
bool phaseTimeUpdate(phase_time_t& phaseTime, float period, float sampleRate);

}

#endif
