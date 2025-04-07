/*
 * AbstractWave.h
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

#ifndef ABSTRACT_WAVE_H_
#define ABSTRACT_WAVE_H_

#include "PqCore.h"
#include "pq_osc_utils.h"

namespace pq {

/**
 * Triangle/sawtooth oscillator.
 */
class AbstractWave : public AnalogSource {
public:
  /**
   * Constructor.
   * @param engine the engine running this unit
   */
  AbstractWave(Engine& engine = Engine::primary());

  /**
   * Constructor.
   * @param period the period of oscillation (in seconds)
   * @param engine the engine running this unit
   */
  AbstractWave(float period, Engine& engine = Engine::primary());

  /**
   * Constructor.
   * @param period the period of oscillation (in seconds)
   * @param width the duty-cycle as a value in [0, 1]
   * @param engine the engine running this unit
   */
  AbstractWave(float period, float width, Engine& engine = Engine::primary());

  virtual ~AbstractWave() {}

  /**
   * Sets the period (in seconds).
   * @param period the period of oscillation (in seconds)
   */
  virtual void period(float period);
  virtual float period() const { return abs(_period); }

  /**
   * Sets the frequency (in Hz).
   * @param frequency the frequency of oscillation (in Hz)
   */
  virtual void frequency(float frequency);

  /// Returns the frequency (in Hz).
  virtual float frequency() const { return (_period == 0 ? FLT_MAX : 1.0f/_period); }

  /**
   * Sets the frequency in beats-per-minute.
   * @param bpm the frequency of oscillation (in BPM)
   */
  virtual void bpm(float bpm);
  
  /// Returns the frequency (in BPM).
  virtual float bpm() const { return (_period == 0 ? FLT_MAX : 60.0f/_period); }

  /**
   * Sets the width of the signal as a % of period.
   * @param width the width as a value in [0, 1]
   */
  virtual void width(float width);

  /// Returns the width of the signal.
  virtual float width() const { return fixedToFloat(_width); }

  /**
   * Sets the amplitude of the wave.
   * @param amplitude a value in [0, 1] that determines the amplitude of the wave (centered at 0.5).
   */
   virtual void amplitude(float amplitude);

   /// Returns the amplitude of the wave.
   virtual float amplitude() const { return fixedToFloat(_amplitude); }

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
    * Returns oscillator's value with given phase shift (in %). Supports negative phase shifts.
    * Eg. shiftBy(0.2) returns future value of oscillator after 20% of its period would have passed.
    * @param phase the phase shift (in % of period)
    * @return the value of oscillator with given phase shift
    */
  virtual float shiftBy(float phaseShift);

  /// Starts/restarts the wave.
  virtual void start();

  /// Interrupts the wave and resets to beginning.
  virtual void stop();

  /// Interrupts the wave.
  virtual void pause();

  /// Resumes process.
  virtual void resume();

  /// Toggles pause/unpause.
  virtual void togglePause();

  /// Forces current time (in seconds).
  virtual void setTime(float time);

  /// Sets the direction of oscillation to move forward in time.
  virtual void forward();

  /// Sets the direction of oscillation to move backward in time.
  virtual void reverse();

  /// Toggles the direction of oscillation.
  virtual void toggleReverse();

  /// Returns true iff the wave is moving forward in time.
  virtual bool isForward() const { return _isForward; }

  /// Returns true iff the wave is currently running.
  bool isRunning() const { return _isRunning; }

protected:
  // Core Plaquette methods.
  virtual void begin();
  virtual void step();

  // Returns value in [0, 1] as fixed-point value (to be defined by subclasses).
  virtual fixed_t _getFixed(fixed_t t) = 0;

  // Returns amplified version of _get(t).
  virtual float _getAmplified(fixed_t t);

  // Period (seconds). Negative period indicates time reversal (going backwards).
  float _period;

  // Phase (in % of period).
  float _phase;

  // Amplitude (in %).
//  float _amplitude;
  uint32_t _amplitude;

  // Width of the signal.
  fixed_t _width;

  // Internal use: holds current phase time.
  fixed_t _phaseTime;

  // Is the wave currently running?

  // The value contained in the unit.
  bool _isRunning : 1;

  // The direction of oscillation.
  bool _isForward : 1;

  // These variables are only used by the AbstractWave.
  bool _onValue : 1;

  // Previous value, used to compute change state.
  bool _prevOnValue : 1; 

  // The change state contained in the unit.
  int8_t _changeState : 2;

  // Unused extra space.
  uint8_t _data : 2;
};

}

#endif
