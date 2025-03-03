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

/// A generic class representing a simple PWM output.
class AnalogOut : public AnalogSource, public PinUnit {
public:
  /**
   * Constructor.
   * @param pin the pin number
   * @param mode the mode (DIRECT or INVERTED)
   */
  AnalogOut(uint8_t pin, uint8_t mode=DIRECT);
  virtual ~AnalogOut() {}

  /// Pushes value into the component and returns its (possibly filtered) value.
  virtual float put(float value);

  /// Inverts value by calling ``put(1-get())`` (eg. 0.2 becomes 0.8).
  virtual void invert() { put(1-get()); }

  /// Writes raw value to the pin.
  void rawWrite(int value);

protected:
  virtual void step();
};

/// A generic class representing a simple digital output.
class DigitalOut : public DigitalSource, public PinUnit {
public:
  /**
   * Constructor.
   * @param pin the pin number
   * @param mode the mode (DIRECT or INVERTED)
   */
  DigitalOut(uint8_t pin, uint8_t mode=DIRECT);

  virtual ~DigitalOut() {}

  /// Changes the mode of the component.
  virtual void mode(uint8_t mode);
  
  /// Write HIGH or LOW value to the pin.
  void rawWrite(int value);

protected:
  virtual void begin();
  virtual void step();
};

} // namespace pq

#endif
