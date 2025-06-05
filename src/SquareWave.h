/*
 * SquareWave.h
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

#ifndef SQUARE_WAVE_H_
#define SQUARE_WAVE_H_

#include "AbstractWave.h"

namespace pq {

/// Square oscillator. Duty cycle is expressed as % of period.
class SquareWave : public AbstractWave {
public:
  /**
   * Constructor.
   * @param engine the engine running this unit
   */
  SquareWave(Engine& engine = Engine::primary());

  /**
   * Constructor.
   * @param period the period of oscillation (in seconds)
   * @param engine the engine running this unit
   */
  SquareWave(float period, Engine& engine = Engine::primary());

  /**
   * Constructor.
   * @param period the period of oscillation (in seconds)
   * @param width the duty-cycle as a value in [0, 1]
   * @param engine the engine running this unit
   */
  SquareWave(float period, float width, Engine& engine = Engine::primary());

  virtual ~SquareWave() {}

  /// Returns true iff the input is "on".
  virtual bool isOn();

  /// Returns true iff the input is "off".
  virtual bool isOff() { return !isOn(); }

  /// Operator that allows usage in conditional expressions.
  explicit operator bool() { return isOn(); }

  /// Returns true if the value rose.
  virtual bool rose() { return changeState() > 0; }

  /// Returns true if the value fell.
  virtual bool fell() { return changeState() < 0; }

  /// Returns true if the value changed.
  virtual bool changed() { return changeState() != 0; }

  /// Difference between current and previous value of the unit.
  virtual int8_t changeState() { return _changeState; }

  /// Registers event callback on rise event.
  virtual void onRise(EventCallback callback)   { onEvent(callback, EVENT_RISE); }

  /// Registers event callback on fall event.
  virtual void onFall(EventCallback callback)   { onEvent(callback, EVENT_FALL); }

  /// Registers event callback on change event.
  virtual void onChange(EventCallback callback) { onEvent(callback, EVENT_CHANGE); }

  /**
   * Returns oscillator's on/off with given phase shift (in % of period).
   * Supports values outside [0,1], which will be wrapped accordingly.
   * @param phase the phase shift (in % of period)
   * @return the boolean value of oscillator with given phase shift
   */
  virtual bool shiftByIsOn(float phaseShift);

  /**
   * Returns the oscillator's on/off at a given absolute phase (in % of period).
   * Supports values outside [0,1], which will be wrapped accordingly.
   * @param phase the absolute phase at which to evaluate the oscillator (in % of period)
   * @return the value of the oscillator at the given phase
   */
  virtual bool atPhaseIsOn(float phase);

  [[deprecated("Use width(float) instead.")]]
  virtual void dutyCycle(float dutyCycle) { width(dutyCycle); }
 
  [[deprecated("Use width() instead.")]]
  virtual float dutyCycle() const { return width(); }
 
protected:
  // Core Plaquette methods.
  virtual void step();

  /// Returns true iff an event of a certain type has been triggered.
  virtual bool eventTriggered(EventType eventType) {
    switch (eventType) {
      case EVENT_CHANGE: return changed();
      case EVENT_RISE:   return rose();
      case EVENT_FALL:   return fell();
      default:           return AbstractWave::eventTriggered(eventType);
    }
  }
  
protected:
  // Returns value in [0, 1].
  virtual fixed_t _getFixed(fixed_t t);
};

[[deprecated("Use SquareWave instead.")]]
typedef SquareWave SquareOsc;

}

#endif
