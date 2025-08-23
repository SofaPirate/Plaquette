/**
 * TimeSliceFieldWave
 *
 * Demonstrates the use of a TimeSliceField to create a scrolling effect with 8 LEDs.
 *
 * The circuit:
 *
 * - Array of LEDs:
 *   - Eight LEDs.
 *   - The anode of the LEDs are connected in series with a 220-ohm resistor to pins 2 to 9.
 *   - Their cathodes connect to ground.
 *  - Potentiometer attached to analog input 0 center pin of the potentiometer
 *    to the analog pin: one side pin (either one) to ground the other side pin to +5V
 *
 * Created in 2025 by Sofian Audry
 *
 * This example code is in the public domain.
 */
#include <Plaquette.h>

// The number of LEDs.
const int N_LEDS = 8;

// A potentimeter.
AnalogIn pot(A0);

// The array of LEDs.
DigitalOut leds[] = { 2, 3, 4, 5, 6, 7, 8, 9 }; // shorthand for DigitalOut leds[] = { DigitalOut(2), DigitalOut(3), DigitalOut(4), ... };

// The time slice field.
TimeSliceField<N_LEDS> timeSlice(1.0f);

// The wave.
Wave wave(SQUARE);

void begin() {
  // Set field to rolling mode.
  timeSlice.rolling();
}

void step() {
  // Adjust wave period.
  wave.period(pot.mapTo(5.0, 0.5));

  // Update the field.
  wave >> timeSlice;

  // Update the LEDs.
  if (timeSlice.updated()) {
    timeSlice.populate(leds, N_LEDS);
  }
}
