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

#include "PqCore.h"

/// Square oscillator. Duty cycle is expressed as % of period.
class SquareOsc : public PqDigitalGetter {
public:
  /**
   * Constructor.
   * @param period the period of oscillation (in seconds)
   * @param dutyCycle the duty-cycle as a value in [0, 1]
   */
  SquareOsc(float period=1.0f, float dutyCycle=0.5f);
  virtual ~SquareOsc() {}

  /// Returns true iff the input is "on".
  virtual bool isOn() { return _isOn; }

  /**
   * Sets the period (in seconds).
   * @param period the period of oscillation (in seconds)
   * @return the unit itself
   */
  virtual SquareOsc& period(float period);

  /**
   * Sets the frequency (in Hz).
   * @param frequency the frequency of oscillation (in Hz)
   * @return the unit itself
   */
  virtual SquareOsc& frequency(float frequency) { return period(1/frequency); }

  /**
   * Sets the duty-cycle (ie. the proportion of time during which the signal is on).
   * @param dutyCycle the duty-cycle as a value in [0, 1]
   * @return the unit itself
   */
  virtual SquareOsc& dutyCycle(float dutyCycle);

protected:
  // Core Plaquette methods.
  virtual void setup();
  virtual void update();

  // Is the signal currently on (high) or off (low).
  bool _isOn;

  // Period (seconds).
  float _period;

  // Duty-cycle (in % of period).
  float _dutyCycle;

  // Start time of each period (in seconds).
  float _startTime;
};

#endif