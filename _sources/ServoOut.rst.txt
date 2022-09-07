.. include:: defs.hrst

ServoOut
========

A source unit that controls a standard servo-motor.

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

Sweeps the shaft of a servo motor back and forth across 180 degrees.

.. code-block:: c++

  #include <Plaquette.h>
  #include <PqServo.h>

  // The servo-motor output on pin 9.
  ServoOut servo(9);

  // Oscillator to make the servo sweep.
  SineOsc oscillator(2.0);

  void begin() {
    // Position the servo in center.
    servo.center();
  }

  void step() {
    // Updates the value and send it back as output.
    oscillator >> servo;
  }

.. doxygenclass:: ServoOut
   :project: Plaquette
   :members: ServoOut, getAngle, putAngle, center, get, put, pin

|SeeAlso|
---------
- :doc:`AnalogOut`
- :doc:`ContinuousServoOut`
