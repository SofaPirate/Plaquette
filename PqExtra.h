/*
 * PqExtra.h
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

#ifndef PQ_EXTRA_H_
#define PQ_EXTRA_H_

#include "Plaquette.h"

/// Simple output that prints value to the serial.
class SerialOut : public PqPutter {
public:
  SerialOut(uint8_t digits=4);
  virtual ~SerialOut() {}

  virtual float put(float value);

  virtual float get() { return _value; }

  // Current value.
  float _value;

  // Number of digits of precision.
  uint8_t _digits;
};

/// Simple moving average transform filter.
class Smoother : public PqPutter {
public:
  Smoother(float factor=0.1f);
  virtual ~Smoother() {}

  virtual float put(float value);

  virtual float get() { return _value; }

  // Current value.
  float _value;

  float _alpha;
};

class SquareOsc : public PqDigitalGetter {
public:
  SquareOsc(float period=1.0f, float dutyCycle=0.5f);
  virtual ~SquareOsc() {}

  virtual bool isOn() { return _isOn; }

  virtual void setup();
  virtual void update();

  bool _isOn;
  unsigned long _period;
  unsigned long _dutyCyclePeriod;
  unsigned long _startTime;
};

#endif
