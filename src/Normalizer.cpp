/*
 * Normalizer.cpp
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

#include "Normalizer.h"
#include "MovingStats.h"
#include "pq_moving_average.h"

namespace pq {

Normalizer::Normalizer(Engine& engine)
  : Normalizer(NORMALIZER_DEFAULT_MEAN, NORMALIZER_DEFAULT_STDDEV, engine)
{}

Normalizer::Normalizer(float timeWindow, Engine& engine)
  : Normalizer(NORMALIZER_DEFAULT_MEAN, NORMALIZER_DEFAULT_STDDEV, timeWindow, engine)
{}

Normalizer::Normalizer(float mean, float stdDev, Engine& engine)
  : MovingFilter(engine), MovingStats()
{
  _init(mean, stdDev);
}

Normalizer::Normalizer(float mean, float stdDev, float timeWindow, Engine& engine)
  : MovingFilter(timeWindow, engine), MovingStats()
{
  _init(mean, stdDev);
}

void Normalizer::reset() {
  MovingFilter::reset();
  MovingStats::reset();
  _currentMeanStep = mean();
  _currentMean2Step = meanSquared();
}

void Normalizer::reset(float estimatedMeanValue) {
  MovingFilter::reset();
  MovingStats::reset(estimatedMeanValue, 1.0f);
  _currentMeanStep = mean();
  _currentMean2Step = meanSquared();
}

void Normalizer::reset(float estimatedMinValue, float estimatedMaxValue) {
  MovingFilter::reset();

  float average = 0.5f * (estimatedMinValue + estimatedMaxValue);
  float stddev = abs(estimatedMaxValue - estimatedMinValue) / MOVING_FILTER_N_STDDEV_RANGE;
  MovingStats::reset(average, stddev);
  _currentMeanStep = mean();
  _currentMean2Step = meanSquared();
}

float Normalizer::put(float value) {
  // First time put() is called this step.
  if (isCalibrating()) {

    float value2 = sq(value);
    float a = alpha();

    // First time put() is called this step: simple update.
    if (_nValuesStep == 0) {
      _currentMeanStep = value;
      _currentMean2Step = value2;
      _nValuesStep = 1;
      _mean. update(value, a);
      _mean2.update(value2, a);
    }

    // This code is executed if put() is called more than one time in same step.
    // Readjust moving average: replace previous value with new value averaged over step.
    else {
      // Update values. Variable _currentValueStep is used to accumlate values as a sum.
      float prevNValuesStep;
      if (_nValuesStep < MOVING_FILTER_N_VALUES_STEP_MAX) {
        _currentMeanStep += value;
        _currentMean2Step += value2;
        prevNValuesStep = _nValuesStep;
        _nValuesStep++;
      }
      else {
        // Add one value in proportion to the previous value.
        _currentMeanStep  = MOVING_FILTER_VALUES_STEP_ADD_ONE_PROPORTION * (_currentMeanStep  + value);
        _currentMean2Step = MOVING_FILTER_VALUES_STEP_ADD_ONE_PROPORTION * (_currentMean2Step + value2);
      }

      // This is based on an expansion of the moving average formula.
      float adjustFactor = a / (prevNValuesStep * _nValuesStep);
      _mean. delta(adjustFactor * (_nValuesStep * value  - _currentMeanStep));
      _mean2.delta(adjustFactor * (_nValuesStep * value2 - _currentMean2Step));
    }
  }

  // Normalize value to target normal.
  return (_value = filter(value));
}

float Normalizer::filter(float value) {
  // Normalize value to target normal.
  value = normalize(value, _targetMean, _targetStdDev);

  // Check for clamp.
  return (isClamped() ? _clamp(value) : value);
}

void Normalizer::step() {
  if (isCalibrating()) {

    // If no values were added during this step, update using previous value.
    // In other words: repeat update with previous value.
    if (_nValuesStep > 0) {
      _currentMeanStep  /= _nValuesStep;
      _currentMean2Step /= _nValuesStep;
      _nValuesStep = 0;
    }

    // Compute base alpha.
    float a = alpha();

    // Update statistics.
    _mean.update(_currentMeanStep, a);
    _mean2.update(_currentMean2Step, a);

    // Increase number of samples.
    if (_nSamples < UINT_MAX)
      _nSamples++;
  }
}

float Normalizer::lowOutlierThreshold(float nStdDev) const {
  return targetMean() - abs(nStdDev) * targetStdDev();
}

float Normalizer::highOutlierThreshold(float nStdDev) const  {
  return targetMean() + abs(nStdDev) * targetStdDev();
}

bool Normalizer::isClamped() const {
  return (_clampStdDev != NORMALIZER_NO_CLAMP);
}

void Normalizer::clamp(float nStdDev) {
  _clampStdDev = abs(nStdDev);
}

void Normalizer::noClamp() {
  _clampStdDev = NORMALIZER_NO_CLAMP;
}

void Normalizer::_init(float mean, float stdDev) {
  _value = mean;

  _currentMeanStep  = mean;
  _currentMean2Step = sq(stdDev) + sq(mean);

  targetMean(mean);
  targetStdDev(stdDev);

  clamp();
}

float Normalizer::_clamp(float value) const {
  float absStdDevOutlier = _clampStdDev * targetStdDev();
  return constrain(value, _targetMean - absStdDevOutlier, _targetMean + absStdDevOutlier);
}

float Normalizer::mapTo(float toLow, float toHigh) {
  float absStdDevOutlier = _targetStdDev *
    (_clampStdDev == NORMALIZER_NO_CLAMP ? NORMALIZER_DEFAULT_CLAMP_STDDEV : _clampStdDev);

  return mapFloat(get() - _targetMean, -absStdDevOutlier, absStdDevOutlier, toLow, toHigh);
}

}
