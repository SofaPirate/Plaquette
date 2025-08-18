/*
 * TimeSliceField.h
 *
 * (c) 2025 Sofian Audry        :: info(@)sofianaudry(.)com
 * (c) 2018 Thomas O Fredericks :: tof(@)t-o-f(.)info
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
class TimeSliceField : public AbstractField
{
public:
  /**
   * Constructor.
   * @param period the period in seconds
   */
  TimeSliceField(float period) : _count(COUNT), _full(false), _rolling(false), _changed(false), _period(period)
  {
      reset();
  }
  virtual ~TimeSliceField() {}

  /**
   * Returns value at given proportion in [0, 1].
   * @param proportion the proportion of the field to read
   * @return the value
   */
  virtual float read(float proportion) override
  {
    // Find index as a floating point value in [0, COUNT).
    proportion = constrain01(proportion);
    if (proportion >= 1) proportion = 0; // wrap
    float indexFloat = proportion * COUNT;

    // Find previous index and linear interpolation (lerp) factor.
    size_t prevIndex = floor(indexFloat);
    float lerpFactor = indexFloat - prevIndex;

    // Update previous index if rolling.
    if (_rolling) {
      prevIndex = (COUNT + _index - prevIndex) % COUNT;
    }

    // Find next index.
    size_t nextIndex = (prevIndex + 1) % COUNT;

    // Interpolate between prev and next value in buffer.
    return mapFrom01(lerpFactor, _buffer[prevIndex], _buffer[nextIndex]);
  }

  /// Returns value.
  virtual float get() override {
    return _lastValue;
  }

  /**
   * Pushes value into the unit.
   * @param value the value sent to the unit
   * @return the new value of the unit
   */
  virtual float put(float value) override {
    _lastValue = value;

    // Fill missing data.
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

    return _lastValue;
  }

  float valueAt(size_t index) {
    return _buffer[index];
  }

  size_t count() const { return _count; }

  /// Returns true if the field has been updated and is ready to be used.
  bool updated()
  {
    return _full && (!_rolling || _changed);
  }

  void reset()
  {
    _index = 0;
    _previousIndex = 0;
    _full = false;
    _changed = false;
  }

  void roll()     { _rolling = true; }
  void noRoll()   { _rolling = false; }
  bool isRolling() const { return _rolling; }

protected:

  virtual void step() override
  {
    // Reset if full.
    if (_full && !_rolling)
      reset();

    // Update phase time.
    if (phaseTimeUpdate(_phaseTime, _period, sampleRate(), true))
    {
      // Overflow.
      _index = _lastIndex;
      put(_lastValue);
      _full = true;
      _changed = true;
    }
    else
    {
      // No overflow.
      size_t nextIndex = floor(fixedToFloat(_phaseTime) * (float)_lastIndex );
      nextIndex = min(nextIndex, _lastIndex);
      _changed = (nextIndex != _index);
      _index = nextIndex;
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

  /// Returns true iff an event of a certain type has been triggered.
  virtual bool eventTriggered(EventType eventType) {
    switch (eventType) {
      case EVENT_UPDATE: return updated();
      default:           return Unit::eventTriggered(eventType);
    }
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
  bool _rolling;
  bool _changed;
  float _lastValue;
  fixed_t _phaseTime;
};

}
#endif
