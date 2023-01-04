/*
 * PqInputs.h
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

#ifndef PQ_INPUTS_H_
#define PQ_INPUTS_H_

#include "PqCore.h"
#include "MovingAverage.h"

#define DEBOUNCE_STABLE        0
#define DEBOUNCE_LOCK_OUT      1
#define DEBOUNCE_PROMPT_DETECT 2
#define DEBOUNCE_DEFAULT       DEBOUNCE_STABLE

namespace pq {

/// Superclass for components that can be smoothed.
class Smoothable {
public:
  Smoothable(float smoothTime=PLAQUETTE_NO_SMOOTH_WINDOW);

  /// Apply smoothing to object.
  virtual void smooth(float smoothTime=PLAQUETTE_DEFAULT_SMOOTH_WINDOW) { timeWindow(smoothTime); }

  /// Remove smoothing.
  virtual void noSmooth() { smooth(PLAQUETTE_NO_SMOOTH_WINDOW); }

  /// Changes the smoothing window (expressed in seconds).
  virtual void timeWindow(float seconds) { _avg.timeWindow(seconds); }

  /// Returns the smoothing window (expressed in seconds).
  float timeWindow() const { return _avg.timeWindow(); }

  /// Changes the smoothing window cutoff frequency (expressed in Hz).
  virtual void cutoff(float hz) { _avg.cutoff(hz); }

  /// Returns the smoothing window cutoff frequency (expressed in Hz).
  float cutoff() const { return _avg.cutoff(); }

protected:
  // Raw read function.
  virtual float _read() = 0;

  // Resets smoothing.
  virtual void _begin();

  // Performs update based on value returned by read().
  virtual void _step();

	// Returns smoothed value.
  virtual float _smoothed() { return _avg.get(); }

  // The moving average.
  MovingAverage _avg;
};

/// Superclass for components that can be debounced.
class Debounceable {
public:
  Debounceable(float debounceTime=PLAQUETTE_NO_DEBOUNCE_WINDOW, uint8_t mode=DEBOUNCE_DEFAULT);

  /// Apply smoothing to object.
  virtual void debounce(float debounceTime=PLAQUETTE_DEFAULT_DEBOUNCE_WINDOW) { timeWindow(debounceTime); }

  /// Remove smoothing.
  virtual void noDebounce() { debounce(PLAQUETTE_NO_DEBOUNCE_WINDOW); }

  /// Deprecated. Left for backwards compatibility.
  virtual void smooth(float smoothTime=PLAQUETTE_DEFAULT_DEBOUNCE_WINDOW) { debounce(smoothTime); }

  /// Remove smoothing.
  virtual void noSmooth() { noDebounce(); }

  /// Changes the debouncing window (expressed in seconds).
  virtual void timeWindow(float seconds) { _interval = seconds; }

  /// Returns the debouncing window (expressed in seconds).
  float timeWindow() const { return _interval; }

  /// Returns the debounce mode.
  uint8_t debounceMode() const { return _debounceMode; }

  /**
   * Sets debounce mode.
   * @param mode the debounce mode (DEBOUNCE_DEFAULT, DEBOUNCE_LOCK_OUT or DEBOUNCE_PROMPT_DETECT)
   */
  void debounceMode(uint8_t mode) { _debounceMode = mode; }

protected:
  // Raw read function.
  virtual bool _isOn() = 0;

  // Resets debouncing.
  virtual void _begin();

  // Performs update based on value returned by read().
  virtual void _step();

	// Returns debounced value.
  virtual bool _debounced();

  inline void _changeState();
  inline void _setStateFlag(const uint8_t flag)    { _state |= flag; }
  inline void _unsetStateFlag(const uint8_t flag)  { _state &= ~flag; }
  inline void _toggleStateFlag(const uint8_t flag) { _state ^= flag; }
  inline bool _getStateFlag(const uint8_t flag)    { return((_state & flag) != 0); }

  // The moving average.
  float _interval;
  float _startTime;
  uint8_t _state;
  uint8_t _debounceMode;
};

/// A generic class representing a simple analog input.
class AnalogIn : public Node, public PinUnit, public Smoothable {
public:
  /**
   * Constructor.
   * @param pin the pin number
   * @param mode the mode (DEFAULT or INVERTED)
   */
  AnalogIn(uint8_t pin, uint8_t mode=DEFAULT);
  virtual ~AnalogIn() {}

  /// Returns value in [0, 1].
  virtual float get() { return _avg.get(); }

  /// Maps value to new range.
  virtual float mapTo(float toLow, float toHigh);

  virtual operator float() { return get(); }

protected:
  virtual void begin();
  virtual void step();
  virtual float _read();
};

/// A generic class representing a simple digital input.
class DigitalIn : public DigitalSource, public PinUnit, public Debounceable {
public:
  /**
   * Constructor.
   * @param pin the pin number
   * @param mode the mode (DEFAULT, INVERTED, or INTERNAL_PULLUP)
   */
  DigitalIn(uint8_t pin, uint8_t mode=DEFAULT);
  virtual ~DigitalIn() {}

protected:
  virtual bool _isOn();

  virtual void begin();
  virtual void step();
};

} // namespace pq

#endif
