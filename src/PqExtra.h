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
#include "pq_time.h"
#include "PqCore.h"
#include "MovingAverage.h"
#include "MovingStats.h"
#include "SimpleStats.h"

/// Stream/serial input. Reads float values using Arduino built-in parseFloat().
class StreamIn : public PqGetter {
public:
  /**
   * Constructor.
   * @param stream a reference to a Stream object
   */
  StreamIn(Stream& stream=Serial);
  virtual ~StreamIn() {}

  /// Returns value read from the stream.
  virtual float get() { return _value; }

protected:
  // Core Plaquette methods.
  virtual void setup();
  virtual void update();

  // Current value.
  float _value;

  // Internal use: keep track of next incoming value in a non-blocking way.
  float _nextValue;
  float _nextFraction;
  bool _nextIsValid : 1;
  bool _nextIsNegative : 1;
  bool _nextIsFraction : 1;

  // The stream.
  Stream* _stream;
};

/// Square oscillator. Duty cycle is expressed as % of period.
class SquareOsc : public PqDigitalGetter {
public:
  /**
   * Constructor.
   * @param period the period of oscillation (in seconds)
   * @param dutyCycle the duty-cycle as a value in [0, 1]
   */
  SquareOsc(float period=1.0f, float dutyCycle=0.5f);
  virtual ~SquareOsc() {}

  /// Returns true iff the input is "on".
  virtual bool isOn() { return _isOn; }

  /**
   * Sets the period (in seconds).
   * @param period the period of oscillation (in seconds)
   * @return the unit itself
   */
  virtual SquareOsc& period(float period);

  /**
   * Sets the frequency (in Hz).
   * @param frequency the frequency of oscillation (in Hz)
   * @return the unit itself
   */
  virtual SquareOsc& frequency(float frequency) { return period(1/frequency); }

  /**
   * Sets the duty-cycle (ie. the proportion of time during which the signal is on).
   * @param dutyCycle the duty-cycle as a value in [0, 1]
   * @return the unit itself
   */
  virtual SquareOsc& dutyCycle(float dutyCycle);

protected:
  // Core Plaquette methods.
  virtual void setup();
  virtual void update();

  // Is the signal currently on (high) or off (low).
  bool _isOn;

  // Period (seconds).
  float _period;

  // Duty-cycle (in % of period).
  float _dutyCycle;

  // Start time of each period (in seconds).
  float _startTime;
};

/// Sine oscillator. Phase is expressed as % of period.
class SineOsc : public PqGetter {
public:
  /**
   * Constructor.
   * @param period the period of oscillation (in seconds)
   */
  SineOsc(float period=1.0f);
  virtual ~SineOsc() {}

  /// Returns value in [0, 1].
  virtual float get() { return _value; }

  /**
   * Sets the period (in seconds).
   * @param period the period of oscillation (in seconds)
   * @return the unit itself
   */
  virtual SineOsc& period(float period);

  /**
   * Sets the frequency (in Hz).
   * @param frequency the frequency of oscillation (in Hz)
   * @return the unit itself
   */
  virtual SineOsc& frequency(float frequency) { return period(1/frequency); }

  /**
   * Sets the phase (ie. the offset, in seconds).
   * @param phrase the phase (in seconds)
   * @return the unit itself
   */
  virtual SineOsc& phase(float phase);

protected:
  // Core Plaquette methods.
  virtual void setup();
  virtual void update();

  void _update(float t);

  // Current value of the signal.
  float _value;

  // Period (seconds).
  float _period;

  // Phase (seconds).
  float _phase;

  // Start time of each period (in seconds).
  float _startTime;
};

/**
 * Triangle/sawtooth oscillator.
 */
class TriOsc : public PqGetter {
public:
  /**
   * Constructor.
   * @param period the period of oscillation (in seconds)
   * @param width a value in [0, 1] that determines the point at which the wave reaches its maximum point (expressed as a fraction of the period)
   */
  TriOsc(float period=1.0f, float width=0.5f);
  virtual ~TriOsc() {}

  /// Returns value in [0, 1].
  virtual float get() { return _value; }

  /**
   * Sets the period (in seconds).
   * @param period the period of oscillation (in seconds)
   * @return the unit itself
   */
  virtual TriOsc& period(float period);

  /**
   * Sets the frequency (in Hz).
   * @param frequency the frequency of oscillation (in Hz)
   * @return the unit itself
   */
  virtual TriOsc& frequency(float freq) { return period(1/freq); }

  /**
   * Sets the width of the wave.
   * @param width a value in [0, 1] that determines the point at which the wave reaches its maximum point (expressed as a fraction of the period)
   * @return the unit itself
   */
  virtual TriOsc& width(float width);

protected:
  // Core Plaquette methods.
  virtual void setup();
  virtual void update();

  // Current value of the signal.
  float _value;

  // Period (seconds).
  float _period;

  // Tipping-point (in % of period).
  float _width;

  // Start time of each period (in seconds).
  float _startTime;
};

/// Stream/serial output. Number of digits of precision is configurable.
class StreamOut : public PqPutter {
public:
  /**
   * Constructor.
   * @param stream a reference to a Stream object
   */
  StreamOut(Stream& stream=Serial);
  virtual ~StreamOut() {}

  /**
   * Pushes value into the unit.
   * @param value the value sent to the unit
   * @return the new value of the unit
   */
  virtual float put(float value);

  /// Returns value.
  virtual float get() { return _value; }

  /**
   * Sets precision of the output.
   * @param digits the number of digits to show after decimal point
   */
  virtual void precision(uint8_t digits);

protected:
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

protected:
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
  /**
   * Constructor.
   * @param factor a parameter in [0, 1] representing the importance of new values as opposed to old values (ie. lower smoothing factor means *more* smoothing)
   */
  Smoother(float factor=0.1f);
  virtual ~Smoother() {}

  /**
   * Pushes value into the unit.
   * @param value the value sent to the unit
   * @return the new value of the unit
   */
  virtual float put(float value);

  /// Returns smoothed value.
  virtual float get() { return MovingAverage::get(); }
};

/**
 * Adaptive normalizer: normalizes values on-the-run using exponential moving
 * averages over mean and standard deviation.
 */
class AdaptiveNormalizer : public PqPutter, public MovingStats {
public:
  /**
   * Default constructor. Will renormalize data around a mean of 0 and a standard
   * deviation of 1.
   * @param smoothFactor a parameter in [0, 1] representing the importance of new values as opposed to old values (ie. lower smoothing factor means *more* smoothing)
   */
   AdaptiveNormalizer(float smoothFactor=0.001f);

  /**
   * Constructor.
   * @param mean the target mean
   * @param stddev the target standard deviation
   * @param smoothFactor a parameter in [0, 1] representing the importance of new values as opposed to old values (ie. lower smoothing factor means *more* smoothing)
   */
  AdaptiveNormalizer(float mean, float stddev, float smoothFactor=0.001f);
  virtual ~AdaptiveNormalizer() {}

  /**
   * Sets target mean of normalized values.
   * @param mean the target mean
   */
  AdaptiveNormalizer& targetMean(float mean) { _mean = mean; return *this; }

  /**
   * Sets target standard deviation of normalized values.
   * @param stddev the target standard deviation
   */
  AdaptiveNormalizer& targetStdDev(float stddev) { _stddev = stddev; return *this; }

  /**
   * Pushes value into the unit.
   * @param value the value sent to the unit
   * @return the new value of the unit
   */
  virtual float put(float value);

  /// Returns normalized value.
  virtual float get() { return _value; }

protected:
  // Current value (normalized).
  float _value;

  // Target normalization parameters.
  float _mean;
  float _stddev;
};

/// Standard normalizer: normalizes values on-the-run using real mean and standard deviation.
class Normalizer : public PqPutter, public SimpleStats {
public:
  /**
   * Default constructor. Will renormalize data around a mean of 0 and a standard
   * deviation of 1.
   */
  Normalizer();

  /**
   * Constructor.
   * @param mean the target mean
   * @param stddev the target standard deviation
   */
  Normalizer(float mean, float stddev);
  virtual ~Normalizer() {}

  /**
   * Sets target mean of normalized values.
   * @param mean the target mean
   */
  Normalizer& targetMean(float mean) { _mean = mean; return *this; }

  /**
   * Sets target standard deviation of normalized values.
   * @param stddev the target standard deviation
   */
  Normalizer& targetStdDev(float stddev) { _stddev = stddev; return *this; }

  /**
   * Pushes value into the unit.
   * @param value the value sent to the unit
   * @return the new value of the unit
   */
  virtual float put(float value);

  /// Returns normalized value.
  virtual float get() { return _value; }

protected:
  // Current value (normalized).
  float _value;

  // Target normalization parameters.
  float _mean;
  float _stddev;
};

/// Regularizes signal into [0,1] by rescaling it using the min and max values.
class MinMaxScaler : public PqPutter {
public:
  /// Constructor.
  MinMaxScaler();
  virtual ~MinMaxScaler() {}

  /**
   * Pushes value into the unit.
   * @param value the value sent to the unit
   * @return the new value of the unit
   */
  virtual float put(float value);

  /// Returns rescaled value.
  virtual float get() { return _value; }

protected:
  // Current value (rescaled).
  float _value;

  // Minmum value ever put.
  float _minValue;

  // Maximum value ever put.
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

  /**
   * Pushes value into the unit.
   * @param value the value sent to the unit
   * @return the new value of the unit
   */
  virtual float put(float value);

  /// Returns true iff the threshold is crossed.
  virtual bool isOn() { return _value; }

protected:
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
