/*
 * PqOutputs.h
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

#ifndef PQ_OUTPUTS_H_
#define PQ_OUTPUTS_H_

#include "PqCore.h"

namespace pq {

// Output constants.
#define SOURCE 0x0
#define SINK   0x1

/// A generic class representing a simple PWM output.
class AnalogOut : public PqAnalogUnit, public PqPinUnit {
public:
  /**
   * Constructor.
   * @param pin the pin number
   * @param mode the mode (SOURCE or SINK)
   */
  AnalogOut(uint8_t pin=9, uint8_t mode=SOURCE);
  virtual ~AnalogOut() {}

  /// Pushes value into the component and returns its (possibly filtered) value.
  virtual float put(float value);

  /// Inverts value by calling ``put(1-get())`` (eg. 0.2 becomes 0.8).
  virtual void invert() { put(1-get()); }
};

/// A generic class representing a simple digital output.
class DigitalOut : public PqDigitalUnit, public PqPinUnit {
public:
  /**
   * Constructor.
   * @param pin the pin number
   * @param mode the mode (SOURCE or SINK)
   */
#ifdef LED_BUILTIN
  DigitalOut(uint8_t pin=LED_BUILTIN, uint8_t mode=SOURCE);
#else
  DigitalOut(uint8_t pin=13, uint8_t mode=SOURCE);
#endif

  virtual ~DigitalOut() {}

  /**
   * Pushes value into the unit.
   * @param value the value sent to the unit
   * @return the new value of the unit
   */
  virtual bool putOn(bool isOn);

  /// Difference between current and previous value of the unit.
  virtual int8_t changeState() { return _changeState; }

protected:
  virtual void begin();

  // Keeps track of state changes.
  int8_t _changeState;
};

} // namespace pq

#endif
