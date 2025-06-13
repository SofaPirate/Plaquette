/*
 * Metronome.h
 *
 * (c) 2018 Sofian Audry        :: info(@)sofianaudry(.)com
 * (c) 2018 Thomas O Fredericks :: tof(@)t-o-f(.)info
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

#ifndef METRO_H_
#define METRO_H_

#include "PqCore.h"
#include "Timeable.h"
#include "pq_osc_utils.h"

namespace pq {

/**
 * Chronometer digital unit which emits 1/true/"on" for one frame, at a regular pace.
 */
class Metronome : public DigitalUnit, public Timeable {
public:
  /**
   * Constructor.
   * @param engine the engine running this unit
   */
  Metronome(Engine& engine = Engine::primary());

  /**
   * Constructor.
   * @param period the period of oscillation (in seconds)
   * @param engine the engine running this unit
   */
  Metronome(float period, Engine& engine = Engine::primary());

  /// Returns true iff the metronome fires.
  virtual bool isOn() { return _onValue; }

  /// The progress of the timer process (in %).
  virtual float progress() const { return fixedToFloat(_phaseTime); }

  /**
   * Sets the period (in seconds).
   * @param period the period of oscillation (in seconds)
   */
  virtual void period(float period);

  /// Returns the period (in seconds).
  virtual float period() const { return _period; }

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

  /**
   * Sets the frequency in beats-per-minute.
   * @param bpm the frequency of oscillation (in BPM)
   */
  virtual void bpm(float bpm);

  /// Returns the frequency (in BPM).
  virtual float bpm() const { return frequency() * HZ_TO_BPM; }

   /**
    * Sets the phase (ie. the offset, in % of period).
    * @param phase the phase (in % of period)
    */
   virtual void phase(float phase);

   /// Returns the phase (in % of period).
   virtual float phase() const { return pq::fixedToFloat(_phaseTime); }

   /**
    * Sets the phase (ie. the offset, in % of period).
    * @param phase the phase (in % of period)
    */
   virtual void phaseShift(float phaseShift);

   /// Returns the phase (in % of period).
   virtual float phaseShift() const { return _phaseShift; }

   /// Forces current time (in seconds).
  virtual void setTime(float time);

  /// Forces current time (in seconds).
  virtual void addTime(float time);

  /// Returns true iff the wave is currently running.
  virtual bool isRunning() const { return _isRunning; }

  /// Registers event callback on metronome tick ("bang") event.
  virtual void onBang(EventCallback callback);

protected:
  virtual void begin();
  virtual void step();

  // Returns true if event is triggered.
  virtual bool eventTriggered(EventType eventType);

  // Sets running state.
  virtual void _setIsRunning(bool isRunning);

  // Phase (in % of period).
  float _period;

#if PQ_OPTIMIZE_FOR_CPU
  // Frequency (Hz).
  float _frequency;
#endif

  // Phase shift (in % of period).
  float _phaseShift;

  // Internal use.
  fixed_t _phaseTime;

  // Value.
  bool _onValue : 1;

  // Is the wave currently running?
  bool _isRunning : 1;

  // Extra data.
  uint8_t data : 6;
};

/// @deprecated
[[deprecated("Use Metronome instead.")]]
typedef Metronome Metro;

}

#endif
