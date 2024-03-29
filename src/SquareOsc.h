/*
 * PqExtra.h
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

#ifndef SQUARE_OSC_H_
#define SQUARE_OSC_H_

#include "Osc.h"

namespace pq {

/// Square oscillator. Duty cycle is expressed as % of period.
class SquareOsc : public Osc {
public:
  /**
   * Constructor.
   * @param period the period of oscillation (in seconds)
   * @param dutyCycle the duty-cycle as a value in [0, 1]
   */
  SquareOsc(float period=1.0f, float dutyCycle=0.5f);
  virtual ~SquareOsc() {}

  /// Returns true iff the input is "on".
  virtual bool isOn();

  /// Returns true iff the input is "off".
  virtual bool isOff() { return !isOn(); }

  /// Operator that allows usage in conditional expressions.
  virtual operator bool() { return isOn(); }

  /**
   * Sets the duty-cycle (ie. the proportion of time during which the signal is on).
   * @param dutyCycle the duty-cycle as a value in [0, 1]
   */
  virtual void dutyCycle(float dutyCycle);

  /// Returns the duty-cycle (as a value in [0, 1]).
  virtual float dutyCycle() const { return phaseTime2float(_dutyCycle); }

protected:
  // Returns value in [0, 1].
  virtual float _get(phase_time_t t);

  // Duty-cycle (in % of period).
  phase_time_t _dutyCycle;
};

}

#endif
