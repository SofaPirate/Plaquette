/*
 * PeakDetector.cpp
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

#include "PeakDetector.h"
#include "pq_map_real.h"

namespace pq {

PeakDetector::PeakDetector(float threshold, uint8_t mode, float resetMinDrop, float fallbackTolerance)
  : DigitalNode()
{
  // Assign mode.
  _mode = constrain(mode, 0, 5);

  // Assign threshold (flip if necessary).
  _threshold = modeInverted() ? -threshold : threshold;

  // Make sure reset and fallback drops are positive.
  resetMinDrop = max(resetMinDrop, 0.0f);
  _fallbackTolerance = max(fallbackTolerance, 0.0f);

  _resetThreshold = _threshold - resetMinDrop;

  // Init peak value to -inf.
  _peakValue = -FLT_MAX;

  // Set everything to false.
  _onValue = _isHigh = _wasLow = _crossed = false;
}


bool PeakDetector::modeInverted() const {
  return (_mode == PEAK_DETECTOR_LOW || _mode == PEAK_DETECTOR_FALLING || _mode == PEAK_DETECTOR_MIN);
}

bool PeakDetector::modeThreshold() const {
  return (_mode == PEAK_DETECTOR_HIGH || _mode == PEAK_DETECTOR_LOW);
}

bool PeakDetector::modeCrossing() const {
  return (_mode == PEAK_DETECTOR_RISING || _mode == PEAK_DETECTOR_FALLING);
}

bool PeakDetector::modePeak() const {
  return (_mode == PEAK_DETECTOR_MAX || _mode == PEAK_DETECTOR_MIN);
}

float PeakDetector::put(float value) {
  // Flip value.
  if (modeInverted())
    value = -value;

  // Get peak value.
  _peakValue = max(_peakValue, value);

  // Compute flags.
  bool high = (value >= _threshold); // value is high if above threshold
  bool rising = (high && _wasLow);   // value is rising if just crossed threshold

	// Reset.
	if (rising) {
    _wasLow  = false;
    _crossed = true;
  }

  // Fallback detected after crossing if either (1) falls below threshold or (2) drops by % tolerance.
  bool fallingBack = (_crossed &&
                       (!high ||
                        (mapTo01(value, _peakValue, _threshold) >= _fallbackTolerance)));

  // Reset.
  if (fallingBack) {
    _crossed = false;
    _peakValue = -FLT_MAX;
  }

  // Check if value is below reset threshold.
  if (value < _resetThreshold)
    _wasLow = true;

  // Assign value depending on mode.
  if (modeThreshold())
    _onValue = high;
  else if (modeCrossing())
    _onValue = rising;
  else
    _onValue = fallingBack;

  return get();
}

}
