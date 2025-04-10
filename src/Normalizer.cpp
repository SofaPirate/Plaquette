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

#include "MovingStats.h"
#include "Normalizer.h"

namespace pq {

Normalizer::Normalizer(Engine& engine)
  : MovingFilter(engine), MovingStats()
{
  _init(NORMALIZER_DEFAULT_MEAN, NORMALIZER_DEFAULT_STDDEV);
}

Normalizer::Normalizer(float timeWindow, Engine& engine)
  : MovingFilter(engine), MovingStats(timeWindow)
{
  _init(NORMALIZER_DEFAULT_MEAN, NORMALIZER_DEFAULT_STDDEV);
}

Normalizer::Normalizer(float mean, float stdDev, Engine& engine)
  : MovingFilter(engine), MovingStats()
{
  _init(mean, stdDev);
}

Normalizer::Normalizer(float mean, float stdDev, float timeWindow, Engine& engine)
  : MovingFilter(engine), MovingStats(timeWindow)
{
  _init(mean, stdDev);
}

void Normalizer::infiniteTimeWindow() {
  MovingStats::infiniteTimeWindow();
}

void Normalizer::timeWindow(float seconds) {
  MovingStats::timeWindow(seconds);
}

float Normalizer::timeWindow() const { return MovingStats::timeWindow(); }

bool Normalizer::timeWindowIsInfinite() const {
  return MovingStats::timeWindowIsInfinite();
}

void Normalizer::reset() {
  MovingStats::reset();
  MovingFilter::reset();
}

float Normalizer::put(float value) {
  // Increment n. values.
  if (_nValuesStep < 128)
    _nValuesStep++;

  // First time put() is called this step.
  if (isCalibrating()) {

    if (_nValuesStep == 1) {
      // Update moving average.
      _value = update(value, sampleRate());
    }
    // If put() is called more than one time in same step, readjust moving average.
    else {
      // Save previous value.
      float prevValueStep = _currentMeanStep;
      float prevValue2Step = _currentMean2Step;

      // Update current step average value.
      float stepValuesAlpha = 1.0f/_nValuesStep;
      MovingAverage::applyUpdate(_currentMeanStep, value, stepValuesAlpha);
      MovingAverage::applyUpdate(_currentMean2Step, sq(value), stepValuesAlpha);

      // Update moving average: replace previous value with new value averaged over step.
      float alpha = _avg.alpha(sampleRate());
      _avg.amendUpdate(prevValueStep, _currentMeanStep, alpha, true);
      MovingAverage::applyAmendUpdate(_mean2, prevValue2Step, _currentMean2Step, alpha);

      _value = normalize(value);
    }
  }

  else
    _value = normalize(value);

  // Normalize value to target normal.
  _value = _value * _targetStdDev + _targetMean;
  
  // Check for clamp.
  if (isClamped())
    _value = _clamp(_value);

  return _value;
}

void Normalizer::step() {
  // If no values were added during this step, update using previous value.
  // In other words: repeat update with previous value.
  if (_nValuesStep == 0) {
    float alpha = _avg.alpha(sampleRate());
    _avg.update(_currentMeanStep, alpha, true); 
    MovingAverage::applyUpdate(_mean2, _currentMean2Step, alpha);
  }
  // Otherwise: reset (but keep _currentMeanStep).
  else
    _nValuesStep = 0;
}

float Normalizer::update(float value, float sampleRate)
{
  // Get alpha.
  float alpha = _avg.alpha(sampleRate);

  // Update average.
  _avg.update(value, alpha, true); // force alpha
  _currentMeanStep = value;

  // Update variance.
  _currentMean2Step = sq(value);
  MovingAverage::applyUpdate(_mean2, _currentMean2Step, alpha);

  return normalize(value);
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

  _currentMeanStep = mean;
  _currentMean2Step = sq(stdDev) - sq(mean);

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
