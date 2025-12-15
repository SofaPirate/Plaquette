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
{}

Smoother::Smoother(float timeWindow_, Engine& engine)
  : MovingFilter(timeWindow_, engine),
    _currentValueStep(0)
{}


void Smoother::reset() {
  MovingFilter::reset();
  _currentValueStep = _value;
}

void Smoother::reset(float estimatedMeanValue) {
  MovingFilter::reset(estimatedMeanValue);
  _currentValueStep = _value;
}

void Smoother::reset(float estimatedMinValue, float estimatedMaxValue) {
  MovingFilter::reset(estimatedMinValue, estimatedMaxValue);
  _currentValueStep = _value;
}

float Smoother::put(float value) {
  if (isCalibrating()) {
    float a = alpha();

    // First time put() is called this step: simple update.
    if (_nValuesStep == 0) {
      _currentValueStep = value;
      _nValuesStep = 1;
      applyMovingAverageUpdate(_value, value, a);
    }

    // This code is executed if put() is called more than one time in same step.
    // Readjust moving average: replace previous value with new value averaged over step.
    else {
      // Update values. Variable _currentValueStep is used to accumlate values as a sum.
      float prevNValuesStep;
      if (_nValuesStep < MOVING_FILTER_N_VALUES_STEP_MAX) {
        _currentValueStep += value;
        prevNValuesStep = _nValuesStep;
        _nValuesStep++;
      }
      else {
        // Add one value in proportion to the previous value.
        _currentValueStep  = MOVING_FILTER_VALUES_STEP_ADD_ONE_PROPORTION * (_currentValueStep + value);
        prevNValuesStep    = MOVING_FILTER_N_VALUES_STEP_MAX_MINUS_ONE;
      }

      // This is based on an expansion of the moving average formula.
      _value += a / (prevNValuesStep * _nValuesStep) * (_nValuesStep * value - _currentValueStep);
    }
  }

  return _value;
}

float Smoother::filter(float value) {
  // Performs one step of moving average.
  return computeMovingAverageUpdate(_value, value, alpha());
}

void Smoother::step() {
  if (isCalibrating()) {

    // If no values were added during this step, update using previous value.
    if (_nValuesStep == 0) {
      applyMovingAverageUpdate(_value, _currentValueStep, alpha());
    }

    // Otherwise: reset (but keep _currentValueStep).
    else {
      // Variable _currentValueStep is saved as an average of values; this is to ensure that if put() is not
      // called in a step, the average is preserved and will be used in the next step by triggering (_nValuesStep == 0) (above).
      _currentValueStep /= _nValuesStep;
      _nValuesStep = 0;
    }

    // Increase number of samples.
    if (_nSamples < UINT_MAX)
      _nSamples++;
  }

}

}
