/**
 * ServoJitter
 *
 * Change the position of a RC servo motor randomly in a jittery manner using
 * a simple "random walk".
 *
 * Circuit:
 *
 * Servo motors have three wires: power, ground, and signal. The power wire is
 * typically red, and should be connected to the 5V pin on the Arduino or Genuino
 * board. The ground wire is typically black or brown and should be connected to
 * a ground pin on the board. The signal pin is typically yellow, orange or white
 * and should be connected to pin 9 on the board.
 *
 * Created in 2019 by Sofian Audry
 *
 */
#include <Plaquette.h>
#include <PqServo.h>

// Controls the rate of change of the servo.
const float JITTER_RATE = 0.1; // 10% per second

// The servo-motor output on pin 9.
ServoOut servo(9);

void begin() {
  // Position the servo in center.
  servo.center();
}

void step() {
  // Updates the value and send it back as output.
  // NOTE: value is multiplied by samplePeriod() to keep motion range proportional
  // even if sample rate changes.
  (servo + randomFloat(-JITTER_RATE, JITTER_RATE) * samplePeriod()) >> servo;
}
