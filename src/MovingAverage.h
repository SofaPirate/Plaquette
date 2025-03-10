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
#include <limits.h>
#include <float.h>

namespace pq {

/// An exponential moving average class.
class MovingAverage {
public:
  /// Default constructor (infinite time window).
  MovingAverage();

  /// Default constructor (finite time window).
  MovingAverage(float timeWindow);

  virtual ~MovingAverage() {}

  /// Sets to "infinite" smoothing window.
  void infiniteTimeWindow();

  /// Changes the smoothing window (expressed in seconds).
  void timeWindow(float seconds);

  /// Returns the smoothing window (expressed in seconds).
  float timeWindow() const { return _smoothTime; }

  /// Returns true if time window is infinite.
  bool timeWindowIsInfinite() const;

  /// Changes the smoothing window cutoff frequency (expressed in Hz).
  void cutoff(float hz);

  /// Returns the smoothing window cutoff frequency (expressed in Hz).
  float cutoff() const;

  /// Returns the alpha value computed from given sample rate.
  float alpha(float sampleRate) const;

  /// Resets the moving average.
  void reset();

  /// Updates the moving average with new value #v# (also returns the current value).
  virtual float update(float v, float sampleRate=1, bool forceAlpha=false);

  /// Returns the value of the moving average. This is undefined if isValid() == false.
  float get() { return _value; }
  float constGet() const { return _value; }

  /// Returns the number of samples processed since reset().
  unsigned int nSamples() const { return _nSamples; }

  /// Performs an amendment of latest update (needs to be called with same #sampleRate# and #forceAlpha# parameters).
  void amendUpdate(float previousValue, float newValue, float sampleRate=1, bool forceAlpha=false);

  /**
   * Applies a single update on #runningValue# with new sample #newValue# and mixing
   * factor #alpha#.
   */
  static void applyUpdate(float& runningValue, float newValue, float alpha);

  /// Performs an amendment of latest update (needs to be called with same #alpha# parameter).
  static void applyAmendUpdate(float& runningValue, float previousValue, float newValue, float alpha);

  /**
   * Computes a single update on #runningValue# with new sample #newValue# and mixing
   * factor #alpha#.
   */
  static float computeUpdate(float runningValue, float newValue, float alpha);

  /// Returns the alpha value computed from given sample rate, time window, and number of samples.
  static float alpha(float sampleRate, float timeWindow, unsigned int nSamples=UINT_MAX);

protected:
  // The smoothing window (in seconds).
  float _smoothTime;

  // The current value of the exponential moving average.
  float _value;

  // Number of samples that have been processed thus far.
  unsigned int _nSamples;
};

} // namespace pq

#endif /* MOVINGAVERAGE_H_ */
