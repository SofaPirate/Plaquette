/*
 * PqCore.h
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

#ifndef PQ_CORE_H_
#define PQ_CORE_H_

#if (defined(ARDUINO) && ARDUINO >= 100) || defined(EPOXY_DUINO)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include "pq_math.h"

#include <stdint.h>
#include <float.h>

#include <limits.h>

#include "HybridArrayList.h"
#include "PqEvents.h"
#include "pq_globals.h"
#include "pq_constrain.h"
#include "pq_map.h"
#include "pq_phase_utils.h"
#include "pq_random.h"
#include "pq_time.h"

namespace pq {

/// @brief Input/output modes.
enum {
  DIRECT,
  INVERTED,
  INTERNAL_PULLUP,
  SOURCE = DIRECT, // deprecated
  SINK = INVERTED  // deprecated
};

class Unit;

/// The main Plaquette static class containing all the units.
class Engine {
  friend class Unit;

public:
  Engine();
  ~Engine();

  /// Initializes all components (calls begin() on all of them).
  void preBegin(unsigned long baudrate=PLAQUETTE_SERIAL_BAUD_RATE);

  /// Performs additional tasks after the class to begin().
  void postBegin();

  /// Updates all components (calls step() on all of them).
  inline void preStep();

  /**
   * Performs additional tasks after the class to step().
   * @return true if the program should
   */
  inline bool timeStep();

  /// Function to be used within the PlaquetteLib context (needs to be called at top of setup() method).
  inline void begin(unsigned long baudrate=PLAQUETTE_SERIAL_BAUD_RATE);

  /// Function to be used within the PlaquetteLib context (needs to be called at top of loop() method).
  inline bool step();

  /**
   * Optional function to be used within the PlaquetteLib context. No need to call it if the program
   * is looping indefinitely. Call if the program stops at some point.
   */
  inline void end();

  /// Returns the current number of units.
  size_t nUnits() { return _unitsEndIndex - _unitsBeginIndex; }

  /**
   * Returns time in seconds. Optional parameter allows to ask for reference time (default)
   * which will yield the same value through one iteration of step(), or "real" time which will
   * return the current total running time.
   * @param referenceTime determines whether the function returns the reference time or the real time
   * @return the time in seconds
   */
  float seconds(bool referenceTime=true) const;

  /**
   * Returns time in milliseconds. Optional parameter allows to ask for reference time (default)
   * which will yield the same value through one iteration of step(), or "real" time which will
   * return the current total running time.
   * @param referenceTime determines whether the function returns the reference time or the real time
   * @return the time in milliseconds
   */
  uint32_t milliSeconds(bool referenceTime=true) const;

  /**
   * Returns time in microseconds. Optional parameter allows to ask for reference time (default)
   * which will yield the same value through one iteration of step(), or "real" time which will
   * return the current total running time.
   * @param referenceTime determines whether the function returns the reference time or the real time
   * @return the time in microseconds
   */
  uint64_t microSeconds(bool referenceTime=true) const;

  /// Returns number of steps.
  unsigned long nSteps() const { return _nSteps; }

  /// Returns true iff the auto sample rate mode is enabled (default).
  bool hasAutoSampleRate() const { return _autoSampleRate; }

  /// Enables auto sample rate mode (default).
  void autoSampleRate();

  /// Sets sample rate to a fixed value, thus disabling auto sampling rate.
  void sampleRate(float sampleRate);

  /// Sets sample period to a fixed value, thus disabling auto sampling rate.
  void samplePeriod(float samplePeriod);

  /// Returns sample rate.
  float sampleRate() const { return _sampleRate; }

  /// Returns sample period.
  float samplePeriod() const { return (_samplePeriod ? _samplePeriod : (_samplePeriod = frequencyToPeriod(_sampleRate))); }

  /// Returns time between steps (in microseconds).
  uint32_t deltaTimeMicroSeconds() const { return _deltaTimeMicroSeconds; }

  /// Returns time between steps, expressed in fixed point propotion.
  float deltaTimeSecondsTimesFixed32Max() const { return _deltaTimeSecondsTimesFixed32Max; }

  /// Returns the main instance of Plaquette.
  static Engine& primary();

  /// Returns true if this Engine is the main.
  bool isPrimary() const { return this == &primary(); }

  /**
   * Randomly triggers an event about once per time window, on average.
   *
   * Call this function once in each step(). It will occasionally return
   * true, with the frequency adjusted so that you get roughly one event for each
   * `timeWindow` period, no matter how fast your loop is running.
   *
   * @param timeWindow duration of the window (in seconds)
   * @return true when an event occurs during this sample
   */
  bool randomTrigger(float timeWindow);

  /// @brief Sets base function returning microseconds. Default: micros().
  /// @param clockFunction pointer to a function returning microseconds
  void referenceClock(unsigned long (*clockFunction)());

private:
  /// Adds a component to Plaquette.
  void add(Unit* component);

  /// Removes a component from Plaquette.
  void remove(Unit* component);

  // Internal use. Sets sample rate and sample period.
  inline void _setSampleRate(float sampleRate);

  // Returns current reference time in microseconds.
  unsigned long _clock() const { return _clockFunction ? _clockFunction() : micros(); }

  // Internal use, needs to be called periodically. Updates _totalGlobalMicroSeconds.
  micro_seconds_t _updateGlobalMicroSeconds() const;

private:
  // Shared static container containing all units. Static because it is shared between all engines.
  // Units from the primary engine are always located at the begnning, then units from other engines.
  static HybridArrayList<Unit*, PLAQUETTE_MAX_UNITS>& units();

  // Range of this engine's units within the _units array list.
  size_t _unitsBeginIndex; // begin index
  size_t _unitsEndIndex;   // end index (= _unitsBeginIndex + nUnits())

  // Sampling rate (ie. how many times per seconds step() is called).
  float _sampleRate;

  // Sampling period (ie. 1.0 / sampleRate()).
  mutable float _samplePeriod;

  // Whether the auto sample rate mode is activated.
  float _targetSampleRate;

  // Snapshot of time in seconds from current step.
  micro_seconds_t _microSeconds;
  // uint32_t _previousMicroSeconds; // This is the 32 first bits of microseconds used for inter-step calculations.

  // Target time for next step (when using sampleRate(float)).
  micro_seconds_t _targetTime;

  // Step state for state machine to manage sample rate.
  enum StepState {
    STEP_INIT,
    STEP_WAIT,
    STEP_WAIT_OVERFLOW
  };

  // Current step state.
  StepState _stepState;

  // Number of microseconds between steps.
  uint32_t _deltaTimeMicroSeconds;

  // Number of seconds between steps time FIXED_32_MAX.
  float _deltaTimeSecondsTimesFixed32Max;

  // Number of microseconds between steps.
  uint32_t _targetDeltaTimeMicroSeconds;

  // Number of steps accomplished.
  unsigned long _nSteps;

  // True if using auto sample rate mode.
  bool _autoSampleRate;

  // True if begin has been completed..
  bool _beginCompleted;

  // True during first run.
  bool _firstRun;

  // Used to keep track of events.
  EventManager _eventManager;

  // Global time in microseconds.
  mutable micro_seconds_t _totalGlobalMicroSeconds;

  // Functions that return time in microseconds (default: micros()).
  unsigned long (*_clockFunction)();

private:
  // Prevent copy-construction and assignment.
  Engine(const Engine&);
  Engine& operator=(const Engine&);
};

/// The Plaquette main.
extern Engine& Plaquette;

/// Sets base function returning microseconds of primary engine. Default: micros().
void referenceClock(unsigned long (*clockFunction)());

/// Returns number of steps of primary engine.
unsigned long nSteps();

/// Returns true iff the auto sample rate mode is enabled (default).
bool hasAutoSampleRate();

/// Enables auto sample rate mode (default).
void autoSampleRate();

/// Sets sample rate to a fixed value, thus disabling auto sampling rate.
void sampleRate(float sampleRate);

/// Sets sample period to a fixed value, thus disabling auto sampling rate.
void samplePeriod(float samplePeriod);

/// Returns sample rate of primary engine.
float sampleRate();

/// Returns sample period of primary engine.
float samplePeriod();

/**
 * Randomly triggers an event about once per time window, on average based on
 * sampling rate of primary engine.
 *
 * Call this function once in each step(). It will occasionally return
 * true, with the frequency adjusted so that you get roughly one event for each
 * `timeWindow` period, no matter how fast your loop is running.
 *
 * @param timeWindow duration of the window (in seconds)
 * @return true when an event occurs during this sample
 */
bool randomTrigger(float timeWindow);

/// Restarts main serial. This method will make sure to flush data from the pipeline.
void beginSerial(unsigned long baudRate);

class Chainable {
  private:
    // Prevents assignation operations by making them private.
  Chainable& operator=(bool);
  Chainable& operator=(int);
  Chainable& operator=(float);
  Chainable& operator=(Chainable&);

  public:
  /// Returns value (typically between 0 and 1, may vary depending on class).
  virtual float get() { return 0.0f; }

  /// Object can be used directly to access its value.
  operator float() { return get(); }

  /// Maps value to new range. This function guarantees that the value is within [toLow, toHigh].
  /// If the unit's values are unbounded, returns get() constrained to [toLow, toHigh].
  virtual float mapTo(float toLow, float toHigh) { return constrain(get(), toLow, toHigh); } // default: just constrain

  /**
   * Pushes value into the unit.
   * @param value the value sent to the unit
   * @return the new value of the unit
   */
  virtual float put(float value) { return get(); } // do nothing by default (read-only)

  /// Converts analog (float) value to digital (bool) value.
  static bool  analogToDigital(float f) { return (f >= 0.5); }

  /// Converts digital (bool) value to analog (float) value.
  static float digitalToAnalog(bool b) { return (b ? 1.0f : 0.0f); }

  public:
  /// Operator that allows usage in conditional expressions.
  // NOTE: This operator is defined as explicit so that boolean expression like
  // "if (obj)" use the bool() operator while other expressions can use the float() operator.
  explicit operator bool() { return Chainable::analogToDigital(get()); }
};

/**
 * A generic class representing a unit in the system.
 * Main class for components to be added to Plaquette.
 * Each such unit can be read using function get(). Values
 * can also be sent to a unit using put().
 */
class Unit : public Chainable {
  friend class Engine;
  friend class EventManager;

protected:
  virtual void begin() {}
  virtual void step() {}

public:
  // Clears all event listeners.
  virtual void clearEvents();

  /// Returns engine time in seconds.
  float seconds() const { return _engine->seconds(); }

  /// Returns engine time in milliseconds.
  uint32_t milliSeconds() const { return _engine->milliSeconds(); }

  /// Returns engine time in microseconds.
  uint64_t microSeconds() const { return _engine->microSeconds(); }

  /// Returns number of engine steps.
  unsigned long nSteps() const { return _engine->nSteps(); }

  /// Returns engine sample rate.
  float sampleRate() const { return _engine->sampleRate(); }

  /// Returns enginesample period.
  float samplePeriod() const { return _engine->samplePeriod(); }

protected:
  /// Constructor.
  Unit(Engine& engine);
  virtual ~Unit();

  /// Returns true iff an event of a certain type has been triggered.
  virtual bool eventTriggered(EventType eventType) { return false; }

  /// Registers event callback.
  virtual void onEvent(EventCallback callback, EventType eventType);

private:
  // The engine that owns this unit.
  Engine* _engine;

protected:
  /// Returns the engine that owns this unit.
  Engine* engine() const { return _engine; }
};

/// A generic class representing a simple digital (true/false)unit.
class DigitalUnit : public Unit {
protected:
  /// Constructor.
  DigitalUnit(Engine& engine) : Unit(engine) {}

public:
  /// Returns true iff the input is "on".
  virtual bool isOn() { return false; }

  /// Returns true iff the input is "off".
  virtual bool isOff() { return !isOn(); }

  /// Returns value as integer (0 or 1).
  virtual int getInt() { return isOn() ? 1 : 0; }

  /// Returns value as float (either 0.0 or 1.0).
  virtual float get() { return getInt(); }

  /// Sets output to "on" (ie. true, 1).
  virtual bool on() { return putOn(true); }

  /// Sets output to "off" (ie. false, 0).
  virtual bool off() { return putOn(false); }

  /**
   * Pushes value into the unit.
   * @param value the value sent to the unit
   * @return the new value of the unit
   */
  virtual float put(float value) {
    return Unit::digitalToAnalog(putOn(Unit::analogToDigital(value)));
  }

  /**
   * Pushes value into the unit.
   * @param value the value sent to the unit
   * @return the new value of the unit
   */
  virtual bool putOn(bool value) { return isOn(); } // do nothing by default (read-only)

  /// Maps value to new range.
  virtual float mapTo(float toLow, float toHigh) { return isOn() ? toHigh : toLow; }

  /// Operator that allows usage in conditional expressions.
  operator bool() { return isOn(); }

  // IMPORTANT: LEAVE COMMENTED
  // virtual operator int() { return getInt(); }

  // IMPORTANT: This operator is redefined as explicit to make default return a bool.
  explicit operator float() { return Unit::operator float(); }
};

/**
 * An analog analog source that contains a value constrained to a finite range
 * (typically in [0, 1]). It is the responsibility of the subclass's programmer to make
 * sure the value stays within range OR to update the mapTo() function accordingly.
 */
class AnalogSource : public Unit {
public:
  /// Constructor.
  AnalogSource(Engine& engine) : AnalogSource(0, engine) {}
  AnalogSource(float initialValue, Engine& engine) : Unit(engine) { _value = constrain01(initialValue); }
  virtual ~AnalogSource() {}

  /// Returns value in [0, 1].
  virtual float get() { return _value; }

  /// Maps value to new range.
  virtual float mapTo(float toLow, float toHigh) { return mapFrom01(get(), toLow, toHigh); }

protected:
  float _value;
};

/// A digital source that contains a true/false value.
class DigitalSource : public DigitalUnit {
public:
  /// Constructor.
  DigitalSource(Engine& engine) : DigitalSource(false, engine) {}
  DigitalSource(bool initialValue, Engine& engine) : DigitalUnit(engine), _onValue(initialValue), _prevOnValue(initialValue), _changeState(0) {}

  /// Returns true iff the input is "on".
  virtual bool isOn() { return _onValue; }

  /**
   * Pushes value into the unit.
   * @param value the value sent to the unit
   * @return the new value of the unit
   */
  virtual bool putOn(bool value) { return (_onValue = value); } // do nothing by default (read-only)

  /// Returns true if the value rose.
  virtual bool rose() { return changeState() > 0; }

  /// Returns true if the value fell.
  virtual bool fell() { return changeState() < 0; }

  /// Returns true if the value changed.
  virtual bool changed() { return changeState() != 0; }

  /// Switches between on and off.
  virtual bool toggle() { return putOn(!isOn()); }

  /// Difference between current and previous value of the unit.
  virtual int8_t changeState() { return _changeState; }

  /// Registers event callback on rise event.
  virtual void onRise(EventCallback callback)   { onEvent(callback, EVENT_RISE); }

  /// Registers event callback on fall event.
  virtual void onFall(EventCallback callback)   { onEvent(callback, EVENT_FALL); }

  /// Registers event callback on change event.
  virtual void onChange(EventCallback callback) { onEvent(callback, EVENT_CHANGE); }

protected:
  void _updateChangeState() {
    _changeState = (int8_t)_onValue - (int8_t)_prevOnValue;
    _prevOnValue = _onValue;
  }

  /// Returns true iff an event of a certain type has been triggered.
  virtual bool eventTriggered(EventType eventType) {
    switch (eventType) {
      case EVENT_CHANGE: return changed();
      case EVENT_RISE:   return rose();
      case EVENT_FALL:   return fell();
      default:           return DigitalUnit::eventTriggered(eventType);
    }
  }

  // The value contained in the unit.
  bool    _onValue     : 1;

  // Previous value, used to compute change state.
  bool    _prevOnValue : 1;

  // The change state contained in the unit.
  int8_t  _changeState : 2;

  // Unused extra space.
  uint8_t _data        : 4;
};

// Value to unit operators ///////////////////////////////////////

// Base value to unit operator.
inline Chainable& operator>>(float value, Chainable& unit) {
  unit.put(value);
  return unit;
}

// NOTE: do not change the order of this operator (it needs to be set *after* the >>(float, Chainable&)).
inline Chainable& operator>>(Chainable& source, Chainable& sink) {
  return pq::operator>>(source.get(), sink);
}

inline Chainable& operator>>(double value, Chainable& unit) {
  return pq::operator>>((float)value, unit);
}

inline Chainable& operator>>(bool value, Chainable& unit) {
  return pq::operator>>(Chainable::digitalToAnalog(value), unit);
}

// This code is needed on the Curie and ARM chips.
// Otherwise it causes an ambiguous operator error.
#if defined(__arc__) || defined(__arm__)
inline Chainable& operator>>(int value, Chainable& unit) {
  return pq::operator>>((float)value, unit);
}
#endif

inline Chainable& operator>>(int8_t value, Chainable& unit) {
  return pq::operator>>((float)value, unit);
}

inline Chainable& operator>>(uint8_t value, Chainable& unit) {
  return pq::operator>>((float)value, unit);
}

inline Chainable& operator>>(int16_t value, Chainable& unit) {
  return pq::operator>>((float)value, unit);
}

inline Chainable& operator>>(uint16_t value, Chainable& unit) {
  return pq::operator>>((float)value, unit);
}

inline Chainable& operator>>(int32_t value, Chainable& unit) {
  return pq::operator>>((float)value, unit);
}

inline Chainable& operator>>(uint32_t value, Chainable& unit) {
  return pq::operator>>((float)value, unit);
}

inline Chainable& operator>>(int64_t value, Chainable& unit) {
  return pq::operator>>((float)value, unit);
}

inline Chainable& operator>>(uint64_t value, Chainable& unit) {
  return pq::operator>>((float)value, unit);
}

// // Unit to value operators ///////////////////////////////////////
// THIS PART IS COMMENTED OUT BECAUSE IT CAUSES AMBIGUOUS OPERATOR ERRORS

// inline bool& operator>>(DigitalUnit& unit, bool& value) {
//   return (value = unit.isOn());
// }

// // This code is needed on the Curie-based AVRs.
// #if defined(__arc__)
// inline int& operator>>(DigitalUnit& unit, int& value) {
//   return (value = unit.getInt());
// }
// #endif

// inline int8_t& operator>>(DigitalUnit& unit, int8_t& value) {
//   return (value = unit.getInt());
// }

// inline uint8_t& operator>>(DigitalUnit& unit, uint8_t& value) {
//   return (value = unit.getInt());
// }

// inline int16_t& operator>>(DigitalUnit& unit, int16_t& value) {
//   return (value = unit.getInt());
// }

// inline uint16_t& operator>>(DigitalUnit& unit, uint16_t& value) {
//   return (value = unit.getInt());
// }

// inline int32_t& operator>>(DigitalUnit& unit, int32_t& value) {
//   return (value = unit.getInt());
// }

// inline uint32_t& operator>>(DigitalUnit& unit, uint32_t& value) {
//   return (value = unit.getInt());
// }

// inline int64_t& operator>>(DigitalUnit& unit, int64_t& value) {
//   return (value = unit.getInt());
// }

// inline uint64_t& operator>>(DigitalUnit& unit, uint64_t& value) {
//   return (value = unit.getInt());
// }

// inline float& operator>>(Unit& unit, float& value) {
//   return (value = unit.get());
// }

// inline double& operator>>(Unit& unit, double& value) {
//   return (value = unit.get());
// }

/// Superclass for pin-based components.
class PinConfig {
public:
  PinConfig(uint8_t pin, uint8_t mode) : _pin(pin), _mode(mode) {}
  virtual ~PinConfig() {}

  /// Returns the pin this component is attached to.
  uint8_t pin() const { return _pin; }

  /// Returns the mode of the component.
  uint8_t mode() const { return _mode; }

  /// Changes the mode of the component.
  virtual void mode(uint8_t mode) { _mode = mode; }

protected:
  // The attached pin.
  uint8_t _pin;

  // The mode (varies according to context).
  uint8_t _mode;
};

// Inline methods.

void Engine::preStep() {
  // Update every component.
  for (size_t i=_unitsBeginIndex; i != _unitsEndIndex; i++) {
    units()[i]->step();
  }

  // Look for events.
  _eventManager.step();
}

bool Engine::timeStep() {
  // Calculate true sample rate.
  _updateGlobalMicroSeconds();

  // Compute inter-step time.
  _deltaTimeMicroSeconds = _totalGlobalMicroSeconds.micros32.base - _microSeconds.micros32.base;
  float trueSampleRate = (_deltaTimeMicroSeconds ? SECONDS_TO_MICROS / _deltaTimeMicroSeconds : PLAQUETTE_MAX_SAMPLE_RATE);

  // If autoSampleRate is off: wait in order to synchronize seconds with real time.
  if (!_autoSampleRate) {

    // Initilize step state.
    if (_stepState == STEP_INIT) {
      // Target time = current time + 1/_targetSampleRate
      _targetTime = _microSeconds;
      _targetTime.micros32.base += _targetDeltaTimeMicroSeconds;

      // Check for overflow.
      if (_targetTime.micros32.base >= _microSeconds.micros32.base) { // if target time is in the future: no overflow.
        _stepState = STEP_WAIT;
      }
      else { // overflow
        _targetTime.micros32.overflows++;
        _stepState = STEP_WAIT_OVERFLOW;
      }
    }

    // Process waiting state: will return false if still waiting, otherwise will complete the function.
    if (_stepState == STEP_WAIT) {
      // Still needs to wait.
      if (_totalGlobalMicroSeconds.micros32.base < _targetTime.micros32.base)
        return false; // break
    }
    else { // stepState == STEP_WAIT_OVERFLOW
      // Still needs to wait.
      if (_totalGlobalMicroSeconds.micros64 < _targetTime.micros64)
        return false; // break
    }

    // Reset state.
    _stepState = STEP_INIT;
  }

  // Update sample rate and current time to "true" / actual values.
  _setSampleRate(trueSampleRate);

  // Calculate delta time in fixed point.
  uint64_t deltaTimeMicroSeconds64 = (uint64_t)_deltaTimeMicroSeconds;
  _deltaTimeSecondsTimesFixed32Max = ((deltaTimeMicroSeconds64 << 32) - deltaTimeMicroSeconds64) * MICROS_TO_SECONDS;

  // Sync reference time with global (true) time.
  _microSeconds = _totalGlobalMicroSeconds;

  // Increment step.
  _nSteps++;

  return true;
}

void Engine::begin(unsigned long baudrate) {
  preBegin(baudrate);
}

bool Engine::step() {
  // On first run: do a post-begin.
  if (_firstRun) { // the compiler should make this branching step effectively CPU-free
    postBegin();
    _firstRun = false;
    return false;
  }

  // Otherwise: do a step.
  else if (timeStep()) { // timeStep() will return false if we need to wait due to restrictive sampleRate(float)
    // Do the pre-step.
    preStep();
    return true;
  }
  else
    return false;
}

void Engine::_setSampleRate(float sampleRate) {
  _sampleRate = max(sampleRate, FLT_MIN); // cannot be zero
  _samplePeriod = 0; // set to zero to reset cache
}

} // namespace pq

#endif
