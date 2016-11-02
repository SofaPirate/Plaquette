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

// Output constants.
#define SOURCE 0x0
#define SINK   0x1

/// A generic class representing a simple PWM output.
class PwmOut : public PqPinComponent, public PqPutter {
public:
  /// Constructor.
  PwmOut(uint8_t pin=3, uint8_t mode=SOURCE);
  virtual ~PwmOut() {}

  /// Returns reading in [0, 1].
  virtual float put(float value);

  virtual float get() { return _value; }

  // Current value.
  float _value;

  virtual float write(float value);
};

/// A generic class representing a simple digital input.
class DigitalOut : public PqPinComponent, public PqDigitalPutter {
public:
  /// Constructor.
  DigitalOut(uint8_t pin=13, uint8_t mode=SOURCE);
  virtual ~DigitalOut() {}

  /// Returns reading in [0, 1].
  virtual float put(float value);

  virtual bool isOn() { return _isOn; }

  // Current value.
  bool _isOn;

protected:
  virtual void setup();
};

#endif
