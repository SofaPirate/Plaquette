/**
 * PqButton
 *
 * Turns on and off a light emitting diode(LED) connected to digital
 * pin 13, when pressing a pushbutton attached to pin 2.
 *
 * The circuit:
 * - LED attached from pin 13 to ground
 * - pushbutton attached to pin 2 from ground
 * Note: on most Arduinos there is already an LED on the board
 * attached to pin 13.
 *
 * Created in 2016 by Sofian Audry
 *
 * This example code is in the public domain.
 *
 * Inspired from the following code:
 * http://www.arduino.cc/en/Tutorial/Button
 */
#include <Plaquette.h>

// The LED.
DigitalOut led; // defaults on pin 13

// The button.
DigitalIn button(2); // defaults to INTERNAL_PULLUP

void begin() {
  button.smooth(); // debounce button
}

void run() {
  // Send button value to LED.
  button >> led;
}
