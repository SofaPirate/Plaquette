/**
 * PqBlinkWithOscillator
 *
 * Turns on and off a light emitting diode (LED) connected to a digital
 * pin using a SquareOsc.
 *
 * The circuit:
 * - LED attached from pin 13 to ground.
 * Note: on most Arduinos, there is already an LED on the board
 * that's attached to pin 13, so no hardware is needed for this example.
 *
 * Created in 2016 by Sofian Audry
 *
 * This example code is in the public domain.
 */
#include <Plaquette.h>

DigitalOut led; // defaults on pin 13
SquareOsc oscillator(2, 0.75); // 2 seconds interval, 75% duty cycle

void begin() {}

void run() {
  // Send oscillator value to LED.
  oscillator >> led;
}
