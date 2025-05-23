/*
 * PqInputs.cpp
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
 * along with this prograxm.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "PqInputs.h"
#include "pq_globals.h"
#include "pq_time.h"
#include "pq_map_real.h"

namespace pq {

Smoothable::Smoothable() : _avg(PLAQUETTE_NO_SMOOTH_WINDOW) {}

void Smoothable::_begin() {
  _avg.reset();
}

void Smoothable::_step() {
  _avg.update( _read(), _avg.alpha( _sampleRate() ), true );
}

#define DEBOUNCED_STATE 0x01
#define UNSTABLE_STATE  0x02
#define CHANGED_STATE   0x04

Debounceable::Debounceable() : _interval(0), _startTime(0), _state(0) {
   debounceMode(DEBOUNCE_STABLE);
 }

void Debounceable::_begin() {
  _startTime = _time();
  if (_isOn()) {
    _setStateFlag(DEBOUNCED_STATE | UNSTABLE_STATE);
  }
}

void Debounceable::_step() {

  _unsetStateFlag(CHANGED_STATE);

  bool currentState = _isOn();

  // No debouncing case.
  if (_interval) {

    float currentTime = _time();

    if (_debounceMode == DEBOUNCE_STABLE) {
      // Read the state of the switch in a temporary variable.

      // If the reading is different from last reading, reset the debounce counter
      if (currentState != _getStateFlag(UNSTABLE_STATE) ) {
        _startTime = currentTime;
        _toggleStateFlag(UNSTABLE_STATE);
      }

      // We have passed the threshold time, so the input is now stable
      // If it is different from last state, set the STATE_CHANGED flag
      else if (currentState != _getStateFlag(DEBOUNCED_STATE) &&
               currentTime - _startTime >= _interval) {
        _startTime = currentTime;
        _changeState();
      }
    }

    else if (_debounceMode == DEBOUNCE_LOCK_OUT) {
      // Ignore everything if we are locked out
      if (currentState != _getStateFlag(DEBOUNCED_STATE) &&
           currentTime - _startTime >= _interval) {
        _startTime = currentTime;
         _changeState();
      }
    }

    else  { // debounce mode : DEBOUNCE_PROMPT_DETECT
      if (currentState != _getStateFlag(DEBOUNCED_STATE) &&
          currentTime - _startTime >= _interval) {
        // We have passed the time threshold, so a new change of state is allowed.
        // set the STATE_CHANGED flag and the new DEBOUNCED_STATE.
        // This will be prompt as long as there has been greater than interval_misllis ms since last change of input.
        // Otherwise debounced state will not change again until bouncing is stable for the timeout period.
        _changeState();
      }

      // If the currentState is different from previous currentState, reset the debounce timer - as input is still unstable
      // and we want to prevent new button state changes until the previous one has remained stable for the timeout.
      if (currentState != _getStateFlag(UNSTABLE_STATE)) {
        _startTime = currentTime;
        // Update Unstable Bit to macth readState
        _toggleStateFlag(UNSTABLE_STATE);
      }
    }
  }

  // No debouncing.
  else if (currentState != _getStateFlag(DEBOUNCED_STATE)) {
    _changeState();
  }

}

bool Debounceable::_debounced() {
  return _getStateFlag(DEBOUNCED_STATE);
}

void Debounceable::_changeState() {
  _toggleStateFlag(DEBOUNCED_STATE);
  _setStateFlag(CHANGED_STATE) ;
}


AnalogIn::AnalogIn(uint8_t pin, Engine& engine)
  : Unit(engine), PinConfig(pin, DIRECT)
{}

AnalogIn::AnalogIn(uint8_t pin, uint8_t mode, Engine& engine)
  : Unit(engine), PinConfig(pin, mode)
{}

float AnalogIn::_read() {
  return (_mode == DIRECT) ? read() : 1.0F - read();
}

void AnalogIn::begin() {
  _begin(); 
}

void AnalogIn::step() {
  _step();
}

float AnalogIn::mapTo(float toLow, float toHigh) {
  return mapFrom01(get(), toLow, toHigh);
}

float AnalogIn::read() const {
  return analogRead(_pin) / float(ANALOG_READ_MAX_VALUE);
}

int AnalogIn::rawRead() const {
  return analogRead(_pin);
}

DigitalIn::DigitalIn(uint8_t pin, Engine& engine)
  : DigitalSource(engine), PinConfig(pin, DIRECT)
{}

DigitalIn::DigitalIn(uint8_t pin, uint8_t mode, Engine& engine)
  : DigitalSource(engine), PinConfig(pin, mode)
{}

bool DigitalIn::_isOn() {
  return (_mode == DIRECT) ? rawRead() : !rawRead();
}

void DigitalIn::_init() {
  pinMode(_pin, _mode == INTERNAL_PULLUP ? INPUT_PULLUP : INPUT);
}

void DigitalIn::mode(uint8_t mode) {
  _mode = mode;
  _init();
}


float DigitalIn::read() const {
  return rawRead();
}

int DigitalIn::rawRead() const {
  return digitalRead(_pin);
}

void DigitalIn::begin() {
  _init();
}

void DigitalIn::step() {
   // Perform basic step.
  _step();
  // Save value.
  _onValue = _debounced();
  // Compute change state.
  _updateChangeState();
}

} // namespace pq
