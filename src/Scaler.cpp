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

// Default low quantile level (corresponds to 1% coverage of value in [0, 1]).
#define SCALER_DEFAULT_SPAN 0.99f

// Minimum quantile level to avoid ill-defined zero quantile.
#define SCALER_MINIMUM_QUANTILE_LEVEL 1e-4f

// Internal use to avoid division by zero.
#define SCALER_ONE_PLUS_SMALL_VALUE 1.0001f

Scaler::Scaler(Engine& engine) : Scaler(MOVING_FILTER_INFINITE_TIME_WINDOW, engine) {}

Scaler::Scaler(float timeWindow, Engine& engine) : Scaler(timeWindow, SCALER_DEFAULT_SPAN, engine) {}

Scaler::Scaler(float timeWindow_, float span, Engine& engine)
  : MovingFilter(engine),
    _currentValueStep(0)
{
  timeWindow(timeWindow_);
  _quantileLevel = max(0.5f * (1 - constrain01(span)), SCALER_MINIMUM_QUANTILE_LEVEL);
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

    if (_nSamples == 0) {
      _lowQuantile = _highQuantile = value;
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


void Scaler::step() {

    // If no values were added during this step, update using previous value.
  if (_nValuesStep > 0 ||      // if at least one value was recorded this step ...
      _lowQuantile != FLT_MAX) {  // ... or at least one value was ever recorded since reset

    // Reset (but keep _currentValueStep).
    _nValuesStep = 0;

    // Compute an adaptive scale factor for the Robbins–Monro updates.
    //
    // The goal is to estimate the "total range" of the underlying signal using
    // only q_low, q_high, and the current value x.  Normally, when x lies between
    // the two quantiles, we can approximate the full range as:
    //
    //      range_est ≈ (q_high - q_low) / (1 - 2*quantileLevel)
    //
    // The division by (1 - 2*quantileLevel) is used to spread the range from
    // |q_high - q_low| to |q_100 - q_0|.
    //
    // However, if the quantiles are almost equal (as is the case at startup) this estimate
    // is extremely small and causes the RM updates to converge very slowly.
    //
    // To avoid this, if the new sample x lies *outside* the current quantile band,
    // we use the distance from x to the opposite quantile as a proxy for the
    // missing range.  This immediately "inflates" the scale when a new extreme
    // value appears, allowing q_low and q_high to separate quickly.
    //
    // Summary:
    //    - If x > q_high:    range = (x -  q_low) / (1 - 1.5*quantileLevel)  (upper excursion)
    //    - If x < q_low:     range = (q_high - x) / (1 - 1.5*quantileLevel) (lower excursion)
    //    - Else:             range = range_est (normal quantile-based update)
    float range;
    if (_currentValueStep > _highQuantile)
      range = (_currentValueStep - _lowQuantile)  / (SCALER_ONE_PLUS_SMALL_VALUE - 1.5f*_quantileLevel);

    else if (_currentValueStep < _lowQuantile)
      range = (_highQuantile - _currentValueStep) / (SCALER_ONE_PLUS_SMALL_VALUE - 1.5f*_quantileLevel);

    else
      range = (_highQuantile - _lowQuantile)      / (SCALER_ONE_PLUS_SMALL_VALUE - 2*_quantileLevel);

    // Compute eta for Robbins–Monro updates, rescaled using range adjustment.
    float eta = MovingAverage::alpha(sampleRate(), _timeWindow, _nSamples) * range;

    // Update quantiles (Robbins–Monro online).
    _updateQuantile(_lowQuantile,      _quantileLevel, eta, _currentValueStep);
    _updateQuantile(_highQuantile, 1 - _quantileLevel, eta, _currentValueStep);

    // Clamp quantiles to avoid inversions.
    if (_lowQuantile > _highQuantile)
      _lowQuantile = _highQuantile = (_lowQuantile + _highQuantile) * 0.5f;

    // Increase number of samples.
    if (_nSamples < UINT_MAX)
      _nSamples++;
  }

}

}
