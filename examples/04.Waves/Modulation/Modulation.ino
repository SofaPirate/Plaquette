/**
 * Modulation
 *
 * Uses a low-frequency oscillator (LFO) to control the frequency of another
 * oscillator.
 *
 * Created in 2019 by Sofian Audry
 *
 * This example code is in the public domain.
 */

#include <Plaquette.h>

// The main oscillator.
Wave osc(SINE);

// The LFO.
Wave lfo(SINE, 10.0); // 10 seconds period

// Serial output.
Plotter out(115200);

void begin() {}

void step() {
  // Modulate oscillator frequency between 1 to 20 Hz.
  osc.frequency( lfo.mapTo(1, 20) );

  // Send to serial output.
  osc >> out;
}
