/**
 * ArrayBlink
 *
 * Demonstrates the use of arrays to turn multiple LEDs on and off according
 * to different square oscillators.
 *
 * The circuit:
 * - Three LEDs.
 * - The anode of the LEDs are connected in series with a 220-ohm resistor to pins 4, 5, and 6. 
 * - Their cathodes connect to ground.
 * 
 * Created in 2024 by Sofian Audry
 *
 * This example code is in the public domain.
 */
#include <Plaquette.h>

// The LED.
const int N_LEDS = 3;

// A square-wave (on/off) oscillator for each LED.
// - The first LED will blink once per second.
// - The second LED will blink once every two seconds with a duty-cycle of 80%.
// - The third LED will blink once every three seconds.
SquareOsc oscillators[] = {
  SquareOsc(1.0), 
  SquareOsc(2.0, 0.8), 
  3.0 // shorthand for SquareOsc(3.0)
};

// The three LEDs.
DigitalOut leds[] = { 4, 5, 6 }; // shorthand for DigitalOut leds[] = { DigitalOut(4), DigitalOut(5), DigitalOut(6) };

void begin() {}

void step() {
  // Send oscillator value to corresponding LED.
  for (int i=0; i<N_LEDS; i++) {
    oscillators[i] >> leds[i];
  }
}
