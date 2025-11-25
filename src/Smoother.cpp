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
#include "pq_moving_average.h"
#include "Smoother.h"

namespace pq {

Smoother::Smoother(Engine& engine)
  : MovingFilter(engine),
    _currentValueStep(0)
{
  infiniteTimeWindow();
}

Smoother::Smoother(float timeWindow_, Engine& engine)
  : MovingFilter(engine),
    _currentValueStep(0)
{
  timeWindow(timeWindow_);
}

float Smoother::put(float value) {
  if (isCalibrating()) {

    if (_nValuesStep < MOVING_FILTER_N_VALUES_STEP_MAX)
      _nValuesStep++;

    if (_nValuesStep == 1) {
      _currentValueStep = value;
      applyMovingAverageUpdate(_value, _currentValueStep, alpha());
    }
    else {
      // Save previous value.
      float prevValueStep = _currentValueStep;

      // Update current step average value.
      applyMovingAverageUpdate(_currentValueStep, value, 1.0f/_nValuesStep);

      // Update moving average: replace previous value with new value averaged over step.
      amendMovingAverageUpdate(_value, prevValueStep, _currentValueStep, alpha());
    }

  }

  return _value;
}

void Smoother::step() {
  // If no values were added during this step, update using previous value.
  if (_nValuesStep == 0)
    applyMovingAverageUpdate(_value, _currentValueStep, alpha());
  // Otherwise: reset (but keep _currentValueStep).
  else
    _nValuesStep = 0;

  // Increase number of samples.
  if (_nSamples < UINT_MAX)
    _nSamples++;
}

}
