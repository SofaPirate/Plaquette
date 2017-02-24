/*
 * PqServo.h
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

#ifndef PQ_SERVO_H_
#define PQ_SERVO_H_

#include "PqCore.h"
#include <Servo.h>

// Output constants.
#define SERVO_DEFAULT  0x0
#define SERVO_CENTERED 0x1

/// Servo-motor absract object.
class AbstractServoOut : public PqPutter, public Servo {
protected:
  AbstractServoOut(uint8_t pin=9);

public:
  virtual ~AbstractServoOut();

  virtual float put(float value);

  virtual float get() { return _value; }

  virtual void setup();

	uint8_t pin() const { return _pin; }

  // Current value.
  float _value;

	// Servo pin (must be PWM).
  uint8_t _pin;
};

/// Standard servo-motor object (angular).
class ServoOut : public AbstractServoOut {
public:
  ServoOut(uint8_t pin=9);

	virtual float putAngle(float angle);
	virtual float getAngle();

	virtual void center() { put(0.5); }
};

/// Continuous servo-motor object.
class ContinuousServoOut : public AbstractServoOut {
public:
	ContinuousServoOut(uint8_t pin=9);

	virtual void stop();
	virtual void reverse();
};

#endif
