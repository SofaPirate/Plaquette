/**
 * ServoSweep
 *
 * Sweeps the shaft of a RC servo motor back and forth across 180 degrees.
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
 * Inspired from the following code:
 * https://www.arduino.cc/en/Tutorial/Sweep
 */
#include <Plaquette.h>
#include <PqServo.h>

// Sine wave source.
SineOut osc(5.0); // period of 5 seconds

// The servo-motor output on pin 9.
ServoOut servo(9);

void begin() {}

void step() {
  // Control the servomotor directly with the sine wave.
  osc >> servo;
}
