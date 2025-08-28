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
  TimeSliceField(float period) : _count(COUNT), _period(period), _full(false), _rolling(false), _changed(false) {
    reset();
  }
  virtual ~TimeSliceField() {}

  /**
   * Returns value at given proportion in [0, 1].
   * @param proportion the proportion of the field to read
   * @return the value
   */
  virtual float at(float proportion) override {
    // Special case: proportion == 1 -> return last value.
    if (proportion >= 1)
      return _buffer[_trueIndex(_lastIndex)];

    // Find index as a floating point value in [0, COUNT-1).
    float indexFloat = max(proportion, 0) * (COUNT-1);

    // Find previous index and linear interpolation (lerp) factor.
    size_t prevIndex = floor(indexFloat); // index in [0, COUNT-1) ie. [0, COUNT-2]
    float lerpFactor = indexFloat - prevIndex; // remainder

    // Update previous index if rolling.
    prevIndex = _trueIndex(prevIndex);

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

    // Fill missing data.
    while (_previousIndex != _index) {
      _buffer[_previousIndex] = value;
      _previousIndex = (_previousIndex + 1) % COUNT;
    }
    _buffer[_previousIndex] = value;

    // Set last value.
    _lastValue = value;

    return _lastValue;
  }

  /// Returns value at given index.
  float atIndex(size_t index) {
    return _buffer[index];
  }

  /// Returns count.
  size_t count() const { return _count; }

  /// Returns true if the field has been updated and is ready to be used.
  bool updated() { return _full && (!_rolling || _changed); }

  /// Resets the field.
  void reset() {
    _index = 0;
    _previousIndex = 0;
    _rollingIndex = 0;
    _full = false;
    _changed = false;
  }

  /**
   * Sets rolling mode.
   * @param rolling the rolling mode
   */
  void setRolling(bool rolling) { _rolling = rolling; }

  /// Activates rolling mode.
  void rolling() { setRolling(true); }

  /// Deactivates rolling mode.
  void noRolling() { setRolling(false); }

  /// Returns true if rolling mode is active.
  bool isRolling() const { return _rolling; }

  /// Registers event callback on update event.
  virtual void onUpdate(EventCallback callback) { onEvent(callback, EVENT_UPDATE); }

protected:

  virtual void step() override {
    // Reset if full (non-rolling).
    if (_full && !_rolling)
      reset();

    // Update phase time.
    if (phase32Update(_phase32, _period, sampleRate(), true)) {
      // Overflow.
      _index = _lastIndex;
      put(_lastValue);
      _full = true;
      _changed = true;
    }
    else {
      // No overflow.
      size_t nextIndex = floor(fixed32ToFloat(_phase32) * (float)_lastIndex );
      nextIndex = min(nextIndex, _lastIndex);
      _changed = (nextIndex != _index);
      _index = nextIndex;
    }

    // Update rolling index.
    if (_full && _rolling && _changed)
      _rollingIndex = (_rollingIndex + 1) % COUNT;
  }

  /// Returns true iff an event of a certain type has been triggered.
  virtual bool eventTriggered(EventType eventType) {
    switch (eventType) {
      case EVENT_UPDATE: return updated();
      default:           return Unit::eventTriggered(eventType);
    }
  }

private:
    // Internal use: return true index by adjusting it if rolling.
  size_t _trueIndex(size_t index) { return (_rolling ? index : (COUNT + _rollingIndex - index)) % COUNT; }

protected:
  float _buffer[COUNT];
  size_t _count;
  const size_t _lastIndex = COUNT - 1;
  size_t _index;
  size_t _previousIndex;
  size_t _rollingIndex;
  float _period;
  //uint64_t _chrono;   // microseconds
  //uint64_t _interval; // microseconds
  bool _full : 1;
  bool _rolling : 1;
  bool _changed : 1;
  uint8_t _unused : 5;
  float _lastValue;
  q0_32u_t _phase32;
};

}
#endif
