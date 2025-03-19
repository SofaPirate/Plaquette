/*
 * Fixtures.h
 *
 * (c) 2023 Sofian Audry        :: info(@)sofianaudry(.)com
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

#ifndef FIXTURES_H_
#define FIXTURES_H_

#include "PqInputs.h"

namespace pq {

#define NO_CALLBACK 0

template<typename T=void>
class AnalogCallbacks {
public:
  // Constructors.
  AnalogCallbacks(float(*getFunc)  (T&),
                  void (*putFunc)  (T&, float),
                  void (*beginFunc)(T&) = NO_CALLBACK,
                  void (*stepFunc) (T&) = NO_CALLBACK) {
    _getFunc  = getFunc;
    _putFunc = putFunc;
    _beginFunc = beginFunc;
    _stepFunc = stepFunc;
  }

  AnalogCallbacks(float(*getFunc)  (T&),
                  void (*beginFunc)(T&) = NO_CALLBACK,
                  void (*stepFunc) (T&) = NO_CALLBACK) {
    _getFunc  = getFunc;
    _putFunc = NO_CALLBACK;
    _beginFunc = beginFunc;
    _stepFunc = stepFunc;
  }

  float get(T& object) {
    return _getFunc(object); 
  }

  void put(T& object, float value) {
    if (_putFunc)
      _putFunc(object, value);
  }
  
  void begin(T& object) { 
    if (_beginFunc) 
      _beginFunc(object); 
  }
  
  void step(T& object) { 
    if (_stepFunc) 
      _stepFunc(object); 
  }

  
private:
  float (*_getFunc) (T&);
  void  (*_putFunc)(T&, float);
  void  (*_beginFunc)(T&);
  void  (*_stepFunc) (T&);
};

template<>
class AnalogCallbacks<void> {
public:
  AnalogCallbacks(float(*getFunc)  (),
                  void (*putFunc)  (float),
                  void (*beginFunc)() = NO_CALLBACK,
                  void (*stepFunc) () = NO_CALLBACK) {
    _getFunc  = getFunc;
    _putFunc = putFunc;
    _beginFunc = beginFunc;
    _stepFunc = stepFunc;
  }

  AnalogCallbacks(float(*getFunc)  (),
                  void (*beginFunc)() = NO_CALLBACK,
                  void (*stepFunc) () = NO_CALLBACK) {
    _getFunc  = getFunc;
    _putFunc = NO_CALLBACK;
    _beginFunc = beginFunc;
    _stepFunc = stepFunc;
  }

  float get() {
    return _getFunc(); 
  }

  void put(float value) {
    if (_putFunc)
      _putFunc(value);
  }
  
  void begin() { 
    if (_beginFunc) 
      _beginFunc(); 
  }
  
  void step() { 
    if (_stepFunc) 
      _stepFunc();
  }
  
private:
  float (*_getFunc) ();
  void  (*_putFunc)(float);
  void  (*_beginFunc)();
  void  (*_stepFunc) ();
};


template <typename T=void>
class AnalogFixture : public Unit, public Smoothable {
public:
  AnalogFixture(T& object, AnalogCallbacks<T> callbacks)
    : _object(&object), _callbacks(callbacks) {}
  
  AnalogFixture(T& object,
                float(*getFunc)  (T&),
                void (*beginFunc)(T&) = NO_CALLBACK,
                void (*stepFunc) (T&) = NO_CALLBACK)
    : _object(&object), _callbacks(getFunc, beginFunc, stepFunc) {}

  AnalogFixture(T& object,
                float(*getFunc)  (T&),
                void (*putFunc)  (T&, float),
                void (*beginFunc)(T&) = NO_CALLBACK,
                void (*stepFunc) (T&) = NO_CALLBACK)
    : _object(&object), _callbacks(getFunc, putFunc, beginFunc, stepFunc) {}

public:
  virtual float get() { return _smoothed(); }
  operator float() { return get(); }

protected:
  virtual void begin() {
    _callbacks.begin(*_object);
    _begin(); 
  }

  virtual void step() {
    _callbacks.step(*_object);
    _step(); 
  }

  virtual float put(float value) {
    _callbacks.put(*_object, value);
    return get();
  }

  virtual float _read() {
    return _callbacks.get(*_object);
  }

  virtual float _sampleRate() const { return sampleRate(); }

  T* _object;
  AnalogCallbacks<T> _callbacks;
};


template <>
class AnalogFixture<void> : public Unit, public Smoothable {
public:
  AnalogFixture(AnalogCallbacks<void> callbacks)
    : _callbacks(callbacks) {}

  AnalogFixture(float(*getFunc)(),               
                void (*beginFunc)() = NO_CALLBACK,
                void (*stepFunc) () = NO_CALLBACK)
    : _callbacks(getFunc, beginFunc, stepFunc) {}

  AnalogFixture(float(*getFunc)(),
                void (*putFunc)(float),
                void (*beginFunc)() = NO_CALLBACK,
                void (*stepFunc) () = NO_CALLBACK)
    : _callbacks(getFunc, putFunc, beginFunc, stepFunc) {}

public:
  virtual float get() { return _smoothed(); }
  virtual operator float() { return get(); }

protected:
  virtual void begin() {
    _callbacks.begin();
    _begin(); 
  }

  virtual void step() {
    _callbacks.step();
    _step(); 
  }

  virtual float put(float value) {
    _callbacks.put(value);
    return get();
  }

  virtual float _read() {
    return _callbacks.get();
  }

  virtual float _sampleRate() const { return sampleRate(); }

  AnalogCallbacks<void> _callbacks;
};

template<typename T=void>
class DigitalCallbacks {
public:
  DigitalCallbacks(bool(*getFunc)  (T&),
                   void(*putFunc)  (T&, bool),
                   void(*beginFunc)(T&) = NO_CALLBACK,
                   void(*stepFunc) (T&) = NO_CALLBACK) {
    _getFunc  = getFunc;
    _putFunc = putFunc;
    _beginFunc = beginFunc;
    _stepFunc = stepFunc;
  }

  DigitalCallbacks(bool(*getFunc)  (T&),
                   void(*beginFunc)(T&) = NO_CALLBACK,
                   void(*stepFunc) (T&) = NO_CALLBACK) {
    _getFunc  = getFunc;
    _putFunc = NO_CALLBACK;
    _beginFunc = beginFunc;
    _stepFunc = stepFunc;
  }

  bool get(T& object) {
    return _getFunc(object); 
  }

  void put(T& object, bool value) {
    if (_putFunc)
      _putFunc(object, value);
  }
  
  void begin(T& object) { 
    if (_beginFunc) 
      _beginFunc(object); 
  }
  
  void step(T& object) { 
    if (_stepFunc) 
      _stepFunc(object); 
  }

private:
  bool (*_getFunc) (T&);
  void (*_putFunc)(T&, bool);
  void (*_beginFunc)(T&);
  void (*_stepFunc) (T&);
};

template<>
class DigitalCallbacks<void> {
public:
  DigitalCallbacks(bool(*getFunc) (),
                   void(*putFunc)(bool),
                   void(*beginFunc)() = NO_CALLBACK,
                   void(*stepFunc) () = NO_CALLBACK) {
    _getFunc  = getFunc;
    _putFunc = putFunc;
    _beginFunc = beginFunc;
    _stepFunc = stepFunc;
  }

  DigitalCallbacks(bool(*getFunc) (),
                   void(*beginFunc)() = NO_CALLBACK,
                   void(*stepFunc) () = NO_CALLBACK) {
    _getFunc  = getFunc;
    _putFunc = NO_CALLBACK;
    _beginFunc = beginFunc;
    _stepFunc = stepFunc;
  }

  bool get() {
    return _getFunc(); 
  }

  void put(bool value) {
    if (_putFunc)
      _putFunc(value);
  }
  
  void begin() { 
    if (_beginFunc) 
      _beginFunc(); 
  }
  
  void step() { 
    if (_stepFunc) 
      _stepFunc();
  }
  
private:
  bool (*_getFunc) ();
  void (*_putFunc)(bool);
  void (*_beginFunc)();
  void (*_stepFunc) ();
};


template <typename T=void>
class DigitalFixture : public DigitalSource, public Debounceable {
public:
  DigitalFixture(T& object, DigitalCallbacks<T> callbacks)
    : _object(&object), _callbacks(callbacks) {}
  
  DigitalFixture(T& object,
                 bool(*getFunc) (T&),
                 void(*beginFunc)(T&) = NO_CALLBACK,
                 void(*stepFunc) (T&) = NO_CALLBACK)
    : _object(&object), _callbacks(getFunc, beginFunc, stepFunc) {}

  DigitalFixture(T& object,
                 bool(*getFunc) (T&),
                 void(*putFunc)(T&, bool),
                 void(*beginFunc)(T&) = NO_CALLBACK,
                 void(*stepFunc) (T&) = NO_CALLBACK)
    : _object(&object), _callbacks(getFunc, putFunc, beginFunc, stepFunc) {}

protected:
  virtual void begin() {
    _callbacks.begin(*_object);
    _begin();
  }

  virtual void step() {
    // Perform basic step.
    _callbacks.step(*_object);
    _step();
    // Save state.
    _onValue = _debounced();
    // Update change state.
    _updateChangeState();
  }

  virtual bool putOn(bool value) {
    _callbacks.put(*_object, value);
    return isOn();
  }

  virtual bool _isOn() {
    return _callbacks.get(*_object);
  }

  virtual float _time() const { return seconds(); }

  T* _object;
  DigitalCallbacks<T> _callbacks;
};


template <>
class DigitalFixture<void> : public DigitalSource, public Debounceable {
public:
  DigitalFixture(DigitalCallbacks<void> callbacks)
    : _callbacks(callbacks) {}

  DigitalFixture(bool(*getFunc)(),               
                 void(*beginFunc)() = NO_CALLBACK,
                 void(*stepFunc) () = NO_CALLBACK)
    : _callbacks(getFunc, beginFunc, stepFunc) {}

  DigitalFixture(bool(*getFunc)(),               
                 void(*putFunc)(bool),
                 void(*beginFunc)() = NO_CALLBACK,
                 void(*stepFunc) () = NO_CALLBACK)
    : _callbacks(getFunc, putFunc, beginFunc, stepFunc) {}

protected:
  virtual void begin() {
    _callbacks.begin();
    _begin();
  }

  virtual void step() {
    // Perform basic step.
    _callbacks.step();
    _step();
    // Save state.
    _onValue = _debounced();
    // Update change state.
    _updateChangeState();
  }

  virtual bool putOn(bool value) {
    _callbacks.put(value);
    return isOn();
  }

  virtual bool _isOn() {
    return _callbacks.get();
  }

  virtual float _time() const { return seconds(); }

  DigitalCallbacks<void> _callbacks;
};

}

#endif