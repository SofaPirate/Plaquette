/*
 * Ramp.cpp
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

#include "Ramp.h"
#include "pq_map.h"
#include "pq_time.h"

namespace pq {


Ramp::Ramp(Engine& engine) : Ramp(1.0f, engine) {}

Ramp::Ramp(float duration, Engine& engine) :
  Unit(engine),
  AbstractTimer(duration),
  _from(0.0f), _to(1.0f), _easing(easeNone),
#if PQ_OPTIMIZE_FOR_CPU
  _speed(0.0f),
#endif
  _mode(RAMP_DURATION), _finishedState(NOT_FINISHED), _valueNeedsUpdate(true)
{
}

float Ramp::get() {
  // Prevents unnecessary computations in the step() function by updating the value on a need basis.
  if (_valueNeedsUpdate) {
      // Compute next value.
    _value = _get();
    _valueNeedsUpdate = false; // reset flag
  }

  return _value;
}


float Ramp::put(float value) {
  if (_mode == RAMP_SPEED) {
    fromTo(value, _to);
  }
  else {
    // Project value on the ramp and set it as the new "from" value.
    float p = progress();
    float projectedFrom = (p >= 1.0f) ? _to : (value - p*_to) / (1 - p);
    fromTo(projectedFrom, _to);
  }

  // Set to value.
  _value = value;
  return _value;
}

void Ramp::easing(easing_function easing) {
  _easing = easing;
}

void Ramp::mode(uint8_t mode) {
  _mode = constrain(mode, (uint8_t)RAMP_DURATION, (uint8_t)RAMP_SPEED);
}

void Ramp::to(float to) {
  fromTo(_value, to);
}

void Ramp::from(float from) {
  fromTo(from, _to);
}

void Ramp::fromTo(float from, float to) {
  // If in speed mode, we need to adjust duration according to speed.
  if (_mode == RAMP_SPEED) {
    float currentSpeed = speed();
    _from = from;
    _to   = to;
    speed(currentSpeed);
  }
  // In duration mode, no need to change duration.
  else {
    _from = from;
    _to   = to;
  }
}

void Ramp::duration(float duration) {
  mode(RAMP_DURATION);
  AbstractTimer::duration(duration);
#if PQ_OPTIMIZE_FOR_CPU
  _speed = durationToSpeed(_duration);
#endif
}

void Ramp::speed(float speed) {
  mode(RAMP_SPEED);

  AbstractTimer::duration( speedToDuration(
#if PQ_OPTIMIZE_FOR_CPU
    _speed = max(speed, 0.f)
#else
    speed
#endif
  ) );
}

float Ramp::speed() const {
#if PQ_OPTIMIZE_FOR_CPU
  return _speed;
#else
  return durationToSpeed(_duration);
#endif
}

void Ramp::start() {
  Timeable::start();
  _finishedState = NOT_FINISHED;
}

void Ramp::start(float to, float duration, easing_function easing_) {
  go(to, duration, easing_);
}

void Ramp::start(float from, float to, float duration_, easing_function easing_) {
  go(from, to, duration_, easing_);
}

void Ramp::go(float from, float to, float durationOrSpeed, easing_function easing_) {
  // Set from and to values.
  fromTo(from, to);

  // Change easing if specified.
  if (easing_)
    easing(easing_);

  // Initialize mode and duration / speed.
  _durationOrSpeed(durationOrSpeed);

  // Start chronometer.
  start();
}

void Ramp::go(float to, float durationOrSpeed, easing_function easing_) {
  go(_value, to, durationOrSpeed, easing_);
}

void Ramp::go(float to, easing_function easing_) {
  go(to, _durationOrSpeed(), easing_);
}

void Ramp::begin() {
  setTime(0);
  _valueNeedsUpdate = true;
  _finishedState = NOT_FINISHED;
}

void Ramp::step() {
  // Call parent update.
  update();

  if (_isRunning) {
    // Compute value if running -- otherwise leave as is.
    _valueNeedsUpdate = true;
  }

  // Check if finished this turn.
  if (_finishedState == NOT_FINISHED) {
    if (isFinished()) {
      _finishedState = JUST_FINISHED;
    }
  }
  else if (_finishedState == JUST_FINISHED) {
    if (isFinished()) {
      _finishedState = POST_FINISHED;
    }
    else {
      _finishedState = NOT_FINISHED;
    }
  }
}

float Ramp::_get() {
  return mapFrom01(_easing(progress()), _from, _to);
}

void Ramp::_durationOrSpeed(float durationOrSpeed) {
  if (_mode == RAMP_DURATION)
    duration(durationOrSpeed);
  else
    speed(durationOrSpeed);
}

float Ramp::_durationOrSpeed() const {
  return _mode == RAMP_DURATION ? duration() : speed();
}

void Ramp::setTime(float time) {
  AbstractTimer::setTime(time);
  _valueNeedsUpdate = true;
}

float Ramp::durationToSpeed(float duration) const
{
  if (duration > 0) {
    float diff = (_to - _from);
    return abs(diff) / duration;
  }
  else if (_to == _from)
    return 0;
  else
    return FLT_MAX; // infinity
}

float Ramp::speedToDuration(float speed) const {
  return durationToSpeed(speed); // both these functions perform the same operation of inversion f(x) = 1/x
}

float Ramp::_time() const {
  return seconds();
}


}
