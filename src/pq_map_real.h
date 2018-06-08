/*
 * pq_map_float.h
 *
 * Equivalent of Arduino map() method but for floats and doubles.
 *
 * (c) 2015 Sofian Audry        :: info(@)sofianaudry(.)com
 * (c) 2015 Thomas O Fredericks :: tof(@)t-o-f(.)info
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

#ifndef PQ_MAP_REAL_H_
#define PQ_MAP_REAL_H_

namespace pq {

/**
 * Re-maps a number from one range to another.
 * @param value the number to map
 * @param fromLow the lower bound of the value’s current range
 * @param fromHigh the upper bound of the value’s current range
 * @param toLow the lower bound of the value’s target range
 * @param toHigh the upper bound of the value’s target range
 * @return the mapped value
 */
float mapFloat(double value, double fromLow, double fromHigh, double toLow, double toHigh);

/**
 * Re-maps a number in range [0, 1] to a new range.
 * @param value the number to map (in [0,1])
 * @param toLow the lower bound of the value’s target range
 * @param toHigh the upper bound of the value’s target range
 * @return the mapped value in [toLow, toHigh]
 */
float mapFrom01(double value, double toLow, double toHigh);

/**
 * Re-maps a number to the [0, 1] range.
 * @param value the number to map
 * @param fromLow the lower bound of the value’s current range
 * @param fromHigh the upper bound of the value’s current range
 * @return the mapped value in [0, 1]
 */
float mapTo01(double value, double fromLow, double fromHigh);

} // namespace pq

#endif
