/*
 * Thresholder.cpp
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

#include "Thresholder.h"

namespace pq {

Thresholder::Thresholder(float threshold, uint8_t mode)
  : PqDigitalGetter(), PqPutter() {
			_init(threshold, mode, threshold);
		}

Thresholder::Thresholder(float threshold, uint8_t mode, float resetThreshold)
	: PqDigitalGetter(), PqPutter() {
			_init(threshold, mode, resetThreshold);
		}

float Thresholder::put(float value) {
  bool high = (value > _threshold);
  bool low  = (value < _threshold);
  bool rising = (high && _wasLow);
  bool falling = (low  && _wasHigh);

	// Reset.
	if (rising) _wasLow  = false;
	if (falling) _wasHigh = false;
  // bool raising = (high && _prev != (+1));
  // bool falling = (low  && _prev != (-1));
  switch (_mode) {
    case THRESHOLD_HIGH:    _value = high;    break;
    case THRESHOLD_LOW:     _value = low;     break;
    case THRESHOLD_RISING:  _value = rising; break;
    case THRESHOLD_FALLING: _value = falling; break;
    case THRESHOLD_CHANGE:
    default:                _value = rising || falling;
  }

	if (value < _resetThreshold)      _wasLow = true;
	else if (value > _resetThreshold) _wasHigh = true;

//  _prev = (value < _threshold2 ? (-1) : (value > _threshold2 ? (+1) : (0)));
  return get();
}

void Thresholder::_init(float threshold, uint8_t mode, float resetThreshold) {
	_threshold = threshold;
	_mode = mode;
	// Set resetThreshold, with correction.
	if (mode == THRESHOLD_RISING)
		_resetThreshold = min(resetThreshold, threshold);
	else if (mode == THRESHOLD_FALLING)
		_resetThreshold = max(resetThreshold, threshold);
	else
		_resetThreshold = threshold;
	_value = _wasLow = _wasHigh = false;
}

}
