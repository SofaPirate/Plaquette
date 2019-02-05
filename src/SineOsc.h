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

namespace pq {

/// Sine oscillator. Phase is expressed as % of period.
class SineOsc : public PqAnalogSource {
public:
  /**
   * Constructor.
   * @param period the period of oscillation (in seconds)
   */
  SineOsc(float period=1.0f);
  virtual ~SineOsc() {}

  /**
   * Sets the period (in seconds).
   * @param period the period of oscillation (in seconds)
   * @return the unit itself
   */
  virtual SineOsc& period(float period);
  virtual float period() const { return _period; }

  /**
   * Sets the frequency (in Hz).
   * @param frequency the frequency of oscillation (in Hz)
   * @return the unit itself
   */
  virtual SineOsc& frequency(float frequency);
  virtual float frequency() const { return (1/_period); }

  /**
   * Sets the phase (ie. the offset, in % of period).
   * @param phase the phase (in % of period)
   * @return the unit itself
   */
  virtual SineOsc& phase(float phase);
  virtual float phase() const { return _phase; }

  /**
   * Sets the amplitude of the wave.
   * @param amplitude a value in [0, 1] that determines the amplitude of the wave (centered at 0.5).
   * @return the unit itself
   */
   virtual SineOsc& amplitude(float amplitude);
   virtual float amplitude() const { return _amplitude * (-32767.0f); }

protected:
  // Core Plaquette methods.
  virtual void begin();
  virtual void step();

  void _updateValue();

  // Period (seconds).
  float _period;

  // Phase (in % of period).
  float _phase;

  // Amplitude (in %).
  float _amplitude;

  // Internal use.
  float _phaseTime;
};

}

#endif
