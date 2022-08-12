/*
 * pq_wrap.h
 *
 * (c) 2019 Sofian Audry        :: info(@)sofianaudry(.)com
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

#include <Arduino.h>

//#define USE_FMODF
#ifdef USE_FMODF
#include <math.h>
#endif

// Source: https://stackoverflow.com/posts/707426/revisions
inline float wrap(float f, float low, float high) {
#ifndef USE_FMODF
  while (f >= high) f -= high;
  while (f <  low)  f += low;
  return f;
#else
  float rangeSize = high - low + 1;

  if (f < low)
    f += rangeSize * ((low - f) / rangeSize + 1);

  return low + fmodf(f - low, rangeSize);
#endif
}

inline float wrap01(float f) {
#ifndef USE_FMODF
  while (f >= 1) f--;
  while (f <  0) f++;
  return f;
#else
  float mod = fmodf(f, 1);
  return (f >= 0 ? mod : 1 + mod);
#endif
}

#endif
