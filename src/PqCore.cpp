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
    _autoSampleRate(true),
    _beginCompleted(false),
    _firstRun(true),
    _eventManager(),
    _totalGlobalMicroSeconds({0}),
    _clockFunction(0)
{}

Engine::~Engine() {
}

void Engine::preBegin() {
  // Initialize console.
  if (isPrimary())
    consoleBegin();

  // Initialize variables.
  _sampleRate = _samplePeriod = _targetSampleRate = 0;
  _microSeconds.micros64 = microSeconds(false);
  _targetTime = _microSeconds;
  _stepState = STEP_INIT;
  _deltaTimeMicroSeconds = 0;
  _deltaTimeSecondsTimesFixed32Max = 0;
  _nSteps = 0;
  _autoSampleRate = true;
  _firstRun = true;

  _setSampleRate(FLT_MAX);

  // Initialize all components.
  for (size_t i = _unitsBeginIndex; i != _unitsEndIndex; i++) {
    units()[i]->begin();
  }

  // Units have been initialized.
  _beginCompleted = true;
}

void Engine::postBegin() {
  // Start timer.
  _microSeconds.micros64 = microSeconds(false);
  // Trick: by setting _nSteps = LONG_MAX, timeStep() will do _nStep++ which will overflow to 0
  _nSteps = ULONG_MAX;
}


void Engine::end() {
  if (_firstRun) {
    postBegin();
    _firstRun = false;
  }
  // else
  //   timeStep();
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
    _unitsBeginIndex = allUnits.size() - 1;
    _unitsEndIndex = _unitsBeginIndex + 1;
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

micro_seconds_t Engine::_updateGlobalMicroSeconds() const {
  // Get current global time.
  uint32_t us = _clock();
  uint32_t prevUs = _totalGlobalMicroSeconds.micros32.base;

  // Detect overflow.
  if (us < prevUs)
    _totalGlobalMicroSeconds.micros32.overflows++;

  // Update previous time.
  _totalGlobalMicroSeconds.micros32.base = us;

  return _totalGlobalMicroSeconds;
}

void Engine::autoSampleRate() {
  // Enable auto sample rate mode.
  _autoSampleRate = true;

  // Reset target sample rate (to keep things clean).
  _targetSampleRate = 0;
  _targetDeltaTimeMicroSeconds = 0; // unused
  _stepState = STEP_INIT;
}

void Engine::sampleRate(float sampleRate) {
  // Disable auto sample rate.
  _autoSampleRate = false;

  // Set target sample rate and delta time in us.
  _targetSampleRate = max(sampleRate, FLT_MIN);
  _targetDeltaTimeMicroSeconds = static_cast<uint32_t>(round(MICROS_PER_SECOND/_targetSampleRate));
}

void Engine::samplePeriod(float samplePeriod) {
  sampleRate(periodToFrequency(samplePeriod));
}

bool Engine::randomTrigger(float timeWindow) {
  return pq::randomTrigger(timeWindow, samplePeriod());
}

void Engine::referenceClock(unsigned long (*clockFunction)()) {
  _clockFunction = clockFunction;
  if (_beginCompleted) {
    begin(); // redo the begin with the new time function
  }
}

void referenceClock(unsigned long (*clockFunction)()) { Plaquette.referenceClock(clockFunction); }
unsigned long nSteps() { return Plaquette.nSteps(); }
bool hasAutoSampleRate() { return Plaquette.hasAutoSampleRate(); }
void autoSampleRate() { Plaquette.autoSampleRate(); }
void sampleRate(float sampleRate) { Plaquette.sampleRate(sampleRate); }
void samplePeriod(float samplePeriod) { Plaquette.samplePeriod(samplePeriod); }
float sampleRate() { return Plaquette.sampleRate(); }
float samplePeriod() { return Plaquette.samplePeriod(); }
bool randomTrigger(float timeWindow) { return Plaquette.randomTrigger(timeWindow); }

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
