#include "Scaler.h"
#include "MovingAverage.h"
#include "pq_map.h"
#include "float.h"
#include <math.h>

namespace pq {

#define SCALER_DEFAULT_SPAN 0.99f

Scaler::Scaler(Engine& engine) : Scaler(MOVING_FILTER_INFINITE_TIME_WINDOW, engine) {}

Scaler::Scaler(float timeWindow, Engine& engine) : Scaler(timeWindow, SCALER_DEFAULT_SPAN, engine) {}

Scaler::Scaler(float timeWindow_, float span, Engine& engine)
  : MovingFilter(engine),
    _currentValueStep(0)
{
  timeWindow(timeWindow_);
  _quantileLevel = max(0.5f * (1 - constrain01(span)), 0.0001f);
  reset();
}

void Scaler::infiniteTimeWindow() {
  _timeWindow = MOVING_FILTER_INFINITE_TIME_WINDOW;
}

void Scaler::timeWindow(float seconds) {
  _timeWindow = max(seconds, 0.0f); // make sure it is positive
}

float Scaler::timeWindow() const { return _timeWindow; }

bool Scaler::timeWindowIsInfinite() const {
  return _timeWindow == MOVING_FILTER_INFINITE_TIME_WINDOW;
}
void Scaler::reset() {
  MovingFilter::reset();

  _lowQuantile = FLT_MAX;
  _highQuantile = -FLT_MAX;

  _value = _meanValue = 0.5f;

  _currentValueStep = 0;
  _nValuesStep = 0;
  _nSamples = 0;
}

void Scaler::updateQuantile(float& q, float alpha, float eta, float x) {
  float indicator = (x <= q) ? 1.0f : 0.0f;
  q += eta * (alpha - indicator);
}

float Scaler::put(float value) {
  if (isCalibrating()) {

    if (_nSamples == 0) {
      _lowQuantile = _highQuantile = _meanValue = value;
    }

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

  // Compute rescaled value.
  _value = mapTo01(value, _lowQuantile, _highQuantile, CONSTRAIN);

  return _value;
}

constexpr float ONE_PLUS_FLT_MIN = 1 + FLT_MIN;
#define MINIMUM_ETA_SCALE 1e-5f

void Scaler::step() {

    // If no values were added during this step, update using previous value.
  if (_nValuesStep > 0 ||      // if at least one value was recorded this step ...
      _lowQuantile != FLT_MAX) {  // ... or at least one value was ever recorded since reset

    // Reset (but keep _currentValueStep).
    _nValuesStep = 0;

    //
    float alpha = MovingAverage::alpha(sampleRate(), _timeWindow, _nSamples);

    // Update average value.
    MovingAverage::applyUpdate(_meanValue, _currentValueStep, alpha);

    // Compute range: absolute range between high and low quantiles, rescaled from quantile level to full range.
    float scale = (abs(_highQuantile - _meanValue) + abs(_meanValue - _lowQuantile)) /
                    (ONE_PLUS_FLT_MIN - 2*_quantileLevel);

    float scaledAlpha = alpha * max(scale, MINIMUM_ETA_SCALE);

    // Update quantiles (Robbins–Monro online).
    updateQuantile(_lowQuantile,      _quantileLevel, scaledAlpha, _currentValueStep);
    updateQuantile(_highQuantile, 1 - _quantileLevel, scaledAlpha, _currentValueStep);

    // Increase number of samples.
    if (_nSamples < UINT_MAX)
      _nSamples++;
  }

}

}
