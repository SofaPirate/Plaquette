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

#include "pq_map_real.h"

#if (defined(EPOXY_DUINO) || defined(CORE_TEENSY))
#define PLAQUETTE_USE_SINGLETON
#endif
#ifndef PLAQUETTE_MAX_UNITS
/// Max. components that can be added. Can be pre-defined.
#define PLAQUETTE_MAX_UNITS 32
#endif

#ifndef PLAQUETTE_SERIAL_BAUD_RATE
#define PLAQUETTE_SERIAL_BAUD_RATE 9600
#endif

#ifndef PLAQUETTE_DEFAULT_SMOOTH_WINDOW
#define PLAQUETTE_DEFAULT_SMOOTH_WINDOW 0.1F
#endif

#define PLAQUETTE_NO_SMOOTH_WINDOW 0.0F
#define PLAQUETTE_INFINITE_SMOOTH_WINDOW FLT_MAX

#ifndef PLAQUETTE_DEFAULT_DEBOUNCE_WINDOW
#define PLAQUETTE_DEFAULT_DEBOUNCE_WINDOW 0.005F // 5 ms
#endif

#define PLAQUETTE_NO_DEBOUNCE_WINDOW 0.0F

// This constant is used in the oscillators to prevent phaseTime from increasing
// too fast (which would be useless).
#define PLAQUETTE_OSC_MIN_SAMPLE_PERIOD_MULTIPLIER 2.000001 // = almost 2

namespace pq {

class PqUnit;

/// The main Plaquette static class containing all the units.
class PlaquetteEnv {
  friend class PqUnit;

private:
  // Used to keep track of units.
  PqUnit* _units[PLAQUETTE_MAX_UNITS];
  uint8_t _nUnits;

  // Snapshot of time in seconds from current step.
  float _seconds;

  // Sampling rate (ie. how many times per seconds step() is called).
  float _sampleRate;

  // Sampling period (ie. 1.0 / sampleRate()).
  float _samplePeriod;

  // Whether the auto sample rate mode is activated.
  float _targetSampleRate;

  // Number of steps accomplished.
  unsigned long _nSteps;

  bool _firstRun;

public:
  PlaquetteEnv();

  /// Initializes all components (calls begin() on all of them).
  void preBegin();

  /// Performs additional tasks after the class to begin().
  void postBegin();

  /// Updates all components (calls step() on all of them).
  inline void preStep();

  /// Performs additional tasks after the class to step().
  inline void postStep();

  /// Function to be used within the PlaquetteLib context (needs to be called at top of setup() method).
  inline void begin();

  /// Function to be used within the PlaquetteLib context (needs to be called at top of loop() method).
  inline void step();

  /**
   * Optional function to be used within the PlaquetteLib context. No need to call it if the program
   * is looping indefinitely. Call if the program stops at some point.
   */
  inline void end();

  /// Returns the current number of units.
  uint8_t nUnits() { return _nUnits; }

  /**
   * Returns time in seconds. Optional parameter allows to ask for reference time (default)
   * which will yield the same value through one iteration of step(), or "real" time which Will
   * return the current total running time.
   * @param referenceTime determines whether the function returns the reference time or the real time
   * @return the time in seconds
   */
  inline float seconds(bool referenceTime=true);

  /// Returns number of steps.
  unsigned long nSteps() { return _nSteps; }

  /// Returns true iff the auto sample rate mode is enabled (default).
  bool autoSampleRate();

  /// Enables auto sample rate mode (default).
  void enableAutoSampleRate();

  /// Sets sample rate to a fixed value, thus disabling auto sampling rate.
  void sampleRate(float sampleRate);

  /// Returns sample rate.
  float sampleRate() const { return _sampleRate; }

  /// Returns sample period.
  float samplePeriod() const { return _samplePeriod; }

  // Returns singleton.
#ifdef PLAQUETTE_USE_SINGLETON
  static PlaquetteEnv& singleton() {
    static PlaquetteEnv inst;
    return inst;
  }
#endif

private:
  /// Adds a component to Plaquette.
  void add(PqUnit * component);

  // Internal use. Sets sample rate and sample period.
  inline void _setSampleRate(float sampleRate);
};

/// The Plaquette singleton.
extern PlaquetteEnv Plaquette;

//float seconds(bool realTime=false);
unsigned long nSteps();

/// Returns true iff the auto sample rate mode is enabled (default).
bool autoSampleRate();

/// Enables auto sample rate mode (default).
void enableAutoSampleRate();

/// Sets sample rate to a fixed value, thus disabling auto sampling rate.
void sampleRate(float sampleRate);

/// Returns sample rate.
float sampleRate();

/// Returns sample period.
float samplePeriod();

/// Restarts main serial. This method will make sure to flush data from the pipeline.
void beginSerial(unsigned long baudRate);

/**
 * Main class for components to be added to Plaquette.
 * Components can be transducers (sensors, actuators) or special integrated circuits.
 */
class PqUnit {
  friend class PlaquetteEnv;

public:
  /// Converts analog (float) value to digital (bool) value.
	static bool  analogToDigital(float f) { return (f >= 0.5); }

	/// Converts digital (bool) value to analog (float) value.
	static float digitalToAnalog(bool b) { return (b ? 1.0f : 0.0f); }

protected:
  /** Class constructor.
   * The parameter addUnit specifies whether we should add the unit to the
   * Plaquette environment. Important to use with multiple inheritance to avoid
   * adding the same instance twice. See how it is used for example in the
   * PqDigitalNode constructor below.
   */
  PqUnit();
  virtual ~PqUnit() {}

protected:
  virtual void begin() {}
  virtual void step() {}
};


/**
 * A generic class representing a node in the system.
 * Each such node can be read using function get(). Values
 * can also be sent to a node using put().
 */
class PqNode : public PqUnit {
public:
  /// Returns value (typically between 0 and 1, may vary depending on class).
  virtual float get() = 0;

  /// Object can be used directly to access its value.
  virtual operator float() { return get(); }

  /**
   * Pushes value into the unit.
   * @param value the value sent to the unit
   * @return the new value of the unit
   */
  virtual float put(float value) { return get(); } // do nothing by default (read-only)

  /// Maps value to new range. If the node's values are unbounded, simply returns get().
  virtual float mapTo(float toLow, float toHigh) { return get(); } // default: do nothing

protected:
  /// Constructor.
  PqNode() : PqUnit() {}

private:
  /// Operator that allows usage in conditional expressions.
	// NOTE: This operator is defined as explicit so that boolean expression like
	// "if (obj)" use the bool() operator while other expressions can use the float() operator.
  virtual explicit operator bool() { return PqUnit::analogToDigital(get()); }

  // Prevents assignation operations by making them private.
  PqNode& operator=(bool);
  PqNode& operator=(int);
  PqNode& operator=(float);
  PqNode& operator=(PqNode&);
  PqNode(const PqNode&);
};

/// A generic class representing a simple source.
class PqDigitalNode : public PqNode {
public:
  /// Constructor.
  PqDigitalNode() : PqNode() {}

  /// Returns true iff the input is "on".
  virtual bool isOn() = 0;

  /// Returns true iff the input is "off".
  virtual bool isOff() { return !isOn(); }

  /// Returns value as integer (0 or 1).
  virtual int getInt() { return isOn() ? 1 : 0; }

  /// Returns value as float (either 0.0 or 1.0).
  virtual float get() { return getInt(); }

  /// Sets output to "on" (ie. false, 0).
  virtual bool on() { return putOn(true); }

  /// Sets output to "off" (ie. true, 1).
  virtual bool off() { return putOn(false); }

  /**
   * Pushes value into the unit.
   * @param value the value sent to the unit
   * @return the new value of the unit
   */
  virtual float put(float value) {
    return PqUnit::digitalToAnalog(putOn(PqUnit::analogToDigital(value)));
  }

  /**
   * Pushes value into the unit.
   * @param value the value sent to the unit
   * @return the new value of the unit
   */
  virtual bool putOn(bool value) { return isOn(); } // do nothing by default (read-only)

  /// Maps value to new range.
  virtual float mapTo(float toLow, float toHigh) { return mapFrom01(get(), toLow, toHigh); }

  /// Operator that allows usage in conditional expressions.
  virtual operator bool() { return isOn(); }

  /// Operator that return 0 or 1 depending on value.
  virtual explicit operator int() { return getInt(); }
};

/**
 * An analog analog source that contains a value (typically in [0, 1]).
 * It is the responsability of the subclass's programmer to make sure the value stays
 * within the [0, 1] range OR to update the mapTo() function accordingly.
 */
class PqAnalogSource : public PqNode {
public:
  /// Constructor.
  PqAnalogSource(float init=0.0f) : PqNode() { _value = constrain(init, 0, 1); }
  virtual ~PqAnalogSource() {}

  /// Returns value in [0, 1].
  virtual float get() { return _value; }

  /// Maps value to new range.
  virtual float mapTo(float toLow, float toHigh) { return mapFrom01(get(), toLow, toHigh); }

protected:
  float _value;
};

/// A digital source that contains a true/false value.
class PqDigitalSource : public PqDigitalNode {
public:
  /// Constructor.
  PqDigitalSource(bool init=false) : PqDigitalNode(), _onValue(init) {}

  /// Returns true iff the input is "on".
  virtual bool isOn() { return _onValue; }

  /// Returns true if the value rose.
  virtual bool rose() { return changeState() > 0; }

  /// Returns true if the value fell.
  virtual bool fell() { return changeState() < 0; }

  /// Returns true if the value changed.
  virtual bool changed() { return changeState() != 0; }

  /// Switches between on and off.
  virtual bool toggle() { return putOn(!isOn()); }

  /// Difference between current and previous value of the unit.
  virtual int8_t changeState() = 0;

protected:
  // The value contained in the node.
  bool _onValue;
};

// Value to node operators ///////////////////////////////////////

// Base value to node operator.
inline PqNode& operator>>(float value, PqNode& unit) {
  unit.put(value);
  return unit;
}

// NOTE: do not change the order of this operator (it needs to be set *after* the >>(float, PqUnit&)).
inline PqNode& operator>>(PqNode& source, PqNode& sink) {
	return pq::operator>>(source.get(), sink);
}

inline PqNode& operator>>(double value, PqNode& unit) {
  return pq::operator>>((float)value, unit);
}

inline PqNode& operator>>(bool value, PqNode& unit) {
	return pq::operator>>(PqUnit::digitalToAnalog(value), unit);
}

// This code is needed on the Curie and ARM chips.
// Otherwise it causes an ambiguous operator error.
#if defined(__arc__) || defined(__arm__)
inline PqNode& operator>>(int value, PqNode& unit) {
	return pq::operator>>((float)value, unit);
}
#endif

inline PqNode& operator>>(int8_t value, PqNode& unit) {
	return pq::operator>>((float)value, unit);
}

inline PqNode& operator>>(uint8_t value, PqNode& unit) {
	return pq::operator>>((float)value, unit);
}

inline PqNode& operator>>(int16_t value, PqNode& unit) {
	return pq::operator>>((float)value, unit);
}

inline PqNode& operator>>(uint16_t value, PqNode& unit) {
	return pq::operator>>((float)value, unit);
}

inline PqNode& operator>>(int32_t value, PqNode& unit) {
	return pq::operator>>((float)value, unit);
}

inline PqNode& operator>>(uint32_t value, PqNode& unit) {
	return pq::operator>>((float)value, unit);
}

inline PqNode& operator>>(int64_t value, PqNode& unit) {
	return pq::operator>>((float)value, unit);
}

inline PqNode& operator>>(uint64_t value, PqNode& unit) {
	return pq::operator>>((float)value, unit);
}

inline bool& operator>>(PqDigitalNode& unit, bool& value) {
  return (value = unit.isOn());
}

// Node to value operators ///////////////////////////////////////

// This code is needed on the Curie-based AVRs.
#if defined(__arc__)
inline int& operator>>(PqDigitalNode& unit, int& value) {
  return (value = unit.getInt());
}
#endif

inline int8_t& operator>>(PqDigitalNode& unit, int8_t& value) {
  return (value = unit.getInt());
}

inline uint8_t& operator>>(PqDigitalNode& unit, uint8_t& value) {
  return (value = unit.getInt());
}

inline int16_t& operator>>(PqDigitalNode& unit, int16_t& value) {
  return (value = unit.getInt());
}

inline uint16_t& operator>>(PqDigitalNode& unit, uint16_t& value) {
  return (value = unit.getInt());
}

inline int32_t& operator>>(PqDigitalNode& unit, int32_t& value) {
  return (value = unit.getInt());
}

inline uint32_t& operator>>(PqDigitalNode& unit, uint32_t& value) {
  return (value = unit.getInt());
}

inline int64_t& operator>>(PqDigitalNode& unit, int64_t& value) {
  return (value = unit.getInt());
}

inline uint64_t& operator>>(PqDigitalNode& unit, uint64_t& value) {
  return (value = unit.getInt());
}

inline float& operator>>(PqNode& unit, float& value) {
  return (value = unit.get());
}

inline double& operator>>(PqNode& unit, double& value) {
  return (value = unit.get());
}

/// Superclass for pin-based components.
class PqPinUnit {
public:
  PqPinUnit(uint8_t pin, uint8_t mode) : _pin(pin), _mode(mode) {}
  virtual ~PqPinUnit() {}

  /// Returns the pin this component is attached to.
  uint8_t pin() const { return _pin; }

  /// Returns the mode of the component.
  uint8_t mode() const { return _mode; }

  // The attached pin.
  uint8_t _pin;

  // The mode (varies according to context).
  uint8_t _mode;
};

// Inline methods.

#include <float.h>

void PlaquetteEnv::preStep() {
  // Update every component.
  for (uint8_t i=0; i<_nUnits; i++)
    _units[i]->step();
}

void PlaquetteEnv::postStep() {
  // Increment step.
  _nSteps++;

  // Calculate true sample rate.
  float newTime = seconds(false);
  float trueSampleRate = 1.0f / (newTime - _seconds + FLT_MIN);
  // If we are in auto sample mode OR if the target sample rate is too fast for the "true" sample rate
  // then we should just assign the true sample rate.
  if (autoSampleRate() || trueSampleRate < _targetSampleRate) {
    _setSampleRate(trueSampleRate);
    _seconds = newTime;
  }

  // Otherwise: Wait in order to synchronize seconds with real time.
  else {
    float targetTime = _seconds + 1.0f/_targetSampleRate;
    unsigned long targetTimeUs = (unsigned long)(targetTime * 1e6);
    while (micros() < targetTimeUs); // wait
    _setSampleRate(_targetSampleRate);
    _seconds = targetTime; // not the exact "true" time but more accurate for computations
  }
}

void PlaquetteEnv::begin() {
  preBegin();
}

void PlaquetteEnv::step() {
  if (_firstRun) {
    postBegin();
    _firstRun = false;
  }
  else
    postStep();

  // Do the pre-step.
  preStep();
}

void PlaquetteEnv::_setSampleRate(float sampleRate) {
  _sampleRate = max(sampleRate, FLT_MIN); // cannot be zero
  _samplePeriod = 1.0f / _sampleRate;
}

float PlaquetteEnv::seconds(bool referenceTime) {
  return referenceTime ? _seconds : (micros() / 1e6f);
}


} // namespace pq

#endif
