/*
 * MinMaxScaler.h
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

#ifndef MIN_MAX_SCALER_H_
#define MIN_MAX_SCALER_H_

#include "PqCore.h"
#include "MovingAverage.h"

namespace pq {

/// Regularizes signal into [0,1] by rescaling it using the min and max values.
class MinMaxScaler : public AnalogSource {
public:
  /// Constructor.
  MinMaxScaler(float decayWindow=PLAQUETTE_INFINITE_SMOOTH_WINDOW);
  virtual ~MinMaxScaler() {}

  /**
   * Pushes value into the unit.
   * @param value the value sent to the unit
   * @return the new value of the unit
   */
  virtual float put(float value);

  /// Returns the current min. value.
  float min() const { return _minValue; }

  /// Returns the current max. value.
  float max() const { return _maxValue; }

  /// Changes the smoothing window (expressed in seconds).
  void time(float seconds);

  /// Returns the smoothing window (expressed in seconds).
  float time() const { return _decayWindow; }

  /// Changes the smoothing window cutoff frequency (expressed in Hz).
  void cutoff(float hz);

  /// Returns the smoothing window cutoff frequency (expressed in Hz).
  float cutoff() const { return (1.0f/time()); }

protected:
  // Minmum value ever put.
  float _minValue;

  // Maximum value ever put.
  float _maxValue;

  // The decay window (in seconds). After that time the min. and max. values will tend to decay towards average.
  float _decayWindow;
};

}

#endif
