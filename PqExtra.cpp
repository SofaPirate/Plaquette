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


SquareOsc::SquareOsc(float period, float dutyCycle) {
  // Convert period in ms.
  period *= 1000;
  _period = round(period);
  _period = max(_period, 1.0f); // at least 1ms
  // Convert duty cycle in ms.
  dutyCycle = constrain(dutyCycle, 0, 1);
  dutyCycle *= _period;
  _dutyCyclePeriod = round(dutyCycle);
}

void SquareOsc::setup() {
  _startTime = millis();
}

void SquareOsc::update() {
  // Check where we are.
  _isOn = ((millis() - _startTime) % _period < _dutyCyclePeriod);
}

SerialOut::SerialOut(uint8_t digits) : _digits(digits) {}

float SerialOut::put(float value) {
  // Copy value.
  _value = value;

  // Print it.
  println(_value, _digits);

  // Return it.
  return _value;
}


OscilloscopeOut::OscilloscopeOut(float minValue, float maxValue, uint8_t precision)
  : _minValue(minValue), _maxValue(maxValue), _precision(precision) {}

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

float Normalizer::put(float value) {
  return (_value = SimpleStats::update(value) * _stddev + _mean);
}

MinMaxScaler::MinMaxScaler()
 : PqPutter(),
   _value(0.5f),
   _min(FLT_MAX),
   _max(FLT_MIN)
{}

float MinMaxScaler::put(float value)
{
  _min = min(value, _min);
  _max = max(value, _max);
  _value = map(value, _min, _max, 0.0f, 1.0f);
}

Thresholder::Thresholder(float threshold, int8_t dir)
  : PqPutter(),
    _threshold(threshold),
    _dir(dir),
    _value(0) {
}

float Thresholder::put(float value) {
  if (_dir != THRESHOLD_BOTH &&
      ( (_dir == THRESHOLD_HIGH) ^ (value > 0) ))
    _value = 0;
  else
    _value = (abs(value) > _threshold ? 1 : 0);
  return (float)_value;
}
