/*
 * Scaler.cpp
 *
 * (c) 2025 Sofian Audry        :: info(@)sofianaudry(.)com
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
 *
 * (c) 2025 Sofian Audry
 *
 * Adaptive quantile-based scaler using Robbins–Monro updates.
 */

#include "Scaler.h"
#include "MovingAverage.h"

namespace pq {

// Minimum quantile level to avoid ill-defined zero quantile.
#define SCALER_MINIMUM_QUANTILE_LEVEL 1e-4f
#define SCALER_MAXIMUM_QUANTILE_LEVEL 0.5f

// Number of standard deviations to cover full range.
#define SCALER_STDDEV_TO_RANGE 6.0f

float lowQuantileLevelToSpan(float level) {
  return level <= SCALER_MINIMUM_QUANTILE_LEVEL ? 1.0f : 1 - 2 * level;
 }

float spanToLowQuantileLevel(float span) {
  return max( mapFrom01(1-span, 0, SCALER_MAXIMUM_QUANTILE_LEVEL), SCALER_MINIMUM_QUANTILE_LEVEL);
}

Scaler::Scaler(Engine& engine) : MovingFilter(engine) {
  infiniteTimeWindow();
  span(SCALER_DEFAULT_SPAN);
  reset();
}

Scaler::Scaler(float timeWindow, Engine& engine) : Scaler(timeWindow, SCALER_DEFAULT_SPAN, engine) {}

Scaler::Scaler(float timeWindow_, float span_, Engine& engine)
  : MovingFilter(engine),
    _currentValueStep(0)
{
  timeWindow(timeWindow_);
  span(span_);
  reset();
}

void Scaler::infiniteTimeWindow() {
  _timeWindow = MOVING_FILTER_INFINITE_TIME_WINDOW;
}

bool Scaler::timeWindowIsInfinite() const {
  return _timeWindow == MOVING_FILTER_INFINITE_TIME_WINDOW;
}

void Scaler::timeWindow(float seconds) {
  _timeWindow = max(seconds, 0.0f); // make sure it is positive
}

float Scaler::timeWindow() const { return _timeWindow; }

void Scaler::span(float span) {
  _quantileLevel = spanToLowQuantileLevel(constrain01(span));
}

float Scaler::span() const {
  return lowQuantileLevelToSpan(_quantileLevel);
}

void Scaler::lowQuantileLevel(float level) {
  level = constrain(level, 0.0f, SCALER_MAXIMUM_QUANTILE_LEVEL);
  span(lowQuantileLevelToSpan(level));
}

void Scaler::highQuantileLevel(float level) {
  lowQuantileLevel(1 - level);
}

void Scaler::reset() {
  MovingFilter::reset();

  _lowQuantile = FLT_MAX;
  _highQuantile = -FLT_MAX;

  _value = 0.5f;

  _currentValueStep = 0;
  _nValuesStep = 0;
  _nSamples = 0;
}

void Scaler::_updateQuantile(float& q, float level, float eta, float x) {
  // Stochastic update rule for quantile (Robbins–Monro).
  q += eta * (level - (x <= q ? 1.0f : 0.0f));
}

float Scaler::put(float value) {
  if (isCalibrating()) {


    // Increment n. values.
    if (_nValuesStep < 128)
      _nValuesStep++;

    if (_nValuesStep == 1) {
      // Save current value.
      _currentValueStep = value;
    }
    else {
      // Update current step average value.
      MovingAverage::applyUpdate(_currentValueStep, value, 1.0f/_nValuesStep);
    }
  }

  // Initialize.
  if (_nSamples == 0) {
    _lowQuantile = _highQuantile = _currentValueStep;
  }

  // Compute rescaled value.
  _value = mapTo01(value, _lowQuantile, _highQuantile, CONSTRAIN);

  return _value;
}

void Scaler::step() {

    // If no values were added during this step, update using previous value.
  if (_nValuesStep > 0 ||      // if at least one value was recorded this step ...
      _lowQuantile != FLT_MAX) {  // ... or at least one value was ever recorded since reset

    // Reset (but keep _currentValueStep).
    _nValuesStep = 0;

    float alpha = MovingAverage::alpha(sampleRate(), _timeWindow, _nSamples);

    // Unbiased estimate of standard deviation of the signal using current value.
    float midQuantile = 0.5f * (_lowQuantile + _highQuantile);
    float deviation = abs(_currentValueStep - midQuantile);

    if (_nSamples == 0)
      _stddev = deviation;
    else
      MovingAverage::applyUpdate(_stddev, deviation, alpha);

    // Compute eta for Robbins–Monro updates, rescaled using range adjustment.
    float eta = MovingAverage::alpha(sampleRate(), _timeWindow, _nSamples);
    eta *= SCALER_STDDEV_TO_RANGE * _stddev; // rescale to full range

    // Precompute: eta x quantile level
    float etaLevel = eta * _quantileLevel;

    // Update quantiles (Robbins–Monro online).
    if (_currentValueStep <= _lowQuantile) { // smaller than both quantiles
      _lowQuantile  -= eta - etaLevel; // decrease
      _highQuantile -= etaLevel;       // decrease
      // Prevent overshooting.
      _lowQuantile  = max(_lowQuantile,  _currentValueStep);
      _highQuantile = max(_highQuantile, _currentValueStep);
    }
    else if (_currentValueStep <= _highQuantile) { // in between
      _lowQuantile  += etaLevel;       // increase
      _highQuantile -= etaLevel;       // decrease
      // Prevent overshooting.
      _lowQuantile  = min(_lowQuantile,  _currentValueStep);
      _highQuantile = max(_highQuantile, _currentValueStep);
    }
    else { // larger than both quantiles
      _lowQuantile  += etaLevel;       // increase
      _highQuantile += eta - etaLevel; // increase
      // Prevent overshooting.
      _lowQuantile  = min(_lowQuantile,  _currentValueStep);
      _highQuantile = min(_highQuantile, _currentValueStep);
    }


    // Clamp quantiles to avoid inversions.
    if (_lowQuantile > _highQuantile)
      _lowQuantile = _highQuantile = 0.5f * (_lowQuantile + _highQuantile);

    // Increase number of samples.
    if (_nSamples < UINT_MAX)
      _nSamples++;
  }

}

}
