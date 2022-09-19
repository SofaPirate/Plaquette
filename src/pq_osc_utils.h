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

namespace pq {

/// Computes new phase time for oscillators and returns when phase time crosses maxPhaseTime.
inline bool phaseTimeUpdate(float& phaseTime, float period, float sampleRate, float maxPhaseTime=1) {
  // Premultiply period.
  period *= sampleRate;

  if (period == 0)
    return true; // crossed
  else {
    phaseTime += maxPhaseTime / period;
    bool crossed = (phaseTime > maxPhaseTime);
    if (crossed)
      phaseTime = maxPhaseTime == 1 ?
                    wrap01(phaseTime) :
                    wrap(phaseTime, 0, maxPhaseTime);
    return crossed;
  }
}

}

#endif
