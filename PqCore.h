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
#define PLAQUETTE_SERIAL_BAUD_RATE 115200
#endif

class Plaquette;

/**
 * Main class for components to be added to Plaquette.
 * Components can be transducers (sensors,actuators) or special integrated circuits.
 */
class PqComponent {
  friend class Plaquette;

protected:
  PqComponent();
  virtual ~PqComponent() {}

protected:
  virtual void setup() {}
  virtual void update() {}
};

/// The main Plaquette singleton class containing all the components.
class Plaquette {
  friend class PqComponent;

private:
  PqComponent* _components[PLAQUETTE_MAX_COMPONENTS];
  uint8_t _nComponents;

public:
  Plaquette();

  /// Initializes all components (calls setup() on all of them).
  void setup();

  /// Updates all components (calls update() on all of them).
  void update();

  /// Returns the current number of components.
  uint8_t nComponents() const { return _nComponents; }

private:
  /// Adds a component to Plaquette.
  void add(PqComponent * component);
};

/// The singleton.
extern Plaquette Pq;

/// A generic class representing a simple source.
class PqGetter : public PqComponent {
public:
  /// Constructor.
  PqGetter() {}
  virtual ~PqGetter() {}

  /// Returns reading (typically between 0 and 1, may vary depending on class).
  virtual float get() = 0;

  /// Object can be used director to access its value.
  operator float() { return get(); }

  // // Logical operators.
  // bool operator==(float value) { return get() == value; }
  // bool operator!=(float value) { return get() != value; }
  // bool operator>(float value)  { return get() >  value; }
  // bool operator>=(float value) { return get() >= value; }
  // bool operator<(float value)  { return get() <  value; }
  // bool operator<=(float value) { return get() <= value; }
  //
  // // Math operators.
  // float operator+(PqGetter& getter) { return get() + getter.get(); }
  // float operator+(float value)      { return get() + value; }
  // float operator+(double value)     { return get() + value; }
  // float operator-(PqGetter& getter) { return get() - getter.get(); }
  // float operator-(float value)      { return get() - value; }
  // float operator-(double value)     { return get() - value; }
  // float operator*(PqGetter& getter) { return get() * getter.get(); }
  // float operator*(float value)      { return get() * value; }
  // float operator*(double value)     { return get() * value; }
  // float operator/(PqGetter& getter) { return get() / getter.get(); }
  // float operator/(float value)      { return get() / value; }
  // float operator/(double value)     { return get() / value; }
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

  /// Returns reading (either 0 or 1).
  virtual float get() { return getInt(); }

  /// Operator that allows to use direction in conditional expressions.
  operator bool() { return isOn(); }

  /// Operator that return 0 or 1 depending on value.
  operator int() { return getInt(); }
};

/// A generic class representing a simple sink.
class PqPutter : virtual public PqGetter {
public:
  /// Constructor.
  PqPutter() {}
  virtual ~PqPutter() {}

  /// Pushes value into the component and returns its (possibly filtered) value.
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

  virtual void setIsOn(bool isOn) {
    put (isOn ? 1 : 0);
  }

  /// Sets output to "on".
  virtual void on() { setIsOn(true); }

  /// Sets output to "off".
  virtual void off() { setIsOn(false); }

  /// Switches between on and off.
  virtual void toggle() {
    setIsOn(!isOn());
  }

  /// Returns reading (either 0 or 1).
  virtual float get() { return PqDigitalGetter::get(); }
};

// Operators /////////////////////////////////////////////////////

inline PqGetter& operator>>(PqGetter& getter, PqPutter& putter) {
  putter.put( getter.get() );
  return putter;
}

inline PqPutter& operator>>(float value, PqPutter& putter) {
  putter.put( value );
  return putter;
}

inline PqPutter& operator>>(double value, PqPutter& putter) {
  return ::operator>>((float)value, putter);
}

inline float& operator>>(PqGetter& getter, float& value) {
  return (value = getter.get());
}

inline double& operator>>(PqGetter& getter, double& value) {
  return (value = getter.get());
}

inline PqPutter& operator>>(int value, PqPutter& putter) {
  putter.put( value );
  return putter;
}

inline int& operator>>(PqGetter& getter, int& value) {
  return (value = round(getter.get()));
}

inline PqPutter& operator>>(bool value, PqPutter& putter) {
  putter.put( value ? 1 : 0 );
  return putter;
}

inline bool& operator>>(PqGetter& getter, bool& value) {
  return (value = getter.get() > 0.5f );
}

// HACK: These operator overrides are there just to avoid making a bitshift
// by error due to a combination of PqGetter operator float() and
// PqDigitalGetter int() and bool() which allow bitshift operations.
inline PqGetter& operator>>(float value, PqGetter& getter) {
  return getter;
}
inline PqGetter& operator>>(int value, PqGetter& getter) {
  return getter;
}
inline PqGetter& operator>>(bool value, PqGetter& getter) {
  return getter;
}

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
