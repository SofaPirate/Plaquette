  /*
  * TimeField.h
  *
  * (c) 2025 Sofian Audry        :: info(@)sofianaudry(.)com
  * (c) 2025 Thomas O Fredericks :: tof(@)t-o-f(.)info
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
#ifndef TIME_FIELD_H_
#define TIME_FIELD_H_

#include "AbstractField.h"

namespace pq {

template <size_t COUNT>
class TimeField : public AbstractField
{
public:
  void reset()
  {
    _index = 0;
    _previousIndex = 0;
    _full = false;
  }

  TimeField(float period) : _count(COUNT), _full(false), _period(period)
  {
      //_interval = floor(_period * 1000000.0); // convert to microseconds
      reset();
  }
  virtual ~TimeField() {}

  /**
   * Returns value at given proportion in [0, 1].
   * @param proportion the proportion of the field to read
   * @return the value
   */
  virtual float read(float proportion) override
  {
    proportion = constrain01(proportion);
    int index = floor(proportion * (float)_lastIndex);
    return _buffer[index];
  }

  float triggered()
  {
      return _full;
  }

  virtual void step() override
  {
    if (_full)
      reset();

    if (phaseTimeUpdate(_phaseTime, _period, sampleRate(), true))
    {
      _index = _lastIndex;
      _write(_lastValue);
      _full = true;
    }
    else
    {
      _index = floor(fixedToFloat(_phaseTime) * (float)_lastIndex );
      _index = min(_index, _lastIndex);
    }

    /* if (time > _chrono)
    {
      uint64_t duration = time - _chrono; // microseconds
      _index = (duration * COUNT / _interval);
      _index = min(_index, _lastIndex);
      if (duration >= _interval)
      {
        _index = _lastIndex;
        _write(_lastValue);
        _full = true;
      }
    }
    else
    {
      _index = 0;
    }
    */
  }

protected:
  virtual float _read() override
  {
    return _full;
  }

  virtual float _write(float value) override {
    _lastValue = value;

    // fill missing data
    if (_previousIndex < _index)
  {
      for (int i = _previousIndex + 1; i <= _index; ++i)
      {
        _buffer[i] = value;
      }
    }
    else
    {
      _buffer[_index] = value;
    }
    _previousIndex = _index;

    return _full;
  }

protected:
  float _buffer[COUNT];
  size_t _count;
  const size_t _lastIndex = COUNT - 1;
  size_t _index;
  size_t _previousIndex;
  float _period;
  //uint64_t _chrono;   // microseconds
  //uint64_t _interval; // microseconds
  bool _full;
  float _lastValue;
  fixed_t _phaseTime;
};

};
#endif
