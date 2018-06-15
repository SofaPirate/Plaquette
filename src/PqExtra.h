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

namespace pq {

/// Stream/serial input. Reads float values using Arduino built-in parseFloat().
class StreamIn : public PqAnalogSource {
public:
  /**
   * Constructor.
   * @param stream a reference to a Stream object
   */
  StreamIn(Stream& stream=Serial);
  virtual ~StreamIn() {}

protected:
  // Core Plaquette methods.
  virtual void begin();
  virtual void step();

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
class SquareOsc : public PqAnalogSource {
public:
  /**
   * Constructor.
   * @param period the period of oscillation (in seconds)
   * @param dutyCycle the duty-cycle as a value in [0, 1]
   */
  SquareOsc(float period=1.0f, float dutyCycle=0.5f);
  virtual ~SquareOsc() {}

  /**
   * Sets the period (in seconds).
   * @param period the period of oscillation (in seconds)
   * @return the unit itself
   */
  virtual SquareOsc& period(float period);
  virtual float period() const { return _period; }

  /**
   * Sets the frequency (in Hz).
   * @param frequency the frequency of oscillation (in Hz)
   * @return the unit itself
   */
  virtual SquareOsc& frequency(float frequency);
  virtual float frequency() const { return (1/_period); }

  /**
   * Sets the duty-cycle (ie. the proportion of time during which the signal is on).
   * @param dutyCycle the duty-cycle as a value in [0, 1]
   * @return the unit itself
   */
  virtual SquareOsc& dutyCycle(float dutyCycle);
  virtual float dutyCycle() const { return _dutyCycle; }

  /**
   * Sets the phase (ie. the offset, in % of period).
   * @param phase the phase (in % of period)
   * @return the unit itself
   */
  virtual SquareOsc& phase(float phase);
  virtual float phase() const { return _phase; }

  /**
   * Sets the amplitude of the wave.
   * @param amplitude a value in [0, 1] that determines the amplitude of the wave (centered at 0.5).
   * @return the unit itself
   */
   virtual SquareOsc& amplitude(float amplitude);
   virtual float amplitude() const { return _amplitude * 2; }

protected:
  // Core Plaquette methods.
  virtual void begin();
  virtual void step();

  void _updateValue();

  // Period (seconds).
  float _period;

  // Duty-cycle (in % of period).
  float _dutyCycle;

  // Phase (in % of period).
  float _phase;

  // Amplitude (in %).
  float _amplitude;

  // Internal use.
  float _phaseTime;
};

/// Sine oscillator. Phase is expressed as % of period.
class SineOsc : public PqAnalogSource {
public:
  /**
   * Constructor.
   * @param period the period of oscillation (in seconds)
   */
  SineOsc(float period=1.0f);
  virtual ~SineOsc() {}

  /**
   * Sets the period (in seconds).
   * @param period the period of oscillation (in seconds)
   * @return the unit itself
   */
  virtual SineOsc& period(float period);
  virtual float period() const { return _period; }

  /**
   * Sets the frequency (in Hz).
   * @param frequency the frequency of oscillation (in Hz)
   * @return the unit itself
   */
  virtual SineOsc& frequency(float frequency);
  virtual float frequency() const { return (1/_period); }

  /**
   * Sets the phase (ie. the offset, in % of period).
   * @param phase the phase (in % of period)
   * @return the unit itself
   */
  virtual SineOsc& phase(float phase);
  virtual float phase() const { return _phase; }

  /**
   * Sets the amplitude of the wave.
   * @param amplitude a value in [0, 1] that determines the amplitude of the wave (centered at 0.5).
   * @return the unit itself
   */
   virtual SineOsc& amplitude(float amplitude);
   virtual float amplitude() const { return _amplitude * (-32767.0f); }

protected:
  // Core Plaquette methods.
  virtual void begin();
  virtual void step();

  void _updateValue();

  // Period (seconds).
  float _period;

  // Phase (in % of period).
  float _phase;

  // Amplitude (in %).
  float _amplitude;

  // Internal use.
  float _phaseTime;
};

/**
 * Triangle/sawtooth oscillator.
 */
class TriOsc : public PqAnalogSource {
public:
  /**
   * Constructor.
   * @param period the period of oscillation (in seconds)
   * @param width a value in [0, 1] that determines the point at which the wave reaches its maximum point (expressed as a fraction of the period)
   */
  TriOsc(float period=1.0f, float width=0.5f);
  virtual ~TriOsc() {}

  /**
   * Sets the period (in seconds).
   * @param period the period of oscillation (in seconds)
   * @return the unit itself
   */
  virtual TriOsc& period(float period);
  virtual float period() const { return _period; }

  /**
   * Sets the frequency (in Hz).
   * @param frequency the frequency of oscillation (in Hz)
   * @return the unit itself
   */
  virtual TriOsc& frequency(float frequency);
  virtual float frequency() const { return (1/_period); }

  /**
   * Sets the width of the wave.
   * @param width a value in [0, 1] that determines the point at which the wave reaches its maximum point (expressed as a fraction of the period)
   * @return the unit itself
   */
  virtual TriOsc& width(float width);
  virtual float width() const { return _width; }

  /**
   * Sets the amplitude of the wave.
   * @param amplitude a value in [0, 1] that determines the amplitude of the wave (centered at 0.5).
   * @return the unit itself
   */
   virtual TriOsc& amplitude(float amplitude);
   virtual float amplitude() const { return _amplitude; }

   /**
    * Sets the phase (ie. the offset, in % of period).
    * @param phase the phase (in % of period)
    * @return the unit itself
    */
   virtual TriOsc& phase(float phase);
   virtual float phase() const { return _phase; }

protected:
  // Core Plaquette methods.
  virtual void begin();
  virtual void step();

  void _updateValue();

  // Period (seconds).
  float _period;

  // Phase (in % of period).
  float _phase;

  // Tipping-point (in % of period).
  float _width;

  // Amplitude (in %).
  float _amplitude;

  // Internal use.
  float _phaseTime;
};

/**
 * Chronometer class which emits a "1" after a certain time.
 */
class Metro : public PqDigitalSource {
public:
  /**
   * Constructor.
   * @param period the period of oscillation (in seconds)
   */
  Metro(float period=1.0f);

  /**
   * Sets the period (in seconds).
   * @param period the period of oscillation (in seconds)
   * @return the unit itself
   */
  virtual Metro& period(float period);
  virtual float period() const { return _period; }

  /**
   * Sets the frequency (in Hz).
   * @param frequency the frequency of oscillation (in Hz)
   * @return the unit itself
   */
  virtual Metro& frequency(float frequency);
  virtual float frequency() const { return (1/_period); }

  /**
   * Sets the phase (ie. the offset, in % of period).
   * @param phase the phase (in % of period)
   * @return the unit itself
   */
  virtual Metro& phase(float phase);
  virtual float phase() const { return _phase; }

protected:
  virtual void begin();
  virtual void step();

  // Phase (in % of period).
  float _period;

  // Phase (in % of period).
  float _phase;

  // Internal use.
  float _phaseTime;
};

class Chrono : virtual public PqUnit {
public:
  Chrono();

  /// Starts/restarts the chronometer.
  virtual void start();

  /// Adds/subtracts time to the chronometer.
  virtual void addTime(float time);

  /// Interrupts the chronometer.
  virtual void stop();

  /// Resumes process.
  virtual void resume();

  /// The time currently elapsed by the chronometer (in seconds).
  virtual float elapsed() const { return _elapsedTime; }

  /// Returns true iff elapsed time has passed given timeout.
  virtual bool hasPassed(float timeout) const;

  /**
   * Returns true iff elapsed time has passed given timeout (optional argument to
   * automatically restart if true).
   */
  virtual bool hasPassed(float timeout, bool restartIfPassed);

  /// Returns true iff the chronometer is currently running.
  bool isStarted() const { return _isStarted; }

protected:
  virtual void begin();
  virtual void step();

  // The starting time (in seconds).
  float _startTime;

  // The offset time
  float _offsetTime;

  // The current elapsed time.
  float _elapsedTime;

  // Is the chrono currently started.
  bool _isStarted;
};

class AbstractTimer : virtual public Chrono {
public:
  AbstractTimer(float duration=1.0f);

  /// Sets the duration of the chronometer.
  virtual void duration(float duration);

  /// The progress of the chrono process (in %).
  virtual float progress() const;

  /// Returns true iff the chronometer has completed its process.
  bool isComplete() const { return progress() >= 1.0; }

protected:
  // The duration.
  float _duration;
};

/**
 * Chronometer class which becomes "on" after a given duration.
 */
class Timer : public AbstractTimer, public PqDigitalGetter {
public:
  Timer();

  virtual bool isOn() { return isStarted() && isComplete(); }
};

// TODO: implement a floating-point version of Chrono in Plaquette and make
// ramp a subclass of that class.
/**
 * Provides a ramping / tweening mechanism that allows smooth transitions between
 * two values.
 */
class Ramp : public PqAnalogSource, public AbstractTimer {
public:
  /**
   * Constructor.
   * @param initialValue the value the ramp starts with
   */
  Ramp(float initialValue=0.0f);

  /**
   * Assign final value of the ramp starting from current value.
   * @param to the final value
   */
  virtual void to(float to);

  /**
   * Assign initial and final values of the ramp.
   * @param from the initial value
   * @param to the final value
   */
  virtual void fromTo(float from, float to);

  /// Starts/restarts the chronometer.
  virtual void start();

  /**
   * Starts a new ramp (starting from current value).
   * @param to the final value
   * @param duration the duration of the ramp (in seconds)
   */
  virtual void start(float to, float duration);

  /**
   * Starts a new ramp.
   * @param from the initial value
   * @param to the final value
   * @param duration the duration of the ramp (in seconds)
   */
  virtual void start(float from, float to, float duration);

protected:

  virtual void step();

  // The starting point.
  float _from;

  // The variation from starting point targetted.
  float _change;
};

/// Stream/serial output. Number of digits of precision is configurable.
class StreamOut : public PqAnalogUnit {
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

  /**
   * Sets precision of the output.
   * @param digits the number of digits to show after decimal point
   */
  virtual void precision(uint8_t digits);

protected:
  // Number of digits of precision.
  uint8_t _digits;

  // The stream.
  Stream* _stream;
};

/// Generates a simple ASCII-based representation of a signal.
/// Precision represents the number of columns used to represent the signal.
class OscilloscopeOut : public PqAnalogUnit {
public:
  OscilloscopeOut(float minValue=0, float maxValue=1, uint8_t precision=100);
  virtual ~OscilloscopeOut() {}

  virtual float put(float value);

protected:
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
  Smoother(float smoothWindow=PLAQUETTE_DEFAULT_SMOOTH_WINDOW);
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
class AdaptiveNormalizer : public PqAnalogUnit, public MovingStats {
public:
  /**
   * Default constructor. Will renormalize data around a mean of 0.5 and a standard deviation of 0.25.
   * @param smoothWindow specifies the approximate "time window" over which the normalization applies(in seconds)
   */
   AdaptiveNormalizer(float smoothWindow=PLAQUETTE_DEFAULT_SMOOTH_WINDOW);

  /**
   * Constructor.
   * @param mean the target mean
   * @param stddev the target standard deviation
   * @param smoothWindow specifies the approximate "time window" over which the normalization applies(in seconds)
   */
  AdaptiveNormalizer(float mean, float stddev, float smoothWindow=PLAQUETTE_DEFAULT_SMOOTH_WINDOW);
  virtual ~AdaptiveNormalizer() {}

  /**
   * Sets target mean of normalized values.
   * @param mean the target mean
   */
  AdaptiveNormalizer& targetMean(float mean) { _targetMean = mean; return *this; }

  /**
   * Sets target standard deviation of normalized values.
   * @param stddev the target standard deviation
   */
  AdaptiveNormalizer& targetStdDev(float stddev) { _targetStddev = stddev; return *this; }

  /**
   * Pushes value into the unit.
   * @param value the value sent to the unit
   * @return the new value of the unit
   */
  virtual float put(float value);

protected:
  // Target normalization parameters.
  float _targetMean;
  float _targetStddev;
};

/// Standard normalizer: normalizes values on-the-run using real mean and standard deviation.
class Normalizer : public PqAnalogUnit, public SimpleStats {
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

protected:
  // Target normalization parameters.
  float _mean;
  float _stddev;
};

/// Regularizes signal into [0,1] by rescaling it using the min and max values.
class MinMaxScaler : public PqAnalogUnit {
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

protected:
  // Minmum value ever put.
  float _minValue;

  // Maximum value ever put.
  float _maxValue;
};
//
// #define THRESHOLD_LOW     0
// #define THRESHOLD_HIGH    1
// #define THRESHOLD_FALLING 2
// #define THRESHOLD_RISING  3
// #define THRESHOLD_CHANGE  4
//
// /**
//  * Emits a signals that tells if a certain threshold has been crossed.
//  * Possible modes are:
//  * - THRESHOLD_LOW  : true iff value < threshold
//  * - THRESHOLD_HIGH : true iff value > threshold
//  * - THRESHOLD_FALLING : true when value becomes < threshold, then waits until it becomes > resetThreshold (*)
//  * - THRESHOLD_RISING  : true when value becomes > threshold, then waits until it becomes < resetThreshold (*)
//  * - THRESHOLD_CHANGE  : true when value crosses threshold
//  * (*) If resetThreshold is not specified, it is set to threshold by default. Parameter resetThreshold is only
//  * used in the FALLING and RISING modes, otherwise it is ignored.
//  */
// class Thresholder : public PqAnalogUnit, public PqDigitalSource {
// public:
//   Thresholder(float threshold, uint8_t mode=THRESHOLD_HIGH);
//   Thresholder(float threshold, uint8_t mode, float resetThreshold);
//   virtual ~Thresholder() {}
//
//   /**
//    * Pushes value into the unit.
//    * @param value the value sent to the unit
//    * @return the new value of the unit
//    */
//   virtual float put(float value);
//
//   /// Returns true iff the threshold is crossed.
//   virtual bool isOn() { return _value; }
//
// protected:
// 	// Threshold values.
//   float _threshold;
// 	float _resetThreshold;
//
// 	// Thresholding mode.
//   uint8_t  _mode  : 3;
//
// 	// Booleans used to keep track of signal value.
//   bool _value     : 1;
// 	bool _wasLow    : 1;
// 	bool _wasHigh   : 1;
//
// private:
// 	void _init(float threshold, uint8_t mode, float resetThreshold);
// };

} // namespace pq

#endif
