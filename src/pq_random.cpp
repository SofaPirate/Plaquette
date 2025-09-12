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
#include "pq_fastmath.h"
#include "pq_random32.h"

namespace pq {

#ifndef RANDOM_MAX
#define	RANDOM_MAX 0x7FFFFFFF
#endif

float randomFloat() {
  return (float)random32() / (float)RANDOM_32_MAX;
}

float randomFloat(float max) {
  return (randomFloat() * max);
}

float randomFloat(float min, float max) {
  return (randomFloat() * (max - min) + min);
}

// Use 5th-order series on a tight interval for speed/precision.
// expm1(-x) ≈ -(x - x^2/2 + x^3/6 - x^4/24 + x^5/120)
// => 1 - exp(-x) = -expm1(-x) ≈ x - x^2/2 + x^3/6 - x^4/24 + x^5/120
float oneMinusExpNegative5thOrder(float t) {
  const float t2 = t * t;
  const float t3 = t2 * t;
  const float t4 = t2 * t2;
  const float t5 = t4 * t;
  return t - 0.5f*t2 + (1.0f/6.0f)*t3 - (1.0f/24.0f)*t4 + (1.0f/120.0f)*t5;
}

// Fast & accurate 1 - exp(-x) for x >= 0, AVR-friendly, no exp/expm1.
// Strategy:
//  - If x <= 1/8, use a 5th-order series (accurate, few ops).
//  - Else (rare in your use), halve once, evaluate, then "double back":
//      expm1(2a) = 2*expm1(a) + [expm1(a)]^2
float oneMinusExpNegative(float x) {
    if (x <= 0.0f) return 0.0f;

    // Very small: linear is optimal (avoid rounding loss).
    if (x < 1e-6f) return x;

    if (x <= 0.125f) {
        return oneMinusExpNegative5thOrder(x);
    }
    else {
        // Single halving keeps accuracy without a loop.
        const float a  = 0.5f * x;
        const float y  = oneMinusExpNegative5thOrder(a); // y ~ 1 - exp(-a)
        // Reconstruct: 1 - exp(-2a) = 1 - (1 - y)^2 = 2y - y^2
        return (2.0f * y) - (y * y);
    }
}

bool randomTrigger(float timeWindow, float samplePeriod) {
  if (timeWindow <= 0.0f || samplePeriod <= 0.0f)
    return false;

  // Compute probability under Poisson distribution.
  const float probability = oneMinusExpNegative(samplePeriod / timeWindow);
//  const float probability = - expm1(- samplePeriod / timeWindow);

  // Generate a random number and compare it to the probability.
  return (rand() <= (probability*RAND_MAX));
}

float randomUniform() { return randomFloat(); }
float randomUniform(float max) { return randomFloat(max); }
float randomUniform(float min, float max) { return randomFloat(min, max); }

} // namespace pq
