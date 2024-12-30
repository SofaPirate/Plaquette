/**
 * UnitList
 *
 * Demonstrates the use of dynamic arrays UnitList to turn multiple LEDs on and off according
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

#if 0
// The LED.
const int N_LEDS = 3;

// A square-wave (on/off) oscillator for each LED.
// - The first LED will blink once per second.
// - The second LED will blink once every two seconds with a duty-cycle of 80%.
// - The third LED will blink once every three seconds.
UnitList sources(N_LEDS);

// The three LEDs.
AnalogOut leds[] = { 4, 5, 6 }; // shorthand for DigitalOut leds[] = { DigitalOut(4), DigitalOut(5), DigitalOut(6) };

void begin() {
  sources.add(new SquareWave(1.0));
  sources.add(new SineWave(1.0));
  sources.add(new AnalogIn(A0));
}

void step() {
  // Send oscillator value to corresponding LED.
  for (int i=0; i<N_LEDS; i++) {
    sources[i] >> leds[i];
  }
}
#endif