/*
 * pq_random.h
 *
 * Provides methods for generating uniform random numbers.
 *
 * (c) 2008 Sofian Audry -- info(@)sofianaudry(.)com
 *
 * This class was adapted from the files Random.{h,cc} in the Torch
 * library (http://torch.ch)
 * Copyright (C) 2003--2004 Ronan Collobert (collober@idiap.ch)
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
#ifndef RANDOM_H_
#define RANDOM_H_

namespace pq {

// Floating point random numbers.

/// Generates a uniform random number in the interval [0,1).
float randomFloat();

/// Generates a uniform random number in the interval [0,max).
float randomFloat(float max);

/// Generates a uniform random number in the interval [min,max) (b>a).
float randomFloat(float min, float max);

/**
 * Randomly triggers an event about once per time window, on average.
 *
 * Call this function once in each loop or sample. It will occasionally return
 * true, with the frequency adjusted so that you get roughly one event for each
 * `timeWindow` period, no matter how fast your loop is running.
 *
 * @param timeWindow  Duration of the window
 * @param samplePeriod Duration of a sample (in same unit as timeWindow eg. seconds)
 * @return true when an event occurs during this sample
 */
bool randomTrigger(float timeWindow, float samplePeriod);

/// @deprecated
[[deprecated("Use randomFloat() instead.")]]
float randomUniform();
/// @deprecated
[[deprecated("Use randomFloat(float) instead.")]]
float randomUniform(float max);
/// @deprecated
[[deprecated("Use randomFloat(float, float) instead.")]]
float randomUniform(float min, float max);

} // namespace pq

#endif
