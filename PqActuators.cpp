/*
 * PqActuators.cpp
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

#include "PqActuators.h"

PqSingleActuator::PqSingleActuator(uint8_t pin, uint8_t mode) : _pin(pin), _mode(mode) {}
  
void PqSingleActuator::setup() {
  pinMode(_pin, OUTPUT);
}

bool PqSingleActuator::isDigital() const
{
  return (digitalPinToTimer(_pin) == NOT_ON_TIMER);
}

void PqSingleActuator::set(float value) {
  setRaw( round( isDigital() ? value : value * 255 ) );
}

void PqSingleActuator::setRaw(int value) {
  if (isDigital()) {
    value = constrain(value, 0, 1);
    value = isInverted() ? 1-value : value;
    digitalWrite( _pin, value ? HIGH : LOW );
  }
  else {
    value = constrain(value, 0, 255);
    analogWrite( _pin, isInverted() ? 255-value : value );
  }
}

LED::LED(uint8_t pin, uint8_t mode) : PqSingleActuator(pin, mode) {}

