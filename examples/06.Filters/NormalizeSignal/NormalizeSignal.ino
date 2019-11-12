/**
 * NormalizeSignal
 *
 * Shows how to normalize an analog signal.
 *
 * The circuit:
 * - A photoresistor connected to analog pin and to +5V
 * - A resistor (around 10k is a good value, higher values gives higher readings) from pin 0 to GND
 *
 * Created in 2018 by Sofian Audry
 *
 * This example code is in the public domain.
 */
#include <Plaquette.h>

// The analog input.
AnalogIn in(A0); // defaults on pin A0

// The normalization unit.
// NOTE: Alternatively you can use an AdaptiveNormalizer.
Normalizer normalizer;

// The serial output.
StreamOut out(Serial);

void begin() {}

void step() {
  // Signal will be renormalized around value 0.5.
  in >> normalizer >> out;
}
