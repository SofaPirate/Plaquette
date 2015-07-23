/*
 * PqSensors.cpp
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

#include "PqSensors.h"

PqSingleSensor::PqSingleSensor(uint8_t pin, uint8_t mode) : _pin(pin), _mode(mode) {}

bool PqSingleSensor::isOn() const
{
  return (get() > 0.5f);
}

float PqSingleSensor::get() const
{
  int rawValue = getRaw();
  return (isDigital() ? rawValue : rawValue / 1023.0f);
}

int PqSingleSensor::getRaw() const
{
  if (isDigital())
  {
    int rawValue = digitalRead(_pin) == HIGH ? 1 : 0;
    return (isInverted() ? 1-rawValue : rawValue);
  }
  else
  {
    int rawValue = analogRead(_pin);
    return (isInverted() ? 1023-rawValue : rawValue);
  }
}
		
void PqSingleSensor::setup() {
  pinMode(_pin, _mode == INTERNAL_PULLUP ? INPUT_PULLUP : INPUT);
}

