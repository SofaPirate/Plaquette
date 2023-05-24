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
  AnalogCallbacks(float(*readFunc) (T&),
                  void (*writeFunc)(T&, float),
                  void (*beginFunc)(T&) = NO_CALLBACK,
                  void (*stepFunc) (T&) = NO_CALLBACK) {
    _readFunc  = readFunc;
    _writeFunc = writeFunc;
    _beginFunc = beginFunc;
    _stepFunc = stepFunc;
  }

  AnalogCallbacks(float(*readFunc) (T&),
                  void (*beginFunc)(T&) = NO_CALLBACK,
                  void (*stepFunc) (T&) = NO_CALLBACK) {
    _readFunc  = readFunc;
    _writeFunc = NO_CALLBACK;
    _beginFunc = beginFunc;
    _stepFunc = stepFunc;
  }

  float read(T& object) {
    return _readFunc(object); 
  }

  void write(T& object, float value) {
    if (_writeFunc)
      _writeFunc(object, value);
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
  float (*_readFunc) (T&);
  void  (*_writeFunc)(T&, float);
  void  (*_beginFunc)(T&);
  void  (*_stepFunc) (T&);
};

template<>
class AnalogCallbacks<void> {
public:
  AnalogCallbacks(float(*readFunc) (),
                  void (*writeFunc)(float),
                  void (*beginFunc)() = NO_CALLBACK,
                  void (*stepFunc) () = NO_CALLBACK) {
    _readFunc  = readFunc;
    _writeFunc = writeFunc;
    _beginFunc = beginFunc;
    _stepFunc = stepFunc;
  }

  AnalogCallbacks(float(*readFunc) (),
                  void (*beginFunc)() = NO_CALLBACK,
                  void (*stepFunc) () = NO_CALLBACK) {
    _readFunc  = readFunc;
    _writeFunc = NO_CALLBACK;
    _beginFunc = beginFunc;
    _stepFunc = stepFunc;
  }

  float read() {
    return _readFunc(); 
  }

  void write(float value) {
    if (_writeFunc)
      _writeFunc(value);
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
  float (*_readFunc) ();
  void  (*_writeFunc)(float);
  void  (*_beginFunc)();
  void  (*_stepFunc) ();
};


template <typename T=void>
class AnalogFixture : public Node, public Smoothable {
public:
  AnalogFixture(T& object, AnalogCallbacks<T> callbacks)
    : _object(&object), _callbacks(callbacks) {}
  
  AnalogFixture(T& object,
                float(*readFunc) (T&),
                void (*beginFunc)(T&) = NO_CALLBACK,
                void (*stepFunc) (T&) = NO_CALLBACK)
    : _object(&object), _callbacks(readFunc, beginFunc, stepFunc) {}

  AnalogFixture(T& object,
                float(*readFunc) (T&),
                void (*writeFunc)(T&, float),
                void (*beginFunc)(T&) = NO_CALLBACK,
                void (*stepFunc) (T&) = NO_CALLBACK)
    : _object(&object), _callbacks(readFunc, writeFunc, beginFunc, stepFunc) {}

public:
  virtual float get() { return _smoothed(); }
  virtual operator float() { return get(); }

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
    _callbacks.write(*_object, value);
    return get();
  }

  virtual float _read() {
    return _callbacks.read(*_object);
  }

  T* _object;
  AnalogCallbacks<T> _callbacks;
};


template <>
class AnalogFixture<void> : public Node, public Smoothable {
public:
  AnalogFixture(AnalogCallbacks<void> callbacks)
    : _callbacks(callbacks) {}

  AnalogFixture(float(*readFunc)(),               
                void (*beginFunc)() = NO_CALLBACK,
                void (*stepFunc) () = NO_CALLBACK)
    : _callbacks(readFunc, beginFunc, stepFunc) {}

  AnalogFixture(float(*readFunc)(),               
                void (*writeFunc)(float),
                void (*beginFunc)() = NO_CALLBACK,
                void (*stepFunc) () = NO_CALLBACK)
    : _callbacks(readFunc, beginFunc, stepFunc) {}

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
    _callbacks.write(value);
    return get();
  }

  virtual float _read() {
    return _callbacks.read();
  }

  AnalogCallbacks<void> _callbacks;
};

template<typename T=void>
class DigitalCallbacks {
public:
  DigitalCallbacks(bool(*readFunc) (T&),
                   void(*writeFunc)(T&, bool),
                   void(*beginFunc)(T&) = NO_CALLBACK,
                   void(*stepFunc) (T&) = NO_CALLBACK) {
    _readFunc  = readFunc;
    _writeFunc = writeFunc;
    _beginFunc = beginFunc;
    _stepFunc = stepFunc;
  }

  DigitalCallbacks(bool(*readFunc) (T&),
                   void(*beginFunc)(T&) = NO_CALLBACK,
                   void(*stepFunc) (T&) = NO_CALLBACK) {
    _readFunc  = readFunc;
    _writeFunc = NO_CALLBACK;
    _beginFunc = beginFunc;
    _stepFunc = stepFunc;
  }

  bool read(T& object) {
    return _readFunc(object); 
  }

  void write(T& object, bool value) {
    if (_writeFunc)
      _writeFunc(object, value);
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
  bool (*_readFunc) (T&);
  void (*_writeFunc)(T&, bool);
  void (*_beginFunc)(T&);
  void (*_stepFunc) (T&);
};

template<>
class DigitalCallbacks<void> {
public:
  DigitalCallbacks(bool(*readFunc) (),
                   void(*writeFunc)(bool),
                   void(*beginFunc)() = NO_CALLBACK,
                   void(*stepFunc) () = NO_CALLBACK) {
    _readFunc  = readFunc;
    _writeFunc = writeFunc;
    _beginFunc = beginFunc;
    _stepFunc = stepFunc;
  }

  DigitalCallbacks(bool(*readFunc) (),
                   void(*beginFunc)() = NO_CALLBACK,
                   void(*stepFunc) () = NO_CALLBACK) {
    _readFunc  = readFunc;
    _writeFunc = NO_CALLBACK;
    _beginFunc = beginFunc;
    _stepFunc = stepFunc;
  }

  bool read() {
    return _readFunc(); 
  }

  void write(bool value) {
    if (_writeFunc)
      _writeFunc(value);
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
  bool (*_readFunc) ();
  void (*_writeFunc)(bool);
  void (*_beginFunc)();
  void (*_stepFunc) ();
};


template <typename T=void>
class DigitalFixture : public DigitalSource, public Debounceable {
public:
  DigitalFixture(T& object, DigitalCallbacks<T> callbacks)
    : _object(&object), _callbacks(callbacks) {}
  
  DigitalFixture(T& object,
                 bool(*readFunc) (T&),
                 void(*beginFunc)(T&) = NO_CALLBACK,
                 void(*stepFunc) (T&) = NO_CALLBACK)
    : _object(&object), _callbacks(readFunc, beginFunc, stepFunc) {}

  DigitalFixture(T& object,
                 bool(*readFunc) (T&),
                 void(*writeFunc)(T&, bool),
                 void(*beginFunc)(T&) = NO_CALLBACK,
                 void(*stepFunc) (T&) = NO_CALLBACK)
    : _object(&object), _callbacks(readFunc, writeFunc, beginFunc, stepFunc) {}

protected:
  virtual void begin() {
    _callbacks.begin(*_object);
    _begin();
  }

  virtual void step() {
    _callbacks.step(*_object);
    // Perform basic step.
    _step();
    // Save state.
    _setOn(_debounced());
  }

  virtual bool putOn(bool value) {
    _callbacks.write(*_object, value);
    return isOn();
  }

  virtual bool _isOn() {
    return _callbacks.read(*_object);
  }

  T* _object;
  DigitalCallbacks<T> _callbacks;
};


template <>
class DigitalFixture<void> : public DigitalSource, public Debounceable {
public:
  DigitalFixture(DigitalCallbacks<void> callbacks)
    : _callbacks(callbacks) {}

  DigitalFixture(bool(*readFunc)(),               
                 void(*beginFunc)() = NO_CALLBACK,
                 void(*stepFunc) () = NO_CALLBACK)
    : _callbacks(readFunc, beginFunc, stepFunc) {}

  DigitalFixture(bool(*readFunc)(),               
                 void(*writeFunc)(bool),
                 void(*beginFunc)() = NO_CALLBACK,
                 void(*stepFunc) () = NO_CALLBACK)
    : _callbacks(readFunc, beginFunc, stepFunc) {}

protected:
  virtual void begin() {
    _callbacks.begin();
    _begin();
  }

  virtual void step() {
    _callbacks.step();
    // Perform basic step.
    _step();
    // Save state.
    _setOn(_debounced());
  }

  virtual bool putOn(bool value) {
    _callbacks.write(value);
    return isOn();
  }

  virtual bool _isOn() {
    return _callbacks.read();
  }

  DigitalCallbacks<void> _callbacks;
};

}

#endif