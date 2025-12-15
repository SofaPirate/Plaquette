/*
 * MinMaxScaler.cpp
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

#include "MinMaxScaler.h"

#include "float.h"
#include "pq_map.h"
#include "pq_moving_average.h"

namespace pq {

MinMaxScaler::MinMaxScaler(Engine& engine)
  : MovingFilter(engine)
{
  reset();
}

MinMaxScaler::MinMaxScaler(float timeWindow, Engine& engine)
  : MovingFilter(timeWindow, engine)
{
  reset();
}

void MinMaxScaler::reset() {
  MovingFilter::reset();

  _minValue =  FLT_MAX;
  _maxValue = -FLT_MAX;
  _smoothedMinValue = 0.5f;
  _smoothedMaxValue = 0.5f;

  _value = 0.5f;

  _nSamples = 0;
}

void MinMaxScaler::reset(float estimatedMeanValue) {
  reset();

  _minValue = _maxValue = _smoothedMinValue = _smoothedMaxValue = estimatedMeanValue;
  _isPreInitialized = true;
}

void MinMaxScaler::reset(float estimatedMinValue, float estimatedMaxValue) {
  reset();

  _minValue = _smoothedMinValue = estimatedMinValue;
  _maxValue = _smoothedMaxValue = estimatedMaxValue;
  _isPreInitialized = true;
}

float MinMaxScaler::put(float value)
{
  if (isCalibrating()) {
    // Update min. value.
    if (value < _minValue) {
      _minValue = value;
      if (_nSamples == 0)
        _smoothedMinValue = _minValue;
    }

    // Update max. value.
    if (value > _maxValue) {
      _maxValue = value;
      if (_nSamples == 0)
        _smoothedMaxValue = _maxValue;
    }
  }

  return (_value = filter(value));
}

float MinMaxScaler::filter(float value) {
  // Compute rescaled value.
  return mapTo01(value, _smoothedMinValue, _smoothedMaxValue, CONSTRAIN);
}

#define MIN_MAX_SCALER_TOLERANCE 0.01
#define SMOOTHED_MIN_MAX_TIME_PROPORTION 0.1
constexpr float MIN_MAX_TIME_PROPORTION = 1.0f - SMOOTHED_MIN_MAX_TIME_PROPORTION;

constexpr float MIN_MAX_TIME_PROPORTION_INV = 1.0f / MIN_MAX_TIME_PROPORTION;
constexpr float SMOOTHED_MIN_MAX_TIME_PROPORTION_INV = 1.0f / SMOOTHED_MIN_MAX_TIME_PROPORTION;

#define SMOOTHED_MIN_MAX_MAXIMUM_TIME_WINDOW 10.0f

void MinMaxScaler::step() {
  if (isCalibrating()) {

    if (_nSamples > 0) {
      // Alpha values for updating min-max and smoothed min-max values.
      float alphaMinMax = 0;
      float alphaSmoothed = 0;

      // Precomputed values.
      bool finiteTimeWindow = !timeWindowIsInfinite();
      float midValue = 0.5f *(_minValue + _maxValue);

      // Tolerance to consider that smoothed value has reach min-max value (based on absolute range).
      float tolerance = (_maxValue - _minValue) * MIN_MAX_SCALER_TOLERANCE;

      // Apply decay on smoothed value until it reaches min/max value.
      // Then both values slowly decay towards current value.

      // Smoothed value has reached min.
      if (_smoothedMinValue == _minValue || // <-- keep this: it avoids unnecessary computation of tolerance condition
          _smoothedMinValue - _minValue < tolerance) {

        // Smoothed value has reached min: open to decaying.
        _smoothedMinValue = _minValue;

        // Decay min value towards mid value.
        if (finiteTimeWindow) {
          alphaMinMax = _alphaMinMax();
          applyMovingAverageUpdate(_minValue, midValue, alphaMinMax);
        }
      }
      // Smoothed min value is still above min value: adjust smoothed min towards min.
      else {
        alphaSmoothed = _alphaSmoothed(finiteTimeWindow);
        applyMovingAverageUpdate(_smoothedMinValue, _minValue, alphaSmoothed);
      }

      // Smoothed value has reached max..
      if (_smoothedMaxValue == _maxValue || // <-- keep this: it avoids unnecessary computation of tolerance condition
          _maxValue - _smoothedMaxValue < tolerance) {

        // Smoothed value has reached max: open to decaying.
        _smoothedMaxValue = _maxValue;

        // Decay max value towards mid value.
        if (finiteTimeWindow) {
          applyMovingAverageUpdate(_maxValue, midValue, alphaMinMax ? alphaMinMax : _alphaMinMax());
        }
      }
      // Smoothed max value is still below max value: adjust smoothed max towards max.
      else {
        applyMovingAverageUpdate(_smoothedMaxValue, _maxValue, alphaSmoothed ? alphaSmoothed : _alphaSmoothed(finiteTimeWindow));
      }
    }

    // Increase number of samples.
    if (_nSamples < UINT_MAX)
      _nSamples++;
  }
}

float MinMaxScaler::_alphaMinMax() const {
  float minMaxTimeWindow = max(_timeWindow - _smoothedTimeWindow(true), 0);
  return movingAverageAlpha(sampleRate(), minMaxTimeWindow);
}

float MinMaxScaler::_alphaSmoothed(bool finiteTimeWindow) const {
  return movingAverageAlpha(sampleRate(), _smoothedTimeWindow(finiteTimeWindow), _nSamples);
}

float MinMaxScaler::_smoothedTimeWindow(bool finiteTimeWindow) const {
  return finiteTimeWindow ? min(SMOOTHED_MIN_MAX_TIME_PROPORTION*_timeWindow, SMOOTHED_MIN_MAX_MAXIMUM_TIME_WINDOW) : SMOOTHED_MIN_MAX_MAXIMUM_TIME_WINDOW;
}

}

