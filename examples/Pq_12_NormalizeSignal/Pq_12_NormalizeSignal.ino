/**
 * Pq_NormalizeSignal
 *
 * Shows how to normalize an analog signal.
 * The circuit:
 * - Potentiometer attached to analog input 0 center pin of the potentiometer
 *   to the analog pin: one side pin (either one) to ground the other side pin to +5V
 * Turns on and off a light emitting diode (LED) at increasingly long
 * durations.
 *
 * The circuit:
 * - LED attached from pin 13 to ground.
 * Note: on most Arduinos, there is already an LED on the board
 * that's attached to pin 13, so no hardware is needed for this example.
 *
 * Created in 2018 by Sofian Audry
 *
 * This example code is in the public domain.
 */
#include <Plaquette.h>

// The analog input.
AnalogIn in; // defaults on pin A0

// The normalization unit.
Normalizer normalizer;

// The serial output.
StreamOut out(Serial);

void begin() {}

void step() {
  // Signal will be renormalized around value 0.5.
  in >> normalizer >> out;
}
