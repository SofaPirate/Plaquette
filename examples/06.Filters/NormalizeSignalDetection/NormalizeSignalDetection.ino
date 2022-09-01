/**
 * NormalizeSignalDetection
 *
 * Shows how to normalize an analog signal in order to detect
 * outliers. In order to do so we use a Normalizer object to
 * to produce a normalized value with mean 0 and stddev 1.
 * We use it to detect outler values based on deviation from
 * mean.
 *
 * The circuit:
 * - A photoresistor connected to analog pin and to +5V.
 * - A resistor (around 10k is a good value, higher values gives higher readings) from pin 0 to GND.
 * - An LED attached from pin 13 to ground.
 * Note: on most Arduinos, there is already an LED on the board
 * that's attached to pin 13, so no hardware is needed for this example.
 *
 * Created in 2018 by Sofian Audry
 *
 * This example code is in the public domain.
 */
#include <Plaquette.h>

// The analog input.
AnalogIn in(A0); // defaults on pin A0

// The normalization unit with mean 0 and stddev 1.
Normalizer normalizer(0, 1); // N(0, 1)

// Digital LED output.
DigitalOut led(13);

void begin() {}

void step() {
  // Signal will be renormalized according to N(0, 1).
  in >> normalizer;

  // Light LED if value differs from mean by more
  // than twice the standard deviation.
  (normalizer > 2.0) >> led;
}
