/*
 * PeakDetector.h
 *
 * (c) 2022 Sofian Audry        :: info(@)sofianaudry(.)com
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

#ifndef PEAK_DETECTOR_H_
#define PEAK_DETECTOR_H_

#include "PqCore.h"

#define PEAK_DETECTOR_HIGH 0
#define PEAK_DETECTOR_LOW  1

#define PEAK_DETECTOR_RISING  2
#define PEAK_DETECTOR_FALLING  3

#define PEAK_DETECTOR_MAX  4
#define PEAK_DETECTOR_MIN  5

namespace pq {

/**
 * Emits a signals that tells if a certain threshold has been crossed.
 */
class PeakDetector : public DigitalNode {
public:
  /**
   * Constructor. Possible modes are:
   * - PEAK_DETECTOR_HIGH : peak detected whenever value >= threshold
   * - PEAK_DETECTOR_LOW  : peak detected whenever value <= threshold
   * - PEAK_DETECTOR_RISING  : peak detected when value becomes >= threshold, then wait until it becomes < resetThreshold (*)
   * - PEAK_DETECTOR_FALLING : peak detected when value becomes <= threshold, then wait until it becomes > resetThreshold (*)
   * - PEAK_DETECTOR_MAX : peak detected after value becomes >= threshold and then falls back after peaking; then waits until it becomes < resetThreshold (*)
   * - PEAK_DETECTOR_MIN : peak detected after value becomes <= threshold and then rises back after peaking; then waits until it becomes > resetThreshold (*)
   * @param threshold value that triggers peak detection
   * @param mode peak detection mode
   * @param resetMinDrop absolute "drop" below (or above) threshold to reset peak detection in certain modes
   * @param fallbackTolerance relative "drop" below (or above) peak, expressed as percentage between threshold and peak point
   */
  PeakDetector(float threshold, uint8_t mode=PEAK_DETECTOR_MAX, float resetMinDrop=0, float fallbackTolerance=0.1);
  virtual ~PeakDetector() {}

  /// Returns true if mode is PEAK_DETECTOR_LOW, PEAK_DETECTOR_FALLING, or PEAK_DETECTOR_MIN.
  bool modeInverted() const;

  /// Returns true if mode is PEAK_DETECTOR_HIGH or PEAK_DETECTOR_LOW.
  bool modeThreshold() const;

  /// Returns true if mode is PEAK_DETECTOR_RISING or PEAK_DETECTOR_FALLING.
  bool modeCrossing() const;

  /// Returns true if mode is PEAK_DETECTOR_MAX or PEAK_DETECTOR_MIN.
  bool modePeak() const;

  /// Returns mode.
  virtual uint8_t mode() const { return _mode; }

  /**
   * Pushes value into the unit.
   * @param value the value sent to the unit
   * @return the new value of the unit
   */
  virtual float put(float value);

  /// Returns true iff the threshold is crossed.
  virtual bool isOn() { return _onValue; }

protected:
	// Threshold values.
  float _threshold;
	float _resetThreshold;
  float _fallbackTolerance;
  float _peakValue;

	// Thresholding mode.
  bool _onValue   : 1;
  uint8_t  _mode  : 3;

	// Booleans used to keep track of signal value.
  bool _isHigh    : 1;
	bool _wasLow    : 1;
  bool _crossed   : 1;
};

}

#endif
