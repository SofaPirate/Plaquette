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

PeakDetector::PeakDetector(float triggerThreshold_, uint8_t mode_)
  : DigitalNode()
{
  // Assign mode.
  mode(mode_);

  // Assign triggerThreshold (flip if necessary).
  triggerThreshold(triggerThreshold_);

  // Set default values.
  reloadThreshold(triggerThreshold_);
  fallbackTolerance(0.1f);

  // Init peak value to -inf.
  _peakValue = -FLT_MAX;

  // Init all flags.
  _onValue = _isHigh = _crossed = false;
  _wasLow = true;
}

void PeakDetector::triggerThreshold(float triggerThreshold) {
  _triggerThreshold = modeInverted() ? -triggerThreshold : triggerThreshold;
}

void PeakDetector::reloadThreshold(float reloadThreshold) {
  if (modeInverted()) reloadThreshold = -reloadThreshold;
  _reloadThreshold = min(reloadThreshold, _triggerThreshold);
}

void PeakDetector::fallbackTolerance(float fallbackTolerance) {
  _fallbackTolerance = constrain(fallbackTolerance, 0.0f, 1.0f);
}

void PeakDetector::mode(uint8_t mode) {
  // Save current state.
  bool wasInverted = modeInverted();

  // Change mode.
  _mode = constrain(mode, (uint8_t)PEAK_RISING, (uint8_t)PEAK_MIN);

  // If mode inversion was changed, adjust triggerThresholds.
  if (modeInverted() != wasInverted) {
    // Flip.
    _triggerThreshold = -_triggerThreshold;
    _reloadThreshold = -_reloadThreshold;
  }
}

bool PeakDetector::modeInverted() const {
  return (_mode == PEAK_FALLING || _mode == PEAK_MIN);
}

bool PeakDetector::modeCrossing() const {
  return (_mode == PEAK_RISING || _mode == PEAK_FALLING);
}

bool PeakDetector::modeApex() const {
  return !modeCrossing();
}

float PeakDetector::put(float value) {
  // Flip value.
  if (modeInverted())
    value = -value;

  // Compute flags.
  bool high     = (value >= _triggerThreshold); // value is high if above triggerThreshold
  bool crossing = (high && _wasLow);            // value is crossing if just crossed triggerThreshold
  bool isMax    = (value > _peakValue);         // value is new max if higher than current peak value

  // Reset.
  if (crossing) {
    _wasLow  = false;
    _crossed = true;
  }

  // Set peak value.
  if (isMax)
    _peakValue = value;

  // Fallback detected after crossing and falling below maximum and either:
  // (1) falls below triggerThreshold (!high) OR
  // (2) drops by % tolerance between peak and triggerThreshold
  bool fallingBack = (_crossed && !isMax &&
                       (!high ||
                        (mapTo01(value, _peakValue, _triggerThreshold) >= _fallbackTolerance &&
                         _peakValue != _triggerThreshold))); // deal with special case where mapTo01(...) would return 0.5 by default

  // Assign value depending on mode.
  _onValue = (modeCrossing() ? crossing : fallingBack);

  // Reset.
  if (fallingBack) {
    _crossed = false;
    _peakValue = -FLT_MAX;
  }

  // Check if value is below reloadThreshold.
  if (value < _reloadThreshold)
    _wasLow = true;

  return get();
}


}
