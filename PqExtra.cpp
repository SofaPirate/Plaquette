/*
 * PqExtra.cpp
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

#include "PqExtra.h"

StreamIn::StreamIn(Stream* stream) : _value(0), _stream(stream) {}

void StreamIn::update() {
	if (_stream->available())
		_value = _stream->parseFloat();
}

SquareOsc::SquareOsc(float period_, float dutyCycle_) {
  period(period_);
  dutyCycle(dutyCycle_);
}

void SquareOsc::setup() {
  _startTime = millis();
}

void SquareOsc::update() {
  // Check where we are.
  _isOn = ((millis() - _startTime) % _period < _dutyCyclePeriod);
}

SquareOsc& SquareOsc::period(float period) {
  // Convert period in ms.
  period *= 1000;
  _period = round(period);
  _period = max(_period, 1.0f); // at least 1ms
	return *this;
}

SquareOsc& SquareOsc::dutyCycle(float dutyCycle) {
  // Convert duty cycle in ms.
  dutyCycle = constrain(dutyCycle, 0, 1);
  dutyCycle *= _period;
  _dutyCyclePeriod = round(dutyCycle);
	return *this;
}

SineOsc::SineOsc(float period_, float phase_) : _value(0.5f) {
  period(period_);
  phase(phase_);
}

void SineOsc::setup() {
  _startTime = millis();
  _update(0);
}

void SineOsc::update() {
  // Check where we are.
  _update( (millis() - _startTime) / 1000.0f );
}

void SineOsc::_update(float t) {
  _value = (sin( (_phase + (t / _period)) * TWO_PI) + 1) / 2;
}

SineOsc& SineOsc::period(float period) {
  _period = period;
	return *this;
}

SineOsc& SineOsc::phase(float phase) {
  _phase = phase;
	return *this;
}

StreamOut::StreamOut(uint8_t digits) : _value(0), _digits(digits), _stream(&Serial) {}
StreamOut::StreamOut(Stream* stream, uint8_t digits) : _value(0), _digits(digits), _stream(stream) {}

float StreamOut::put(float value) {
  // Copy value.
  _value = value;

  // Print it.
  _stream->println(_value, _digits);

  // Return it.
  return _value;
}


OscilloscopeOut::OscilloscopeOut(float minValue, float maxValue, uint8_t precision)
  : _value(0), _minValue(minValue), _maxValue(maxValue), _precision(precision) {}

float OscilloscopeOut::put(float value) {
  // Copy value.
  _value = value;

  // Convert to bin.
  float mapped = map(_value, _minValue, _maxValue, 0.0f, 1.0f);
  int bin = round( mapped * _precision );
  bin = constrain(bin, 0, _precision-1);

  // Print.
  print(_minValue, 2);
  print(" |");
  for (int i=0; i<_precision; i++)
    print(i == bin ? '*' : ' ');
  print("| ");
  print(_maxValue, 2);
  println();

  // Return it.
  return _value;
}

Smoother::Smoother(float factor)
  : PqPutter(),
    MovingAverage(factor) {
}

float Smoother::put(float value) {
  return MovingAverage::update(value);
}

AdaptiveNormalizer::AdaptiveNormalizer(float smoothFactor)
  : PqPutter(),
    MovingStats(smoothFactor),
    _value(0.5f),
    _mean(0.5f),
    _stddev(0.25f)
{}

AdaptiveNormalizer::AdaptiveNormalizer(float mean, float stddev, float smoothFactor)
	: PqPutter(),
    MovingStats(smoothFactor),
    _value(mean),
    _mean(mean),
    _stddev(abs(stddev))
{}

float AdaptiveNormalizer::put(float value) {
  return (_value = MovingStats::update(value) * _stddev + _mean);
}

Normalizer::Normalizer()
  : PqPutter(),
    SimpleStats(),
    _value(0.5f),
    _mean(0.5f),
    _stddev(0.25f)
{}

Normalizer::Normalizer(float mean, float stddev)
	: PqPutter(),
    SimpleStats(),
    _value(mean),
    _mean(mean),
    _stddev(abs(stddev))
{}

float Normalizer::put(float value) {
  return (_value = SimpleStats::update(value) * _stddev + _mean);
}

MinMaxScaler::MinMaxScaler()
 : PqPutter(),
   _value(0.5f),
   _minValue(FLT_MAX),
   _maxValue(FLT_MIN)
{}

float MinMaxScaler::put(float value)
{
  _minValue = min(value, _minValue);
  _maxValue = max(value, _maxValue);
  _value = (_minValue == _maxValue ? 0.5f : map(value, _minValue, _maxValue, 0.0f, 1.0f));
	return _value;
}

Thresholder::Thresholder(float threshold, uint8_t mode)
  : PqPutter() {
			_init(threshold, mode, threshold);
		}

Thresholder::Thresholder(float threshold, uint8_t mode, float resetThreshold)
	: PqPutter() {
			_init(threshold, mode, resetThreshold);
		}

float Thresholder::put(float value) {
  bool high = (value > _threshold);
  bool low  = (value < _threshold);
  bool raising = (high && _wasLow);
  bool falling = (low  && _wasHigh);

	// Reset.
	if (raising) _wasLow  = false;
	if (falling) _wasHigh = false;
  // bool raising = (high && _prev != (+1));
  // bool falling = (low  && _prev != (-1));
  switch (_mode) {
    case THRESHOLD_HIGH:    _value = high;    break;
    case THRESHOLD_LOW:     _value = low;     break;
    case THRESHOLD_RISING:  _value = raising; break;
    case THRESHOLD_FALLING: _value = falling; break;
    case THRESHOLD_CHANGE:
    default:                _value = raising || falling;
  }

	if (value < _resetThreshold)      _wasLow = true;
	else if (value > _resetThreshold) _wasHigh = true;

//  _prev = (value < _threshold2 ? (-1) : (value > _threshold2 ? (+1) : (0)));
  return get();
}

void Thresholder::_init(float threshold, uint8_t mode, float resetThreshold) {
	_threshold = threshold;
	_mode = mode;
	// Set resetThreshold, with correction.
	if (mode == THRESHOLD_RISING)
		_resetThreshold = min(resetThreshold, threshold);
	else if (mode == THRESHOLD_FALLING)
		_resetThreshold = max(resetThreshold, threshold);
	else
		_resetThreshold = threshold;
	_value = _wasLow = _wasHigh = false;
}
