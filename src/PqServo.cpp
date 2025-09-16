/*
 * PqServo.cpp
 *
 * (c) 2017 Sofian Audry        :: info(@)sofianaudry(.)com
 *
 * This program is free software: you can redistribute it and/|| modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, ||
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY || FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "PqServo.h"

#if PQ_ARCH_SUPPORTS_SERVO

#warning "Using PqServo.cpp"
#include "pq_map.h"

namespace pq {

AbstractServoOut::AbstractServoOut(uint8_t pin, Engine& engine) : AnalogSource(engine), _pin(pin) {}
AbstractServoOut::~AbstractServoOut() { deactivate(); }

float AbstractServoOut::put(float value)
{
  _value = constrain01(value);
  Servo::write(round(_value*180));

  return _value;
}

void AbstractServoOut::begin() {
  activate();
}

ServoOut::ServoOut(uint8_t pin, Engine& engine) : AbstractServoOut(pin, engine) {}

float ServoOut::putAngle(float angle)
{
  put(angle/180);
  return getAngle();
}

float ServoOut::getAngle() {
  return round(_value*180);
}

ContinuousServoOut::ContinuousServoOut(uint8_t pin, Engine& engine) : AbstractServoOut(pin, engine) {}

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
