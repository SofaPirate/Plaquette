/**
 * Modulation
 *
 * Uses a low-frequency oscillator (LFO) to control the frequency of another
 * oscillator.
 * Sends values to the serial output: best visualized using the Arduino Serial Plotter.
 *
 * Created in 2019 by Sofian Audry
 *
 * This example code is in the public domain.
 */

#include <Plaquette.h>

// The main oscillator.
Wave osc(SQUARE);

// The LFO.
Wave lfo(SINE, 20.0); // 20 seconds period

// The LED.
DigitalOut led(LED_BUILTIN);

void step() {
  // Modulate oscillator BPM between 30 to 180.
  osc.bpm( lfo.mapTo(30, 180) );

  // Send to LED.
  osc >> led;
}
