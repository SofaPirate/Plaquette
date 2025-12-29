/*
 * AbstractOscillator.h
 *
 * (c) 2025 Sofian Audry        :: info(@)sofianaudry(.)com
 * (c) 2025 Thomas O Fredericks :: tof(@)t-o-f(.)info
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

#ifndef ABSTRACT_OSCILLATOR_H_
#define ABSTRACT_OSCILLATOR_H_

#include "PqCore.h"
#include "Timeable.h"
#include "pq_phase_utils.h"

namespace pq {

/**
 * Triangle/sawtooth oscillator.
 */
class AbstractOscillator : public Timeable {
public:
  typedef ParameterSlot<AbstractOscillator> Parameter;

  /**
   * Constructor.
   * @param period the period of oscillation (in seconds)
   */
  AbstractOscillator(float period);

  virtual ~AbstractOscillator() {}

  /// Starts/restarts the oscillator.
  virtual void start();

  /**
   * Sets the period (in seconds).
   * @param period the period of oscillation (in seconds)
   */
  virtual void period(float period);

  /// Returns the period (in seconds).
  virtual float period() const { return _period; }

  /// Returns the period as a parameter.
  Parameter period() { return Parameter(this, &AbstractOscillator::period, &AbstractOscillator::period); }

  /**
   * Sets the frequency (in Hz).
   * @param frequency the frequency of oscillation (in Hz)
   */
  virtual void frequency(float frequency);

  /// Returns the frequency (in Hz).
  virtual float frequency() const {
#if PQ_OPTIMIZE_FOR_CPU
    return _frequency;
#else
    return periodToFrequency(_period);
#endif
  }

  /// Returns the frequency as a parameter.
  Parameter frequency() { return Parameter(this, &AbstractOscillator::frequency, &AbstractOscillator::frequency); }

  /**
   * Sets the frequency in beats-per-minute.
   * @param bpm the frequency of oscillation (in BPM)
   */
  virtual void bpm(float bpm);

  /// Returns the frequency (in BPM).
  virtual float bpm() const { return frequency() * HZ_TO_BPM; }

  /// Returns the BPM as a parameter.
  Parameter bpm() { return Parameter(this, &AbstractOscillator::bpm, &AbstractOscillator::bpm); }

  /**
  * Sets the phase at % of period.
  * @param phase the phase (in % of period)
  */
  virtual void phase(float phase);

  /// Returns the phase (in % of period).
  virtual float phase() const { return pq::fixed32ToFloat(_phase32); }

  /// Returns the phase as a parameter.
  Parameter phase() { return Parameter(this, &AbstractOscillator::phase, &AbstractOscillator::phase); }

  /**
  * Sets the phase shift (ie. the offset, in % of period).
  *
  * @param phaseShift the phase shift (in % of period)
  * @warning This function is disabled if randomness() > 0.
  */
  virtual void phaseShift(float phaseShift);

  /**
  * Returns the phase shift (ie. the offset, in % of period).
  * @warning This function always returns 0 when randomness() > 0.
  */
  virtual float phaseShift() const;

  /// Returns the phase as a parameter.
  Parameter phaseShift() { return Parameter(this, &AbstractOscillator::phaseShift, &AbstractOscillator::phaseShift); }

  /// Sets the jittering level in [0, 1] (0: no jitter, 1: max jitter).
  virtual void jitter(float jitterLevel);

  /// Returns the randomness level in [0, 1].
  virtual float jitter() const;

  /// Disables jittering.
  virtual void noJitter() { jitter(0); }

  /// Returns the jitter as a parameter.
  Parameter jitter() { return Parameter(this, &AbstractOscillator::jitter, &AbstractOscillator::jitter); }

  /**
   * Returns the period actually used for the current cycle.
   *
   * When jitter() == 0, this is identical to period(). When jitter() > 0,
   * this returns the stochastic (jittered) period currently in effect.
   */
  virtual float jitteredPeriod() const;

  /**
   * Returns the frequency actually used for the current cycle.
   *
   * When jitter() == 0, this is identical to frequency(). When jitter() > 0,
   * this returns the stochastic (jittered) frequency currently in effect.
   */
  virtual float jitteredFrequency() const;

   /**
    * Utility function to convert time to phase.
    * @param time relative time in seconds
    * @return the equivalent phase
    */
  virtual float timeToPhase(float time) const;

  /**
   * Forces current time (in seconds).
   * @warning This function is disabled if randomness() > 0.
   */
  virtual void setTime(float time);

  /**
   * Adds time to current time (in seconds).
   * @warning This function is disabled if randomness() > 0.
   */
  virtual void addTime(float time);

  /// Returns true iff the wave is currently running.
  virtual bool isRunning() const { return _isRunning; }

  /// Returns true iff the wave is moving forward in time.
  virtual bool isForward() const { return _isForward; }

  /**
   * Sets the direction of oscillation.
   * @param isForward true iff the wave is moving forward in time
   */
  virtual void setForward(bool isForward) { _isForward = isForward; }

  /// Sets the direction of oscillation to move forward in time.
  virtual void forward() { _isForward = true; }

  /// Sets the direction of oscillation to move backward in time.
  virtual void reverse() { _isForward = false; }

  /// Toggles the direction of oscillation.
  virtual void toggleReverse() { _isForward = !_isForward; }

protected:
  // Perform step with under
  void _stepPhase(float deltaTimeSecondsTimesFixed32Max);

  // Sets phase time.
  virtual void _setPhase32(q0_32u_t phase32);

  // Sets running state.
  virtual void _setRunning(bool isRunning);

  // Picks next random frequency multiplier (for randomized oscillation).
  void _randomPickNext();

  // Period (seconds).
  float _period;

#if PQ_OPTIMIZE_FOR_CPU
  // Frequency (Hz).
  float _frequency;
#endif

  // Non-random mode: Phase shift (in % of period).
  // Random mode: random frequency ratio (in % of frequency).
  float _phaseShiftOrRandomFrequencyRatio;

  // Internal use: holds current phase time.
  q0_32u_t _phase32;

  // Internal use : overflow indicator
  bool _overflowed : 1;

  // Is the wave currently running?
  bool _isRunning : 1;

  // The direction of oscillation.
  bool _isForward : 1;

  // Flag that makes sure the value is updated only on a need basis.
  bool _valueNeedsUpdate : 1;

  // Jittering level.
  uint8_t _jitterLevel : 4;
};

}

#endif
