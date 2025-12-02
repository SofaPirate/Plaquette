/**
 * ExtremeValuesDetection
 *
 * Shows how to normalize an analog signal in order to detect extreme values
 * (aka outliers). In order to do so we use a Normalizer object to to produce
 * a normalized value. We use it to detect outlier values based on deviation from mean.
 *
 * The circuit:
 * - A photoresistor connected to analog pin and to +5V.
 * - A resistor (around 10k is a good value, higher values gives higher readings) from pin 0 to GND.
 * - An LED attached from pin 13 to ground.
 * Note: on most Arduinos, there is already an LED on the board
 * that's attached to pin 13, so no hardware is needed for this example.
 *
 * Created in 2018 by Sofian Audry
 * Modified in 2022 and 2025 by Sofian Audry
 *
 * This example code is in the public domain.
 */
#include <Plaquette.h>

// The analog input.
AnalogIn in(A0); // defaults on pin A0

// The normalization unit.
Normalizer normalizer;

// Digital LED output.
DigitalOut led(LED_BUILTIN);

void begin() {
  // Smooth incoming signal.
  in.smooth();

  // Set a time window of 1 minute (60 seconds) on normalizer.
  // This will allow the normalier to slowly readjust itself
  // if the lighting conditions change.
  normalizer.timeWindow(60.0f);
}

void step() {
  // Signal is normalized.
  in >> normalizer;

  // Light LED if photo cell value drops below mean in a
  // significant way. You can adjust sensitivity by calling
  // isLowOutlier() with different parameter eg.
  // normalizer.isLowOutlier(in, 1.2) (more sensitive)
  // normalizer.isLowOutlier(in, 2.5) (less sensitive)
  normalizer.isLowOutlier(in) >> led;
}
