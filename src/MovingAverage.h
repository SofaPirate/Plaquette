/*
 * MovingAverage.h
 *
 * Tool for moving averages.
 *
 * This file is part of Qualia https://github.com/sofian/qualia
 *
 * (c) 2011 Sofian Audry -- info(@)sofianaudry(.)com
 * Inspired by code by Karsten Kutza
 * http://www.ip-atlas.com/pub/nap/nn-src/bpn.txt
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
#ifndef MOVINGAVERAGE_H_
#define MOVINGAVERAGE_H_

#include <Arduino.h>

namespace pq {

/// An exponential moving average class.
class MovingAverage {
protected:
  // The smoothing window (in seconds).
  float _smoothTime;

  // The current value of the exponential moving average.
  float _value;

  // Is the moving average started or not.
  bool _isStarted;

public:
  /**
   * Constructs the moving average.
   */
  MovingAverage(float smooth);
  // MovingAverage(float seconds, float startValue);
  virtual ~MovingAverage() {}

  /// Changes the smoothing window (expressed in seconds).
  void time(float seconds);

  /// Returns the smoothing window (expressed in seconds).
  float time() const { return _smoothTime; }

  /// Changes the smoothing window cutoff frequency (expressed in Hz).
  void cutoff(float hz);

  /// Returns the smoothing window cutoff frequency (expressed in Hz).
  float cutoff() const { return (1.0f/time()); }

  /// Returns the alpha value computed from given sample rate.
  float alpha(float sampleRate) const {
    // Rule of thumb: alpha = 2 / (n_samples+1).
    float a = 2 / (_smoothTime*sampleRate + 1);
    return min(a, 1.f); // make sure it does not get over 1
  }

  /// Resets the moving average.
  virtual void reset();

  // /// Resets the moving average to #startValue#.
  // virtual void reset(float startValue);

  /// Updates the moving average with new value #v# (also returns the current value).
  virtual float update(float v, float sampleRate=1, bool forceAlpha=false);

  /// Returns the value of the moving average. This is undefined if isValid() == false.
  float get() { return _value; }
  float constGet() const { return _value; }

  /// Returns true iff the moving average has already been started.
  bool isStarted() const;

  /**
   * Applies a single update on #runningValue# with new sample #newValue# and mixing
   * factor #alpha#.
   */
  static float applyUpdate(float& runningValue, float newValue, float alpha);

protected:
  void _setStarted(bool start);

};

} // namespace pq

#endif /* MOVINGAVERAGE_H_ */
