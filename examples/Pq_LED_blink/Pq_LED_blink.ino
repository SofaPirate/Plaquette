/**
 * Pq_LED_blink
 *
 * Turns on and off a light emitting diode (LED) connected to a digital
 * pin, without using the delay() function.  This means that other code
 * can run at the same time without being interrupted by the LED code.
 *
 * The circuit:
 * - LED attached from pin 13 to ground.
 * - Note: on most Arduinos, there is already an LED on the board
 *   that's attached to pin 13, so no hardware is needed for this example.
 *
 * This example code is in the public domain.
 *
 * Based on code:
 * http://www.arduino.cc/en/Tutorial/BlinkWithoutDelay
 *
 * created 2005
 * by David A. Mellis
 * modified 8 Feb 2010
 * by Paul Stoffregen
 * modified 11 Nov 2013
 * by Scott Fitzgerald
 * modified 3 Nov 2016
 * by Sofian Audry
 */
#include <Plaquette.h>

DigitalOut led; // defaults on pin 13

// Stores last time LED was updated.
unsigned long previousMillis = 0;        // will store last time LED was updated

// Interval at which to blink (milliseconds).
const long interval = 1000;

void begin() {}

void run() {
  // Check to see if it's time to blink the LED.
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // Save the last time you blinked the LED.
    previousMillis = currentMillis;

    // Change stats.
    led.toggle();
  }
}
