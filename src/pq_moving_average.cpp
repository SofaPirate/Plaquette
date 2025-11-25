/*
 * pq_moving_average.cpp
 *
 * (c) 2011-2025 Sofian Audry -- info(@)sofianaudry(.)com
 * Inspired by code by Karsten Kutza
 * http://www.ip-atlas.com/pub/nap/nn-src/bpn.txt
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
#include "pq_moving_average.h"

namespace pq {

float movingAverageAlpha(float sampleRate, float smoothTime, unsigned int nSamples, bool preInitialized) {

  // Finite time window.
  if (smoothTime >= 0) {

    if (preInitialized) {
      // Return alpha based on approximative number of samples in time window.
      return movingAverageExponentialAlpha(smoothTime * sampleRate);
    }
    else {
      // Approximative number of samples in time window.
      float nSamplesTarget = smoothTime * sampleRate;

      // In order to do a smooth transition and prevent first values to take too much weight compared to
      // later values, we start by averaging using a non-moving average for the first nSamplesTarget values.
      float nSamplesPlusOne = nSamples + 1.0f;
      if (nSamplesPlusOne < nSamplesTarget)
        return 1.0f / nSamplesPlusOne; // = movingAverageSimpleAlpha(nSamples) (avoids one +1 addition)
      else
        return movingAverageExponentialAlpha(nSamplesTarget);
    }
  }

  // Infinite time window.
  else {

    if (preInitialized) {
      unsigned int nSamplesStabilized = (unsigned int)(sampleRate * PRE_INITIALIZED_STABILIZATION_TIME);
      // If number of samples is less than stabilization time, use average alpha.
      if (nSamples <= nSamplesStabilized)
        return movingAverageExponentialAlpha(nSamplesStabilized);
      else
        return movingAverageSimpleAlpha(nSamples);
    }

    else {
      // Constant decay (simple average over all values).
      return movingAverageSimpleAlpha(nSamples);
    }

  }
}

}
