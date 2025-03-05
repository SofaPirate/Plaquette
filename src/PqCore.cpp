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

Engine& Plaquette = Engine::singleton();

Engine::Engine() : _units(), _microSeconds({0}), _sampleRate(0), _targetSampleRate(0), _nSteps(0), _beginCompleted(false), _firstRun(true) {
}

Engine::~Engine() {
}

Engine& Engine::singleton() {
  static Engine instance;
  return instance;
}

void Engine::preBegin(unsigned long baudrate) {
  // Initialize serial.
  if (baudrate)
    beginSerial(baudrate);

  // Initialize variables.
  _microSeconds.micros64 = microSeconds(false);
  _targetSampleRate = 0;
  _nSteps = 0;
  _firstRun = true;

  _setSampleRate(FLT_MAX);

  // Initialize all components.
  for (size_t i=0; i<_units.size(); i++) {
    _units[i]->begin();
  }

  // Units have been initialized.
  _beginCompleted = true;
}

void Engine::postBegin() {
  // Start timer.
  _microSeconds.micros64 = microSeconds(false);
}

void Engine::end() {
  if (_firstRun) {
    postBegin();
    _firstRun = false;
  }
  else
    postStep();
}

float Engine::seconds(bool referenceTime) {
  return microSeconds(referenceTime) * SECONDS_TO_MICROS;
}

uint32_t Engine::milliSeconds(bool referenceTime) {
  return static_cast<uint32_t>(microSeconds(referenceTime) * MILLIS_TO_MICROS);
}

uint64_t Engine::microSeconds(bool referenceTime) {
  return (referenceTime ? _microSeconds.micros64 : _updateGlobalMicroSeconds().micros64);
}

void Engine::add(Unit* component) {
  for (size_t i=0; i<_units.size(); i++) {
    if (_units[i] == component) {
      return; // do not add existing component
    }
  }

  // Append component to list.
  _units.add(component);
  if (_beginCompleted)
    component->begin();
}

void Engine::remove(Unit* component) {
  // Remove component from list.
  _units.removeItem(component);
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

//float seconds(bool realTime) { return Engine::seconds(); }
unsigned long nSteps() { return Plaquette.nSteps(); }
void sampleRate(float sampleRate) { Plaquette.sampleRate(sampleRate); }
void samplePeriod(float samplePeriod) { Plaquette.samplePeriod(samplePeriod); }
float sampleRate() { return Plaquette.sampleRate(); }
float samplePeriod() { return Plaquette.samplePeriod(); }

void beginSerial(unsigned long baudRate) {
  // Wait for last transmitted data to be sent.
  Serial.flush();
  // Start serial with new baudrate.
  Serial.begin(baudRate);
  // Empty  out possible garbage from input buffer.
  while (Serial.available()) Serial.read();
}

Unit::Unit() {
  Engine::singleton().add(this);
}

Unit::~Unit() {
  Engine::singleton().remove(this);
}

void Unit::clearEvents() {
  Plaquette._eventManager.clearListeners(this);
}

void Unit::onEvent(EventCallback callback, EventType eventType) {
  Plaquette._eventManager.addListener(this, callback, eventType);
}

} // namespace pq
