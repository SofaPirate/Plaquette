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
#include "pq_map_real.h"
#include "PqCore.h"
#include "MovingAverage.h"
#include "MovingStats.h"
#include "SimpleStats.h"

/// Stream/serial input. Reads float values using Arduino built-in parseFloat().
class StreamIn : public PqGetter {
public:
  StreamIn(Stream* stream=&Serial);
  virtual ~StreamIn() {}

  virtual float get() { return _value; }

  virtual void update();

  // Current value.
  float _value;

	// The stream.
	Stream* _stream;
};

/// Square oscillator. DutyCycle is in % of period.
class SquareOsc : public PqDigitalGetter {
public:
  SquareOsc(float period=1.0f, float dutyCycle=0.5f);
  virtual ~SquareOsc() {}

  virtual bool isOn() { return _isOn; }

  virtual SquareOsc& period(float period);
  virtual SquareOsc& freq(float freq) { period(1/freq); }
  virtual SquareOsc& dutyCycle(float dutyCycle);

  virtual void setup();
  virtual void update();

  bool _isOn;
  unsigned long _period;
	float _dutyCycle;
  unsigned long _startTime;
};

/// Sine oscillator. Phase is in % of period.
class SineOsc : public PqGetter {
public:
  SineOsc(float period=1.0f, float phase=0.0f);
  virtual ~SineOsc() {}

  virtual float get() { return _value; }

  virtual SineOsc& period(float period);
  virtual SineOsc& freq(float freq) { period(1/freq); }
  virtual SineOsc& phase(float phase);

  virtual void setup();
  virtual void update();

  void _update(float t);

  float _period;
  float _phase;
  unsigned long _startTime;
  float _value;
};

/**
 * Triangle/sawtooth oscillator. Parameter width between 0 and 1 determines the point
 * at which the wave reaches maximum.
 */
class TriOsc : public PqGetter {
public:
  TriOsc(float period=1.0f, float width=0.5f);
  virtual ~TriOsc() {}

  virtual float get() { return _value; }

  virtual TriOsc& period(float period);
  virtual TriOsc& freq(float freq) { period(1/freq); }
  virtual TriOsc& width(float width);

  virtual void setup();
  virtual void update();

  void _update(float t);

  unsigned long _period;
  unsigned long _width;
  unsigned long _startTime;
  float _value;
};

/// Stream/serial output. Number of digits of precision is configurable.
class StreamOut : public PqPutter {
public:
  StreamOut(uint8_t digits=4);
  StreamOut(Stream* stream, uint8_t digits=4);
  virtual ~StreamOut() {}

  virtual float put(float value);

  virtual float get() { return _value; }

  // Current value.
  float _value;

  // Number of digits of precision.
  uint8_t _digits;

	// The stream.
	Stream* _stream;
};

/// Generates a simple ASCII-based representation of a signal.
/// Precision represents the number of columns used to represent the signal.
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

/// Adaptive normalizer: normalizes values on-the-run using exponential moving
/// averages over mean and stddev.
class AdaptiveNormalizer : public PqPutter, public MovingStats {
public:
  AdaptiveNormalizer(float smoothFactor=0.001f);
  AdaptiveNormalizer(float mean, float stddev, float smoothFactor=0.001f);
  virtual ~AdaptiveNormalizer() {}

  void setMean(float mean) { _mean = mean; }
  void setStddev(float stddev) { _stddev = stddev; };

  virtual float put(float value);

  virtual float get() { return _value; }

  float _value;
  float _mean;
  float _stddev;
};

/// Standard normalizer: normalizes values on-the-run using real mean and stddev.
class Normalizer : public PqPutter, public SimpleStats {
public:
  Normalizer();
  Normalizer(float mean, float stddev);
  virtual ~Normalizer() {}

  void setMean(float mean) { _mean = mean; }
  void setStddev(float stddev) { _stddev = stddev; };

  virtual float put(float value);

  virtual float get() { return _value; }

  float _value;
  float _mean;
  float _stddev;
};

/// Regularizes signal into [0,1] by rescaling it using the min and max values.
class MinMaxScaler : public PqPutter {
public:
  MinMaxScaler();
  virtual ~MinMaxScaler() {}

  virtual float put(float value);

  virtual float get() { return _value; }

  float _value;
  float _minValue;
  float _maxValue;
};

#define THRESHOLD_LOW     0
#define THRESHOLD_HIGH    1
#define THRESHOLD_FALLING 2
#define THRESHOLD_RISING  3
#define THRESHOLD_CHANGE  4

/**
 * Emits a signals that tells if a certain threshold has been crossed.
 * Possible modes are:
 * - THRESHOLD_LOW  : true iff value < threshold
 * - THRESHOLD_HIGH : true iff value > threshold
 * - THRESHOLD_FALLING : true when value becomes < threshold, then waits until it becomes > resetThreshold (*)
 * - THRESHOLD_RISING  : true when value becomes > threshold, then waits until it becomes < resetThreshold (*)
 * - THRESHOLD_CHANGE  : true when value crosses threshold
 * (*) If resetThreshold is not specified, it is set to threshold by default. Parameter resetThreshold is only
 * used in the FALLING and RISING modes, otherwise it is ignored.
 */
class Thresholder : public PqPutter, public PqDigitalGetter {
public:
  Thresholder(float threshold, uint8_t mode=THRESHOLD_HIGH);
  Thresholder(float threshold, uint8_t mode, float resetThreshold);
  virtual ~Thresholder() {}

  virtual float put(float value);

  virtual bool isOn() { return _value; }

	// Threshold values.
  float _threshold;
	float _resetThreshold;

	// Thresholding mode.
  uint8_t  _mode  : 3;

	// Booleans used to keep track of signal value.
  bool _value     : 1;
	bool _wasLow    : 1;
	bool _wasHigh   : 1;

private:
	void _init(float threshold, uint8_t mode, float resetThreshold);
};

#endif
