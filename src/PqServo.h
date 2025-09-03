/*
 * PqServo.h
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
#ifndef PQ_SERVO_H_
#define PQ_SERVO_H_

#ifndef PQ_ARCH_SUPPORTS_SERVO
#if (defined(ARDUINO_ARCH_AVR) || defined(ARDUINO_ARCH_SAM) || \
     defined(ARDUINO_ARCH_SAMD) || defined(ARDUINO_ARCH_STM32F4) || defined(ARDUINO_ARCH_NRF52))
  #define PQ_ARCH_SUPPORTS_SERVO 1
#else
  #define PQ_ARCH_SUPPORTS_SERVO 0
#endif
#endif

#if PQ_ARCH_SUPPORTS_SERVO

#include "PqCore.h"
#include <Servo.h>

namespace pq {

/// Servo-motor absract object.
class AbstractServoOut : public AnalogSource, public Servo {
protected:
  AbstractServoOut(uint8_t pin, Engine& engine = Engine::primary());

public:
  virtual ~AbstractServoOut();

  /**
   * Pushes value into the unit.
   * @param value the value sent to the unit
   * @return the new value of the unit
   */
  virtual float put(float value);

  /// Returns the pin this servomotor is attached to.
  uint8_t pin() const { return _pin; }

  /// Activates the servomotor (default).
  virtual void activate() { attach(_pin); }

  /// Deactivates the servomotor.
  virtual void deactivate() { detach(); }

  /// Returns true if the servomotor is active.
  virtual bool isActive() { return attached(); }

protected:
  virtual void begin();

  // Servo pin (must be PWM).
  uint8_t _pin;
};

/// Standard servo-motor (angular).
class ServoOut : public AbstractServoOut {
public:
  /**
   * Constructor for a standard servo-motor.
   * @param pin the pin number
   */
  ServoOut(uint8_t pin, Engine& engine = Engine::primary());

  /**
   * Sets the servomotor position to a specific angle between 0 and 180 degrees.
   * @param angle the angle in degrees
   * @returns the current angle
   */
  virtual float putAngle(float angle);

  /// Return the current angular angle in [0, 180].
  virtual float getAngle();

  /// Re-centers the servo-motor.
  virtual void center() { put(0.5); }
};

/// Continuous servo-motor.
class ContinuousServoOut : public AbstractServoOut {
public:
  /**
   * Constructor for a continuous rotation servo-motor.
   * @param pin the pin number
   */
  ContinuousServoOut(uint8_t pin, Engine& engine = Engine::primary());

  /// Stops the servo-motor.
  virtual void stop();

  /// Sends servo-motor in reverse mode.
  virtual void reverse();

protected:
  virtual void begin();
};

} // namespace pq

#endif
#endif
