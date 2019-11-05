/*
 * PqServo.cpp
 *
 * (c) 2017 Sofian Audry        :: info(@)sofianaudry(.)com
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

#include "PqServo.h"

#if defined(ARDUINO_ARCH_AVR) or defined(ARDUINO_ARCH_SAM) or defined(ARDUINO_ARCH_SAMD) or defined(ARDUINO_ARCH_STM32F4) or defined(ARDUINO_ARCH_NRF52)
#include "pq_map_real.h"

namespace pq {

AbstractServoOut::AbstractServoOut(uint8_t pin) : _pin(pin) {}
AbstractServoOut::~AbstractServoOut() { detach(); }

float AbstractServoOut::put(float value)
{
	_value = constrain(value, 0, 1);
  value = _value * 180;
  Servo::write(round(value));

  return _value;
}

void AbstractServoOut::begin() {
  attach(_pin);
}

ServoOut::ServoOut(uint8_t pin) : AbstractServoOut(pin) {}

float ServoOut::putAngle(float angle)
{
  put(angle/180);
  return getAngle();
}

float ServoOut::getAngle() { return round(_value*180); }

ContinuousServoOut::ContinuousServoOut(uint8_t pin) : AbstractServoOut(pin) {}

void ContinuousServoOut::begin() {
	AbstractServoOut::begin();
	stop();
}

void ContinuousServoOut::stop()
{
	_value = 0.5f;
	Servo::write(90); // to be sure
}

void ContinuousServoOut::reverse()
{
	put( 1 - get() );
}

} // namespace pq

#endif
