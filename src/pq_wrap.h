/*
 * pq_wrap.h
 *
 * (c) 2023 Sofian Audry        :: info(@)sofianaudry(.)com
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
#ifndef PQ_WRAP_H
#define PQ_WRAP_H

#if (defined(ARDUINO) && ARDUINO >= 100) || defined(EPOXY_DUINO)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#define USE_FMODF
#ifdef USE_FMODF
#include <math.h>
#endif

#include <pq_map_real.h>

namespace pq {

/// @brief Wraps value in range [0, 1).
/// @param x the value to wrap
/// @return the value wrapped around [0, 1).
float wrap(float f);

/// @brief Wraps value in range [0, high).
/// @param x the value to wrap
/// @param high the higher bound
/// @return the value wrapped around [0, high) or [high, 0) if high is negative
float wrap(float x, float high);

/// @brief Wraps value in range [low, high).
/// @param x the value to wrap
/// @param low the lower boundary
/// @param high the higher boundary
/// @return the value wrapped around [low, high) or [high, low) if high < low
float wrap(float x, float low, float high);

}

#endif
