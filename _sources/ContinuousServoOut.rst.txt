.. include:: defs.hrst

ContinuousServoOut
==================

A source unit that controls a continous rotation servo-motor. A continuous
servo-motor can move indefinitely forward or backwards.

Servo motors have three wires: power, ground, and signal. The power wire is
typically red, and should be connected to the 5V pin on the Arduino board.
The ground wire is typically black or brown and should be connected to a ground
pin on the Arduino board. The signal pin is typically yellow, orange or white
and should be connected to a digital pin on the Arduino board. Note that servos
draw considerable power, so if you need to drive more than one or two, you'll
probably need to power them from a separate supply (i.e. not the +5V pin on
your Arduino). Be sure to connect the grounds of the Arduino and external power
supply together.

|Example|
---------

Everytime a button is pushed, the motor is stopped. Then upon button release it
starts moving in the opposite direction.

.. code-block:: c++

  #include <Plaquette.h>
  #include <PqServo.h>

  // The servo-motor output on pin 9.
  ContinuousServoOut servo(9);

  // The push-button.
  DigitalIn button(2);

  // Preserves the servo last speed value.
  float lastValue = 0;

  void begin() {
    // Debounce button.
    button.debounce();
    // Starts the servo.
    servo.put(1.0);
  }

  void step() {
    if (button) {
      // Save speed.
      lastValue = servo.get();
      // Stop servo.
      servo.stop();
    }
    else if (button.fell()) {
      // Reset speed.
      servo.put(lastValue);
      // ... then invert it.
      servo.reverse();
    }
  }

.. doxygenclass:: ContinuousServoOut
   :project: Plaquette
   :members: ContinuousServoOut, stop, reverse, get, put, pin


|SeeAlso|
---------
- :doc:`AnalogOut`
- :doc:`ServoOut`
