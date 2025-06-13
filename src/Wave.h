/*
 * Wave.h
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

#ifndef WAVE_H
#define WAVE_H

#include "PqCore.h"
#include "pq_osc_utils.h"

namespace pq {

class FloatAndQ032 {
public:
  using Q032 = uint32_t;

private:
  float _valueFloat;
  Q032 _valueQ032;
  bool _dirtyFloat;
  bool _dirtyQ032;

public:
  FloatAndQ032() : _valueFloat(0.0f), _valueQ032(0), _dirtyFloat(false), _dirtyQ032(false) {
  }

  // Set a new float value
  void setFloat(float value) {
    //if (value != _valueFloat) {
      _valueFloat = value;
      _dirtyQ032 = true;
    //}
  }

    // Set a new float value
  void setQ032(Q032 value) {
   // if (value != _valueQ032) {
      _valueQ032 = value;
      _dirtyFloat = true;
   // }
  }

  // Get the float value
  float getFloat()  {
    if (_dirtyFloat) {
      _valueFloat = fixedToFloat(_valueQ032);
      _dirtyFloat = false;
    }
    return _valueFloat;
  }

  // Get the fixed value (updates if needed)
  Q032 getQ032() {
     if (_dirtyQ032) {
      _valueQ032 = floatTofixed(_valueFloat);
      _dirtyQ032 = false;
    }
    return _valueQ032;
  }

  


};

class Wave : public Unit {
public:
  enum Shape { Sine, Square, Triangle, Ramp, Random };

  /**
   * Constructor.
   * @param engine the engine running this unit
   */
  Wave(Shape shape, Engine& engine = Engine::primary());

  /**
   * Constructor.
   * @param period the period of oscillation (in seconds)
   * @param engine the engine running this unit
   */
  Wave(Shape shape, float period, Engine& engine = Engine::primary());

  /**
   * Constructor.
   * @param period the period of oscillation (in seconds)
   * @param skew the duty-cycle as a value in [0, 1]
   * @param engine the engine running this unit
   */
  Wave(Shape shape, float period, float skew, Engine& engine = Engine::primary());

  virtual ~Wave() {};

  void begin() override;
  void step() override;
  float get() override;
  float put(float f) override;
  void start();
  void stop();
  void pause();
  void skew(float f);
  void period(float f);
  void frequency(float f);

protected:
private:
  Shape _shape;
  // Period (seconds).
  FloatAndQ032 _frequency;
  // Skew (%)
  FloatAndQ032 _skew;
  FloatAndQ032 _phasor;
  bool _running;
  float _value;
};

}  // namespace pq

#endif
