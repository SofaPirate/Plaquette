/**
 * RandomBlink
 *
 * Uses a potentiometer to control the randomness of a blink with a stable average period.
 *
 * The circuit:
 * - Potentiometer attached to analog input 0 center pin of the potentiometer
 *   to the analog pin: one side pin (either one) to ground the other side pin to +5V
 *
 * Created in 2025 by Sofian Audry
 *
 * This example code is in the public domain.
 */
 #include <Plaquette.h>

// The analog input.
AnalogIn pot(A0);

// The main oscillator.
Wave osc(SQUARE, 1.0); // average period of 1 second

// The LED.
DigitalOut led(LED_BUILTIN);

void begin() {}

void step() {
  // Randomize oscillator according to potentiometer value.
  osc.jitter(pot);

  // Send to LED.
  osc >> led;
}
