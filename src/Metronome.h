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
#include "pq_osc_utils.h"

namespace pq {

/**
 * Chronometer digital unit which emits 1/true/"on" for one frame, at a regular pace.
 */
class Metronome : public DigitalUnit {
public:
  /**
   * Constructor.
   * @param period the period of oscillation (in seconds)
   */
  Metronome(float period=1.0f);

  /// Returns true iff the metronome fires.
  virtual bool isOn() { return _onValue; }

  /// The progress of the timer process (in %).
  virtual float progress() const { return phaseTime2float(_phaseTime); }

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
  virtual float frequency() const { return (1/_period); }

  /**
   * Sets the frequency in beats-per-minute.
   * @param bpm the frequency of oscillation (in BPM)
   */
  virtual void bpm(float bpm);

  /// Returns the frequency (in BPM).
  virtual float bpm() const { return (60/_period); }

  /**
   * Sets the phase (ie. the offset, in % of period).
   * @param phase the phase (in % of period)
   */
  virtual void phase(float phase);

  /// Returns the phase (in % of period).
  virtual float phase() const { return _phase; }

  /// Register event callback on bang event.
  virtual void onBang(EventCallback callback);

protected:
  virtual void begin();
  virtual void step();

  // Returns true if event is triggered.
  virtual bool eventTriggered(EventType eventType);

  // Phase (in % of period).
  float _period;

  // Phase (in % of period).
  float _phase;

  // Internal use.
  phase_time_t _phaseTime;

  // Value.
  bool _onValue;
};

[[deprecated("Use Metronome instead.")]]
typedef Metronome Metro;

}

#endif
