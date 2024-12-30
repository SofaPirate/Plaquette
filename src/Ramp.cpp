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
#include "pq_map_real.h"
#include "pq_time.h"

namespace pq {

Ramp::Ramp(float from) :
  AbstractTimer(1.0f),
  _from(from), _to(from), _easing(easeNone), _mode(RAMP_DURATION)
{
}

// Ramp::Ramp(float from, float to, float duration, easing_function easing) :
//   AbstractTimer(duration),
//   _from(from), _to(to), _easing(easing), _mode(DURATION)
// {
// }

float Ramp::put(float value) {
  if (_mode == RAMP_SPEED) {
    fromTo(value, _to);
  }
  else {
    float p = progress();
    float projectedFrom = (value - p*_to) / (1 - p); ////// verify this
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
}

void Ramp::speed(float speed) {
  mode(RAMP_SPEED);
  float diff = (_to - _from);
  AbstractTimer::duration( abs(diff) / abs(speed) );
}

float Ramp::speed() const {
  if (_duration > 0) {
    float diff = (_to - _from);
    return abs(diff) / _duration;
  }
  else if (_to == _from)
    return 0;
  else
    return FLT_MAX;
}

void Ramp::start() {
  AbstractTimer::start();
}

void Ramp::start(float to, float duration, easing_function easing_) {
  go(to, duration, easing_);
}

void Ramp::start(float from, float to, float duration_, easing_function easing_) {
  go(from, to, duration_, easing_);
}

// void Ramp::go(float from, float to, float durationOrSpeed, uint8_t mode_, easing_function easing_) {
//   // Set from and to values.
//   fromTo(from, to);

//   // Change easing if specified.
//   if (easing_)
//     easing(easing_);

//   // Set mode.
//   mode(mode_);

//   // Initialize mode and duration / speed.
//   _durationOrSpeed(durationOrSpeed);

//   // Start chronometer.
//   start();

// }

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

// void Ramp::go(float from, float to, easing_function easing_) {
//   go(from, to, _durationOrSpeed(), easing_);
// }

// void Ramp::go(float to, float durationOrSpeed, uint8_t mode_, easing_function easing_) {
//   go(_value, to, durationOrSpeed, mode_, easing_);
// }

void Ramp::go(float to, float durationOrSpeed, easing_function easing_) {
  go(_value, to, durationOrSpeed, easing_);
}

void Ramp::go(float to, easing_function easing_) {
  go(to, _durationOrSpeed(), easing_);
}

void Ramp::step() {
  AbstractTimer::step();

  if (_isStarted) {
    // Compute value if running -- otherwise leave as is.
    _value = _get();
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

}
