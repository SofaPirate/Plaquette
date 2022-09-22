/**
 * PeakDetection
 *
 * Shows how to use the PeakDetector unit to precisely find peaks
 * in an incoming signal.
 *
 * The circuit:
 * - A photoresistor connected to analog pin and to +5V.
 * - A resistor (around 10k is a good value, higher values gives higher readings) from pin 0 to GND.
 * - An LED attached from pin 13 to ground.
 * Note: on most Arduinos, there is already an LED on the board
 * that's attached to pin 13, so no hardware is needed for this example.
 *
 * Created in 2018 by Sofian Audry
 * Modified in 2022 by Sofian Audry
 *
 * This example code is in the public domain.
 */
#include <Plaquette.h>

// The analog input.
AnalogIn in(A0); // defaults on pin A0

// The normalization unit. Will normalize according to N(0, 1) (mean=0, stddev=1).
Normalizer normalizer(0, 1);

// The peak detector. Threshold is set at 1.5 standard deviations above normal.
PeakDetector detector(1.5f);

// Digital LED output.
DigitalOut led;

void begin() {
  // Adjust reload threshold (by default = threshold).
  detector.reloadThreshold(1.0f);

  // Default mode is PEAK_MAX.
  // Try changing to other modes eg.
  // detector.mode(PEAK_RISING)
  // If you use "inverted" modes PEAK_FALLING or PEAK_MIN you need to also invert
  // the trigger and reload thresholds eg.
  // detector.triggerThreshold(-1.5f);
  // detector.reloadThreshold(-1.0f);

  // Set a time window of 1 minute (60 seconds) on normalizer.
  // This will allow the normalier to slowly readjust itself
  // if the lighting conditions change.
  normalizer.timeWindow(60.0f);
};

void step() {
  // Signal is normalized and sent to peak detector.
  in >> normalizer >> detector;

  // Toggle LED when peak detector triggers.
  if (detector)
    led.toggle();
}
