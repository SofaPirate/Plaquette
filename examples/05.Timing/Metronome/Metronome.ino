/**
 * Metronome
 *
 * Uses the Metronome class and the Ramp class to simulate a metronome.
 *
 * Potentiometer:
 * - Potentiometer attached to analog input 0 center pin of the potentiometer
 *   to the analog pin: one side pin (either one) to ground the other side pin to +5V
 * 
 * LED:
 * - LED cathode (short leg) attached to ground
 * - LED anode (long leg) attached to 220-330 Ohm resistor
 * - resistor attached to analog (PWM) output 9
 *
 * Created in 2025 by Sofian Audry
 * 
 * This example code is in the public domain.
 */
#include <Plaquette.h>

// The LED.
AnalogOut led(9);

// The analog input.
AnalogIn pot(A0);

// The metronome object.
Metronome metronome;

// An alarm object (to make sure the LED light stays visible for a short time of 5ms).
Ramp ledRamp;

void begin() {
  // Ramp from 100% to 0% in 200ms.
  ledRamp.fromTo(1.0f, 0.0f);
  ledRamp.duration(0.2f);

  // Set initial value of ledRamp and LED to zero/off.
  0 >> ledRamp >> led;
}

void step() {
  // Adjust metronome period with potentiometer from 30 to 120 bpm.
  metronome.bpm(pot.mapTo(30, 120));

  // When the metronome "hits": start Ramp.
  if (metronome) {
    // Restart ramp.
    ledRamp.start();
  }

  // Send value of ramp to LED.
  ledRamp >> led;
}
