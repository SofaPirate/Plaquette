/**
 * ServoKnob
 *
 * Control the position of a RC (hobby) servo motor with your Arduino and a potentiometer.
 *
 * Circuit:
 *
 * Servo motors have three wires: power, ground, and signal. The power wire is
 * typically red, and should be connected to the 5V pin on the Arduino or Genuino
 * board. The ground wire is typically black or brown and should be connected to
 * a ground pin on the board. The signal pin is typically yellow, orange or white
 * and should be connected to pin 9 on the board.
 *
 * The potentiometer should be wired so that its two outer pins are connected to
 * power (+5V) and ground, and its middle pin is connected to analog input 0 on the board.
 *
 * Note that servos draw considerable power, so if you need to drive more than
 * one or two, you'll probably need to power them from a separate supply (i.e. not
 * the +5V pin on your Arduino). Be sure to connect the grounds of the Arduino and
 * external power supply together.
 *
 * Created in 2019 by Sofian Audry
 *
 * Inspired from the following code:
 * https://www.arduino.cc/en/Tutorial/Knob
 */
#include <Plaquette.h>
#include <PqServo.h>

// The analog input.
AnalogIn pot(A0);

// The servo-motor output on pin 9.
ServoOut servo(9);

void begin() {}

void step() {
  // Control the servomotor directly using the potentiometer.
  pot >> servo;
}
