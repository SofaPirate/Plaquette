/*
 * Callback.h
 *
 * (c) 2022 Sofian Audry        :: info(@)sofianaudry(.)com
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

#ifndef CALLBACK_H_
#define CALLBACK_H_

#include "PqCore.h"

namespace pq {

template <typename T>
union callback_t {
  float (*readVoid)();
  float (*readObject)(T&);
  void  (*writeVoid)(float);
  void  (*writeObject)(T&, float);
};

/**
 * Chronometer digital unit which emits 1/true/"on" for one frame, at a regular pace.
 */
template <typename T=void>
class AnalogCallback : public AnalogSource {
public:
  AnalogCallback(float(*read)(),
                 void(*write)(float) = 0)
    : _object(0) {
    _read.readVoid = read;
    _write.writeVoid = write;
  }
  AnalogCallback(T& object, float(*read)(T&),
                 void(*write)(T&, float) = 0)
    : _object(&object) {
    _read.readObject   = read;
    _write.writeObject = write;
  }

  virtual void begin() {}
  virtual void step() {
    _value = _object ? _read.readObject(*_object) : _read.readVoid();
  }

  virtual float put(float value) {
    if (_object) {
      if (_write.writeObject)
        _write.writeObject(*_object, value);
    }
    else if (_write.writeVoid)
      _write.writeVoid(value);
    
    return get();
  }

protected:
  T* _object;
  callback_t<T> _read;
  callback_t<T> _write;
};

}

#endif