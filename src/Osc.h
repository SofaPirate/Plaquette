/*
 * Osc.h
 *
 * (c) 2022 Sofian Audry        :: info(@)sofianaudry(.)com
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

#ifndef OSC_H_
#define OSC_H_

#include "PqCore.h"
#include "pq_osc_utils.h"

namespace pq {

/**
 * Triangle/sawtooth oscillator.
 */
class Osc : public AnalogSource {
public:
  /**
   * Constructor.
   * @param period the period of oscillation (in seconds)
   */
  Osc(float period=1.0f);
  virtual ~Osc() {}

  /**
   * Sets the period (in seconds).
   * @param period the period of oscillation (in seconds)
   * @return the unit itself
   */
  virtual Node& period(float period);
  virtual float period() const { return _period; }

  /**
   * Sets the frequency (in Hz).
   * @param frequency the frequency of oscillation (in Hz)
   * @return the unit itself
   */
  virtual Node& frequency(float frequency);
  virtual float frequency() const { return (1/_period); }

  /**
   * Sets the amplitude of the wave.
   * @param amplitude a value in [0, 1] that determines the amplitude of the wave (centered at 0.5).
   * @return the unit itself
   */
   virtual Node& amplitude(float amplitude);
   virtual float amplitude() const { return _amplitude; }

   /**
    * Sets the phase (ie. the offset, in % of period).
    * @param phase the phase (in % of period)
    * @return the unit itself
    */
   virtual Node& phase(float phase);
   virtual float phase() const { return _phase; }

protected:
  // Core Plaquette methods.
  virtual void begin();
  virtual void step();

  virtual void _updateValue() = 0;

  // Period (seconds).
  float _period;

  // Phase (in % of period).
  float _phase;

  // Amplitude (in %).
  float _amplitude;

  // Internal use.
  phase_time_t _phaseTime;
};

}

#endif
