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

namespace pq {

class PqUnit;

/// The main Plaquette static class containing all the components.
class Plaquette {
  friend class PqUnit;

private:
  // Used to keep track of components.
  static PqUnit* _units[PLAQUETTE_MAX_UNITS];
  static uint8_t _nUnits;

  // Snapshot of time in seconds from current step.
  static float _seconds;

  // Sampling rate (ie. how many times per seconds step() is called).
  static float _sampleRate;

  // // Internal use.
  // static float _secondsPerStep;

  // Whether the auto sample rate mode is activated.
  static float _targetSampleRate;

  // Number of steps accomplished.
  static unsigned long _nSteps;

  static bool _firstRun;

public:
  /// Initializes all components (calls begin() on all of them).
  static void preBegin();

  /// Performs additional tasks after the class to begin().
  static void postBegin();

  /// Updates all components (calls step() on all of them).
  static inline void preStep();

  /// Performs additional tasks after the class to step().
  static inline void postStep();

  /// Function to be used within the PlaquetteLib context (needs to be called at top of setup() method).
  static inline void begin();

  /// Function to be used within the PlaquetteLib context (needs to be called at top of loop() method).
  static inline void step();

  /**
   * Optional function to be used within the PlaquetteLib context. No need to call it if the program
   * is looping indefinitely. Call if the program stops at some point.
   */
  static inline void end();

  /// Returns the current number of units.
  static uint8_t nUnits() { return _nUnits; }

  /// Returns time in seconds.
  static float seconds(bool realTime=false);

  /// Returns number of steps.
  static unsigned long nSteps() { return _nSteps; }

  /// Returns true iff the auto sample rate mode is enabled (default).
  static bool autoSampleRate();

  /// Enables auto sample rate mode (default).
  static void enableAutoSampleRate();

  /// Sets sample rate to a fixed value, thus disabling auto sampling rate.
  static void sampleRate(float sampleRate);

  /// Returns sample rate.
  static float sampleRate() { return _sampleRate; }

private:
  /// Adds a component to Plaquette.
  static void add(PqUnit * component);
};

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


/**
 * Main class for components to be added to Plaquette.
 * Components can be transducers (sensors,actuators) or special integrated circuits.
 */
class PqUnit {
  friend class Plaquette;

public:
	/// Converts analog (float) value to digital (bool) value.
	static bool  analogToDigital(float f);

	/// Converts digital (bool) value to analog (float) value.
	static float digitalToAnalog(bool b);

protected:
  PqUnit();
  virtual ~PqUnit() {}

protected:
  virtual void begin() {}
  virtual void step() {}
};

/// A generic class representing a simple source.
class PqGetter {
public:
  /// Constructor.
  PqGetter() {}
  virtual ~PqGetter() {}

  /// Returns value (typically between 0 and 1, may vary depending on class).
  virtual float get() = 0;

  /// Operator that allows usage in conditional expressions.
	// NOTE: This operator is defined as explicit so that boolean expression like
	// "if (obj)" use the bool() operator while other expressions can use the float() operator.
  virtual explicit operator bool() { return PqUnit::analogToDigital(get()); }

  /// Object can be used directly to access its value.
  operator float() { return get(); }

private:
  // Prevents assignation operations by making them private.
  PqGetter& operator=(bool);
  PqGetter& operator=(int);
  PqGetter& operator=(float);
  PqGetter& operator=(PqGetter&);
  PqGetter(const PqGetter&);

  // // Logical operators.
  // bool operator==(float value) { return get() == value; }
  // bool operator!=(float value) { return get() != value; }
  // bool operator>(float value)  { return get() >  value; }
  // bool operator>=(float value) { return get() >= value; }
  // bool operator<(float value)  { return get() <  value; }
  // bool operator<=(float value) { return get() <= value; }
  //

  // Math operators.
  // virtual float operator+(PqGetter& getter) { return get() + getter.get(); }
  // virtual float operator+(int8_t value)     { return get() + value; }
  // virtual float operator+(uint8_t value)    { return get() + value; }
  // virtual float operator+(int16_t value)    { return get() + value; }
  // virtual float operator+(uint16_t value)   { return get() + value; }
  // virtual float operator+(int32_t value)    { return get() + value; }
  // virtual float operator+(uint32_t value)   { return get() + value; }
  // virtual float operator+(int64_t value)    { return get() + value; }
  // virtual float operator+(uint64_t value)   { return get() + value; }
  // virtual float operator+(float value)      { return get() + value; }
  // virtual float operator+(double value)     { return get() + value; }
  // virtual float operator-(PqGetter& getter) { return get() - getter.get(); }
  // virtual float operator-(int8_t value)     { return get() - value; }
  // virtual float operator-(uint8_t value)    { return get() - value; }
  // virtual float operator-(int16_t value)    { return get() - value; }
  // virtual float operator-(uint16_t value)   { return get() - value; }
  // virtual float operator-(int32_t value)    { return get() - value; }
  // virtual float operator-(uint32_t value)   { return get() - value; }
  // virtual float operator-(int64_t value)    { return get() - value; }
  // virtual float operator-(uint64_t value)   { return get() - value; }
  // virtual float operator-(float value)      { return get() - value; }
  // virtual float operator-(double value)     { return get() - value; }
  // virtual float operator*(PqGetter& getter) { return get() * getter.get(); }
  // virtual float operator*(int8_t value)     { return get() * value; }
  // virtual float operator*(uint8_t value)    { return get() * value; }
  // virtual float operator*(int16_t value)    { return get() * value; }
  // virtual float operator*(uint16_t value)   { return get() * value; }
  // virtual float operator*(int32_t value)    { return get() * value; }
  // virtual float operator*(uint32_t value)   { return get() * value; }
  // virtual float operator*(int64_t value)    { return get() * value; }
  // virtual float operator*(uint64_t value)   { return get() * value; }
  // virtual float operator*(float value)      { return get() * value; }
  // virtual float operator*(double value)     { return get() * value; }
  // virtual float operator/(PqGetter& getter) { return get() / getter.get(); }
  // virtual float operator/(int8_t value)     { return get() / value; }
  // virtual float operator/(uint8_t value)    { return get() / value; }
  // virtual float operator/(int16_t value)    { return get() / value; }
  // virtual float operator/(uint16_t value)   { return get() / value; }
  // virtual float operator/(int32_t value)    { return get() / value; }
  // virtual float operator/(uint32_t value)   { return get() / value; }
  // virtual float operator/(int64_t value)    { return get() / value; }
  // virtual float operator/(uint64_t value)   { return get() / value; }
  // virtual float operator/(float value)      { return get() / value; }
  // virtual float operator/(double value)     { return get() / value; }
};

/// A generic class representing a simple source.
class PqDigitalGetter : virtual public PqGetter {
public:
  /// Constructor.
  PqDigitalGetter() {}
  virtual ~PqDigitalGetter() {}

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

/// A generic class representing a simple sink.
class PqPutter {
public:
  /// Constructor.
  PqPutter() {}
  virtual ~PqPutter() {}

  /**
   * Pushes value into the unit.
   * @param value the value sent to the unit
   * @return the new value of the unit
   */
  virtual float put(float value) = 0;

  // // Math operators.
  // PqPutter& operator+=(PqGetter& getter) { put(get()+getter.get()); return *this; }
  // PqPutter& operator+=(float value)      { put(get()+value); return *this; }
  // PqPutter& operator-=(PqGetter& getter) { put(get()-getter.get()); return *this; }
  // PqPutter& operator-=(float value)      { put(get()-value); return *this; }
  // PqPutter& operator*=(PqGetter& getter) { put(get()*getter.get()); return *this; }
  // PqPutter& operator*=(float value)      { put(get()*value); return *this; }
  // PqPutter& operator/=(PqGetter& getter) { put(get()/getter.get()); return *this; }
  // PqPutter& operator/=(float value)      { put(get()/value); return *this; }

  // Assignation operators (disabled, replaced for now by the >> operators).
  //PqPutter& operator=(const PqGetter& x) { put(const_cast<PqGetter*>(&x)->get()); return *this; }
  //PqPutter& operator=(float value)  { put(value); return *this; }
};

/// A generic class representing a simple source.
class PqDigitalPutter : public PqPutter {
public:
  /// Constructor.
  PqDigitalPutter() {}
  virtual ~PqDigitalPutter() {}

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
    putOn(PqUnit::analogToDigital(value));
  }

  /**
   * Pushes value into the unit.
   * @param value the value sent to the unit
   * @return the new value of the unit
   */
  virtual bool putOn(bool value) = 0;
  // /// Returns reading (either 0 or 1).
  // virtual float get() { return PqDigitalGetter::get(); }
};

class PqAnalogSource : public PqUnit, public PqGetter {
public:
  /// Constructor.
  PqAnalogSource() : _value(0) {}
  virtual ~PqAnalogSource() {}

  /// Returns value (typically between 0 and 1, may vary depending on class).
  virtual float get() { return _value; }

protected:
  float _value;
};

class PqDigitalSource : public PqUnit, public PqDigitalGetter {
public:
  /// Constructor.
  PqDigitalSource() : _isOn(false) {}
  virtual ~PqDigitalSource() {}

  /// Returns true iff the input is "on".
  virtual bool isOn() { return _isOn; }

  /// Returns true if the value rose since last call to step().
  virtual bool rose() { return changeState() > 0; }

  /// Returns true if the value fell since last call to step().
  virtual bool fell() { return changeState() < 0; }

  /// Returns true if the value changed since last call to step().
  virtual bool changed() { changeState() == 0; }

  /// Difference between current and previous value of the object since last call to step().
  virtual int8_t changeState() = 0;

protected:
  bool _isOn;
};

class PqAnalogUnit : public PqAnalogSource, public PqPutter {
public:
  /// Constructor.
  PqAnalogUnit() {}
  virtual ~PqAnalogUnit() {}

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
  PqDigitalUnit() {}
  virtual ~PqDigitalUnit() {}

  /// Switches between on and off.
  virtual bool toggle() {
    return putOn(!isOn());
  }

  /**
   * Pushes value into the unit.
   * @param value the value sent to the unit
   * @return the new value of the unit
   */
  virtual bool putOn(bool isOn) { return (_isOn = isOn); }
};

// Operators /////////////////////////////////////////////////////

inline PqAnalogUnit& operator>>(float value, PqAnalogUnit& putter) {
  putter.put( value );
  return putter;
}

// NOTE: do not change the order of this operator (it needs to be set *after* the >>(float, PqPutter&)).
inline PqAnalogSource& operator>>(PqAnalogSource& getter, PqAnalogUnit& putter) {
	return pq::operator>>(getter.get(), putter);
}

inline PqAnalogUnit& operator>>(double value, PqAnalogUnit& putter) {
  return pq::operator>>((float)value, putter);
}

inline PqAnalogUnit& operator>>(bool value, PqAnalogUnit& putter) {
	return pq::operator>>(PqUnit::digitalToAnalog(value), putter);
}

// This code is needed on the Curie-based AVRs.
#if defined(__arc__)
inline PqAnalogUnit& operator>>(int value, PqAnalogUnit& putter) {
	return pq::operator>>((float)value, putter);
}
#endif

inline PqAnalogUnit& operator>>(int8_t value, PqAnalogUnit& putter) {
	return pq::operator>>((float)value, putter);
}

inline PqAnalogUnit& operator>>(uint8_t value, PqAnalogUnit& putter) {
	return pq::operator>>((float)value, putter);
}

inline PqAnalogUnit& operator>>(int16_t value, PqAnalogUnit& putter) {
	return pq::operator>>((float)value, putter);
}

inline PqAnalogUnit& operator>>(uint16_t value, PqAnalogUnit& putter) {
	return pq::operator>>((float)value, putter);
}

inline PqAnalogUnit& operator>>(int32_t value, PqAnalogUnit& putter) {
	return pq::operator>>((float)value, putter);
}

inline PqAnalogUnit& operator>>(uint32_t value, PqAnalogUnit& putter) {
	return pq::operator>>((float)value, putter);
}

inline PqAnalogUnit& operator>>(int64_t value, PqAnalogUnit& putter) {
	return pq::operator>>((float)value, putter);
}

inline PqAnalogUnit& operator>>(uint64_t value, PqAnalogUnit& putter) {
	return pq::operator>>((float)value, putter);
}

inline bool& operator>>(PqDigitalGetter& getter, bool& value) {
  return (value = getter.isOn());
}

// This code is needed on the Curie-based AVRs.
#if defined(__arc__)
inline int& operator>>(PqDigitalGetter& getter, int& value) {
  return (value = getter.getInt());
}
#endif

inline int8_t& operator>>(PqDigitalGetter& getter, int8_t& value) {
  return (value = getter.getInt());
}

inline uint8_t& operator>>(PqDigitalGetter& getter, uint8_t& value) {
  return (value = getter.getInt());
}

inline int16_t& operator>>(PqDigitalGetter& getter, int16_t& value) {
  return (value = getter.getInt());
}

inline uint16_t& operator>>(PqDigitalGetter& getter, uint16_t& value) {
  return (value = getter.getInt());
}

inline int32_t& operator>>(PqDigitalGetter& getter, int32_t& value) {
  return (value = getter.getInt());
}

inline uint32_t& operator>>(PqDigitalGetter& getter, uint32_t& value) {
  return (value = getter.getInt());
}

inline int64_t& operator>>(PqDigitalGetter& getter, int64_t& value) {
  return (value = getter.getInt());
}

inline uint64_t& operator>>(PqDigitalGetter& getter, uint64_t& value) {
  return (value = getter.getInt());
}

inline float& operator>>(PqGetter& getter, float& value) {
  return (value = getter.get());
}

inline double& operator>>(PqGetter& getter, double& value) {
  return (value = getter.get());
}

// HACK: These operator overrides are there just to avoid making a bitshift
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
class PqPinUnit : public PqUnit {
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



void Plaquette::preStep() {
  // Update every component.
  for (uint8_t i=0; i<_nUnits; i++)
    _units[i]->step();
}

void Plaquette::postStep() {
  // Increment step.
  _nSteps++;

  // Calculate true sample rate.
  float newTime = seconds(true);
  float trueSampleRate = 1.0f / (newTime - _seconds + FLT_MIN);
  // If we are in auto sample mode OR if the target sample rate is too fast for the "true" sample rate
  // then we should just assign the true sample rate.
  if (autoSampleRate() || trueSampleRate < _targetSampleRate) {
    _sampleRate = trueSampleRate;
    _seconds = newTime;
  }

  // Otherwise: Wait in order to synchronize seconds with real time.
  else {
    float targetTime = _seconds + 1.0f/_targetSampleRate;
    unsigned long targetTimeUs = (unsigned long)(targetTime * 1e6);
    while (micros() < targetTimeUs); // wait
    _sampleRate = _targetSampleRate;
    _seconds = targetTime; // not the exact "true" time but more accurate for computations
  }
}

void Plaquette::begin() {
  preBegin();
}

void Plaquette::step() {
  if (_firstRun) {
    postBegin();
    _firstRun = false;
  }
  else
    postStep();

  // Do the pre-step.
  preStep();
}

} // namespace pq

#endif
