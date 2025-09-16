/*
 * pq_random32.cpp
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
#include "pq_random32.h"

namespace pq {

uint64_t pcgState = 0x853c49e6748fea9bull;
uint64_t pcgInc   = 0xda3e39cb94b95bdbull; // must be odd

uint32_t random32() {
  uint64_t old = pcgState;
  pcgState = old * 6364136223846793005ull + pcgInc;
  uint32_t xorShifted = (uint32_t)(((old >> 18u) ^ old) >> 27u);
  uint32_t rot = (uint32_t)(old >> 59u);
  return (xorShifted >> rot) | (xorShifted << ((-(int)rot) & 31));
}

void random32Seed(uint64_t s, uint64_t inc) {
  pcgState = s;
  pcgInc = (inc << 1) | 1ull; // makes sure inc is odd
}

void random32Seed(uint64_t seed) {
    // Use SplitMix64 to scramble the input into two 64-bit values
    uint64_t s = seed + 0x9e3779b97f4a7c15ull;
    s = (s ^ (s >> 30)) * 0xbf58476d1ce4e5b9ull;
    s = (s ^ (s >> 27)) * 0x94d049bb133111ebull;
    uint64_t state0 = s ^ (s >> 31);

    s = seed + 0x5851f42d4c957f2dull;
    s = (s ^ (s >> 30)) * 0xbf58476d1ce4e5b9ull;
    s = (s ^ (s >> 27)) * 0x94d049bb133111ebull;
    uint64_t inc0 = (s ^ (s >> 31)) | 1ull; // must be odd

    pcgState = state0;
    pcgInc   = inc0;
}

} // namespace pq
