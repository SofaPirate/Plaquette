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

namespace pq {
  
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

}

#endif
