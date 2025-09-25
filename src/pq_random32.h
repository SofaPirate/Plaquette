/*
 * pq_random32.h
 *
 * Provides methods for generating 32-bits random numbers.
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
#ifndef RANDOM_32_H_
#define RANDOM_32_H_

#include <limits.h>
#include <stdlib.h>
#include <inttypes.h>

namespace pq {

// 32-bit random number generator max value.
constexpr uint32_t RANDOM_32_MAX = UINT32_MAX;

// 32-bit random numbers generator.
uint32_t random32();

/// Seeds the 32-bit random number generator.
void random32Seed(uint64_t seed);

/// Original way to seed the 32-bit random number generator.
void random32Seed(uint64_t s, uint64_t inc);

} // namespace pq

#endif
