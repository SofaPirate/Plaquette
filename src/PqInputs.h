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

// Output constants.
#define INTERNAL_PULLUP   0x0
#define EXTERNAL_PULLUP   0x1
#define EXTERNAL_PULLDOWN 0x2

#define ANALOG_DEFAULT    0x4
#define ANALOG_INVERTED   0x8

#define DEBOUNCE_DEFAULT       0x01
#define DEBOUNCE_LOCK_OUT      0x02
#define DEBOUNCE_PROMPT_DETECT 0x04

namespace pq {

/// Superclass for components that can be smoothed.
class PqSmoothable {
public:
  PqSmoothable(float smoothTime=PLAQUETTE_NO_SMOOTH_WINDOW);

  /// Apply smoothing to object.
  virtual void smooth(float smoothTime=PLAQUETTE_DEFAULT_SMOOTH_WINDOW) { time(smoothTime); }

  /// Remove smoothing.
  virtual void noSmooth() { smooth(PLAQUETTE_NO_SMOOTH_WINDOW); }

  /// Changes the smoothing window (expressed in seconds).
  virtual void time(float seconds) { _avg.time(seconds); }

  /// Returns the smoothing window (expressed in seconds).
  float time() const { return _avg.time(); }

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
class PqDebounceable {
public:
  PqDebounceable(float debounceTime=PLAQUETTE_NO_DEBOUNCE_WINDOW, uint8_t mode=DEBOUNCE_DEFAULT);

  /// Apply smoothing to object.
  virtual void debounce(float debounceTime=PLAQUETTE_DEFAULT_DEBOUNCE_WINDOW) { time(debounceTime); }

  /// Remove smoothing.
  virtual void noDebounce() { debounce(PLAQUETTE_NO_DEBOUNCE_WINDOW); }

  /// Deprecated. Left for backwards compatibility.
  virtual void smooth(float smoothTime=PLAQUETTE_DEFAULT_DEBOUNCE_WINDOW) { debounce(smoothTime); }

  /// Remove smoothing.
  virtual void noSmooth() { noDebounce(); }

  /// Changes the debouncing window (expressed in seconds).
  virtual void time(float seconds) { _interval = seconds; }

  /// Returns the debouncing window (expressed in seconds).
  float time() const { return _interval; }

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
class AnalogIn : public PqMappable, public PqPinUnit, public PqSmoothable {
public:
  /**
   * Constructor.
   * @param pin the pin number
   * @param mode the mode (ANALOG_DEFAULT or ANALOG_INVERTED)
   */
  AnalogIn(uint8_t pin=A0, uint8_t mode=ANALOG_DEFAULT);
  virtual ~AnalogIn() {}
  /// Returns value (typically between 0 and 1, may vary depending on class).

  virtual float get() { return _avg.get(); }

protected:
  virtual void begin();
  virtual void step();
  virtual float _read();
};

/// A generic class representing a simple digital input.
class DigitalIn : public PqDigitalSource, public PqPinUnit, public PqDebounceable {
public:
  /**
   * Constructor.
   * @param pin the pin number
   * @param mode the mode (INTERNAL_PULLUP, EXTERNAL_PULLUP, or EXTERNAL_PULLDOWN)
   */
  DigitalIn(uint8_t pin=0, uint8_t mode=INTERNAL_PULLUP);
  virtual ~DigitalIn() {}

  /// Difference between current and previous value of the unit.
  virtual int8_t changeState() { return _changeState; }

protected:
  virtual bool _isOn();

  virtual void begin();
  virtual void step();

  // Keeps track of state changes.
  int8_t _changeState;
};

} // namespace pq

#endif
