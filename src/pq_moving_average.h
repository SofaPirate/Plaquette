
/*
 * pq_moving_average.h
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
#ifndef PQ_MOVING_AVERAGE_H_
#define PQ_MOVING_AVERAGE_H_

#include "PqCore.h"

namespace pq {

// When using a pre-initialized moving average value, we use this value as the stabilization time (in seconds).
// In other words: we presume that the pre-initialized value as if it had been stabilized over that amount of time.
constexpr float PRE_INITIALIZED_STABILIZATION_TIME = 60.0f;

/**
 * Applies a single update on #runningValue# with new sample #newValue# and mixing
 * factor #alpha#.
 */
inline void applyMovingAverageUpdate(float& runningValue, float newValue, float alpha) {
  runningValue += alpha * (newValue - runningValue);
}

/// Performs an amendment of latest update (needs to be called with same #alpha# parameter).
inline void amendMovingAverageUpdate(float& runningValue, float previousValue, float newValue, float alpha) {
  runningValue += alpha * (newValue - previousValue);
}

/// Returns the value of a single update on #runningValue# with new sample #newValue# and mixing factor #alpha#.
inline float computeMovingAverageUpdate(float runningValue, float newValue, float alpha) {
  applyMovingAverageUpdate(runningValue, newValue, alpha);
  return runningValue;
}

/// Returns the alpha value computed from given number of samples.
inline float movingAverageExponentialAlpha(float nSamples) {
  // Formula used is standard formula: 2 /(nSamplesTarget+1); set maximum alpha to 1.
  return (nSamples > 1.0f ?
            2.0f / (nSamples + 1) :
            1.0f);
}

/// Returns the alpha value computed from given number of samples.
inline float movingAverageSimpleAlpha(float nSamples) {
  return 1.0f / ((float)nSamples + 1);
}

/// Returns the alpha value computed from given sample rate, time window, and number of samples.
float movingAverageAlpha(float sampleRate, float timeWindow=INFINITE_TIME_WINDOW, unsigned int nSamples=UINT_MAX, bool preInitialized=false);

}

#endif
