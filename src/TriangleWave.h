/*
 * TriangleWave.h
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

#ifndef TRIANGLE_WAVE_H_
#define TRIANGLE_WAVE_H_

#include "AbstractWave.h"

namespace pq {

/**
 * Triangle/sawtooth oscillator.
 */
class TriangleWave : public AbstractWave {
public:
  /**
   * Constructor.
   * @param period the period of oscillation (in seconds)
   * @param width a value in [0, 1] that determines the point at which the wave reaches its maximum point (expressed as a fraction of the period)
   */
  TriangleWave(float period=1.0f, float width=0.5f);
  virtual ~TriangleWave() {}

protected:
  // Returns value in [0, 1].
  virtual float _get(phase_time_t t);
};

[[deprecated("Use TriangleWave instead.")]]
typedef TriangleWave TriOsc;

}

#endif
