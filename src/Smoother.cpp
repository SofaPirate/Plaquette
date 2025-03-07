/*
 * Smoother.cpp
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

#include "MovingAverage.h"
#include "Smoother.h"

namespace pq {

Smoother::Smoother(Engine& engine) : Smoother(PLAQUETTE_DEFAULT_SMOOTH_WINDOW, engine) {}

Smoother::Smoother(float timeWindow, Engine& engine)
  : Unit(engine),
    MovingAverage(timeWindow),
    _currentValueStep(0),
    _nValuesStep(0)
 {
}

float Smoother::put(float value) {
  // Increment n. values.
  if (_nValuesStep < 255)
    _nValuesStep++;

  // First time put() is called this step.
  if (_nValuesStep == 1) {
    // Save current value.
    _currentValueStep = value;

    // Update moving average.
    update(value, sampleRate());
  }
  // If put() is called more than one time in same step, readjust moving average.
  else {
    // Save previous value.
    float prevValueStep = _currentValueStep;

    // Update current step average value.
    MovingAverage::applyUpdate(_currentValueStep, value, 1.0f/_nValuesStep);

    // Update moving average: replace previous value with new value averaged over step.
    amendUpdate(prevValueStep, _currentValueStep, sampleRate());
  }

  // Return smoothed value.
  return get();
}

void Smoother::step() {
  // If no values were added during this step, update using previous value.
  if (_nValuesStep == 0)
    update(_currentValueStep, sampleRate()); // in other words: repeat update with previous value
  // Otherwise: reset (but keep _currentValueStep).
  else
    _nValuesStep = 0;
}

}