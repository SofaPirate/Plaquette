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

#ifndef PQ_INPUTS_H_
#define PQ_INPUTS_H_

#include "PqCore.h"
#include "MovingAverage.h"

// Output constants.
#define INTERNAL_PULLUP   0x0
#define EXTERNAL_PULLUP   0x1
#define EXTERNAL_PULLDOWN 0x2
#define ANALOG_DEFAULT    0x4
#define ANALOG_INVERTED   0x8

/// Superclass for components that can be smoothed.
class PqSmoothable {
public:
  PqSmoothable(float smoothWindow=PLAQUETTE_NO_SMOOTH_WINDOW);

  /// Apply smoothing to object.
  virtual void smooth(float smoothWindow=PLAQUETTE_DEFAULT_SMOOTH_WINDOW);

  /// Remove smoothing.
  virtual void noSmooth() { smooth(PLAQUETTE_NO_SMOOTH_WINDOW); }

  /// Returns current smoothing window.
  float window() const { return _avg.window(); }

protected:
  // Raw read function.
  virtual float _read() = 0;

	// Returns smoothed value.
  virtual float _smoothed();

  MovingAverage _avg;
};

/// A generic class representing a simple analog input.
class AnalogIn : public PqPinUnit, public PqSmoothable, public PqGetter {
public:
  /**
   * Constructor.
   * @param pin the pin number
   * @param mode the mode (ANALOG_DEFAULT or ANALOG_INVERTED)
   */
  AnalogIn(uint8_t pin=A0, uint8_t mode=ANALOG_DEFAULT);
  virtual ~AnalogIn() {}

  /// Returns reading in [0, 1].
  virtual float get() { return _value; }

protected:
  virtual float _read();

  virtual void begin();
  virtual void step();

	float _value;
};

/// A generic class representing a simple digital input.
class DigitalIn : public PqPinUnit, public PqSmoothable, public PqDigitalGetter {
public:
  /**
   * Constructor.
   * @param pin the pin number
   * @param mode the mode (INTERNAL_PULLUP, EXTERNAL_PULLUP, or EXTERNAL_PULLDOWN)
   */
  DigitalIn(uint8_t pin, uint8_t mode=INTERNAL_PULLUP);
  virtual ~DigitalIn() {}

  /// Returns true iff the input is "on".
  virtual bool isOn() { return _isOn; }

protected:
  virtual float _read();

  virtual void begin();
  virtual void step();

	bool _isOn;
};

#endif
