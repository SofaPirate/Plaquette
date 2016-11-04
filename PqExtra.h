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

#include "pq_print.h"
#include "PqCore.h"
#include "MovingAverage.h"
#include "MovingStats.h"
#include "SimpleStats.h"

/// Square oscillator.
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

#include "pq_map_real.h"

class OscilloscopeOut : public PqPutter {
public:
  OscilloscopeOut(float minValue=0, float maxValue=1, uint8_t precision=100);
  virtual ~OscilloscopeOut() {}

  virtual float put(float value);

  virtual float get() { return _value; }

  // Current value.
  float _value;

  float _minValue;
  float _maxValue;
  // Number of spaces of precision.
  uint8_t _precision;
};

/// Simple moving average transform filter.
class Smoother : public PqPutter, public MovingAverage {
public:
  Smoother(float factor=0.1f);
  virtual ~Smoother() {}

  virtual float put(float value);

  virtual float get() { return MovingAverage::get(); }
};

class AdaptiveNormalizer : public PqPutter, public MovingStats {
public:
  AdaptiveNormalizer(float smoothFactor=0.001f);
  virtual ~AdaptiveNormalizer() {}

  void setMean(float mean) { _mean = mean; }
  void setStddev(float stddev) { _stddev = stddev; };

  virtual float put(float value);

  virtual float get() { return _value; }

  float _value;
  float _mean;
  float _stddev;
};

class Normalizer : public PqPutter, public SimpleStats {
public:
  Normalizer();
  virtual ~Normalizer() {}

  void setMean(float mean) { _mean = mean; }
  void setStddev(float stddev) { _stddev = stddev; };

  virtual float put(float value);

  virtual float get() { return _value; }

  float _value;
  float _mean;
  float _stddev;
};

class MinMaxScaler : public PqPutter {
public:
  MinMaxScaler();
  virtual ~MinMaxScaler() {}

  virtual float put(float value);

  virtual float get() { return _value; }

  float _value;
  float _min;
  float _max;
};

#define THRESHOLD_LOW     0
#define THRESHOLD_HIGH    1
#define THRESHOLD_FALLING 2
#define THRESHOLD_RISING  3
#define THRESHOLD_CHANGE  4

class Thresholder : public PqPutter {
public:
  Thresholder(float threshold, uint8_t mode=HIGH);
  virtual ~Thresholder() {}

  virtual float put(float value);

  virtual float get() { return (float)_value; }

  float _threshold;
  int8_t _prev; // was previous variable equal (0), lower (-1), or higher (1) than threshold
  bool _value;
  uint8_t  _mode;
};

#endif
