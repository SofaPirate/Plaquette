/*
 * Ramp.h
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

#ifndef RAMP_H_
#define RAMP_H_

#include "PqCore.h"
#include "AbstractTimer.h"
#include "pq_easing.h"

namespace pq {

/// @brief Ramp modes.
enum {
  RAMP_DURATION,
  RAMP_SPEED
};

/**
 * Provides a ramping / tweening mechanism that allows smooth transitions between
 * two values.
 */
class Ramp : public Unit, public AbstractTimer {
public:
  /**
   * Constructor.
   * @param engine the engine running this unit
   */
  Ramp(Engine& engine = Engine::primary());

    /**
   * Constructor with duration.
   * @param duration duration of the ramp
   * @param engine the engine running this unit
   */
  Ramp(float duration, Engine& engine = Engine::primary());

  /// Returns value of ramp.
  virtual float get();

  /**
   * Forces value in the ramp. If this happens while the ramp is running, it will
   * interrupt the ramp.
   * @param value the value sent to the unit
   * @return the new value of the unit
   */
  virtual float put(float value);

  /// Maps value to new range.
  virtual float mapTo(float toLow, float toHigh);

  /**
   * Sets easing function to apply to ramp.
   * @param easing the easing function
   */
  void easing(easing_function easing);

  /// Remove easing function (linear/no easing).
  void noEasing() { easing(easeNone); }

  /**
   * Assign final value of the ramp starting from current value.
   * @param to the final value
   */
  virtual void to(float to);

  /**
   * Assign initial value of the ramp.
   * @param from the initial value
   */
  virtual void from(float from);

  /**
   * Assign initial and final values of the ramp.
   * @param from the initial value
   * @param to the final value
   */
  virtual void fromTo(float from, float to);

  /// Sets the duration of the chronometer.
  virtual void duration(float duration);

  /// Returns duration.
  virtual float duration() const { return AbstractTimer::duration(); }

  /// Sets the speed (rate of change) of the ramp in change-per-second.
  virtual void speed(float speed);

  /// Returns speed (rate of change) of the ramp in change-per-second.
  virtual float speed() const;

  /// Starts/restarts the ramp. Will repeat the last ramp.
  virtual void start();

  // virtual void go(float from, float to, float durationOrSpeed, uint8_t mode, easing_function easing=0);
  /**
   * Starts a new ramp.
   * @param from the initial value
   * @param to the final value
   * @param durationOrSpeed the duration of the ramp (in seconds) or speed (in change-per-second) depending on mode
   * @param easing the easing function (optional).
   */
  virtual void go(float from, float to, float durationOrSpeed, easing_function easing=0);

  /**
   * Starts a new ramp, starting from current value.
   * @param to the final value
   * @param durationOrSpeed the duration of the ramp (in seconds) or speed (in change-per-second) depending on mode
   * @param easing the easing function (optional)
   */
  virtual void go(float to, float durationOrSpeed, easing_function easing=0);

  /**
   * Starts a new ramp, starting from current value (keeping the same duration/speed).
   * @param to the final value
   * @param easing the easing function (optional)
   */
  virtual void go(float to, easing_function easing=0);

  /// Changes the mode of the component (RAMP_DURATION or RAMP_SPEED).
  virtual void mode(uint8_t mode);

  /// Returns the mode of the component (RAMP_DURATION or RAMP_SPEED).
  uint8_t mode() const { return _mode; }

  /// Returns true iff the ramp just finished its process this step.
  virtual bool finished() { return (_finishedState == JUST_FINISHED); }

  /// Registers event callback on finish event.
  virtual void onFinish(EventCallback callback) { onEvent(callback, EVENT_FINISH); }

  /// Forces current time (in seconds).
  virtual void setTime(float time);

  /// Returns speed based on duration.
  float durationToSpeed(float duration) const;

    /// Returns duration based on speed.
  float speedToDuration(float speed) const;

  /// @deprecated
  [[deprecated("Use go(float,easing_function) instead.")]]
  virtual void start(float to, float durationOrSpeed, easing_function easing=0);

  /// @deprecated
  [[deprecated("Use go(float,float,easing_function) instead.")]]
  virtual void start(float from, float to, float durationOrSpeed, easing_function easing=0);

private:
  // Finished states.
  enum {
    NOT_FINISHED,  // before finished
    JUST_FINISHED, // just finished this step
    POST_FINISHED  // after finished triggered
  };

protected:
  virtual void begin();
  virtual void step();

  /// Returns true iff an event of a certain type has been triggered.
  virtual bool eventTriggered(EventType eventType) {
    switch (eventType) {
      case EVENT_FINISH: return finished();
      default:           return Unit::eventTriggered(eventType);
    }
  }

  // Sets duration or speed (depending on current mode).
  void _durationOrSpeed(float durationOrSpeed);
  float _durationOrSpeed() const;

  // Returns current absolute time (in seconds).
  virtual float _time() const;

  // The starting point.
  float _from;

  // The end point.
  float _to;

  // The current value.
  float _value;

  // Optional easing function.
  easing_function _easing;

#if PQ_OPTIMIZE_FOR_CPU
  // Speed.
  float _speed;
#endif

  // Mode (DURATION or SPEED).
  uint8_t _mode          : 1;

  // Finished flag.
  uint8_t _finishedState : 2;

  // Flag that makes sure the value is updated only on a need basis.
  bool _valueNeedsUpdate : 1;

  // Unused extra space.
  uint8_t _data          : 4;
};

}

#endif
