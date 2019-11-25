/**
 * Button
 *
 * Turns on and off a light emitting diode (LED) connected to digital
 * pin 13, when pressing a pushbutton attached to digital pin 2.
 *
 * The circuit:
 * - LED attached from digital pin 13 to ground (*)
 * - pushbutton attached to digital pin 2 from ground
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
DigitalOut led(LED_BUILTIN);

// The button.
DigitalIn button(2); // defaults to INTERNAL_PULLUP

void begin() {
  button.debounce(); // debounce button
}

void step() {
  // Flip the LED each time the button is pressed.
  if (button.rose())
    led.toggle();
}
