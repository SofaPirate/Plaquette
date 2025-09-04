/*
 * PqCore.cpp
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
#include "PqCore.h"

#include <float.h>

namespace pq {

HybridArrayList<Unit*, PLAQUETTE_MAX_UNITS>& Engine::units() {
  static HybridArrayList<Unit*, PLAQUETTE_MAX_UNITS> instance;
  return instance;
}

Engine& Engine::primary() {
  static Engine instance;
  return instance;
}

Engine& Plaquette = Engine::primary();

Engine::Engine()
  : _unitsBeginIndex(0), _unitsEndIndex(0),
    _sampleRate(0.0f), _samplePeriod(0.0f), _targetSampleRate(0.0f),
    _microSeconds{},
    _targetTime{}, _stepState(STEP_INIT),
    _deltaTimeMicroSeconds(0),
    _deltaTimeSecondsTimesFixed32Max(0.0f),
    _nSteps(0),
    _beginCompleted(false),
    _firstRun(true),
    _eventManager() // default constructed
{}

Engine::~Engine() {
}

void Engine::preBegin(unsigned long baudrate) {
  // Initialize serial.
  if (isPrimary() && baudrate)
    beginSerial(baudrate);

  // Initialize variables.
  _sampleRate = _samplePeriod = _targetSampleRate = 0;
  _microSeconds.micros64 = microSeconds(false);
  _targetTime = _microSeconds;
  _stepState = STEP_INIT;
  _deltaTimeMicroSeconds = 0;
  _deltaTimeSecondsTimesFixed32Max = 0;
  _nSteps = 0;
  _firstRun = true;

  _setSampleRate(FLT_MAX);

  // Initialize all components.
  for (size_t i=_unitsBeginIndex; i != _unitsEndIndex; i++) {
    units()[i]->begin();
  }

  // Units have been initialized.
  _beginCompleted = true;
}

void Engine::postBegin() {
  // Start timer.
  _microSeconds.micros64 = microSeconds(false);
  // Trick: by setting _nSteps = LONG_MAX, stepTime() will do _nStep++ which will overflow to 0
  _nSteps = ULONG_MAX;
}


void Engine::end() {
  if (_firstRun) {
    postBegin();
    _firstRun = false;
  }
  else
    stepTime();
}

float Engine::seconds(bool referenceTime) const {
  return microsToSeconds(microSeconds(referenceTime));
}

uint32_t Engine::milliSeconds(bool referenceTime) const {
  return microsToMillis(microSeconds(referenceTime));
}

uint64_t Engine::microSeconds(bool referenceTime) const {
  return (referenceTime ? _microSeconds.micros64 : _updateGlobalMicroSeconds().micros64);
}

void Engine::add(Unit* component) {
  HybridArrayList<Unit*, PLAQUETTE_MAX_UNITS>& allUnits = units();
  if (component->engine()) {
    return; // XXX does not support moving components between engines
  }

  // Find the right place to insert the component.

  // If there are already units, insert it after the last unit. Singleton always
  // adds units in the first part of the array list for efficency.
  if (nUnits() > 0 || isPrimary()) {
    // Insert component.
    if (_unitsEndIndex == allUnits.size()) {
      allUnits.add(component);
      _unitsEndIndex++;
    }
    else {
      allUnits.insert(_unitsEndIndex++, component);
      // Shift indices of next engines in the array.
      for (size_t i=_unitsEndIndex; i<allUnits.size(); ) {
        Engine* engine = allUnits[i]->engine();
        ++engine->_unitsBeginIndex;
        i = ++engine->_unitsEndIndex;
      }
    }
  }

  // If there are no units yet, insert it at the end.
  else {
    allUnits.add(component);
    _unitsBeginIndex = allUnits.size()-1;
    _unitsEndIndex = _unitsBeginIndex+1;
  }

  // Assign parent engine.
  component->_engine = this;

  // Initialize component if needed.
  if (_beginCompleted)
    component->begin();

  // if (component->engine != this) {
  //   // Remove component from old engine.
  //   if (component->engine)
  //     component->engine->remove(component);

  //   // Append component to new engine.
  //   _units.add(component);
  //   // Assign new engine.
  //   component->engine = this;
  // }

  // // Initialize component if needed.
  // if (_beginCompleted)
  //   component->begin();
}

void Engine::remove(Unit* component) {
  // Remove component from list.
  units().removeItem(component);
}

micro_seconds_t Engine::_totalGlobalMicroSeconds = { 0 };

micro_seconds_t Engine::_updateGlobalMicroSeconds() {
  // Get current global time.
  uint32_t us = micros();
  uint32_t prevUs = _totalGlobalMicroSeconds.micros32.base;

  // Detect overflow.
  if (us < prevUs)
    _totalGlobalMicroSeconds.micros32.overflows++;

  // Update previous time.
  _totalGlobalMicroSeconds.micros32.base = us;

  return _totalGlobalMicroSeconds;
}

bool Engine::autoSampleRate() { return (_targetSampleRate <= 0); }

void Engine::enableAutoSampleRate() {
  _targetSampleRate = 0;
}

void Engine::sampleRate(float sampleRate) {
  _targetSampleRate = max(sampleRate, FLT_MIN);
}

void Engine::samplePeriod(float samplePeriod) {
  if (samplePeriod > 0)
    sampleRate(1.0f / samplePeriod);
  else
    autoSampleRate();
}

bool Engine::randomTrigger(float timeWindow) {
  return pq::randomTrigger(timeWindow, samplePeriod());
}

unsigned long nSteps() { return Plaquette.nSteps(); }
float sampleRate() { return Plaquette.sampleRate(); }
float samplePeriod() { return Plaquette.samplePeriod(); }
bool randomTrigger(float timeWindow) { return Plaquette.randomTrigger(timeWindow); }

void beginSerial(unsigned long baudRate) {
  // Wait for last transmitted data to be sent.
  Serial.flush();
  // Start serial with new baudrate.
  Serial.begin(baudRate);
  // Empty  out possible garbage from input buffer.
  while (Serial.available()) Serial.read();
}

Unit::Unit(Engine& engineRef) : _engine(0) {
  engineRef.add(this);
}

Unit::~Unit() {
}

void Unit::clearEvents() {
  _engine->_eventManager.clearListeners(this);
}

void Unit::onEvent(EventCallback callback, EventType eventType) {
  _engine->_eventManager.addListener(this, callback, eventType);
}

} // namespace pq
