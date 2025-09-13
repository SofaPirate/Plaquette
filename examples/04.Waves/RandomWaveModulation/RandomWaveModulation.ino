/**
 * RandomWaveModulation
 *
 * Uses a low-frequency oscillator (LFO) to control the randomness of a wave.
 *
 * Sends values to the serial output: best visualized using the Arduino Serial Plotter.
 *
 * Created in 2025 by Sofian Audry
 *
 * This example code is in the public domain.
 */
#include <Plaquette.h>

// The main oscillator.
Wave osc(SINE, 0.5); // half-second period

// The LFO.
Wave lfo(SINE, 20.0); // 20 seconds period

void begin() {}

void step() {
  // Modulate randomization level.
  osc.randomize(lfo);

  // Send to serial output.
  print(osc); print(" "); println(lfo);
}
