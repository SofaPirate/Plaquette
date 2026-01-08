/**
 * RescaleSignal
 *
 * Rescales an analog input using the min-max scaler.
 *
 * The circuit:
 * - A photoresistor connected to analog pin and to +5V
 * - A resistor (around 10k is a good value, higher values gives higher readings) from pin 0 to GND
 * - LED cathode (short leg) attached to ground
 * - LED anode (long leg) attached to 220-330 Ohm resistor
 * - resistor attached to analog (PWM) output 9
 *
 * Created in 2025 by Sofian Audry
 *
 * This example code is in the public domain.
 *
 * Inspired from the following code:
 * http://www.arduino.cc/en/Tutorial/AnalogInput
 */
#include <Plaquette.h>

// The LED.
AnalogOut led(9); // PWM pin 9

// The analog input.
AnalogIn in(A0);

// This unit will rescale signal to full range [0, 1].
// TIP: Try replacing the min-max scaler with a robust scaler or a normalizer, which are
// more robust to outliers. Just replace MinMaxScaler with RobustScaler or Normalizer.
MinMaxScaler scaler;

// Serial plotter.
Plotter plotter(115200, "raw,rescaled");

void begin() {
  // Set a time window of 1 minute (60 seconds) on scaler.
  // This will allow the scaler to slowly readjust itself
  // if the lighting conditions change.
  scaler.timeWindow(60.0f);
}

void step() {
	// Analog input is rescaled then sent as LED value.
	in >> scaler >> led;

  // Prints raw and rescaled values, for comparison using the Serial plotter.
  in >> plotter;
  scaler >> plotter;
}
