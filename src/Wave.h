/*
 * Wave.h
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

#ifndef WAVE_H_
#define WAVE_H_

#include "AbstractWave.h"

namespace pq
{

enum WaveShape {
  SQUARE,
  TRIANGLE,
  SINE,
  N_SHAPES
};

/// Sine oscillator. Phase is expressed as % of period.
class Wave : public AbstractWave
{
public:
    /**
     * Constructor (creates default square wave).
     * @param period the period of oscillation (in seconds)
     * @param engine the engine running this unit
     */
    Wave(float period, Engine &engine = Engine::primary());

    /**
     * Constructor. Defaults to period of 1 second.
     * @param shape the wave shape
     * @param engine the engine running this unit
     */
    explicit Wave(WaveShape shape, Engine &engine = Engine::primary());

    /**
     * Constructor (creates default square wave).
     * @param period the period of oscillation (in seconds)
     * @param skew the duty-cycle as a value in [0, 1]
     * @param engine the engine running this unit
     */
    Wave(float period, float skew, Engine &engine = Engine::primary());

    /**
     * Constructor.
     * @param shape the wave shape
     * @param period the period of oscillation (in seconds)
     * @param engine the engine running this unit
     */
    Wave(WaveShape shape, float period, Engine &engine = Engine::primary());

    /**
     * Constructor.
     * @param shape the wave shape
     * @param period the period of oscillation (in seconds)
     * @param skew the duty-cycle as a value in [0, 1]
     * @param engine the engine running this unit
     */
    Wave(WaveShape shape, float period, float skew, Engine &engine = Engine::primary());

    virtual ~Wave() {}

    /**
     * Sets wave shape.
     * @param shape the wave shape (SQUARE, TRIANGLE, SINE)
     */
    void shape(WaveShape shape);

    /// Returns current wave shape
    WaveShape shape() const { return _shape; }

protected:
    // Returns value in [0, 1].
    //  virtual float _get(q0_32u_t t);
    virtual q0_32u_t _getFixed32(q0_32u_t t) const;

private:
    WaveShape _shape;
};

}

#endif
