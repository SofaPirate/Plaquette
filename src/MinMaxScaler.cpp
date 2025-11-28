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
  infiniteTimeWindow();
  reset();
}

MinMaxScaler::MinMaxScaler(float timeWindow_, Engine& engine)
  : MovingFilter(engine)
{
  timeWindow(timeWindow_);
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

  // Compute rescaled value.
  _value = mapTo01(value, _smoothedMinValue, _smoothedMaxValue, CONSTRAIN);

  return _value;
}

#define MIN_MAX_SCALER_TOLERANCE 1e-3
#define SMOOTHED_MIN_MAX_TIME_PROPORTION 0.1
constexpr float MIN_MAX_TIME_PROPORTION = 1.0f - SMOOTHED_MIN_MAX_TIME_PROPORTION;

constexpr float MIN_MAX_TIME_PROPORTION_INV = 1.0f / MIN_MAX_TIME_PROPORTION;
constexpr float SMOOTHED_MIN_MAX_TIME_PROPORTION_INV = 1.0f / SMOOTHED_MIN_MAX_TIME_PROPORTION;

void MinMaxScaler::step() {
  if (isCalibrating()) {

    if (_nSamples > 0) {
      // Compute alpha to slowly move min and max values towards current value.
      float alpha = 0;
      float alphaMinMax = 0;
      float alphaSmoothed = 0;
      bool finiteTimeWindow = !timeWindowIsInfinite();

      float midValue = 0.5f *(_minValue + _maxValue);

      float range = abs(_maxValue - _minValue);
      float tolerance = range * MIN_MAX_SCALER_TOLERANCE;

      // Apply decay on smoothed value until it reaches min/max value.
      // Then both values slowly decay towards current value.

      // Serial.println("------");
      // Serial.print("min: "); Serial.println(_minValue);
      // Serial.print("max: "); Serial.println(_maxValue);
      // Serial.print("smoothed min: "); Serial.println(_smoothedMinValue);
      // Serial.print("smoothed max: "); Serial.println(_smoothedMaxValue);

      // Smoothed value has reached min.
      if (_smoothedMinValue == _minValue || // <-- keep this: it avoids unnecessary computation of tolerance condition
          abs(_smoothedMinValue - _minValue) < tolerance) {

        // Smoothed value has reached min: open to decaying.
        _smoothedMinValue = _minValue;

        // Decay min value towards mid value.
        if (finiteTimeWindow) {
          alpha = movingAverageAlpha(sampleRate(), _timeWindow, _nSamples);
          alphaMinMax = alpha * MIN_MAX_TIME_PROPORTION;
          applyMovingAverageUpdate(_minValue, midValue, alphaMinMax);
        }
      }
      // Smoothed min value is still above min value: adjust smoothed min towards min.
      else {
        alpha = alpha ? alpha : movingAverageAlpha(sampleRate(), _timeWindow, _nSamples);
        alphaSmoothed = alpha * SMOOTHED_MIN_MAX_TIME_PROPORTION;

        applyMovingAverageUpdate(_smoothedMinValue, _minValue, alphaSmoothed);
      }

      // Smoothed value has reached max..
      if (_smoothedMaxValue == _maxValue || // <-- keep this: it avoids unnecessary computation of tolerance condition
        abs(_smoothedMaxValue - _maxValue) < tolerance) {

        // Smoothed value has reached max: open to decaying.
        _smoothedMaxValue = _maxValue;

        // Decay max value towards mid value.
        if (finiteTimeWindow) {
          if (!alpha) {
            alpha = movingAverageAlpha(sampleRate(), _timeWindow, _nSamples);
            alphaMinMax = alpha * MIN_MAX_TIME_PROPORTION;
          }
          applyMovingAverageUpdate(_maxValue, midValue, alphaMinMax);
        }
      }
      // Smoothed max value is still below max value: adjust smoothed max towards max.
      else {
        alpha = alpha ? alpha : movingAverageAlpha(sampleRate(), _timeWindow, _nSamples);
        alphaSmoothed = alphaSmoothed ? alphaSmoothed : alpha * SMOOTHED_MIN_MAX_TIME_PROPORTION;
        applyMovingAverageUpdate(_smoothedMaxValue, _maxValue, alphaSmoothed);
      }

      //     Serial.println("---->>>");
      // Serial.print("min: "); Serial.println(_minValue);
      // Serial.print("max: "); Serial.println(_maxValue);
      // Serial.print("smoothed min: "); Serial.println(_smoothedMinValue);
      // Serial.print("smoothed max: "); Serial.println(_smoothedMaxValue);
    }

    // Increase number of samples.
    if (_nSamples < UINT_MAX)
      _nSamples++;
    }
  }
}
