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
#include "AbstractOscillator.h"
#include "pq_phase_utils.h"

namespace pq {

/**
 * Triangle/sawtooth oscillator.
 */
class AbstractWave : public AnalogSource, public AbstractOscillator {
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
   * @param skew the duty-cycle as a value in [0, 1]
   * @param engine the engine running this unit
   */
  AbstractWave(float period, float skew, Engine& engine = Engine::primary());

  virtual ~AbstractWave() {}

  /// Returns value in [0, 1].
  virtual float get();

  /**
   * Returns oscillator's value with given phase shift (in % of period).
   * Supports values outside [0,1], which will be wrapped accordingly.
   * Eg. shiftBy(0.2) returns future value of oscillator after 20% of its period would have passed.
   * @param phaseShift the phase shift (in % of period)
   * @return the value of oscillator with given phase shift
   */
  virtual float shiftBy(float phaseShift);

  /**
   * Returns oscillator's value with given phase shift expressed in time (in seconds).
   * @param timeShift the shift in time (seconds)
   * @return the value of oscillator with time shift
   */
  virtual float shiftByTime(float timeShift);

  /**
   * Returns the oscillator's value at a given absolute phase (in % of period).
   * Supports values outside [0,1], which will be wrapped accordingly.
   * Eg: atPhase(0.25) returns the oscillator value at 25% of its period.
   *
   * @param phase the absolute phase at which to evaluate the oscillator (in % of period)
   * @return the value of the oscillator at the given phase
  */
  virtual float atPhase(float phase);

  /**
   * Sets the amplitude of the wave.
   * @param amplitude a value in [0, 1] that determines the amplitude of the wave (centered at 0.5).
   */
  [[deprecated("Use wave.mapTo(min,max) instead.")]]
   virtual void amplitude(float amplitude);

   /// Returns the amplitude of the wave.
  [[deprecated("Use wave.mapTo(min,max) instead.")]]
   virtual float amplitude() const { return fixed32ToFloat(_amplitude); }

  /**
   * Sets the skew of the signal as a % of period.
   * @param skew the skew as a value in [0, 1]
   */
  virtual void skew(float skew);

  /// Returns the skew of the signal.
  virtual float skew() const { return fixed32ToFloat(_skew32); }

  /**
   * @deprecated
   * Sets the width of the signal as a % of period.
   * @param width the skew as a value in [0, 1]
   */
  [[deprecated("Use skew(float) instead.")]]
  virtual void width(float width) { skew(width); }

  /// @deprecated
  /// Returns the skew of the signal.
  [[deprecated("Use skew() instead.")]]
  virtual float width() const { return skew(); }

  /// Returns true on the step where the wave passed the period (end cycle).
  virtual bool passedPeriod() const { return _overflowed; }

  /// Returns true on the step where the wave passed the skew point.
  virtual bool passedSkew() const { return _passedSkew; }

  /// Registers event callback on wave pass period event.
  virtual void onPassPeriod(EventCallback callback);

  /// Registers event callback on wave pass skew point event.
  virtual void onPassSkew(EventCallback callback);

protected:
  // Core Plaquette methods.
  virtual void begin();
  virtual void step();

  // Returns true if event is triggered.
  virtual bool eventTriggered(EventType eventType);

  // Returns value in [0, 1] as fixed32-point value (to be defined by subclasses).
  virtual q0_32u_t _getFixed32(q0_32u_t t) = 0;

  // Returns amplified version of _get(t).
  virtual float _getAmplified(q0_32u_t t);

  bool _isPreSkew() const {
    return (_phase32 <= _skew32);
  }

  // Call after a change.
  void _updatePassedSkew();

  // Amplitude (in %).
  q0_32u_t _amplitude;

  // Skew of the signal.
  q0_32u_t _skew32;
};

}

#endif
