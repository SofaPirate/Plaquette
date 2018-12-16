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
