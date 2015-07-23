/*
 * PqActuators.h
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

#ifndef PQ_ACTUATORS_H_
#define PQ_ACTUATORS_H_

#include "PqCore.h"

// Output constants.
#define SOURCE 0
#define SINK   1

/// A generic class representing a simple digital/analog output.
class PqSingleActuator : public PqComponent {
public:
  /// Constructor.
  PqSingleActuator(uint8_t pin, uint8_t mode=SOURCE);
  virtual ~PqSingleActuator() {}
  
  /// Returns the pin this actuator is attached to.
  uint8_t pin() const { return _pin; }

  /// Returns the mode of the actuator (either SOURCE or SINK).
  uint8_t mode() const { return _mode; }

  /// Returns true iff the current is inverted (ie. if the mode is SINK).
  bool isInverted() const { return (_mode == SINK); }

  /// Switches the actuator to "ON".
  virtual void on();

  /// Switches the actuator to "OFF".
  virtual void off();

  /// Sets the value between 0 and 1.
  virtual void set(float value);

  /// Raw sets the value between 0 and 255.
  virtual void setRaw(int value);

protected:
  /// Implementation of the PqComponent::setup() method.
  virtual void setup();

  // The output pin.
  uint8_t _pin;

  // The mode (SOURCE or SINK).
  uint8_t _mode;
};

class LED : public PqSingleActuator {
public:
  LED(uint8_t pin=13, uint8_t mode=SOURCE);
};


#endif
