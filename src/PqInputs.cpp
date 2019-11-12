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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "PqInputs.h"
#include "pq_time.h"

namespace pq {

PqSmoothable::PqSmoothable(float smoothTime) : _avg(smoothTime) {}

void PqSmoothable::_begin() {
  _avg.reset();
}

void PqSmoothable::_step() {
  _avg.update( _read(), _avg.alpha(sampleRate()), true );
}



#define DEBOUNCED_STATE 0x01
#define UNSTABLE_STATE  0x02
#define CHANGED_STATE   0x04

PqDebounceable::PqDebounceable(float debounceTime, uint8_t mode) : _interval(0), _startTime(0), _state(0) {
   time(debounceTime);
   debounceMode(mode);
 }

void PqDebounceable::_begin() {
  _startTime = seconds();
  if (_isOn()) {
    _setStateFlag(DEBOUNCED_STATE | UNSTABLE_STATE);
  }
}

void PqDebounceable::_step() {

  _unsetStateFlag(CHANGED_STATE);

  if (_debounceMode == DEBOUNCE_DEFAULT) {
    // Read the state of the switch in a temporary variable.
    bool currentState = _isOn();

    // If the reading is different from last reading, reset the debounce counter
    if ( currentState != _getStateFlag(UNSTABLE_STATE) ) {
      _startTime = seconds();
      _toggleStateFlag(UNSTABLE_STATE);
    }

    // We have passed the threshold time, so the input is now stable
    // If it is different from last state, set the STATE_CHANGED flag
    else if (currentState != _getStateFlag(DEBOUNCED_STATE) &&
             seconds() - _startTime >= _interval) {
      _startTime = seconds();
      _changeState();
    }
  }

  else if (_debounceMode == DEBOUNCE_LOCK_OUT) {
    // Ignore everything if we are locked out
    if (seconds() - _startTime >= _interval) {
        bool currentState = _isOn();
        if ( currentState != _getStateFlag(DEBOUNCED_STATE) ) {
          _startTime = seconds();
          _changeState();
        }
    }
  }

  // DEBOUNCE_PROMPT_DETECT
  else  {
    // Read the state of the switch port into a temporary variable.
    bool currentState = _isOn();

    if ( currentState != _getStateFlag(DEBOUNCED_STATE) &&
         seconds() - _startTime >= _interval) {
      // We have passed the time threshold, so a new change of state is allowed.
      // set the STATE_CHANGED flag and the new DEBOUNCED_STATE.
      // This will be prompt as long as there has been greater than interval_misllis ms since last change of input.
      // Otherwise debounced state will not change again until bouncing is stable for the timeout period.
      _changeState();
    }

    // If the currentState is different from previous currentState, reset the debounce timer - as input is still unstable
    // and we want to prevent new button state changes until the previous one has remained stable for the timeout.
    if ( currentState != _getStateFlag(UNSTABLE_STATE) ) {
      _startTime = seconds();
      // Update Unstable Bit to macth readState
      _toggleStateFlag(UNSTABLE_STATE);
    }
  }
}

bool PqDebounceable::_debounced() {
  return _getStateFlag(DEBOUNCED_STATE);
}

void PqDebounceable::_changeState() {
  _toggleStateFlag(DEBOUNCED_STATE);
	_setStateFlag(CHANGED_STATE) ;
}


AnalogIn::AnalogIn(uint8_t pin, uint8_t mode)
  : PqMappable(), PqPinUnit(pin, mode), PqSmoothable()
{}

#ifdef ESP8266
#define ANALOG_READ_MAX_VALUE 1024
#elif defined(ESP32) or defined(ARDUINO_ARCH_ESP32)
#define ANALOG_READ_MAX_VALUE 4095 // applies only to default resolution of 12 bits
#else
#define ANALOG_READ_MAX_VALUE 1023
#endif
float AnalogIn::_read() {
  // Convert
  int rawValue = analogRead(_pin);
  if (_mode == ANALOG_INVERTED)
    rawValue = ANALOG_READ_MAX_VALUE - rawValue;
  return rawValue / float(ANALOG_READ_MAX_VALUE);
}

void AnalogIn::begin() {
  _begin();
}

void AnalogIn::step() {
  _step();
}

DigitalIn::DigitalIn(uint8_t pin, uint8_t mode)
  : PqDigitalSource(), PqPinUnit(pin, mode), PqDebounceable(), _changeState(0)
{}

bool DigitalIn::_isOn() {
  bool isHigh = (digitalRead(_pin) == HIGH);
  if (_mode == INTERNAL_PULLUP || _mode == EXTERNAL_PULLUP) // inverted
    isHigh = !isHigh;
  return isHigh;
}

void DigitalIn::begin() {
  pinMode(_pin, _mode == INTERNAL_PULLUP ? INPUT_PULLUP : INPUT);
  _changeState = 0;
}

void DigitalIn::step() {
  // Perform basic step.
  _step();
  // Read state.
	bool isOn = analogToDigital(_debounced());
  // Register difference between previous and new state.
  _changeState = (int8_t)isOn - (int8_t)_onValue;
  // Save state.
  _onValue = isOn;
}

} // namespace pq
