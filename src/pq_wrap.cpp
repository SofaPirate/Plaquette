/*
 * pq_wrap.cpp
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
#include "pq_wrap.h"

namespace pq {

float wrap01(double x) {
    return wrap(x, 1.0f);
}

float wrap(double x, double high) {
  // If high is zero, return 0.
  if (high == 0 || high == -0)
    return 0;
  
  // If high is negative, return wrapped value in [high, 0).
  else if (high < 0)
    return high + wrap(x, -high);

  // Compute wrapping.
  else {
    // Calculate modulo.
    double mod = fmodf(x, high);

    // If x is positive, fmodf behaves similarly to integer modulo (%).
    if (x >= 0)
      return mod;
    
    // If x is negative then mod will also be negative *unless* it is zero.
    else if (mod < 0)
      return (high + mod);
    
    // Always means that (mod == 0) which means that (f % high == 0).
    else 
      return 0;
  }
}

  
float wrap(double x, double low, double high) {
  return (low <= high) ? wrap(x-low, high-low) + low : wrap(x-high, low-high) + high;
}

}
