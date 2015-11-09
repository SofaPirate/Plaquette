/*
 * PqInputs.h
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

#ifndef PQ_INPUTS_H
#define PQ_INPUTS_H

#include "PqCore.h"

// Output constants.
#define INTERNAL_PULLUP   0x0
#define EXTERNAL_PULLUP   0x1
#define EXTERNAL_PULLDOWN 0x2
#define ANALOG_DEFAULT    0x4
#define ANALOG_INVERTED   0x8

/// A generic class representing a simple analog input.
class AnalogIn : public PqPinComponent, PqGetter {
public:
  /// Constructor.
  AnalogIn(uint8_t pin=A0, uint8_t mode=ANALOG_DEFAULT);
  virtual ~AnalogIn() {}

  /// Returns reading in [0, 1].
  virtual float get();
};

/// A generic class representing a simple digital input.
class DigitalIn : public PqPinComponent, PqDigitalGetter {
public:
  /// Constructor.
  DigitalIn(uint8_t pin, uint8_t mode=INTERNAL_PULLUP);
  virtual ~DigitalIn() {}

  /// Returns true iff the input is "on".
  virtual bool isOn();

protected:
  virtual void setup();
};

#endif
