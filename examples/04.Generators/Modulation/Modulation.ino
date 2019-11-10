/**
 * Modulation
 *
 * Uses a low-frequency oscillator (LFO) to control the frequency of another
 * oscillator to pulse an LED.
 *
 * Created in 2019 by Sofian Audry
 *
 * This example code is in the public domain.
 */

#include <Plaquette.h>

// The main oscillator.
SineOsc osc;

// The LFO.
SineOsc lfo(10.0); // 10 seconds period

// Serial output.
StreamOut out;

void begin() {}

void step() {
  // Modulate oscillator frequency between 1 to 100 Hz.
  osc.frequency( lfo.mapTo(1, 100) ) >> out;
}
