/*
 * pq_random.cpp
 *
 * (c) 2008 Sofian Audry -- info(@)sofianaudry(.)com
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

#include "pq_random.h"

namespace pq {

float randomFloat() {
  return (float)rand() / (float) RAND_MAX;
}

float randomFloat(float max) {
  return (randomFloat() * max);
}

float randomFloat(float min, float max) {
  return (randomFloat() * (max - min) + min);
}

float randomUniform() { return randomFloat(); }
float randomUniform(float max) { return randomFloat(max); }
float randomUniform(float min, float max) { return randomFloat(min, max); }

} // namespace pq
