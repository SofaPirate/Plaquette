/*
 * SineOsc.h
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

#ifndef SINE_OSC_H_
#define SINE_OSC_H_

#include "PqCore.h"

/// Sine oscillator. Phase is expressed as % of period.
class SineOsc : public PqGetter {
public:
  /**
   * Constructor.
   * @param period the period of oscillation (in seconds)
   */
  SineOsc(float period=1.0f);
  virtual ~SineOsc() {}

  /// Returns value in [0, 1].
  virtual float get() { return _value; }

  /**
   * Sets the period (in seconds).
   * @param period the period of oscillation (in seconds)
   * @return the unit itself
   */
  virtual SineOsc& period(float period);

  /**
   * Sets the frequency (in Hz).
   * @param frequency the frequency of oscillation (in Hz)
   * @return the unit itself
   */
  virtual SineOsc& frequency(float frequency) { return period(1/frequency); }

  /**
   * Sets the phase (ie. the offset, in seconds).
   * @param phrase the phase (in seconds)
   * @return the unit itself
   */
  virtual SineOsc& phase(float phase);

protected:
  // Core Plaquette methods.
  virtual void setup();
  virtual void update();

  void _update(float t);

  // Current value of the signal.
  float _value;

  // Period (seconds).
  float _period;

  // Phase (seconds).
  float _phase;

  // Start time of each period (in seconds).
  float _startTime;
};

#endif