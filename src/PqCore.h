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

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
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

#define PLAQUETTE_NO_SMOOTH_WINDOW 0

#ifndef PLAQUETTE_DEFAULT_DEBOUNCE_WINDOW
#define PLAQUETTE_DEFAULT_DEBOUNCE_WINDOW 0.005F // 5 ms
#endif

#define PLAQUETTE_NO_DEBOUNCE_WINDOW 0

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
#if defined(CORE_TEENSY)
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
	static bool  analogToDigital(float f);

	/// Converts digital (bool) value to analog (float) value.
	static float digitalToAnalog(bool b);

protected:
  /** Class constructor.
   * The parameter addUnit specifies whether we should add the unit to the
   * Plaquette environment. Important to use with multiple inheritance to avoid
   * adding the same instance twice. See how it is used for example in the
   * PqDigitalPutter constructor below.
   */
  PqUnit();
  virtual ~PqUnit() {}

protected:
  virtual void begin() {}
  virtual void step() {}
};


/// A generic class representing a simple source.
class PqGetter : public PqUnit {
public:
  /// Returns value (typically between 0 and 1, may vary depending on class).
  virtual float get() = 0;

  /// Object can be used directly to access its value.
  operator float() { return get(); }

protected:
  /// Constructor.
  PqGetter() : PqUnit() {}

private:
  /// Operator that allows usage in conditional expressions.
	// NOTE: This operator is defined as explicit so that boolean expression like
	// "if (obj)" use the bool() operator while other expressions can use the float() operator.
  virtual explicit operator bool() { return PqUnit::analogToDigital(get()); }

  // Prevents assignation operations by making them private.
  PqGetter& operator=(bool);
  PqGetter& operator=(int);
  PqGetter& operator=(float);
  PqGetter& operator=(PqGetter&);
  PqGetter(const PqGetter&);
};

/// A generic class representing a simple filtering unit or sink.
class PqPutter : public PqGetter {
public:
  /// Constructor.
  PqPutter() : PqGetter() {}

  /**
   * Pushes value into the unit.
   * @param value the value sent to the unit
   * @return the new value of the unit
   */
  virtual float put(float value) = 0;
};

/// A generic class representing a simple source.
class PqDigitalGetter : public PqGetter {
public:
  /// Constructor.
  PqDigitalGetter() : PqGetter() {}

  /// Returns true iff the input is "on".
  virtual bool isOn() = 0;

  /// Returns true iff the input is "off".
  virtual bool isOff() { return !isOn(); }

  /// Returns value as integer (0 or 1).
  virtual int getInt() { return isOn() ? 1 : 0; }

  /// Returns value as float (either 0.0 or 1.0).
  virtual float get() { return getInt(); }

  /// Operator that allows usage in conditional expressions.
  virtual explicit operator bool() { return isOn(); }

  /// Operator that return 0 or 1 depending on value.
  explicit operator int() { return getInt(); }
};

/// A generic class representing a simple source.
class PqDigitalPutter : public PqDigitalGetter, public PqPutter {
public:
  /// Constructor.
  PqDigitalPutter() : PqDigitalGetter(), PqPutter() {}

  /// Sets output to "on".
  virtual bool on() { return putOn(true); }

  /// Sets output to "off".
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
  virtual bool putOn(bool value) = 0;
};

class PqMappable : public PqGetter {
public:
  /// Constructor.
  PqMappable() : PqGetter() {}
  virtual ~PqMappable() {}

  /// Maps value to new range.
  virtual float mapTo(float toLow, float toHigh);

protected:
  // This is the function that needs to be overriden by subclasses.
  virtual float _map(float value, float toLow, float toHigh);
};

class PqAnalogSource : public PqGetter {
public:
  /// Constructor.
  PqAnalogSource(float init=0.0f) : PqGetter(), _value(init) {}
  virtual ~PqAnalogSource() {}

  /// Returns value (typically between 0 and 1, may vary depending on class).
  virtual float get() { return _value; }

protected:
  float _value;
};

class PqDigitalSource : public PqDigitalGetter {
public:
  /// Constructor.
  PqDigitalSource(bool init=false) : PqDigitalGetter(), _onValue(init) {}

  /// Returns true iff the input is "on".
  virtual bool isOn() { return _onValue; }

  /// Returns true if the value rose.
  virtual bool rose() { return changeState() > 0; }

  /// Returns true if the value fell.
  virtual bool fell() { return changeState() < 0; }

  /// Returns true if the value changed.
  virtual bool changed() { return changeState() != 0; }

  /// Difference between current and previous value of the unit.
  virtual int8_t changeState() = 0;

protected:
  bool _onValue;
};

class PqAnalogUnit : public PqAnalogSource, public PqPutter {
public:
  /// Constructor.
  PqAnalogUnit(float init=0.0f) : PqAnalogSource(init), PqPutter() {}

  /// Returns value (typically between 0 and 1, may vary depending on class).
  virtual float get() { return PqAnalogSource::get(); }

  /// Object can be used directly to access its value.
  // Somehow we need to re-write this operator here, otherwise it causes problems.
  operator float() { return get(); }

  /**
   * Pushes value into the unit.
   * @param value the value sent to the unit
   * @return the new value of the unit
   */
  virtual float put(float value) { return (_value = value); }
};

class PqDigitalUnit : public PqDigitalSource, public PqDigitalPutter {
public:
  /// Constructor.
  PqDigitalUnit(bool init=false) : PqDigitalSource(init), PqDigitalPutter() {}

  /// Returns value (typically between 0 and 1, may vary depending on class).
  virtual float get() { return PqDigitalSource::get(); }

  /// Returns true iff the input is "on".
  virtual bool isOn() { return PqDigitalSource::isOn(); }

  /// Switches between on and off.
  virtual bool toggle() {
    return putOn(!isOn());
  }

  /**
   * Pushes value into the unit.
   * @param value the value sent to the unit
   * @return the new value of the unit
   */
  virtual bool putOn(bool on) { return (_onValue = on); }

  // NOTE: These operators need to be repeated here to prevent compilation errors
  // related to ambiguous call.

  /// Operator that allows usage in conditional expressions.
  virtual explicit operator bool() { return PqDigitalSource::operator bool(); }

  /// Operator that return 0 or 1 depending on value.
  explicit operator int() { return PqDigitalSource::operator int(); }

};

// Operators /////////////////////////////////////////////////////
inline PqPutter& operator>>(float value, PqPutter& unit) {
  unit.put( value );
  return unit;
}

// NOTE: do not change the order of this operator (it needs to be set *after* the >>(float, PqUnit&)).
inline PqGetter& operator>>(PqGetter& getter, PqPutter& putter) {
	return pq::operator>>(getter.get(), putter);
}

inline PqPutter& operator>>(double value, PqPutter& unit) {
  return pq::operator>>((float)value, unit);
}

inline PqPutter& operator>>(bool value, PqPutter& unit) {
	return pq::operator>>(PqUnit::digitalToAnalog(value), unit);
}

// This code is needed on the Curie and ARM chips.
// Otherwise it causes an ambiguous operator error.
#if defined(__arc__) || defined(__arm__)
inline PqPutter& operator>>(int value, PqPutter& unit) {
	return pq::operator>>((float)value, unit);
}
#endif

inline PqPutter& operator>>(int8_t value, PqPutter& unit) {
	return pq::operator>>((float)value, unit);
}

inline PqPutter& operator>>(uint8_t value, PqPutter& unit) {
	return pq::operator>>((float)value, unit);
}

inline PqPutter& operator>>(int16_t value, PqPutter& unit) {
	return pq::operator>>((float)value, unit);
}

inline PqPutter& operator>>(uint16_t value, PqPutter& unit) {
	return pq::operator>>((float)value, unit);
}

inline PqPutter& operator>>(int32_t value, PqPutter& unit) {
	return pq::operator>>((float)value, unit);
}

inline PqPutter& operator>>(uint32_t value, PqPutter& unit) {
	return pq::operator>>((float)value, unit);
}

inline PqPutter& operator>>(int64_t value, PqPutter& unit) {
	return pq::operator>>((float)value, unit);
}

inline PqPutter& operator>>(uint64_t value, PqPutter& unit) {
	return pq::operator>>((float)value, unit);
}

inline bool& operator>>(PqDigitalPutter& unit, bool& value) {
  return (value = unit.isOn());
}

// This code is needed on the Curie-based AVRs.
#if defined(__arc__)
inline int& operator>>(PqDigitalPutter& unit, int& value) {
  return (value = unit.getInt());
}
#endif

inline int8_t& operator>>(PqDigitalPutter& unit, int8_t& value) {
  return (value = unit.getInt());
}

inline uint8_t& operator>>(PqDigitalPutter& unit, uint8_t& value) {
  return (value = unit.getInt());
}

inline int16_t& operator>>(PqDigitalPutter& unit, int16_t& value) {
  return (value = unit.getInt());
}

inline uint16_t& operator>>(PqDigitalPutter& unit, uint16_t& value) {
  return (value = unit.getInt());
}

inline int32_t& operator>>(PqDigitalPutter& unit, int32_t& value) {
  return (value = unit.getInt());
}

inline uint32_t& operator>>(PqDigitalPutter& unit, uint32_t& value) {
  return (value = unit.getInt());
}

inline int64_t& operator>>(PqDigitalPutter& unit, int64_t& value) {
  return (value = unit.getInt());
}

inline uint64_t& operator>>(PqDigitalPutter& unit, uint64_t& value) {
  return (value = unit.getInt());
}

inline float& operator>>(PqPutter& unit, float& value) {
  return (value = unit.get());
}

inline double& operator>>(PqPutter& unit, double& value) {
  return (value = unit.get());
}

// NOTE: These operator overrides are there just to avoid making a bitshift
// by error due to a combination of PqGetter operator float() and
// PqDigitalGetter int() and bool() which allow bitshift operations.
inline PqGetter& operator>>(float value,    PqGetter& getter) { return getter; }
inline PqGetter& operator>>(double value,   PqGetter& getter) { return getter; }
// This code is needed on the Curie-based AVRs.
#if defined(__arc__)
inline PqGetter& operator>>(int value,      PqGetter& getter) { return getter; }
#endif
inline PqGetter& operator>>(bool value,     PqGetter& getter) { return getter; }
inline PqGetter& operator>>(int8_t value,   PqGetter& getter) { return getter; }
inline PqGetter& operator>>(uint8_t value,  PqGetter& getter) { return getter; }
inline PqGetter& operator>>(int16_t value,  PqGetter& getter) { return getter; }
inline PqGetter& operator>>(uint16_t value, PqGetter& getter) { return getter; }
inline PqGetter& operator>>(int32_t value,  PqGetter& getter) { return getter; }
inline PqGetter& operator>>(uint32_t value, PqGetter& getter) { return getter; }
inline PqGetter& operator>>(int64_t value,  PqGetter& getter) { return getter; }
inline PqGetter& operator>>(uint64_t value, PqGetter& getter) { return getter; }

// inline float operator+(PqGetter& getter1, PqGetter& getter2) { return getter1.get() + getter2.get(); }
// inline float operator+(PqGetter& getter, int8_t value)     { return getter.get() + value; }
// inline float operator+(PqGetter& getter, uint8_t value)    { return getter.get() + value; }
// inline float operator+(PqGetter& getter, int16_t value)    { return getter.get() + value; }
// inline float operator+(PqGetter& getter, uint16_t value)   { return getter.get() + value; }
// inline float operator+(PqGetter& getter, int32_t value)    { return getter.get() + value; }
// inline float operator+(PqGetter& getter, uint32_t value)   { return getter.get() + value; }
// inline float operator+(PqGetter& getter, int64_t value)    { return getter.get() + value; }
// inline float operator+(PqGetter& getter, uint64_t value)   { return getter.get() + value; }
// inline float operator+(PqGetter& getter, float value)      { return getter.get() + value; }
// inline float operator+(PqGetter& getter, double value)     { return getter.get() + value; }
// inline float operator+(int8_t value,   PqGetter& getter)   { return getter.get() + value; }
// inline float operator+(uint8_t value,  PqGetter& getter)   { return getter.get() + value; }
// inline float operator+(int16_t value,  PqGetter& getter)   { return getter.get() + value; }
// inline float operator+(uint16_t value, PqGetter& getter)   { return getter.get() + value; }
// inline float operator+(int32_t value,  PqGetter& getter)   { return getter.get() + value; }
// inline float operator+(uint32_t value, PqGetter& getter)   { return getter.get() + value; }
// inline float operator+(int64_t value,  PqGetter& getter)   { return getter.get() + value; }
// inline float operator+(uint64_t value, PqGetter& getter)   { return getter.get() + value; }
// inline float operator+(float value,    PqGetter& getter)   { return getter.get() + value; }
// inline float operator+(double value,   PqGetter& getter)   { return getter.get() + value; }

//inline PqDigitalPutter& operator>>(bool value, PqDigitalPutter& putter) {
//  putter.setIsOn(value);
//  return putter;
//}
//
//inline bool& operator>>(PqDigitalGetter& getter, bool& value) {
//  return (value = getter.isOn());
//}

//inline float& operator>>(float& valueOut, float& valueIn) {
//  return (valueIn = valueOut);
//}

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
