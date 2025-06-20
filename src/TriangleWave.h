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
#include "pq_easing.h"

namespace pq {

/**
 * @deprecated
 * Triangle/sawtooth oscillator.
 */
[[deprecated("Use Wave(TRIANGLE) instead.")]]
class TriangleWave : public AbstractWave {
public:
  /**
   * Constructor.
   * @param engine the engine running this unit
   */
  TriangleWave(Engine& engine = Engine::primary());

  /**
   * Constructor.
   * @param period the period of oscillation (in seconds)
   * @param engine the engine running this unit
   */
  TriangleWave(float period, Engine& engine = Engine::primary());

  /**
   * Constructor.
   * @param period the period of oscillation (in seconds)
   * @param skew the duty-cycle as a value in [0, 1]
   * @param engine the engine running this unit
   */
  TriangleWave(float period, float skew, Engine& engine = Engine::primary());

  virtual ~TriangleWave() {}

  /**
   * Sets easing function to apply to wave rise.
   * @param easing the easing function
   */
  void riseEasing(easing_function easing) { _riseEasing = easing; }

  /// Remove easing function for wave rise.
  void noRiseEasing() { riseEasing(easeNone); }

    /**
   * Sets easing function to apply to wave rise.
   * @param easing the easing function
   */
  void fallEasing(easing_function easing) { _fallEasing = easing; }

  /// Remove easing function for wave rise.
  void noFallEasing() { fallEasing(easeNone); }

  /**
   * Sets easing function to apply to both rise and fall of the wave.
   * @param easing the easing function
   */
  void easing(easing_function easing) { _riseEasing = _fallEasing = easing; }

  /// Remove easing functions.
  void noEasing() { easing(easeNone); }

protected:
  // Returns value in [0, 1].
//  virtual float _get(fixed_t t);
  virtual fixed_t _getFixed(fixed_t t);


  virtual float _getAmplified(fixed_t t);

  // Optional easing to apply on the rise and fall of the wave.
  easing_function _riseEasing;
  easing_function _fallEasing;
};

/// @deprecated
[[deprecated("Use TriangleWave instead.")]]
typedef TriangleWave TriOsc;

}

#endif
