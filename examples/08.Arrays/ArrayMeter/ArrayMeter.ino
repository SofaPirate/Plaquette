/**
 * ArrayBlink
 *
 * Demonstrates the use of arrays to create a VU-meter with 5 LEDs.
 *
 * The circuit:
 * - Five LEDs.
 * - The anode of the LEDs are connected in series with a 220-ohm resistor to pins 4 to 8.
 * - Their cathodes connect to ground.
 * 
 * Created in 2024 by Sofian Audry
 *
 * This example code is in the public domain.
 */
#include <Plaquette.h>

// The LED.
const int N_LEDS = 5;

// An analog sensor such as a microphone or photocell.
AnalogIn sensor(A0);

// The three LEDs.
DigitalOut leds[] = { 4, 5, 6, 7, 8 }; // shorthand for DigitalOut leds[] = { DigitalOut(4), DigitalOut(5), DigitalOut(6), ... };

void begin() {}

void step() {
  // Map sensor value to number of LEDs to turn on.
  // NOTE: Due to the cast to int, the sensor value will need to be 1.0 (100%) to turn on all LEDs. 
  // This can be adjusted to be more tolerant by changing the call to mapFrom01(sensor, 0, N_LEDS+1).
  int nLedsOn = (int)mapFrom01(sensor, 0, N_LEDS);
  
  // Light up LEDs.
  for (int i=0; i<N_LEDS; i++) {
    // Turn on LED if i < nLedsOn.
    (i < nLedsOn) >> leds[i];
  }
}
