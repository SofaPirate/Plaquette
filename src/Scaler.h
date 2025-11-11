/*
 * Scaler.h
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

#ifndef SCALER_H_
#define SCALER_H_

#include "PqCore.h"
#include "MovingFilter.h"

namespace pq {

/// Regularizes signal into [0,1] using adaptive quantile tracking (robust to outliers).
class Scaler : public MovingFilter {
public:
  /// Default constructor.
  Scaler(Engine& engine = Engine::primary());

  /**
   * Constructor with custom quantile levels and time window.
   * @param timeWindow The adaptation window in seconds.
   */
  Scaler(float timeWindow, Engine& engine = Engine::primary());

  /**
   * Constructor with custom quantile levels and time window.
   * @param timeWindow The adaptation window in seconds.
   * @param span Corresponds to percentage coverage of value in [0, 1].
   */
  Scaler(float timeWindow, float span, Engine& engine = Engine::primary());

  virtual ~Scaler() {}

    /// Sets time window to infinite.
  virtual void infiniteTimeWindow();

  /// Returns true if time window is infinite.
  virtual bool timeWindowIsInfinite() const;

  /// Sets the adaptation time window (in seconds).
  virtual void timeWindow(float seconds);

  /// Returns the current time window.
  virtual float timeWindow() const;

  /// Resets the filter state.
  virtual void reset();

  /// Returns value of scaler.
  virtual float get() { return _value; }

  /// Pushes a new value and returns the scaled output.
  virtual float put(float value);

protected:
  virtual void step();

  // Internal quantile update (Robbins–Monro)
  inline void updateQuantile(float& q, float alpha, float eta, float x);

//protected:
public:
  float _timeWindow;

  float _quantileLevel;

  float _lowQuantile;
  float _highQuantile;

  float _meanValue;

  // Variables used to compute current value average during a step (in case of multiple calls to put()).
  float _currentValueStep;

  uint32_t _nSamples;
};

}

#endif
