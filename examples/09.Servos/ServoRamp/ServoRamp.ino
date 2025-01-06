/**
 * ServoRamp
 *
 * Use ramps to move the position of a RC servo motor with your Arduino and a potentiometer.
 * 
 * Circuit:
 *
 * Servo motors have three wires: power, ground, and signal. The power wire is
 * typically red, and should be connected to the 5V pin on the Arduino or Genuino
 * board. The ground wire is typically black or brown and should be connected to
 * a ground pin on the board. The signal pin is typically yellow, orange or white
 * and should be connected to pin 9 on the board.
 *
 * Note that servos draw considerable power, so if you need to drive more than
 * one or two, you'll probably need to power them from a separate supply (i.e. not
 * the +5V pin on your Arduino). Be sure to connect the grounds of the Arduino and
 * external power supply together.
 *
 * Created in 2025 by Sofian Audry
 *
 * Inspired from the following code:
 * https://www.arduino.cc/en/Tutorial/Knob
 * https://www.arduino.cc/en/Tutorial/Sweep
 */
#include <Plaquette.h>

// The metronome will launch a new ramp every 5 seconds.
Metronome metro(5.0);

// Ramp that controls the angle of the servo.
Ramp ramp;

// The servo-motor output on pin 9.
ServoOut servo(9);

void begin() {
  // Initialize ramp.
  ramp.speed(30); // 30 degrees per second
  ramp.easing(easeOutQuint);
}

void step() {
  // When metronome ticks, launch a new ramp.
  if (metro)
    // Ramp to random position.
    ramp.go(randomFloat(180));
  
  // Send value of ramp to servo (as an angle between 0 and 180 degrees).
  servo.putAngle(ramp);
}
