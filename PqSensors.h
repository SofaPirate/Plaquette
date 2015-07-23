/*
 * PqSensors.h
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

#ifndef PQ_SENSORS_H_
#define PQ_SENSORS_H_

#include "PqCore.h"

// Output constants.
#define INTERNAL_PULLUP   0x0
#define EXTERNAL_PULLUP   0x1
#define EXTERNAL_PULLDOWN 0x2
#define ANALOG_DEFAULT    0x4
#define ANALOG_INVERTED   0x8

/// A generic class representing a simple digital/analog output.
class PqSingleSensor : public PqComponent {
public:
  /// Constructor.
  PqSingleSensor(uint8_t pin, uint8_t mode);
  virtual ~PqSingleSensor() {}
  
  /// Returns the pin this sensor is attached to.
  uint8_t pin() const { return _pin; }

  /// Returns the mode of the sensor (either SOURCE or SINK).
  uint8_t mode() const { return _mode; }


  bool isAnalog() const { return (_mode & (ANALOG_DEFAULT | ANALOG_INVERTED)); }
  bool isDigital() const { return !isAnalog(); }

  bool isInverted() const { return (_mode & (INTERNAL_PULLUP | EXTERNAL_PULLUP | ANALOG_INVERTED)); }

  bool isPullUp() const { return (_mode != EXTERNAL_PULLDOWN); }

  bool isExternal() const { return (_mode != INTERNAL_PULLUP); }

  /// Returns true iff the sensor is "ON".
  virtual bool isOn() const;
  virtual bool isOff() const { return !isOn(); }

  /// Returns sensor reading as a value between 0 and 1.
  virtual float get() const;

  /**
   * Returns the raw sensor reading. If this is a digital input the value will either be 0 or 1, whereas
   * for analog sensors it will be between 0 and 1023.
   */
  virtual int getRaw() const;

protected:
  /// Implementation of the PqComponent::setup() method.
  virtual void setup();

  // The output pin.
  uint8_t _pin;

  // The mode.
  uint8_t _mode;
};

class DigitalSensor : public PqSingleSensor {
public:
  DigitalSensor(uint8_t pin, uint8_t mode=INTERNAL_PULLUP) : PqSingleSensor(pin, mode) {}
};

class AnalogSensor : public PqSingleSensor {
public:
  AnalogSensor(uint8_t pin, uint8_t mode=ANALOG_DEFAULT) : PqSingleSensor(pin, mode) {}
};

typedef DigitalSensor Switch;
typedef AnalogSensor PhotoCell;

#endif
