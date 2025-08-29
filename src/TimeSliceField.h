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

/**
  * TimeSliceField generic class
  * @tparam COUNT the size of the buffer
  */
template <size_t COUNT>
class TimeSliceField : public AbstractField
{
public:
  /**
   * Constructor.
   * @param period the period in seconds
   */
  TimeSliceField(float period) : _period(period), _full(false), _rolling(false), _changed(false) {
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
      return _buffer[_trueIndex(LAST_INDEX)];

    // Find index as a floating point value in [0, COUNT-1).
    float indexFloat = max(proportion, 0) * LAST_INDEX;

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
  virtual float get() override { return _lastValue; }

  /**
   * Pushes value into the unit.
   * @param value the value sent to the unit
   * @return the new value of the unit
   */
  virtual float put(float value) override {

    // Update value.
    _nValuesStep++;
    _currentSumValuesStep += value;

    // Save last value.
    _lastValue = value;

    return _lastValue;
  }

  /**
   * Sets period over which the time slice occurs.
   * @param period the new period (in seconds)
   */
  void period(float period) { _period = max(period, 0.0f); }

  /// Returns period.
  float period() const { return _period; }

  /// Returns value at given index.
  float atIndex(size_t index) { return _buffer[index]; }

  /// Returns count.
  size_t count() const { return COUNT; }

  /// Returns true if the field has been updated and is ready to be used.
  bool updated() { return _full && (!_rolling || _changed); }

  /// Resets the field.
  void reset() {
    _index = 0;
    _previousIndex = 0;
    _currentSumValuesStep = 0;
    _nValuesStep = 0;
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

  void _updateBuffer() {

    // Compute value as average.
    float value = (_nValuesStep ? _currentSumValuesStep / _nValuesStep : _lastValue);

    // Fill missing data.
    while (_previousIndex != _index) {
      _buffer[_previousIndex] = value;
      _previousIndex = (_previousIndex + 1) % COUNT;
    }
    _buffer[_index] = value; // last element
  }

  virtual void step() override {
    // Reset if full (non-rolling).
    if (_full && !_rolling)
      reset();

    // Save previous index.
    size_t prevIndex = _index;

    // Track if we need to update buffer.
    bool needsUpdate = false;

    // Update phase time.
    if (phase32Update(_phase32, _period, sampleRate(), true)) {
      // Overflow.
      _index = LAST_INDEX;
      _full = true;
      needsUpdate = true;
    }
    else {
      // No overflow: set index as proportion of phase.
      _index = floor(fixed32ToFloat(_phase32) * COUNT);
      needsUpdate = (_index < LAST_INDEX);
    }

    // Record change.
    _changed = (_index != prevIndex);

    // Upon index change: update buffer.
    if (needsUpdate && _changed) {
      // Update buffer.
      _updateBuffer();

      // Reset.
      _currentSumValuesStep = 0;
      _nValuesStep = 0;
    }
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
  size_t _trueIndex(size_t index) {
    // Started rolling: adjust index.
    // _index corresponds to end position - used to perform the rolling
    if (_full && _rolling)
      index += _index + 1;

    // Return true index (clamped).
    return index % COUNT;
  }

protected:
  // Internal use: precompiled last index.
  static constexpr size_t LAST_INDEX = COUNT - 1;

  // Buffer containing values.
  float _buffer[COUNT];

  // Current index in buffer.
  size_t _index;

  // Previous index updated in buffer.
  size_t _previousIndex;

  // Period in seconds.
  float _period;

  // Last value recorded by put(float);
  float _lastValue;

  // Current sum of values between steps + n values (used to compute average).
  float _currentSumValuesStep;
  uint16_t _nValuesStep;

  // Phase in [0, 1).
  q0_32u_t _phase32;

  // Flags.
  bool _full      : 1; // is the buffer full
  bool _rolling   : 1; // rolling mode
  bool _changed   : 1; // did we change index
  uint8_t _unused : 5; // unused data

};

}
#endif
