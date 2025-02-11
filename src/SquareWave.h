/*
 * SquareWave.h
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

#ifndef SQUARE_WAVE_H_
#define SQUARE_WAVE_H_

#include "AbstractWave.h"

namespace pq {

/// Square oscillator. Duty cycle is expressed as % of period.
class SquareWave : public AbstractWave {
public:
  /**
   * Constructor.
   * @param period the period of oscillation (in seconds)
   * @param width the duty-cycle as a value in [0, 1]
   */
  SquareWave(float period=1.0f, float width=0.5f);
  virtual ~SquareWave() {}

  /// Returns true iff the input is "on".
  virtual bool isOn();

  /// Returns true iff the input is "off".
  virtual bool isOff() { return !isOn(); }

  /// Operator that allows usage in conditional expressions.
  explicit operator bool() { return isOn(); }

  [[deprecated("Use width(float) instead.")]]
  virtual void dutyCycle(float dutyCycle) { width(dutyCycle); }

  [[deprecated("Use width() instead.")]]
  virtual float dutyCycle() const { return width(); }
  
protected:
  // Returns value in [0, 1].
//  virtual float _get(fixed_t t);
  virtual fixed_t _getFixed(fixed_t t);
};

[[deprecated("Use SquareWave instead.")]]
typedef SquareWave SquareOsc;

}

#endif
