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

// Cheap inverse normal approximation for 0<p<1 (float, AVR/ESP32 friendly)
static float normalQuantileFast(float p)
{
  if (p <= 0.0f) return -6.0f;  // "far tail enough"
  if (p >= 1.0f) return  6.0f;

  float x  = 2.0f * p - 1.0f;   // map to [-1..1]
  float x2 = x * x;
  return 1.4142f * x * (1.0f + 0.147f * x2);  // ≈ Φ⁻¹
}

void Scaler::_initializeRange(float minValue, float maxValue)
{
  // Swap if user accidentally inverted bounds
  if (minValue > maxValue) {
    float tmp = minValue;
    minValue  = maxValue;
    maxValue  = tmp;
  }

  // Degenerate case: collapse to a single value
  if (minValue == maxValue) {
    _lowQuantile  = _highQuantile = minValue;
    return;
  }

  // --- Gaussian Prior Assumption ------------------------------------
  // The provided range [min,max] corresponds to ±3σ around the mean μ.
  //    μ = (min + max)/2
  //    σ = (max - min)/6

  const float mean    = 0.5f * (minValue + maxValue);
  const float stddev = (maxValue - minValue) / 6.0f;

  // --- Convert user's quantile level (τ) to a truncated Gaussian quantile
  // We map τ into [Φ(-3), Φ(3)] and compute a Z-score using a fast approximation.
  constexpr float PHI_NEGATIVE_3 = 0.001349898f;  // Φ(-3)
  constexpr float PHI_POSITIVE_3 = 0.998650102f;  // Φ(3)

  // Map τ onto truncated support
  const float pLow = PHI_NEGATIVE_3 + _quantileLevel * (PHI_POSITIVE_3 - PHI_NEGATIVE_3);
  const float zLow = normalQuantileFast(pLow);  // lightweight probit

  // Symmetric poles (τ and 1−τ)
  const float variation = stddev * zLow;
  _lowQuantile  = mean + variation;
  _highQuantile = mean - variation;
}

void Scaler::reset() {
  MovingFilter::reset();

  _lowQuantile = _highQuantile = 0.5f;

  _value = 0.5f;
  _stddev = 0.0f;

  _currentValueStep = 0;
  _nSamples = 0;
}

void Scaler::reset(float estimatedMeanValue) {
  reset();

  _lowQuantile = _highQuantile = estimatedMeanValue;
  _isPreInitialized = true;
}


void Scaler::reset(float estimatedMinValue, float estimatedMaxValue) {
  reset();

  _initializeRange(estimatedMinValue, estimatedMaxValue);
  _stddev = (estimatedMaxValue - estimatedMinValue) / SCALER_STDDEV_TO_RANGE;
  _isPreInitialized = true;
}

void Scaler::_updateQuantile(float& q, float level, float eta, float x) {
  // Stochastic update rule for quantile (Robbins–Monro).
  q += eta * (level - (x <= q ? 1.0f : 0.0f));
}

float Scaler::put(float value) {
  if (isCalibrating()) {

    // Increment n. values.
    if (_nValuesStep <= MOVING_FILTER_N_VALUES_STEP_MAX) {
      _nValuesStep++;
      _currentValueStep += value;
    }
    else {
      // Add one value in proportion to the previous value.
      _currentValueStep = MOVING_FILTER_VALUES_STEP_ADD_ONE_PROPORTION * (_currentValueStep + value);
    }
  }

  // Compute rescaled value.
  _value = mapTo01(value, _lowQuantile, _highQuantile, CONSTRAIN);

  return _value;
}

void Scaler::step() {

  // If no values were added during this step, update using previous value.
  if (_nValuesStep > 0) {

    // Reset.
    _currentValueStep /= _nValuesStep;
    _nValuesStep = 0;

    // Unbiased estimate of standard deviation of the signal using current value.
    float midQuantile = 0.5f * (_lowQuantile + _highQuantile);
    float deviation = abs(_currentValueStep - midQuantile);

    // Compute base alpha.
    float alpha = MovingAverage::alpha(sampleRate(), _timeWindow, _nSamples, isPreInitialized());

    if (!isPreInitialized() && _nSamples == 0) {
      _stddev = deviation;
      _lowQuantile = _highQuantile = _currentValueStep;
    }
    else
      // Adjust standard deviation.
      MovingAverage::applyUpdate(_stddev, deviation, alpha);


    // Compute eta for Robbins–Monro updates, rescaled using range adjustment.
    float eta = alpha * SCALER_STDDEV_TO_RANGE * _stddev; // rescale to full range

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

    // Decay quantiles.
    if (!timeWindowIsInfinite()) {
      MovingAverage::applyUpdate(_lowQuantile,  midQuantile,  alpha);
      MovingAverage::applyUpdate(_highQuantile, midQuantile,  alpha);
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
