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
   */
  virtual void period(float period);
  virtual float period() const { return _period; }

  /**
   * Sets the frequency (in Hz).
   * @param frequency the frequency of oscillation (in Hz)
   */
  virtual void frequency(float frequency);

  /// Returns the frequency (in Hz).
  virtual float frequency() const { return (1/_period); }

  /**
   * Sets the frequency in beats-per-minute.
   * @param bpm the frequency of oscillation (in BPM)
   */
  virtual void bpm(float bpm);
  
  /// Returns the frequency (in BPM).
  virtual float bpm() const { return (60/_period); }

  /**
   * Sets the amplitude of the wave.
   * @param amplitude a value in [0, 1] that determines the amplitude of the wave (centered at 0.5).
   */
   virtual void amplitude(float amplitude);

   /// Returns the amplitude of the wave.
   virtual float amplitude() const { return _amplitude; }

   /**
    * Sets the phase (ie. the offset, in % of period).
    * @param phase the phase (in % of period)
    */
   virtual void phase(float phase);

   /// Returns the phase (in % of period).
   virtual float phase() const { return _phase; }

   /**
    * Utility function to convert time to phase.
    * @param t relative time in seconds
    * @return the equivalent phase
    */
  virtual float timeToPhase(float time) const;

   /**
    * Sets value with a certain phase shift (in %).
    * @param phase the phase shift (in % of period)
    * @return the value
    */
   virtual float shiftBy(float phase);

protected:
  // Core Plaquette methods.
  virtual void begin();
  virtual void step();

  // Returns value in [0, 1] (to be defined by subclasses).
  virtual float _get(phase_time_t t) = 0;

  // Returns amplified version of _get(t).
  virtual float _getAmplified(phase_time_t t);

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
