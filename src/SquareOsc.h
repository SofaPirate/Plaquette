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

namespace pq {

/// Square oscillator. Duty cycle is expressed as % of period.
class SquareOsc : public PqAnalogSource {
public:
  /**
   * Constructor.
   * @param period the period of oscillation (in seconds)
   * @param dutyCycle the duty-cycle as a value in [0, 1]
   */
  SquareOsc(float period=1.0f, float dutyCycle=0.5f);
  virtual ~SquareOsc() {}

  /**
   * Sets the period (in seconds).
   * @param period the period of oscillation (in seconds)
   * @return the unit itself
   */
  virtual SquareOsc& period(float period);
  virtual float period() const { return _period; }

  /**
   * Sets the frequency (in Hz).
   * @param frequency the frequency of oscillation (in Hz)
   * @return the unit itself
   */
  virtual SquareOsc& frequency(float frequency);
  virtual float frequency() const { return (1/_period); }

  /**
   * Sets the duty-cycle (ie. the proportion of time during which the signal is on).
   * @param dutyCycle the duty-cycle as a value in [0, 1]
   * @return the unit itself
   */
  virtual SquareOsc& dutyCycle(float dutyCycle);
  virtual float dutyCycle() const { return _dutyCycle; }

  /**
   * Sets the phase (ie. the offset, in % of period).
   * @param phase the phase (in % of period)
   * @return the unit itself
   */
  virtual SquareOsc& phase(float phase);
  virtual float phase() const { return _phase; }

  /**
   * Sets the amplitude of the wave.
   * @param amplitude a value in [0, 1] that determines the amplitude of the wave (centered at 0.5).
   * @return the unit itself
   */
   virtual SquareOsc& amplitude(float amplitude);
   virtual float amplitude() const { return _amplitude * 2; }

protected:
  // Core Plaquette methods.
  virtual void begin();
  virtual void step();

  void _updateValue();

  // Period (seconds).
  float _period;

  // Duty-cycle (in % of period).
  float _dutyCycle;

  // Phase (in % of period).
  float _phase;

  // Amplitude (in %).
  float _amplitude;

  // Internal use.
  float _phaseTime;
};

}

#endif
