/*
 * AdaptiveNormalizer.h
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

#ifndef ADAPTIVE_NORMALIZER_H_
#define ADAPTIVE_NORMALIZER_H_

#include "PqCore.h"

namespace pq {

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

}

#endif
