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
  infiniteTimeWindow();
  _init(mean, stdDev);
}

Normalizer::Normalizer(float mean, float stdDev, float timeWindow_, Engine& engine)
  : MovingFilter(engine), MovingStats()
{
  timeWindow(timeWindow_);
  _init(mean, stdDev);
}

void Normalizer::reset() {
  MovingFilter::reset();
  MovingStats::reset();
}

void Normalizer::reset(float estimatedMeanValue) {
  MovingFilter::reset();
  MovingStats::reset(estimatedMeanValue, 1.0f);
}

void Normalizer::reset(float estimatedMinValue, float estimatedMaxValue) {
  MovingFilter::reset();

  float mean = 0.5f * (estimatedMinValue + estimatedMaxValue);
  float stddev = abs(estimatedMaxValue - estimatedMinValue) / MOVING_FILTER_N_STDDEV_RANGE;
  MovingStats::reset(mean, stddev);
}

float Normalizer::put(float value) {
  // First time put() is called this step.
  if (isCalibrating()) {

    float value2 = sq(value);

    // Increment n. values.
    if (_nValuesStep == 0) {
      _currentMeanStep  = _currentMean2Step = 0;
    }

    if (_nValuesStep < MOVING_FILTER_N_VALUES_STEP_MAX) {
      _currentMeanStep  += value;
      _currentMean2Step += value2;
      _nValuesStep++;
    }
    else {
      // Add one value in proportion to the previous value.
      _currentMeanStep  = MOVING_FILTER_VALUES_STEP_ADD_ONE_PROPORTION * (_currentMeanStep  + value);
      _currentMean2Step = MOVING_FILTER_VALUES_STEP_ADD_ONE_PROPORTION * (_currentMean2Step + value2);
    }

    // Serial.println("-=------");
    // Serial.print("value: "); Serial.println(value);
    // Serial.print("value2: "); Serial.println(value2);
    // Serial.print("currentMeanStep: "); Serial.println(_currentMeanStep);
    // Serial.print("currentMean2Step: "); Serial.println(_currentMean2Step);

    // if (_nValuesStep == 1) {
    //     // Update moving average.
    //     _avg.update(value, alpha, true); // force alpha
    //     _currentMeanStep = value;

    //     // Update variance.
    //     _currentMean2Step = sq(value);
    //     applyMovingAverageUpdate(_mean2, _currentMean2Step, alpha);
    // }
    // // If put() is called more than one time in same step, readjust moving average.
    // else {
    //   // Save previous value.
    //   float prevValueStep = _currentMeanStep;
    //   float prevValue2Step = _currentMean2Step;

    //   // Update current step average value.
    //   float stepValuesAlpha = 1.0f/_nValuesStep;
    //   applyMovingAverageUpdate(_currentMeanStep, value, stepValuesAlpha);
    //   applyMovingAverageUpdate(_currentMean2Step, sq(value), stepValuesAlpha);

    //   // Update moving average: replace previous value with new value averaged over step.
    //   _avg.amendUpdate(prevValueStep, _currentMeanStep, alpha, true);
    //   MovingAverage::applyAmendUpdate(_mean2, prevValue2Step, _currentMean2Step, alpha);
    // }
  }

  // Normalize value to target normal.
  _value = normalize(value, _targetMean, _targetStdDev);


  // Check for clamp.
  if (isClamped())
    _value = _clamp(_value);

  return _value;
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
    _avg.update(_currentMeanStep, a);
    applyMovingAverageUpdate(_mean2, _currentMean2Step, a);

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
