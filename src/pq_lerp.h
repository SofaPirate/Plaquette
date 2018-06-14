/*
 * pq_lerp.h
 *
 * Equivalent of Processing lerp() method.
 *
 * (c) 2017 Sofian Audry        :: info(@)sofianaudry(.)com
 * (c) 2017 Thomas O Fredericks :: tof(@)t-o-f(.)info
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

#ifndef PQ_LERP_H_
#define PQ_LERP_H_

namespace pq {

/**
 * Calculates a number between two numbers at a specific increment.
 * @param from the starting value
 * @param to the end value
 * @param amount the amount to interpolate between the two values in [0, 1]
 * @return the linear interpolation of the two numbers
 */
float lerp(float from, float to, float amount);

} // namespace pq

#endif
