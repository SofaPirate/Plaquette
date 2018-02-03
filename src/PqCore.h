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

#ifndef PLAQUETTE_MAX_COMPONENTS
/// Max. components that can be added. Can be pre-defined.
#define PLAQUETTE_MAX_COMPONENTS 32
#endif

#ifndef PLAQUETTE_SERIAL_BAUD_RATE
#define PLAQUETTE_SERIAL_BAUD_RATE 9600
#endif

class PqComponent;

/// The main Plaquette static class containing all the components.
class Plaquette {
  friend class PqComponent;

private:
  static PqComponent* _components[PLAQUETTE_MAX_COMPONENTS];
  static uint8_t _nComponents;

public:
  /// Initializes all components (calls setup() on all of them).
  static void setup();

  /// Updates all components (calls update() on all of them).
  static void update();

  /// Returns the current number of components.
  static uint8_t nComponents() { return _nComponents; }

private:
  /// Adds a component to Plaquette.
  static void add(PqComponent * component);
};

/**
 * Main class for components to be added to Plaquette.
 * Components can be transducers (sensors,actuators) or special integrated circuits.
 */
class PqComponent {
  friend class Plaquette;

public:
	/// Converts analog (float) value to digital (bool) value.
	static bool  analogToDigital(float f);

	/// Converts digital (bool) value to analog (float) value.
	static float digitalToAnalog(bool b);

protected:
  PqComponent();
  virtual ~PqComponent() {}

protected:
  virtual void setup() {}
  virtual void update() {}
};

/// A generic class representing a simple source.
class PqGetter : public PqComponent {
public:
  /// Constructor.
  PqGetter() {}
  virtual ~PqGetter() {}

  /// Returns value (typically between 0 and 1, may vary depending on class).
  virtual float get() = 0;

  /// Operator that allows usage in conditional expressions.
	// NOTE: This operator is defined as explicit so that boolean expression like
	// "if (obj)" use the bool() operator while other expressions can use the float() operator.
  virtual explicit operator bool() { return analogToDigital(get()); }

  /// Object can be used directly to access its value.
  operator float() { return get(); }

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
class PqPutter : virtual public PqGetter {
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
class PqDigitalPutter : public PqPutter, public PqDigitalGetter {
public:
  /// Constructor.
  PqDigitalPutter() {}
  virtual ~PqDigitalPutter() {}

  void _setIsOn(bool isOn) {
    put (isOn ? 1 : 0);
  }

  /// Sets output to "on".
  virtual void on() { _setIsOn(true); }

  /// Sets output to "off".
  virtual void off() { _setIsOn(false); }

  /// Switches between on and off.
  virtual void toggle() {
    _setIsOn(!isOn());
  }

  /// Returns reading (either 0 or 1).
  virtual float get() { return PqDigitalGetter::get(); }

};

// Operators /////////////////////////////////////////////////////

inline PqGetter& operator>>(PqGetter& getter, PqPutter& putter) {
	return ::operator>>(getter.get(), putter);
}

inline PqPutter& operator>>(float value, PqPutter& putter) {
  putter.put( value );
  return putter;
}

inline PqPutter& operator>>(double value, PqPutter& putter) {
  return ::operator>>((float)value, putter);
}

inline PqPutter& operator>>(bool value, PqPutter& putter) {
	return ::operator>>(value ? 1.0f : 0.0f, putter);
}

// This code is needed on the Curie-based AVRs.
#if defined(__arc__)
inline PqPutter& operator>>(int value, PqPutter& putter) {
	return ::operator>>((float)value, putter);
}
#endif

inline PqPutter& operator>>(int8_t value, PqPutter& putter) {
	return ::operator>>((float)value, putter);
}

inline PqPutter& operator>>(uint8_t value, PqPutter& putter) {
	return ::operator>>((float)value, putter);
}

inline PqPutter& operator>>(int16_t value, PqPutter& putter) {
	return ::operator>>((float)value, putter);
}

inline PqPutter& operator>>(uint16_t value, PqPutter& putter) {
	return ::operator>>((float)value, putter);
}

inline PqPutter& operator>>(int32_t value, PqPutter& putter) {
	return ::operator>>((float)value, putter);
}

inline PqPutter& operator>>(uint32_t value, PqPutter& putter) {
	return ::operator>>((float)value, putter);
}

inline PqPutter& operator>>(int64_t value, PqPutter& putter) {
	return ::operator>>((float)value, putter);
}

inline PqPutter& operator>>(uint64_t value, PqPutter& putter) {
	return ::operator>>((float)value, putter);
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
class PqPinComponent : public PqComponent {
public:
  PqPinComponent(uint8_t pin, uint8_t mode) : _pin(pin), _mode(mode) {}
  virtual ~PqPinComponent() {}

  /// Returns the pin this component is attached to.
  uint8_t pin() const { return _pin; }

  /// Returns the mode of the component.
  uint8_t mode() const { return _mode; }

  // The attached pin.
  uint8_t _pin;

  // The mode (varies according to context).
  uint8_t _mode;
};

#endif
